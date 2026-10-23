#include "FileManager.h"
#include <iostream>
#include <fstream>
#include <sstream>
#include <iomanip>
#include <stdexcept>
#include "Utilities.h"

// Chỉ dùng using namespace std ở file .cpp để không làm bẩn không gian tên ở file .h
using namespace std;

// Chuyên biệt hóa hàm load cho SLALevel: nạp danh sách Mức SLA từ file văn bản
vector<SLALevel> FileManager<SLALevel>::load(const string& filePath) {
    vector<SLALevel> result;
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
        // Bỏ qua dòng trống hoặc dòng kẻ ngang phân cách bảng
        if (firstChar == string::npos || line[firstChar] == '-' || line[firstChar] == '+') continue;

        stringstream ss(line);
        string id, name, respStr, resStr, prioStr;

        // Tách từng trường dữ liệu ngăn cách bởi dấu '|'
        if (getline(ss, id, '|') &&
            getline(ss, name, '|') &&
            getline(ss, respStr, '|') &&
            getline(ss, resStr, '|') &&
            getline(ss, prioStr, '|')) {
            id = Utils::trim(id);
            name = Utils::trim(name);
            respStr = Utils::trim(respStr);
            resStr = Utils::trim(resStr);
            prioStr = Utils::trim(prioStr);

            // Bỏ qua dòng tiêu đề bảng
            if (id == "Ma ID") {
                continue;
            }

            // Kiểm tra định dạng mã SLA phải có tiền tố "SL" (SL001, SL002,...)
            if (Utils::extractIdNumber(id, "SL") == -1) {
                cout << "[Canh bao] Dong sai dinh dang: \"" << line << "\". Bo qua dong nay." << endl;
                continue;
            }

            try {
                int resp = stoi(respStr);
                int res = stoi(resStr);
                int prio = stoi(prioStr);

                SLALevel sla(name, resp, res, prio);
                sla.setId(id);
                result.push_back(sla);
            }
            catch (...) {
                cout << "[Canh bao] Dong sai dinh dang: \"" << line << "\". Bo qua dong nay." << endl;
            }
        }
        else {
            cout << "[Canh bao] Dong sai dinh dang: \"" << line << "\". Bo qua dong nay." << endl;
        }
    }
    return result;
}

// Chuyên biệt hóa hàm save cho SLALevel: ghi toàn bộ danh sách Mức SLA ra file văn bản kẻ cột
void FileManager<SLALevel>::save(const string& filePath, const vector<SLALevel>& data) {
    ofstream outFile(filePath);
    if (!outFile.is_open()) {
        throw runtime_error("Khong the mo file de ghi: " + filePath);
    }

    outFile << left
            << setw(8)  << "Ma ID"          << " | "
            << setw(20) << "Ten muc SLA"    << " | "
            << setw(15) << "Phan hoi(phut)" << " | "
            << setw(12) << "Xu ly(gio)"     << " | "
            << "Uu tien"                    << "\n";
    outFile << string(75, '-') << "\n";

    for (const auto& s : data) {
        outFile << left
                << setw(8)  << s.getId()              << " | "
                << setw(20) << s.getName()            << " | "
                << setw(15) << s.getResponseMinutes() << " | "
                << setw(12) << s.getResolveHours()    << " | "
                << s.getPriority()                    << "\n";
    }

    outFile.flush();
    if (!outFile) {
        throw runtime_error("Khong the ghi du lieu vao file: " + filePath);
    }
}
