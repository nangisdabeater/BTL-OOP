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

// Chuyên biệt hóa hàm load cho Customer: đọc danh sách Khách hàng từ file văn bản
vector<Customer> FileManager<Customer>::load(const string& path) {
    vector<Customer> out;
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
        // Kiểm tra đủ 5 cột dữ liệu (Mã ID, Họ tên, Số điện thoại, Email, Địa chỉ)
        if (v.size() != 5) {
            cout << "[Canh bao] Dong sai dinh dang: \"" << l << "\". Bo qua dong nay." << endl;
            continue;
        }

        // Bỏ qua dòng tiêu đề bảng
        if (v[0] == "Ma ID") continue;

        // Bỏ qua dòng nếu mã ID không đúng quy chuẩn tiền tố "KH"
        if (Utils::extractIdNumber(v[0], "KH") == -1) {
            cout << "[Canh bao] Dong sai dinh dang: \"" << l << "\". Bo qua dong nay." << endl;
            continue;
        }

        try {
            Customer x(v[1], v[2], v[3], v[4]);
            x.setId(v[0]);
            out.push_back(x);
        } catch (...) {
            cout << "[Canh bao] Dong sai dinh dang: \"" << l << "\". Bo qua dong nay." << endl;
        }
    }
    return out;
}

// Chuyên biệt hóa hàm save cho Customer: ghi danh sách Khách hàng ra file văn bản kẻ cột
void FileManager<Customer>::save(const string& path, const vector<Customer>& data) {
    ofstream f(path);
    if (!f) throw runtime_error("Khong the mo file de ghi: " + path);

    // Ghi tiêu đề bảng và đường kẻ ngang
    f << left
      << setw(8)  << "Ma ID"         << " | "
      << setw(24) << "Ho va ten"     << " | "
      << setw(16) << "So dien thoai" << " | "
      << setw(28) << "Email"         << " | "
      << "Dia chi" << "\n";
    f << string(101, '-') << "\n";

    // Ghi từng bản ghi khách hàng
    for (const auto& x : data) {
        f << left
          << setw(8)  << x.getId()       << " | "
          << setw(24) << x.getFullName() << " | "
          << setw(16) << x.getPhone()    << " | "
          << setw(28) << x.getEmail()    << " | "
          << x.getAddress() << "\n";
    }

    f.flush();
    if (!f) throw runtime_error("Khong the ghi du lieu vao file: " + path);
}
