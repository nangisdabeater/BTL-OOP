#include "FileManager.h"
#include <iostream>
#include <fstream>
#include <sstream>
#include <filesystem>
#include <iomanip>
#include <stdexcept>
#include <Utilities.h>

using namespace std;

/**
 * @brief Nạp danh sách Ticket từ file văn bản có kẻ hàng cột
 * @param filePath Đường dẫn file (data/tickets.txt)
 * @return Danh sách vector các đối tượng Ticket đã đọc được
 */
vector<Ticket> FileManager<Ticket>::load(const string& filePath)
{
    vector<Ticket> result;
    ifstream inFile(filePath);
    if (!inFile.is_open())
    {
        return result; // Nếu file chưa tồn tại thì trả về danh sách rỗng
    }

    string line;
    while (getline(inFile, line))
    {
        if (line.empty()) continue;
        size_t firstChar = line.find_first_not_of(" \t\r\n");
        // Bỏ qua dòng trống hoặc dòng kẻ ngang phân cách
        if (firstChar == string::npos || line[firstChar] == '-') continue;

        stringstream ss(line);
        string id, title, desc, customerId, sgId, statusStr;
        // Đọc các trường dữ liệu ngăn cách bằng ký tự '|'
        if (getline(ss, id, '|') &&
            getline(ss, title, '|') &&
            getline(ss, desc, '|') &&
            getline(ss, customerId, '|') &&
            getline(ss, sgId, '|') &&
            getline(ss, statusStr, '|'))
        {
            id = Utils::trim(id);
            title = Utils::trim(title);
            desc = Utils::trim(desc);
            customerId = Utils::trim(customerId);
            sgId = Utils::trim(sgId);
            statusStr = Utils::trim(statusStr);

            // Bỏ qua dòng tiêu đề cột
            if (id == "Ma ID" || id.length() != 5 || id.substr(0, 2) != "NP")
            {
                continue;
            }

            Ticket t(title, desc, customerId, sgId);
            t.setId(id);
            try
            {
                int st = stoi(statusStr);
                if (st >= 0 && st <= 3)
                {
                    t.changeStatus(static_cast<TicketStatus>(st));
                }
            }
            catch (...)
            {
                t.changeStatus(TicketStatus::OPEN);
            }
            result.push_back(t);
        }
    }
    return result;
}

/**
 * @brief Ghi toàn bộ danh sách Ticket ra file văn bản kẻ hàng cột ngay ngắn
 * @param filePath Đường dẫn file (data/tickets.txt)
 * @param data Dữ liệu cần ghi
 */
void FileManager<Ticket>::save(const string& filePath, const vector<Ticket>& data)
{
    // Đảm bảo thư mục cha tồn tại trước khi tạo file
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

    // Kẻ bảng: Dòng tiêu đề cột và dòng gạch ngang phân cách
    outFile << left
            << setw(8)  << "Ma ID"          << " | "
            << setw(25) << "Tieu de"        << " | "
            << setw(40) << "Mo ta chi tiet" << " | "
            << setw(10) << "Ma KH"          << " | "
            << setw(10) << "Ma Nhom DV"     << " | "
            << "Trang thai" << "\n";
    outFile << string(110, '-') << "\n";

    for (const auto& t : data)
    {
        outFile << left
                << setw(8)  << t.getId()             << " | "
                << setw(25) << t.getTitle()          << " | "
                << setw(40) << t.getDescription()    << " | "
                << setw(10) << t.getCustomerId()     << " | "
                << setw(10) << t.getServiceGroupId() << " | "
                << static_cast<int>(t.getStatus())   << "\n";
    }

    outFile.flush();
    if (!outFile)
    {
        throw runtime_error("Khong the ghi du lieu vao file: " + filePath);
    }
}
