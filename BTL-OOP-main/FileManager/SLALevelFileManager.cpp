#include "FileManager.h"
#include <fstream>
#include <sstream>
#include <filesystem>
#include <iomanip>
#include <stdexcept>
#include "Utilities.h"

using namespace std;

/**
 * @brief Nap danh sach Muc SLA tu file (data/sla_levels.txt)
 * - File chua ton tai -> tra ve danh sach rong
 * - Dong sai dinh dang / sai rang buoc -> bo qua, doc tiep cac dong sau
 */
vector<SLALevel> FileManager<SLALevel>::load(const string& filePath)
{
    vector<SLALevel> result;
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
        string id, name, resp, resolve, prio;
        if (getline(ss, id, '|') &&
            getline(ss, name, '|') &&
            getline(ss, resp, '|') &&
            getline(ss, resolve, '|') &&
            getline(ss, prio, '|'))
        {
            id = Utils::trim(id);
            name = Utils::trim(name);
            resp = Utils::trim(resp);
            resolve = Utils::trim(resolve);
            prio = Utils::trim(prio);

            if (id == "Ma ID") continue;                           // dong tieu de
            if (Utils::extractIdNumber(id, "SL") == -1) continue;  // ID sai dinh dang

            try
            {
                SLALevel s(name, stoi(resp), stoi(resolve), stoi(prio));
                s.setId(id);
                result.push_back(s);
            }
            catch (...)
            {
                // Sai kieu so hoac vi pham rang buoc -> bo qua dong nay
                continue;
            }
        }
    }
    return result;
}

/**
 * @brief Ghi toan bo danh sach Muc SLA ra file (data/sla_levels.txt)
 */
void FileManager<SLALevel>::save(const string& filePath, const vector<SLALevel>& data)
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
            << setw(8)  << "Ma ID"        << " | "
            << setw(20) << "Ten muc SLA"  << " | "
            << setw(15) << "Phan hoi(phut)" << " | "
            << setw(12) << "Xu ly(gio)"   << " | "
            << "Uu tien" << "\n";
    outFile << string(75, '-') << "\n";

    for (const auto& s : data)
    {
        outFile << left
                << setw(8)  << s.getId()              << " | "
                << setw(20) << s.getName()            << " | "
                << setw(15) << s.getResponseMinutes() << " | "
                << setw(12) << s.getResolveHours()    << " | "
                << s.getPriority() << "\n";
    }

    outFile.flush();
    if (!outFile)
    {
        throw runtime_error("Khong the ghi du lieu vao file: " + filePath);
    }
}
