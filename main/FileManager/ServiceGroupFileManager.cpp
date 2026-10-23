#include "FileManager.h"
#include <iostream>
#include <fstream>
#include <sstream>
#include <iomanip>
#include <stdexcept>
#include "Utilities.h"

// Chỉ dùng using namespace std ở file .cpp để không làm bẩn không gian tên ở file .h
using namespace std;

// Chuyên biệt hóa hàm load cho ServiceGroup: đọc file văn bản có kẻ bảng thay vì file nhị phân để dễ kiểm tra trực tiếp
vector<ServiceGroup> FileManager<ServiceGroup>::load(const string& filePath){
    vector<ServiceGroup> result;
    ifstream inFile(filePath);
    if (!inFile.is_open()){
        // Tự động tạo file rỗng nếu file chưa tồn tại để chương trình không bị văng lỗi lúc khởi động
        ofstream outFile(filePath);
        outFile.close();
        return result;
    }

    string line;
    while (getline(inFile, line)){
        if (line.empty()) continue;
        size_t firstChar = line.find_first_not_of(" \t\r\n");
        // Bỏ qua dòng trống hoặc dòng kẻ ngang phân cách bảng (bắt đầu bằng '-' hoặc '+')
        if (firstChar == string::npos || line[firstChar] == '-' || line[firstChar] == '+') continue;

        stringstream ss(line);
        string id, name, desc;

        // Dùng ký tự '|' làm dấu phân cách cột thay vì dấu cách vì tên và mô tả dịch vụ có chứa khoảng trắng
        if (getline(ss, id, '|') &&getline(ss, name, '|') &&getline(ss, desc, '|')){
            id = Utils::trim(id);
            name = Utils::trim(name);
            desc = Utils::trim(desc);

            // Bỏ qua dòng tiêu đề cột của bảng
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
            int baseFee = 0;
            string createdDate = "01/01/2026";
            bool isActive = true;

            if (remainingCols.size() >= 5){
                // Định dạng đầy đủ: phòng ban | giờ SLA | phí cơ bản | ngày ban hành | trạng thái
                department = remainingCols[0];
                try { slaHours = stoi(remainingCols[1]); } catch (...) { slaHours = 24; }
                try { baseFee = static_cast<int>(stod(remainingCols[2])); } catch (...) { baseFee = 0; }
                createdDate = remainingCols[3];
                if (createdDate.length() == 10 && createdDate[4] == '-' && createdDate[7] == '-')
                {
                    createdDate = createdDate.substr(8, 2) + "/" + createdDate.substr(5, 2) + "/" + createdDate.substr(0, 4);
                }
                isActive = (remainingCols[4] != "0");
            }
            else if (!remainingCols.empty()){
                // Định dạng rút gọn: chỉ có cột trạng thái hoạt động
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

//Định dạng hàm save cho ServiceGroup: dùng setw căn lề trái để file văn bản thẳng hàng như bảng
void FileManager<ServiceGroup>::save(const string& filePath, const vector<ServiceGroup>& data){

    ofstream outFile(filePath);
    if (!outFile.is_open()){
        throw runtime_error("Khong the mo file de ghi: " + filePath);
    }

    // Ghi dòng tiêu đề cột và dòng gạch ngang phân cách bảng
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
                << setw(12) << sg.getBaseFee()     << " | "
                << setw(14) << sg.getCreatedDate() << " | "
                << (sg.isActive() ? 1 : 0)         << "\n";
    }

    // Đẩy toàn bộ bộ đệm xuống đĩa ngay lập tức để tránh mất dữ liệu
    outFile.flush();
    if (!outFile){
        throw runtime_error("Khong the ghi du lieu vao file: " + filePath);
    }
}
