#include "Feedback.h"
#include "../Utilities/Utilities.h"
#include <stdexcept>
#include <iostream>

using namespace std;

// Khởi tạo đối tượng Feedback bằng danh sách khởi tạo thành viên
Feedback::Feedback(const string& c, int s, const string& m, const string& d)
    : BaseEntity(), customerId(c), comment(m), date(d), score(s) {}

const string& Feedback::getCustomerId() const {
    return customerId; // Trả về mã khách hàng thực hiện đánh giá
}

const string& Feedback::getComment() const {
    return comment; // Trả về nội dung nhận xét
}

const string& Feedback::getDate() const {
    return date; // Trả về ngày đánh giá
}

int Feedback::getScore() const {
    return score; // Trả về điểm đánh giá (1 -> 5)
}

void Feedback::setCustomerId(const string& v) {
    // Mã khách hàng không được để trống
    if (v.empty()) {
        throw invalid_argument("Ma khach hang khong duoc rong");
    }
    customerId = v;
}

void Feedback::setComment(const string& v) {
    comment = v; // Nhận xét có thể để trống tùy ý khách hàng
}

void Feedback::setDate(const string& v) {
    // Kiểm tra ngày đánh giá phải đúng định dạng DD/MM/YYYY
    if (!Utils::isValidDate(v)) {
        throw invalid_argument("Ngay phai co dang dd/mm/yyyy hop le");
    }
    date = v;
}

void Feedback::setScore(int v) {
    // Điểm đánh giá hài lòng bắt buộc nằm trong thang từ 1 đến 5 sao
    if (v < 1 || v > 5) {
        throw invalid_argument("Diem danh gia phai tu 1 den 5");
    }
    score = v;
}

void Feedback::printInfo() const {
    // Hiển thị thông tin chi tiết phiếu đánh giá ra màn hình
    cout << "Feedback ID: " << id
         << " | Ma KH: " << customerId
         << " | Diem: " << score << "/5"
         << " | Ngay: " << date
         << " | Nhan xet: " << comment << endl;
}
