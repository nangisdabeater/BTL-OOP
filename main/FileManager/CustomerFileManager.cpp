#include "FileManager.h"
#include "Utilities.h"
#include <iostream>
#include <fstream>
#include <sstream>
#include <iomanip>
#include <stdexcept>

using namespace std;

static vector<string> fields(const string& line) {
    vector<string> v;
    string x;
    stringstream s(line);
    while (getline(s, x, '|')) {
        v.push_back(Utils::trim(x));
    }
    return v;
}

/**
 * @brief Đọc danh sách Khách hàng từ file văn bản có định dạng bảng
 * @param path Đường dẫn file dữ liệu (mặc định data/customers.txt)
 * @return std::vector<Customer> Danh sách các đối tượng Customer nạp được
 */
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
        // Kiểm tra số lượng cột dữ liệu (Ma ID, Ho ten, So dien thoai, Email, Dia chi)
        if (v.size() != 5) {
            cout << "[Canh bao] Dong sai dinh dang: \"" << l << "\". Bo qua dong nay." << endl;
            continue;
        }

        // Bỏ qua dòng tiêu đề bảng
        if (v[0] == "Ma ID") continue;

        // Bỏ qua dòng nếu mã ID không đúng quy chuẩn (tiền tố 'KH')
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

/**
 * @brief Ghi danh sách Khách hàng ra file văn bản theo định dạng bảng căn lề chuẩn
 * @param path Đường dẫn file dữ liệu (mặc định data/customers.txt)
 * @param data Danh sách đối tượng Customer cần lưu
 */
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
