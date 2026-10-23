#include "FileManager.h"
#include <iostream>
#include <fstream>
#include <sstream>
#include <iomanip>
#include <stdexcept>
#include "Utilities.h"

// Chỉ dùng using namespace std ở file .cpp để không làm bẩn không gian tên ở file .h
using namespace std;

// Chuyên biệt hóa hàm load cho TransferTicket: nạp danh sách Phiếu chuyển xử lý từ file văn bản
vector<TransferTicket> FileManager<TransferTicket>::load(const string& filePath) {
    vector<TransferTicket> result;
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
        string id, slaId, fromDept, toDept, reason, statusStr;

        // Tách từng trường dữ liệu ngăn cách bởi dấu '|'
        if (getline(ss, id, '|') &&
            getline(ss, slaId, '|') &&
            getline(ss, fromDept, '|') &&
            getline(ss, toDept, '|') &&
            getline(ss, reason, '|') &&
            getline(ss, statusStr, '|')) {
            id = Utils::trim(id);
            slaId = Utils::trim(slaId);
            fromDept = Utils::trim(fromDept);
            toDept = Utils::trim(toDept);
            reason = Utils::trim(reason);
            statusStr = Utils::trim(statusStr);

            // Bỏ qua dòng tiêu đề bảng
            if (id == "Ma ID") {
                continue;
            }

            // Kiểm tra định dạng mã phiếu chuyển phải có tiền tố "TF" (TF001, TF002,...)
            if (Utils::extractIdNumber(id, "TF") == -1) {
                cout << "[Canh bao] Dong sai dinh dang: \"" << line << "\". Bo qua dong nay." << endl;
                continue;
            }

            try {
                TransferTicket ticket(slaId, fromDept, toDept, reason);
                ticket.setId(id);
                try {
                    int st = stoi(statusStr);
                    if (st >= 0 && st <= 2) {
                        // Dùng restoreStatus để gán thẳng trạng thái khi nạp từ file mà không bị chặn bởi kiểm tra chuyển trạng thái
                        ticket.restoreStatus(static_cast<TransferStatus>(st));
                    }
                }
                catch (...) {}

                result.push_back(ticket);
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

// Chuyên biệt hóa hàm save cho TransferTicket: ghi toàn bộ danh sách Phiếu chuyển xử lý ra file văn bản
void FileManager<TransferTicket>::save(const string& filePath, const vector<TransferTicket>& data) {
    ofstream outFile(filePath);
    if (!outFile.is_open()) {
        throw runtime_error("Khong the mo file de ghi: " + filePath);
    }

    outFile << left
            << setw(8)  << "Ma ID"          << " | "
            << setw(10) << "Ma SLA"         << " | "
            << setw(22) << "Bo phan chuyen" << " | "
            << setw(22) << "Bo phan nhan"   << " | "
            << setw(36) << "Ly do"          << " | "
            << "Trang thai"                 << "\n";
    outFile << string(117, '-') << "\n";

    for (const auto& t : data) {
        outFile << left
                << setw(8)  << t.getId()                   << " | "
                << setw(10) << t.getSlaId()                << " | "
                << setw(22) << t.getFromDept()             << " | "
                << setw(22) << t.getToDept()               << " | "
                << setw(36) << t.getReason()               << " | "
                // Ép kiểu enum class sang số nguyên khi ghi file
                << static_cast<int>(t.getStatus())         << "\n";
    }

    outFile.flush();
    if (!outFile) {
        throw runtime_error("Khong the ghi du lieu vao file: " + filePath);
    }
}
