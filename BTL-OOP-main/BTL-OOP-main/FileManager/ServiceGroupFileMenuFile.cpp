#include "FileManager.h"
#include <iostream>
#include <fstream>
#include <sstream>
#include <filesystem>
#include <iomanip>
#include <stdexcept>
#include "Utilities.h"

using namespace std;

/**
 * @brief Nạp danh sách Nhóm dịch vụ từ file văn bản có kẻ hàng cột
 * @param filePath Đường dẫn file (data/service_groups.txt)
 * @return Danh sách vector các đối tượng ServiceGroup đã đọc được
 */
vector<ServiceGroup> FileManager<ServiceGroup>::load(const string& filePath)
{
    vector<ServiceGroup> result;
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
        // Bỏ qua dòng trống hoặc dòng kẻ ngang phân cách (bắt đầu bằng '-')
        if (firstChar == string::npos || line[firstChar] == '-') continue;

        stringstream ss(line);
        string id, name, desc, activeStr;
        // Đọc các trường dữ liệu ngăn cách bằng ký tự '|'
        if (getline(ss, id, '|') &&
            getline(ss, name, '|') &&
            getline(ss, desc, '|') &&
            getline(ss, activeStr, '|'))
        {
            id = Utils::trim(id);
            name = Utils::trim(name);
            desc = Utils::trim(desc);
            activeStr = Utils::trim(activeStr);

            // Bỏ qua dòng tiêu đề cột
            if (id == "Ma ID")
            {
                continue;
            }

            // KIỂM TRA LỖI Ở ĐÂY: Sử dụng Utils để validate ID với tiền tố "SG"
            // Nếu hàm trả về -1 nghĩa là ID sai định dạng (không phải SGxxx hoặc không phải số)
            if (Utils::extractIdNumber(id, "SG") == -1) 
            {
                continue; // Bỏ qua dữ liệu rác
            }

            ServiceGroup sg(name, desc);
            sg.setId(id);
            if (activeStr == "0")
            {
                sg.deactivate();
            }
            else
            {
                sg.activate();
            }
            result.push_back(sg);
        }
    }
    return result;
}

/**
 * @brief Ghi toàn bộ danh sách Nhóm dịch vụ ra file văn bản kẻ hàng cột ngay ngắn
 * @param filePath Đường dẫn file (data/service_groups.txt)
 * @param data Dữ liệu cần ghi
 */
void FileManager<ServiceGroup>::save(const string& filePath, const vector<ServiceGroup>& data)
{
    // Đảm bảo thư mục cha tồn tại trước khi tạo file
    filesystem::path p(filePath);
    if (p.has_parent_path())
    {
        filesystem::create_directories(p.parent_path());
    }
    cout <<"Check error" << filesystem::absolute(filePath);

    ofstream outFile(filePath);
    if (!outFile.is_open())
    {
        throw runtime_error("Khong the mo file de ghi: " + filePath);
    }

    // Kẻ bảng: Dòng tiêu đề cột và dòng gạch ngang phân cách
    outFile << left
            << setw(8)  << "Ma ID"            << " | "
            << setw(25) << "Ten nhom dich vu" << " | "
            << setw(45) << "Mo ta"            << " | "
            << "Trang thai" << "\n";
    outFile << string(95, '-') << "\n";

    for (const auto& sg : data)
    {
        outFile << left
                << setw(8)  << sg.getId()          << " | "
                << setw(25) << sg.getName()        << " | "
                << setw(45) << sg.getDescription() << " | "
                << (sg.isActive() ? 1 : 0)         << "\n";
    }

    outFile.flush();
    if (!outFile)
    {
        throw runtime_error("Khong the ghi du lieu vao file: " + filePath);
    }
}