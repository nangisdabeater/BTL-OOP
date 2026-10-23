#include "FileManager.h"
#include <iostream>
#include <fstream>
#include <sstream>
#include <iomanip>
#include <stdexcept>
#include "Utilities.h"

// Chỉ dùng using namespace std ở file .cpp để không làm bẩn không gian tên ở file .h
using namespace std;

// Chuyên biệt hóa hàm load cho Agent: đọc danh sách Điện thoại viên từ file văn bản
vector<Agent> FileManager<Agent>::load(const string& filePath) {
    vector<Agent> result;
    ifstream inFile(filePath);
    if (!inFile.is_open()) {
        // Tự tạo file rỗng nếu chưa tồn tại
        ofstream outFile(filePath);
        outFile.close();
        return result;
    }

    string line;
    while (getline(inFile, line)) {
        if (line.empty()) continue;
        size_t firstChar = line.find_first_not_of(" \t\r\n");
        // Bỏ qua dòng trống hoặc dòng kẻ ngang phân cách bảng
        if (firstChar == string::npos || line[firstChar] == '-' || line[firstChar] == '+') continue;

        stringstream ss(line);
        string id, fullName, phoneNumber, email, extensionPhone, status;

        // Tách từng trường ngăn cách bởi dấu '|' và cắt bỏ khoảng trắng thừa ở hai đầu
        if (getline(ss, id, '|') &&
            getline(ss, fullName, '|') &&
            getline(ss, phoneNumber, '|') &&
            getline(ss, email, '|') && 
            getline(ss, extensionPhone, '|') &&
            getline(ss, status, '|')) {
            id = Utils::trim(id);
            fullName = Utils::trim(fullName);
            phoneNumber = Utils::trim(phoneNumber);
            email = Utils::trim(email);
            extensionPhone = Utils::trim(extensionPhone);
            status = Utils::trim(status);

            // Bỏ qua dòng tiêu đề cột
            if (id == "Ma ID") {
                continue;
            }

            // Kiểm tra mã định danh có đúng tiền tố "AG" hay không
            if (Utils::extractIdNumber(id, "AG") == -1) {
                cout << "[Canh bao] Dong sai dinh dang: \"" << line << "\". Bo qua dong nay." << endl;
                continue;
            }

            try {
                Agent ag(fullName, phoneNumber, email, extensionPhone);
                ag.setId(id);
                if (status == "0") {
                    ag.deactivate();
                } else {
                    ag.activate();
                }

                result.push_back(ag);
            } catch (...) {
                cout << "[Canh bao] Dong sai dinh dang: \"" << line << "\". Bo qua dong nay." << endl;
            }
        } else {
            cout << "[Canh bao] Dong sai dinh dang: \"" << line << "\". Bo qua dong nay." << endl;
        }
    }

    return result;
}

// Chuyên biệt hóa hàm save cho Agent: ghi toàn bộ danh sách Điện thoại viên ra file văn bản
void FileManager<Agent>::save(const string& filePath, const vector<Agent>& data) {
    ofstream outFile(filePath);
    if (!outFile.is_open()) {
        throw runtime_error("Khong the mo file de ghi: " + filePath);
    }

    // Ghi tiêu đề bảng và đường kẻ ngang
    outFile << left
            << setw(8)  << "Ma ID"         << " | "
            << setw(25) << "Ho va ten"     << " | "
            << setw(25) << "So dien thoai" << " | "
            << setw(25) << "Email"         << " | " 
            << setw(25) << "So tong dai"   << " | "
            << "Trang thai"                 << "\n";
    outFile << string(95, '-') << "\n";

    for (const auto& ag: data) {
        outFile << left
                << setw(8)  << ag.getId()             << " | "
                << setw(25) << ag.getFullName()        << " | "
                << setw(25) << ag.getPhoneNumber()     << " | "
                << setw(25) << ag.getEmail()           << " | " 
                << setw(25) << ag.getExtensionPhone()  << " | "
                << (ag.isActive() ? 1 : 0)             << "\n";
    }
    outFile.flush();
    if (!outFile) {
        throw runtime_error("Khong the ghi du lieu vao file: " + filePath);
    }
}
