#include "FileManager.h"
#include <iostream>
#include <fstream>
#include <sstream>
#include <iomanip>
#include <stdexcept>
#include "Utilities.h"

// Chỉ dùng using namespace std ở file .cpp để không làm bẩn không gian tên ở file .h
using namespace std;

// Chuyên biệt hóa hàm load cho AgentShift: đọc danh sách Ca trực từ file văn bản
vector<AgentShift> FileManager<AgentShift>::load(const string& filePath) {
    vector<AgentShift> result;
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
        string id, shiftName, shiftDate, shiftStart, shiftEnd, agentStrs, status;

        // Tách các cột ngăn cách bởi ký tự '|'
        if (getline(ss, id, '|') &&
            getline(ss, shiftName, '|') &&
            getline(ss, shiftDate, '|') &&
            getline(ss, shiftStart, '|') && 
            getline(ss, shiftEnd, '|') &&
            getline(ss, agentStrs, '|') &&
            getline(ss, status, '|')) {
            id = Utils::trim(id);
            shiftName = Utils::trim(shiftName);
            shiftDate = Utils::trim(shiftDate);
            shiftStart = Utils::trim(shiftStart);
            shiftEnd = Utils::trim(shiftEnd);
            agentStrs = Utils::trim(agentStrs);
            status = Utils::trim(status);

            // Bỏ qua dòng tiêu đề cột
            if (id == "Ma ID") {
                continue;
            }

            // Kiểm tra định dạng mã ca trực phải có tiền tố "AS"
            if (Utils::extractIdNumber(id, "AS") == -1) {
                cout << "[Canh bao] Dong sai dinh dang: \"" << line << "\". Bo qua dong nay." << endl;
                continue;
            }

            try {
                AgentShift as(shiftName, shiftDate, shiftStart, shiftEnd);
                as.setId(id);
                if (status == "0") {
                    as.deactivate();
                } else {
                    as.activate();
                }

                // Tách danh sách mã nhân viên ngăn cách bởi dấu phẩy ',' trong cột Danh sách NV
                vector<string> parsedIds;
                if (!agentStrs.empty()) {
                    stringstream idStream(agentStrs);
                    string singleId;
                    while (getline(idStream, singleId, ',')) {
                        string trimmedId = Utils::trim(singleId);
                        if (!trimmedId.empty()) {
                            parsedIds.push_back(trimmedId);
                        }
                    }
                }

                if (!parsedIds.empty()) {
                    as.setAgentIds(parsedIds);
                }
                result.push_back(as);
            } catch (...) {
                cout << "[Canh bao] Dong sai dinh dang: \"" << line << "\". Bo qua dong nay." << endl;
            }
        } else {
            cout << "[Canh bao] Dong sai dinh dang: \"" << line << "\". Bo qua dong nay." << endl;
        }
    }

    return result;
}

// Chuyên biệt hóa hàm save cho AgentShift: ghi toàn bộ danh sách Ca trực ra file văn bản
void FileManager<AgentShift>::save(const string& filePath, const vector<AgentShift>& data) {
    ofstream outFile(filePath);
    if (!outFile.is_open()) {
        throw runtime_error("Khong the mo file de ghi: " + filePath);
    }

    outFile << left
            << setw(8)  << "Ma ID"        << " | "
            << setw(25) << "Ten ca truc"  << " | "
            << setw(25) << "Ngay truc"    << " | "
            << setw(25) << "Bat dau"      << " | " 
            << setw(25) << "Ket thuc"     << " | "
            << setw(30) << "Danh sach NV" << "|"
            << "Trang thai"                << "\n";
    outFile << string(145, '-') << "\n";

    for (const auto& as: data) {
        // Nối mảng mã nhân viên thành chuỗi ngăn cách bằng dấu phẩy để lưu gọn vào 1 cột
        string agentsStr = Utils::join(as.getAgentIds(), ", ");
        outFile << left
                << setw(8)  << as.getId()         << " | "
                << setw(25) << as.getShiftName()  << " | "
                << setw(25) << as.getShiftDate()  << " | "
                << setw(25) << as.getShiftStart() << " | " 
                << setw(25) << as.getShiftEnd()   << " | "
                << setw(30) << agentsStr          << "|"
                << (as.isActive() ? 1 : 0)        << "\n";
    }
    outFile.flush();
    if (!outFile) {
        throw runtime_error("Khong the ghi du lieu vao file: " + filePath);
    }
}
