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
 * @brief Đọc danh sách Phiếu đánh giá hài lòng từ file văn bản có định dạng bảng
 * @param path Đường dẫn file dữ liệu (mặc định data/feedback.txt)
 * @return std::vector<Feedback> Danh sách các đối tượng Feedback nạp được
 */
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
        // Kiểm tra số lượng cột dữ liệu (Ma ID, Ma KH, Ma Ticket, Diem, Ngay ban hanh, Noi dung)
        if (v.size() != 6) {
            cout << "[Canh bao] Dong sai dinh dang: \"" << l << "\". Bo qua dong nay." << endl;
            continue;
        }

        // Bỏ qua dòng tiêu đề bảng
        if (v[0] == "Ma ID") continue;

        // Bỏ qua dòng nếu mã ID không đúng quy chuẩn (tiền tố 'DG')
        if (Utils::extractIdNumber(v[0], "DG") == -1) {
            cout << "[Canh bao] Dong sai dinh dang: \"" << l << "\". Bo qua dong nay." << endl;
            continue;
        }

        try {
            int score = stoi(v[3]);
            Feedback x(v[1], v[2], score, v[5], v[4]);
            x.setId(v[0]);
            out.push_back(x);
        } catch (...) {
            cout << "[Canh bao] Dong sai dinh dang: \"" << l << "\". Bo qua dong nay." << endl;
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
