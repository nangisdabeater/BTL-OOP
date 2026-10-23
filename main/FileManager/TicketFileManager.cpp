#include "FileManager.h"
#include <iostream>
#include <fstream>
#include <sstream>
#include <iomanip>
#include <stdexcept>
#include "Utilities.h"

// Chỉ dùng using namespace std ở file .cpp để không làm bẩn không gian tên ở file .h
using namespace std;

// Chuyên biệt hóa hàm load cho Ticket: đọc dữ liệu phiếu yêu cầu từ file văn bản có kẻ bảng
vector<Ticket> FileManager<Ticket>::load(const string& filePath){
    vector<Ticket> result;
    ifstream inFile(filePath);
    if (!inFile.is_open()){
        // Tự động tạo file rỗng nếu chưa tồn tại
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
        string id, title, desc, sgId;

        // Tách các trường bằng dấu '|' thay vì toán tử >> vì nội dung văn bản có chứa khoảng trắng
        if (getline(ss, id, '|') &&
            getline(ss, title, '|') &&
            getline(ss, desc, '|') &&
            getline(ss, sgId, '|')){
            id = Utils::trim(id);
            title = Utils::trim(title);
            desc = Utils::trim(desc);
            sgId = Utils::trim(sgId);

            // Bỏ qua dòng tiêu đề cột
            if (id == "Ma ID"){
                continue;
            }

            // Kiểm tra định dạng ID với tiền tố "TK" (TKxxx)
            if (Utils::extractIdNumber(id, "TK") == -1){
                cout << "[Canh bao] Dong sai dinh dang: \"" << line << "\". Bo qua dong nay." << endl;
                continue;
            }

            // Đọc các cột còn lại trên dòng
            string nextCol;
            vector<string> remainingCols;
            while (getline(ss, nextCol, '|')){
                remainingCols.push_back(Utils::trim(nextCol));
            }

            int priority = 2;
            int estimatedFee = 0;
            string createdDate = "01/01/2026";
            int statusCode = 0;

            if (remainingCols.size() >= 4){
                // Định dạng đầy đủ: mức ưu tiên | chi phí dự kiến | ngày tạo | mã trạng thái
                try { priority = stoi(remainingCols[0]); } catch (...) { priority = 2; }
                try { estimatedFee = static_cast<int>(stod(remainingCols[1])); } catch (...) { estimatedFee = 0; }
                createdDate = remainingCols[2];
                if (createdDate.length() == 10 && createdDate[4] == '-' && createdDate[7] == '-')
                {
                    createdDate = createdDate.substr(8, 2) + "/" + createdDate.substr(5, 2) + "/" + createdDate.substr(0, 4);
                }
                try { statusCode = stoi(remainingCols[3]); } catch (...) { statusCode = 0; }
            }
            else if (!remainingCols.empty()){
                try { statusCode = stoi(remainingCols[0]); } catch (...) { statusCode = 0; }
            }
            else{
                cout << "[Canh bao] Dong sai dinh dang: \"" << line << "\". Bo qua dong nay." << endl;
                continue;
            }

            try{
                Ticket t(title, desc, sgId, priority, estimatedFee, createdDate);
                t.setId(id);
                if (statusCode >= 0 && statusCode <= 3)
                {
                    t.changeStatus(static_cast<TicketStatus>(statusCode));
                }
                else
                {
                    t.changeStatus(TicketStatus::OPEN);
                }
                result.push_back(t);
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

//Định dạng  hàm save cho Ticket: ghi danh sách phiếu yêu cầu ra file văn bản kẻ bảng
void FileManager<Ticket>::save(const string& filePath, const vector<Ticket>& data){

    ofstream outFile(filePath);
    if (!outFile.is_open()){
        throw runtime_error("Khong the mo file de ghi: " + filePath);
    }

    // Ghi dòng tiêu đề cột và dòng gạch ngang phân cách
    outFile << left
            << setw(8)  << "Ma ID"          << " | "
            << setw(24) << "Tieu de"        << " | "
            << setw(36) << "Mo ta chi tiet" << " | "
            << setw(10) << "Ma DV"          << " | "
            << setw(10) << "Uu tien"       << " | "
            << setw(12) << "Chi phi"        << " | "
            << setw(12) << "Ngay tao"       << " | "
            << "Trang thai" << "\n";
    outFile << string(129, '-') << "\n";

    for (const auto& t : data){
        outFile << left
                << setw(8)  << t.getId()             << " | "
                << setw(24) << t.getTitle()          << " | "
                << setw(36) << t.getDescription()    << " | "
                << setw(10) << t.getServiceGroupId() << " | "
                << setw(10) << t.getPriority()       << " | "
                << setw(12) << t.getEstimatedFee()   << " | "
                << setw(12) << t.getCreatedDate()    << " | "
                // Ép kiểu enum class sang số nguyên khi lưu file để gọn và dễ đọc lại
                << static_cast<int>(t.getStatus())   << "\n";
    }

    outFile.flush();
    if (!outFile){
        throw runtime_error("Khong the ghi du lieu vao file: " + filePath);
    }
}
