#include "FileManager.h"
#include <iostream>
#include <fstream>
#include <sstream>
#include <filesystem>
#include <iomanip>
#include <stdexcept>
#include "Utilities.h"

using namespace std;

/**
Nạp danh sách Ticket từ file văn bản có kẻ hàng cột
Định dạng ID yêu cầu: TK001, TK002,...
Đầu vào: Đường dẫn file (data/tickets.txt)
Trả về danh sách vector các đối tượng Ticket đã đọc được
 */
vector<Ticket> FileManager<Ticket>::load(const string& filePath)
{
    vector<Ticket> result;
    ifstream inFile(filePath);
    if (!inFile.is_open()){
        // Tự tạo file rỗng nếu chưa tồn tại
        filesystem::path p(filePath);
        if (p.has_parent_path()){
            filesystem::create_directories(p.parent_path());
        }
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
        string id, title, desc, customerId, sgId;

        // Đọc các trường cơ bản ngăn cách bằng '|'
        if (getline(ss, id, '|') &&
            getline(ss, title, '|') &&
            getline(ss, desc, '|') &&
            getline(ss, customerId, '|') &&
            getline(ss, sgId, '|')){
            id = Utils::trim(id);
            title = Utils::trim(title);
            desc = Utils::trim(desc);
            customerId = Utils::trim(customerId);
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

            // Đọc các cột tiếp theo
            string nextCol;
            vector<string> remainingCols;
            while (getline(ss, nextCol, '|')){
                remainingCols.push_back(Utils::trim(nextCol));
            }

            int priority = 2;
            double estimatedFee = 0.0;
            string createdDate = "01/01/2026";
            int statusCode = 0;

            if (remainingCols.size() >= 4){
                // Định dạng mới: priority | estimatedFee | createdDate | status
                try { priority = stoi(remainingCols[0]); } catch (...) { priority = 2; }
                try { estimatedFee = stod(remainingCols[1]); } catch (...) { estimatedFee = 0.0; }
                createdDate = remainingCols[2];
                if (createdDate.length() == 10 && createdDate[4] == '-' && createdDate[7] == '-')
                {
                    createdDate = createdDate.substr(8, 2) + "/" + createdDate.substr(5, 2) + "/" + createdDate.substr(0, 4);
                }
                try { statusCode = stoi(remainingCols[3]); } catch (...) { statusCode = 0; }
            }
            else if (!remainingCols.empty()){
                // Định dạng cũ: chỉ có status
                try { statusCode = stoi(remainingCols[0]); } catch (...) { statusCode = 0; }
            }
            else{
                cout << "[Canh bao] Dong sai dinh dang: \"" << line << "\". Bo qua dong nay." << endl;
                continue;
            }

            try{
                Ticket t(title, desc, customerId, sgId, priority, estimatedFee, createdDate);
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

/**
Ghi toàn bộ danh sách Ticket ra file văn bản kẻ hàng cột ngay ngắn
Đầu vào: filePath là  Đường dẫn file (data/tickets.txt) và data là  Dữ liệu cần ghi
 */
void FileManager<Ticket>::save(const string& filePath, const vector<Ticket>& data){
    // Đảm bảo thư mục cha tồn tại trước khi tạo file
    filesystem::path p(filePath);
    if (p.has_parent_path()){
        filesystem::create_directories(p.parent_path());
    }

    ofstream outFile(filePath);
    if (!outFile.is_open()){
        throw runtime_error("Khong the mo file de ghi: " + filePath);
    }

    // Kẻ bảng: Dòng tiêu đề cột và dòng gạch ngang phân cách
    outFile << left
            << setw(8)  << "Ma ID"          << " | "
            << setw(24) << "Tieu de"        << " | "
            << setw(35) << "Mo ta chi tiet" << " | "
            << setw(10) << "Ma KH"          << " | "
            << setw(10) << "Ma DV"          << " | "
            << setw(10) << "Uu tien"       << " | "
            << setw(12) << "Chi phi"        << " | "
            << setw(12) << "Ngay tao"       << " | "
            << "Trang thai" << "\n";
    outFile << string(140, '-') << "\n";

    for (const auto& t : data){
        outFile << left
                << setw(8)  << t.getId()             << " | "
                << setw(24) << t.getTitle()          << " | "
                << setw(35) << t.getDescription()    << " | "
                << setw(10) << t.getCustomerId()     << " | "
                << setw(10) << t.getServiceGroupId() << " | "
                << setw(10) << t.getPriority()       << " | "
                << setw(12) << fixed << setprecision(2) << t.getEstimatedFee() << " | "
                << setw(12) << t.getCreatedDate()    << " | "
                << static_cast<int>(t.getStatus())   << "\n";
    }

    outFile.flush();
    if (!outFile){
        throw runtime_error("Khong the ghi du lieu vao file: " + filePath);
    }
}
