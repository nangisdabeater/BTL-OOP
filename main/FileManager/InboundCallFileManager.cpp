/**
 * @file InboundCallFileManager.cpp
 * @brief Cài đặt đọc/ghi file cho Thực thể Cuộc gọi tiếp nhận (InboundCall)
 * [PHẦN THÊM MỚI]
 * Quản lý file: data/inbound_calls.txt
 * Định dạng ID yêu cầu: CA001, CA002,...
 */

#include "FileManager.h"
#include <iostream>
#include <fstream>
#include <sstream>
#include <iomanip>
#include <stdexcept>
#include "Utilities.h"

using namespace std;

/**
 * @brief Nạp danh sách Cuộc gọi tiếp nhận từ file data/inbound_calls.txt
 * @param filePath Đường dẫn file
 * @return Danh sách vector<InboundCall>
 */
vector<InboundCall> FileManager<InboundCall>::load(const string& filePath)
{
    vector<InboundCall> result;
    ifstream inFile(filePath);
    if (!inFile.is_open())
    {

        ofstream outFile(filePath);
        outFile.close();
        return result;
    }

    string line;
    while (getline(inFile, line))
    {
        if (line.empty()) continue;
        size_t firstChar = line.find_first_not_of(" \t\r\n");
        // Bỏ qua dòng trống hoặc dòng phân cách gạch ngang
        if (firstChar == string::npos || line[firstChar] == '-' || line[firstChar] == '+') continue;

        stringstream ss(line);
        string id, phone, chId, agId, time, durStr, status, note;

        // Đọc các trường dữ liệu ngăn cách bằng ký tự '|'
        if (getline(ss, id, '|') &&
            getline(ss, phone, '|') &&
            getline(ss, chId, '|') &&
            getline(ss, agId, '|') &&
            getline(ss, time, '|') &&
            getline(ss, durStr, '|') &&
            getline(ss, status, '|') &&
            getline(ss, note, '|'))
        {
            id = Utils::trim(id);
            phone = Utils::trim(phone);
            chId = Utils::trim(chId);
            agId = Utils::trim(agId);
            time = Utils::trim(time);
            durStr = Utils::trim(durStr);
            status = Utils::trim(status);
            note = Utils::trim(note);

            // Bỏ qua dòng tiêu đề bảng
            if (id == "Ma ID")
            {
                continue;
            }

            // KIỂM TRA ĐỊNH DẠNG ID: Yêu cầu định dạng tiền tố "CA" (CA001, CA002,...)
            if (Utils::extractIdNumber(id, "CA") == -1)
            {
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
                InboundCall call(phone, chId, agId, time, dur, status, note);
                call.setId(id);
                result.push_back(call);
            } catch (...) {
                cout << "[Canh bao] Dong sai dinh dang: \"" << line << "\". Bo qua dong nay." << endl;
            }
        }
        else
        {
            cout << "[Canh bao] Dong sai dinh dang: \"" << line << "\". Bo qua dong nay." << endl;
        }
    }
    return result;
}

/**
 * @brief Ghi danh sách Cuộc gọi tiếp nhận ra file data/inbound_calls.txt
 * @param filePath Đường dẫn file
 * @param data Dữ liệu cần ghi
 */
void FileManager<InboundCall>::save(const string& filePath, const vector<InboundCall>& data)
{


    ofstream outFile(filePath);
    if (!outFile.is_open())
    {
        throw runtime_error("Khong the mo file de ghi cuoc goi tiep nhan: " + filePath);
    }

    // Tiêu đề bảng
    outFile << left
            << setw(8)  << "Ma ID"         << " | "
            << setw(13) << "So dien thoai" << " | "
            << setw(9)  << "Ma kenh"       << " | "
            << setw(9)  << "Ma DTV"        << " | "
            << setw(18) << "Thoi gian"     << " | "
            << setw(10) << "Thoi luong"    << " | "
            << setw(12) << "Trang thai"    << " | "
            << "Ghi chu"                   << "\n";
    outFile << string(110, '-') << "\n";

    // Ghi từng cuộc gọi
    for (const auto& call : data)
    {
        outFile << left
                << setw(8)  << call.getId()          << " | "
                << setw(13) << call.getPhoneNumber() << " | "
                << setw(9)  << call.getChannelId()   << " | "
                << setw(9)  << call.getAgentId()     << " | "
                << setw(18) << call.getCallTime()    << " | "
                << setw(10) << call.getDuration()    << " | "
                << setw(12) << call.getStatus()      << " | "
                << call.getNote()                    << "\n";
    }

    outFile.flush();
    if (!outFile)
    {
        throw runtime_error("Loi xay ra khi ghi file cuoc goi tiep nhan: " + filePath);
    }
}
