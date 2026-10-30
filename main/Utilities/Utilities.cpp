#include "Utilities.h"
#include <iostream>
#include <iomanip>
#include <sstream>
#include <cctype>
#include <stdexcept>
#include "../Entity/BaseEntity.h"

// Trong file .cpp có thể dùng "using namespace std;" an toàn vì phạm vi chỉ gói gọn trong file này,
// không làm ảnh hưởng tới các file khác như khi đặt trong file .h.
using namespace std;

namespace Utils {
    string trim(const string& str) {
        // Tìm vị trí ký tự đầu tiên không phải khoảng trắng, tab hoặc xuống dòng
        size_t first = str.find_first_not_of(" \t\r\n");
        if (first == string::npos) {
            return ""; // Nếu toàn bộ chuỗi chỉ là khoảng trắng thì trả về chuỗi rỗng
        }
        // Tìm vị trí ký tự cuối cùng không phải khoảng trắng
        size_t last = str.find_last_not_of(" \t\r\n");
        // Cắt và trả về chuỗi con nằm giữa vị trí first và last
        return str.substr(first, last - first + 1);
    }

    string inputNonEmptyString(const string& prompt) {
        string input;
        while (true) {
            cout << prompt;
            getline(cin, input);
            input = trim(input); // Xóa khoảng trắng thừa để chặn trường hợp người dùng chỉ ấn dấu cách rồi Enter
            if (!input.empty()) {
                return input;
            }
            cout << "Gia tri khong duoc de trong! Vui long nhap lai." << endl;
        }
    }

    string formatId(const string& prefix, int number, int padding) {
        stringstream ss;
        // Dùng setw(padding) và setfill('0') tự động chèn đủ số 0 vào trước (VD: 1 -> 001, 25 -> 025).
        ss << prefix << setw(padding) << setfill('0') << number;
        return ss.str();
    }

    int extractIdNumber(const string& id, const string& prefix) {
        // Kiểm tra chuỗi ID phải dài hơn tiền tố và phần đầu phải khớp hoàn toàn với tiền tố (VD: "TK")
        if (id.length() <= prefix.length() || id.substr(0, prefix.length()) != prefix) {
            return -1; // Trả về -1 báo hiệu mã ID không hợp lệ
        }

        // Cắt lấy phần chuỗi phía sau tiền tố (VD: "TK001" -> lấy "001")
        string numberStr = id.substr(prefix.length());
        
        // Kiểm tra từng ký tự phía sau tiền tố bắt buộc phải là chữ số từ 0 đến 9
        for (char c : numberStr) {
            if (!isdigit(static_cast<unsigned char>(c))) {
                return -1;
            }
        }

        try {
            int number = stoi(numberStr); // Chuyển chuỗi số sang kiểu số nguyên int
            if (number < 1 || number > 999) return -1; // Chỉ chấp nhận số thứ tự ID từ 1 đến 999
            return number;
        }
        catch (...) {
            return -1; // Bắt mọi lỗi tràn số hoặc lỗi chuyển đổi của hàm stoi
        }
    }

    bool isValidDate(const string& dateStr) {
        // Kiểm tra độ dài chuẩn của định dạng DD/MM/YYYY bắt buộc là 10 ký tự
        if (dateStr.length() != 10) return false;
        // Vị trí thứ 2 và thứ 5 bắt buộc phải là dấu gạch chéo '/'
        if (dateStr[2] != '/' || dateStr[5] != '/') return false;

        // Các vị trí còn lại (ngày, tháng, năm) bắt buộc phải là chữ số
        for (int i = 0; i < 10; ++i) {
            if (i == 2 || i == 5) continue;
            if (!isdigit(static_cast<unsigned char>(dateStr[i]))) return false;
        }

        try {
            int d = stoi(dateStr.substr(0, 2)); // Tách 2 ký tự đầu làm ngày
            int m = stoi(dateStr.substr(3, 2)); // Tách 2 ký tự giữa làm tháng
            int y = stoi(dateStr.substr(6, 4)); // Tách 4 ký tự cuối làm năm

            if (y < 1900) return false;         // Giới hạn năm hợp lệ từ 1900 trở đi
            if (m < 1 || m > 12) return false;  // Tháng phải từ 1 đến 12

            // Mảng lưu số ngày tối đa của 12 tháng trong năm thường
            int maxDays[] = { 31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31 };

            // Kiểm tra năm nhuận: chia hết cho 4 nhưng không chia hết cho 100, hoặc chia hết cho 400
            if ((y % 4 == 0 && y % 100 != 0) || (y % 400 == 0)) {
                maxDays[1] = 29; // Năm nhuận thì tháng 2 có 29 ngày
            }

            // Kiểm tra ngày nhập vào không được vượt quá số ngày tối đa của tháng đó
            if (d < 1 || d > maxDays[m - 1]) return false;

            return true;
        }
        catch (...) {
            return false;
        }
    }

    int compareDates(const string& date1, const string& date2) {
        string cleanDate1 = trim(date1);
        string cleanDate2 = trim(date2);

        if (!isValidDate(cleanDate1) || !isValidDate(cleanDate2)) {
            throw invalid_argument("Dinh dang ngay thang khong hop le de so sanh (yeu cau DD/MM/YYYY)!");
        }

        int d1 = stoi(cleanDate1.substr(0, 2));
        int m1 = stoi(cleanDate1.substr(3, 2));
        int y1 = stoi(cleanDate1.substr(6, 4));

        int d2 = stoi(cleanDate2.substr(0, 2));
        int m2 = stoi(cleanDate2.substr(3, 2));
        int y2 = stoi(cleanDate2.substr(6, 4));

        // Quy đổi sang số nguyên dạng YYYYMMDD để so sánh thứ tự thời gian chính xác từ Năm -> Tháng -> Ngày
        int val1 = y1 * 10000 + m1 * 100 + d1;
        int val2 = y2 * 10000 + m2 * 100 + d2;

        if (val1 < val2) return -1;
        if (val1 > val2) return 1;
        return 0;
    }

    string inputDateString(const string& prompt) {
        while (true) {
            string date = inputNonEmptyString(prompt); // Nhập chuỗi không rỗng
            if (isValidDate(date)) {                   // Nếu đúng định dạng DD/MM/YYYY thì trả về kết quả
                return date;
            }
            cout << "Dinh dang ngay khong hop le! Vui long nhap theo dinh dang DD/MM/YYYY (VD: 12/10/2026)." << endl;
        }
    }

    int inputInt(const string& prompt, int minVal, int maxVal) {
        while (true) {
            // - Nếu dùng "cin >> val" mà người dùng nhập "12abc", cin sẽ lấy số 12 và để lại "abc" trong bộ đệm gây lỗi.
            // - Dùng stoi(s, &pos) cho phép kiểm tra "pos == s.length()", đảm bảo toàn bộ chuỗi nhập vào đều là chữ số.
            string s = inputNonEmptyString(prompt);
            try {
                size_t pos = 0;
                int val = stoi(s, &pos);
                // Kiểm tra đã đọc hết toàn bộ chuỗi (không dư ký tự chữ) và giá trị nằm trong khoảng cho phép
                if (pos == s.length() && val >= minVal && val <= maxVal) {
                    return val;
                }
            } 
            catch (...) {}

            if (maxVal == INT_MAX) {
                cout << "Gia tri khong hop le! Vui long nhap so nguyen >= " << minVal << "." << endl;
            } 
            else {
                cout << "Gia tri khong hop le! Vui long nhap so nguyen trong khoang [" 
                     << minVal << " - " << maxVal << "]." << endl;
            }
        }
    }

    string join(const vector<string>& vec, const string& delemiter) {
        string result = "";
        for (size_t i = 0; i < vec.size(); i++) {
            result += vec[i];
            // Chỉ thêm dấu ngăn cách ở giữa các phần tử, phần tử cuối cùng thì không thêm
            if (i < vec.size() - 1) result += delemiter;
        }

        // Dùng toán tử 3 ngôi: nếu danh sách trống thì hiển thị "Chua phan cong", ngược lại trả về chuỗi đã nối
        return result.empty() ? "Chua phan cong" : result;
    }

    void backChoice() {
        string input;
        while (true) {
            cout << "\n0. Quay lai menu" << endl;
            cout << "Moi nhap lua chon: ";

            getline(cin, input);
            input = trim(input);

            // Bắt buộc người dùng chỉ nhập đúng số "0" mới thoát khỏi màn hình hiện tại
            if (input == "0") {
                break;
            } else {
                cout << "Loi: Chuc nang khong ton tai! Vui long chi nhap duy nhat so 0 de quay lai." << endl;
            }
        }
    }

    void printEntityDetails(const BaseEntity& entity) {
        cout << "\n--- Thong tin chi tiet thuc the ---" << endl;
/** 
Thể hiện tính Đa hình: dù tham số truyền vào có kiểu là lớp cha BaseEntity&,
nhưng vì printInfo() là hàm ảo (virtual) nên chương trình sẽ tự động gọi đúng hàm printInfo()
của lớp con tương ứng (Ticket, ServiceGroup, Customer,...) lúc chạy chương trình.
*/
        entity.printInfo();
        cout << "-----------------------------------" << endl;
    }
}
