#include "Feedback.h"
#include "../Utilities/Utilities.h"
#include <stdexcept>
#include <iostream>

using namespace std;

/**
 * @brief Constructor khởi tạo đối tượng Feedback
 */
Feedback::Feedback(const string& c, const string& t, int s, const string& m, const string& d)
    : customerId(c), ticketId(t), comment(m), date(d), score(s)
{
}

const string& Feedback::getCustomerId() const {
    return customerId;
}

const string& Feedback::getTicketId() const {
    return ticketId;
}

const string& Feedback::getComment() const {
    return comment;
}

const string& Feedback::getDate() const {
    return date;
}

int Feedback::getScore() const {
    return score;
}

/**
 * @brief Cập nhật mã khách hàng, yêu cầu không được để trống
 */
void Feedback::setCustomerId(const string& v) {
    if (v.empty()) {
        throw invalid_argument("Ma khach hang khong duoc rong");
    }
    customerId = v;
}

/**
 * @brief Cập nhật mã ticket, yêu cầu không được để trống
 */
void Feedback::setTicketId(const string& v) {
    if (v.empty()) {
        throw invalid_argument("Ma ticket khong duoc rong");
    }
    ticketId = v;
}

/**
 * @brief Cập nhật nội dung nhận xét, yêu cầu không được để trống
 */
void Feedback::setComment(const string& v) {
    if (v.empty()) {
        throw invalid_argument("Noi dung danh gia khong duoc rong");
    }
    comment = v;
}

/**
 * @brief Cập nhật ngày đánh giá, yêu cầu đúng định dạng dd/mm/yyyy
 */
void Feedback::setDate(const string& v) {
    if (!Utils::isValidDate(v)) {
        throw invalid_argument("Ngay phai co dang dd/mm/yyyy hop le");
    }
    date = v;
}

/**
 * @brief Cập nhật điểm đánh giá, thang điểm từ 1 đến 5
 */
void Feedback::setScore(int v) {
    if (v < 1 || v > 5) {
        throw invalid_argument("Diem danh gia phai tu 1 den 5");
    }
    score = v;
}

/**
 * @brief Hiển thị thông tin tóm tắt phiếu đánh giá
 */
void Feedback::printInfo() const {
    cout << id << " | " << customerId << " | " << ticketId << " | "
         << score << " | " << date << " | " << comment << '\n';
}
