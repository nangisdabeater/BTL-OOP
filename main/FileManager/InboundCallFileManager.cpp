#include "FileManager.h"
#include <iostream>
#include <fstream>
#include <sstream>
#include <iomanip>
#include <stdexcept>
#include "Utilities.h"

// Chỉ dùng using namespace std ở file .cpp để không làm bẩn không gian tên ở file .h
using namespace std;

// Chuyên biệt hóa hàm load cho InboundCall: nạp danh sách Cuộc gọi tiếp nhận từ file văn bản
vector<InboundCall> FileManager<InboundCall>::load(const string& filePath) {
    vector<InboundCall> result;
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
        // Bỏ qua dòng trống hoặc dòng phân cách gạch ngang
        if (firstChar == string::npos || line[firstChar] == '-' || line[firstChar] == '+') continue;

        stringstream ss(line);
        string id, phone, chId, time, durStr, status, note;

        // Tách từng trường ngăn cách bởi dấu '|'
        if (getline(ss, id, '|') &&
            getline(ss, phone, '|') &&
            getline(ss, chId, '|') &&
            getline(ss, time, '|') &&
            getline(ss, durStr, '|') &&
            getline(ss, status, '|') &&
            getline(ss, note, '|')) {
            id = Utils::trim(id);
            phone = Utils::trim(phone);
            chId = Utils::trim(chId);
            time = Utils::trim(time);
            durStr = Utils::trim(durStr);
            status = Utils::trim(status);
            note = Utils::trim(note);

            // Bỏ qua dòng tiêu đề bảng
            if (id == "Ma ID") {
                continue;
            }

            // Kiểm tra tiền tố CA (CA001, CA002,...)
            if (Utils::extractIdNumber(id, "CA") == -1) {
                cout << "[Canh bao] Dong sai dinh dang: \"" << line << "\". Bo qua dong nay." << endl;
                continue;
            }

            int dur = 0;
            try {
                dur = stoi(durStr);
            } catch (...) {
                cout << "[Canh bao] Dong sai dinh dang: \"" << line << "\". Bo qua dong nay." << endl;
                continue;
            }

            try {
                InboundCall call(phone, chId, time, dur, status, note);
                call.setId(id);
                result.push_back(call);
            } catch (...) {
                cout << "[Canh bao] Dong sai dinh dang: \"" << line << "\". Bo qua dong nay." << endl;
            }
        } else {
            cout << "[Canh bao] Dong sai dinh dang: \"" << line << "\". Bo qua dong nay." << endl;
        }
    }
    return result;
}

// Chuyên biệt hóa hàm save cho InboundCall: ghi danh sách Cuộc gọi tiếp nhận ra file văn bản
void FileManager<InboundCall>::save(const string& filePath, const vector<InboundCall>& data) {
    ofstream outFile(filePath);
    if (!outFile.is_open()) {
        throw runtime_error("Khong the mo file de ghi cuoc goi tiep nhan: " + filePath);
    }

    outFile << left
            << setw(8)  << "Ma ID"         << " | "
            << setw(13) << "So dien thoai" << " | "
            << setw(9)  << "Ma kenh"       << " | "
            << setw(18) << "Thoi gian"     << " | "
            << setw(10) << "Thoi luong"    << " | "
            << setw(12) << "Trang thai"    << " | "
            << "Ghi chu"                   << "\n";
    outFile << string(100, '-') << "\n";

    for (const auto& call : data) {
        outFile << left
                << setw(8)  << call.getId()          << " | "
                << setw(13) << call.getPhoneNumber() << " | "
                << setw(9)  << call.getChannelId()   << " | "
                << setw(18) << call.getCallTime()    << " | "
                << setw(10) << call.getDuration()    << " | "
                << setw(12) << call.getStatus()      << " | "
                << call.getNote()                    << "\n";
    }

    outFile.flush();
    if (!outFile) {
        throw runtime_error("Loi xay ra khi ghi file cuoc goi tiep nhan: " + filePath);
    }
}
