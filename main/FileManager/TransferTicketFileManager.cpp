#include "FileManager.h"
#include <fstream>
#include <sstream>
#include <filesystem>
#include <iomanip>
#include <stdexcept>
#include "Utilities.h"

using namespace std;

/**
 * @brief Nap danh sach Phieu chuyen xu ly tu file (data/transfer_tickets.txt)
 */
vector<TransferTicket> FileManager<TransferTicket>::load(const string& filePath)
{
    vector<TransferTicket> result;
    ifstream inFile(filePath);
    if (!inFile.is_open())
    {
        return result;
    }

    string line;
    while (getline(inFile, line))
    {
        size_t firstChar = line.find_first_not_of(" \t\r\n");
        if (firstChar == string::npos || line[firstChar] == '-') continue;

        stringstream ss(line);
        string id, ticketId, fromId, toId, reason, statusStr;
        if (getline(ss, id, '|') &&
            getline(ss, ticketId, '|') &&
            getline(ss, fromId, '|') &&
            getline(ss, toId, '|') &&
            getline(ss, reason, '|') &&
            getline(ss, statusStr, '|'))
        {
            id = Utils::trim(id);
            ticketId = Utils::trim(ticketId);
            fromId = Utils::trim(fromId);
            toId = Utils::trim(toId);
            reason = Utils::trim(reason);
            statusStr = Utils::trim(statusStr);

            if (id == "Ma ID") continue;
            if (Utils::extractIdNumber(id, "TF") == -1) continue;

            try
            {
                int st = stoi(statusStr);
                if (st < 0 || st > 2) continue;   // trang thai ngoai khoang -> dong loi

                TransferTicket t(ticketId, fromId, toId, reason);
                t.setId(id);
                t.restoreStatus(static_cast<TransferStatus>(st));
                result.push_back(t);
            }
            catch (...)
            {
                continue;
            }
        }
    }
    return result;
}

/**
 * @brief Ghi toan bo danh sach Phieu chuyen xu ly ra file (data/transfer_tickets.txt)
 */
void FileManager<TransferTicket>::save(const string& filePath, const vector<TransferTicket>& data)
{
    filesystem::path p(filePath);
    if (p.has_parent_path())
    {
        filesystem::create_directories(p.parent_path());
    }

    ofstream outFile(filePath);
    if (!outFile.is_open())
    {
        throw runtime_error("Khong the mo file de ghi: " + filePath);
    }

    outFile << left
            << setw(8)  << "Ma ID"      << " | "
            << setw(10) << "Ma Ticket"  << " | "
            << setw(10) << "Agent chuyen" << " | "
            << setw(10) << "Agent nhan" << " | "
            << setw(30) << "Ly do"      << " | "
            << "Trang thai" << "\n";
    outFile << string(90, '-') << "\n";

    for (const auto& t : data)
    {
        outFile << left
                << setw(8)  << t.getId()          << " | "
                << setw(10) << t.getTicketId()    << " | "
                << setw(10) << t.getFromAgentId() << " | "
                << setw(10) << t.getToAgentId()   << " | "
                << setw(30) << t.getReason()      << " | "
                << static_cast<int>(t.getStatus()) << "\n";
    }

    outFile.flush();
    if (!outFile)
    {
        throw runtime_error("Khong the ghi du lieu vao file: " + filePath);
    }
}
