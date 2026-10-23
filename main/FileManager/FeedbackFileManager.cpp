#include "FileManager.h"
#include "Utilities.h"
#include <iostream>
#include <fstream>
#include <sstream>
#include <iomanip>
#include <stdexcept>

// Chỉ dùng using namespace std ở file .cpp để không làm bẩn không gian tên ở file .h
using namespace std;

// Hàm nội bộ tách một dòng văn bản thành mảng các cột dựa theo ký tự phân cách '|'
static vector<string> fields(const string& line) {
    vector<string> v;
    string x;
    stringstream s(line);
    while (getline(s, x, '|')) {
        v.push_back(Utils::trim(x));
    }
    return v;
}

// Chuyên biệt hóa hàm load cho Feedback: đọc danh sách Phiếu đánh giá hài lòng từ file văn bản
vector<Feedback> FileManager<Feedback>::load(const string& path) {
    vector<Feedback> out;
    ifstream f(path);
    if (!f.is_open()) {
        ofstream outFile(path);
        outFile.close();
        return out;
    }

    string l;
    while (getline(f, l)) {
        if (l.empty()) continue;

        // Bỏ qua dòng kẻ phân cách bảng bắt đầu bằng '-' hoặc '+'
        size_t firstChar = l.find_first_not_of(" \t\r\n");
        if (firstChar == string::npos || l[firstChar] == '-' || l[firstChar] == '+') continue;

        auto v = fields(l);
        // Kiểm tra số lượng cột dữ liệu (Mã ID, Mã KH, Điểm, Ngày ban hành [, Nội dung đánh giá])
        if (v.size() != 5 && v.size() != 4) {
            cout << "[Canh bao] Dong sai dinh dang: \"" << l << "\". Bo qua dong nay." << endl;
            continue;
        }

        // Bỏ qua dòng tiêu đề bảng
        if (v[0] == "Ma ID") continue;

        // Bỏ qua dòng nếu mã ID không đúng quy chuẩn tiền tố "DG"
        if (Utils::extractIdNumber(v[0], "DG") == -1) {
            cout << "[Canh bao] Dong sai dinh dang: \"" << l << "\". Bo qua dong nay." << endl;
            continue;
        }

        try {
            int score = stoi(v[2]);
            string comment = (v.size() == 5) ? v[4] : "";
            Feedback x(v[1], score, comment, v[3]);
            x.setId(v[0]);
            out.push_back(x);
        } catch (...) {
            cout << "[Canh bao] Dong sai dinh dang: \"" << l << "\". Bo qua dong nay." << endl;
        }
    }
    return out;
}

// Chuyên biệt hóa hàm save cho Feedback: ghi danh sách Phiếu đánh giá hài lòng ra file văn bản
void FileManager<Feedback>::save(const string& path, const vector<Feedback>& data) {
    ofstream f(path);
    if (!f) throw runtime_error("Khong the mo file de ghi: " + path);

    // Ghi tiêu đề bảng và đường kẻ ngang
    f << left
      << setw(8)  << "Ma ID"         << " | "
      << setw(10) << "Ma KH"         << " | "
      << setw(6)  << "Diem"          << " | "
      << setw(14) << "Ngay ban hanh" << " | "
      << "Noi dung danh gia" << "\n";
    f << string(86, '-') << "\n";

    // Ghi từng bản ghi đánh giá hài lòng
    for (const auto& x : data) {
        f << left
          << setw(8)  << x.getId()         << " | "
          << setw(10) << x.getCustomerId() << " | "
          << setw(6)  << x.getScore()      << " | "
          << setw(14) << x.getDate()       << " | "
          << x.getComment() << "\n";
    }

    f.flush();
    if (!f) throw runtime_error("Khong the ghi du lieu vao file: " + path);
}
