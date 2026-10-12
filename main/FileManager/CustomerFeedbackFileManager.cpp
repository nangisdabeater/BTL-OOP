#include "FileManager.h"
#include "../Utilities/Utilities.h"
#include <fstream>
#include <sstream>
#include <filesystem>
#include <iomanip>
#include <stdexcept>

using namespace std;

/**
 * @brief Hàm phụ trợ tách một dòng văn bản thành các trường theo ký tự phân cách '|'
 * và tự động loại bỏ khoảng trắng thừa ở hai đầu mỗi trường.
 * @param line Chuỗi dòng văn bản cần tách
 * @return Vector chứa các chuỗi con đã được cắt tỉa khoảng trắng
 */
static vector<string> fields(const string& line) {
    vector<string> v;
    string x;
    stringstream s(line);
    while (getline(s, x, '|')) {
        v.push_back(Utils::trim(x));
    }
    return v;
}

// =========================================================================
//                   QUẢN LÝ DỮ LIỆU KHÁCH HÀNG (CUSTOMER)
// =========================================================================

/**
 * @brief Đọc danh sách Khách hàng từ file văn bản có định dạng bảng
 * @param path Đường dẫn file dữ liệu (mặc định data/customers.txt)
 * @return std::vector<Customer> Danh sách các đối tượng Customer nạp được
 */
vector<Customer> FileManager<Customer>::load(const string& path) {
    vector<Customer> out;
    ifstream f(path);
    if (!f.is_open()) return out;

    string l;
    while (getline(f, l)) {
        if (l.empty()) continue;

        // Bỏ qua dòng kẻ phân cách bảng bắt đầu bằng '-'
        size_t firstChar = l.find_first_not_of(" \t\r\n");
        if (firstChar == string::npos || l[firstChar] == '-') continue;

        auto v = fields(l);
        // Kiểm tra số lượng cột dữ liệu (Ma ID, Ho ten, So dien thoai, Email, Dia chi)
        if (v.size() != 5) continue;

        // Bỏ qua dòng tiêu đề bảng
        if (v[0] == "Ma ID") continue;

        // Bỏ qua dòng nếu mã ID không đúng quy chuẩn (tiền tố 'KH')
        if (Utils::extractIdNumber(v[0], "KH") == -1) continue;

        try {
            Customer x(v[1], v[2], v[3], v[4]);
            x.setId(v[0]);
            out.push_back(x);
        } catch (...) {
            // Bỏ qua dòng nếu dữ liệu không hợp lệ
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
    filesystem::path p(path);
    if (p.has_parent_path()) {
        filesystem::create_directories(p.parent_path());
    }
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

// =========================================================================
//              QUẢN LÝ DỮ LIỆU ĐÁNH GIÁ HÀI LÒNG (FEEDBACK)
// =========================================================================

/**
 * @brief Đọc danh sách Phiếu đánh giá hài lòng từ file văn bản có định dạng bảng
 * @param path Đường dẫn file dữ liệu (mặc định data/feedback.txt)
 * @return std::vector<Feedback> Danh sách các đối tượng Feedback nạp được
 */
vector<Feedback> FileManager<Feedback>::load(const string& path) {
    vector<Feedback> out;
    ifstream f(path);
    if (!f.is_open()) return out;

    string l;
    while (getline(f, l)) {
        if (l.empty()) continue;

        // Bỏ qua dòng kẻ phân cách bảng bắt đầu bằng '-'
        size_t firstChar = l.find_first_not_of(" \t\r\n");
        if (firstChar == string::npos || l[firstChar] == '-') continue;

        auto v = fields(l);
        // Kiểm tra số lượng cột dữ liệu (Ma ID, Ma KH, Ma Ticket, Diem, Ngay ban hanh, Noi dung)
        if (v.size() != 6) continue;

        // Bỏ qua dòng tiêu đề bảng
        if (v[0] == "Ma ID") continue;

        // Bỏ qua dòng nếu mã ID không đúng quy chuẩn (tiền tố 'DG')
        if (Utils::extractIdNumber(v[0], "DG") == -1) continue;

        try {
            Feedback x(v[1], v[2], stoi(v[3]), v[5], v[4]);
            x.setId(v[0]);
            out.push_back(x);
        } catch (...) {
            // Bỏ qua dòng nếu dữ liệu không hợp lệ
        }
    }
    return out;
}

/**
 * @brief Ghi danh sách Phiếu đánh giá hài lòng ra file văn bản theo định dạng bảng căn lề chuẩn
 * @param path Đường dẫn file dữ liệu (mặc định data/feedback.txt)
 * @param data Danh sách đối tượng Feedback cần lưu
 */
void FileManager<Feedback>::save(const string& path, const vector<Feedback>& data) {
    filesystem::path p(path);
    if (p.has_parent_path()) {
        filesystem::create_directories(p.parent_path());
    }
    ofstream f(path);
    if (!f) throw runtime_error("Khong the mo file de ghi: " + path);

    // Ghi tiêu đề bảng và đường kẻ ngang
    f << left
      << setw(8)  << "Ma ID"         << " | "
      << setw(10) << "Ma KH"         << " | "
      << setw(12) << "Ma Ticket"     << " | "
      << setw(6)  << "Diem"          << " | "
      << setw(14) << "Ngay ban hanh" << " | "
      << "Noi dung danh gia" << "\n";
    f << string(101, '-') << "\n";

    // Ghi từng bản ghi đánh giá hài lòng
    for (const auto& x : data) {
        f << left
          << setw(8)  << x.getId()         << " | "
          << setw(10) << x.getCustomerId() << " | "
          << setw(12) << x.getTicketId()   << " | "
          << setw(6)  << x.getScore()      << " | "
          << setw(14) << x.getDate()       << " | "
          << x.getComment() << "\n";
    }

    f.flush();
    if (!f) throw runtime_error("Khong the ghi du lieu vao file: " + path);
}
