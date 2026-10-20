#include "FileManager.h"
#include <iostream>
#include <fstream>
#include <sstream>
#include <iomanip>
#include <stdexcept>
#include "Utilities.h"

using namespace std;

/**
Nạp danh sách Dịch vụ (ServiceManager / ServiceGroup) từ file văn bản
Định dạng ID yêu cầu: SM001, SM002,...
Đường dẫn file (data/service_groups.txt)
Trả về danh sách vector các đối tượng ServiceGroup
 */
vector<ServiceGroup> FileManager<ServiceGroup>::load(const string& filePath){
    vector<ServiceGroup> result;
    ifstream inFile(filePath);
    if (!inFile.is_open()){
        // Tự tạo file rỗng nếu chưa tồn tại

        ofstream outFile(filePath);
        outFile.close();
        return result;
    }

    string line;
    while (getline(inFile, line)){
        if (line.empty()) continue;
        size_t firstChar = line.find_first_not_of(" \t\r\n");
        // Bỏ qua dòng trống hoặc dòng kẻ ngang phân cách (bắt đầu bằng '-' hoặc '+')
        if (firstChar == string::npos || line[firstChar] == '-' || line[firstChar] == '+') continue;

        stringstream ss(line);
        string id, name, desc;

        // Đọc các trường dữ liệu ngăn cách bằng ký tự '|'
        if (getline(ss, id, '|') &&getline(ss, name, '|') &&getline(ss, desc, '|')){
            id = Utils::trim(id);
            name = Utils::trim(name);
            desc = Utils::trim(desc);

            // Bỏ qua dòng tiêu đề cột
            if (id == "Ma ID"){
                continue;
            }

            // Kiểm tra định dạng ID với tiền tố "SM" (SMxxx)
            if (Utils::extractIdNumber(id, "SM") == -1) {
                cout << "[Canh bao] Dong sai dinh dang: \"" << line << "\". Bo qua dong nay." << endl;
                continue;
            }

            // Đọc các cột tiếp theo (hỗ trợ cả định dạng mới 8 cột và định dạng cũ 4 cột)
            string nextCol;
            vector<string> remainingCols;
            while (getline(ss, nextCol, '|')){
                remainingCols.push_back(Utils::trim(nextCol));
            }

            string department = "Phong CSKH";
            int slaHours = 24;
            double baseFee = 0.0;
            string createdDate = "01/01/2026";
            bool isActive = true;

            if (remainingCols.size() >= 5){
                // Định dạng mới: dept | sla | baseFee | createdDate | active
                department = remainingCols[0];
                try { slaHours = stoi(remainingCols[1]); } catch (...) { slaHours = 24; }
                try { baseFee = stod(remainingCols[2]); } catch (...) { baseFee = 0.0; }
                createdDate = remainingCols[3];
                if (createdDate.length() == 10 && createdDate[4] == '-' && createdDate[7] == '-')
                {
                    createdDate = createdDate.substr(8, 2) + "/" + createdDate.substr(5, 2) + "/" + createdDate.substr(0, 4);
                }
                isActive = (remainingCols[4] != "0");
            }
            else if (!remainingCols.empty()){
                // Định dạng cũ: chỉ có activeStr
                isActive = (remainingCols[0] != "0");
            }
            else{
                cout << "[Canh bao] Dong sai dinh dang: \"" << line << "\". Bo qua dong nay." << endl;
                continue;
            }

            try{
                ServiceGroup sg(name, desc, department, slaHours, baseFee, createdDate);
                sg.setId(id);
                if (isActive)
                {
                    sg.activate();
                }
                else
                {
                    sg.deactivate();
                }
                result.push_back(sg);
            }
            catch (...){
                cout << "[Canh bao] Dong sai dinh dang: \"" << line << "\". Bo qua dong nay." << endl;
            }
        }
        else{
            cout << "[Canh bao] Dong sai dinh dang: \"" << line << "\". Bo qua dong nay." << endl;
        }
    }
    return result;
}

/**
Ghi toàn bộ danh sách Dịch vụ ra file văn bản kẻ hàng cột ngay ngắn
Đường dẫn file (data/service_groups.txt)
Tham số đầu vào: data (Dữ liệu cần ghi)
 */
void FileManager<ServiceGroup>::save(const string& filePath, const vector<ServiceGroup>& data){


    ofstream outFile(filePath);
    if (!outFile.is_open()){
        throw runtime_error("Khong the mo file de ghi: " + filePath);
    }

    // Kẻ bảng: Dòng tiêu đề cột và dòng gạch ngang phân cách
    outFile << left
            << setw(8)  << "Ma ID"            << " | "
            << setw(25) << "Ten dich vu"      << " | "
            << setw(35) << "Mo ta"            << " | "
            << setw(18) << "Phong ban"        << " | "
            << setw(10) << "SLA(gio)"         << " | "
            << setw(12) << "Phi co ban"       << " | "
            << setw(14) << "Ngay ban hanh"    << " | "
            << "Trang thai"                    << "\n";
    outFile << string(137, '-') << "\n";

    for (const auto& sg : data){
        outFile << left
                << setw(8)  << sg.getId()          << " | "
                << setw(25) << sg.getName()        << " | "
                << setw(35) << sg.getDescription() << " | "
                << setw(18) << sg.getDepartment()  << " | "
                << setw(10) << sg.getSlaHours()    << " | "
                << setw(12) << fixed << setprecision(2) << sg.getBaseFee() << " | "
                << setw(14) << sg.getCreatedDate() << " | "
                << (sg.isActive() ? 1 : 0)         << "\n";
    }

    outFile.flush();
    if (!outFile){
        throw runtime_error("Khong the ghi du lieu vao file: " + filePath);
    }
}
