#include "FileManager.h"
#include <iostream>
#include <fstream>
#include <sstream>
#include <iomanip>
#include <stdexcept>
#include "Utilities.h"

// Chỉ dùng using namespace std ở file .cpp để không làm bẩn không gian tên ở file .h
using namespace std;

// Chuyên biệt hóa hàm load cho ReceptionChannel: nạp danh sách Kênh tiếp nhận từ file văn bản
vector<ReceptionChannel> FileManager<ReceptionChannel>::load(const string& filePath) {
    vector<ReceptionChannel> result;
    ifstream inFile(filePath);
    if (!inFile.is_open()) {
        ofstream outFile(filePath);
        outFile.close();
        return result;
    }

    string line;
    while (getline(inFile, line)) {
        if (line.empty()) continue;
        size_t firstChar = line.find_first_not_of(" \t\r\n");
        // Bỏ qua dòng trống hoặc dòng gạch ngang phân cách bảng
        if (firstChar == string::npos || line[firstChar] == '-' || line[firstChar] == '+') continue;

        stringstream ss(line);
        string id, name, desc, activeStr;

        // Tách từng trường dữ liệu ngăn cách bởi dấu '|'
        if (getline(ss, id, '|') &&
            getline(ss, name, '|') &&
            getline(ss, desc, '|') &&
            getline(ss, activeStr, '|')) {
            id = Utils::trim(id);
            name = Utils::trim(name);
            desc = Utils::trim(desc);
            activeStr = Utils::trim(activeStr);

            // Bỏ qua dòng tiêu đề bảng
            if (id == "Ma ID") {
                continue;
            }

            // Kiểm tra định dạng tiền tố CN (CN001, CN002,...)
            if (Utils::extractIdNumber(id, "CN") == -1) {
                cout << "[Canh bao] Dong sai dinh dang: \"" << line << "\". Bo qua dong nay." << endl;
                continue;
            }

            try {
                ReceptionChannel ch(name, desc);
                ch.setId(id);
                if (activeStr == "0") {
                    ch.deactivate();
                } else {
                    ch.activate();
                }

                result.push_back(ch);
            } catch (...) {
                cout << "[Canh bao] Dong sai dinh dang: \"" << line << "\". Bo qua dong nay." << endl;
            }
        } else {
            cout << "[Canh bao] Dong sai dinh dang: \"" << line << "\". Bo qua dong nay." << endl;
        }
    }
    return result;
}

// Chuyên biệt hóa hàm save cho ReceptionChannel: ghi danh sách Kênh tiếp nhận ra file văn bản
void FileManager<ReceptionChannel>::save(const string& filePath, const vector<ReceptionChannel>& data) {
    ofstream outFile(filePath);
    if (!outFile.is_open()) {
        throw runtime_error("Khong the mo file de ghi kenh tiep nhan: " + filePath);
    }

    outFile << left
            << setw(8)  << "Ma ID"              << " | "
            << setw(25) << "Ten kenh tiep nhan" << " | "
            << setw(40) << "Mo ta"              << " | "
            << "Trang thai"                     << "\n";
    outFile << string(90, '-') << "\n";

    for (const auto& ch : data) {
        outFile << left
                << setw(8)  << ch.getId()          << " | "
                << setw(25) << ch.getName()        << " | "
                << setw(40) << ch.getDescription() << " | "
                << (ch.isActive() ? 1 : 0)         << "\n";
    }

    outFile.flush();
    if (!outFile) {
        throw runtime_error("Loi xay ra khi ghi file kenh tiep nhan: " + filePath);
    }
}
