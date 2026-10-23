#pragma once

#include "BaseEntity.h"
#include <string>

// Kế thừa từ BaseEntity để quản lý thống nhất mã ID (DGxxx) và tính đa hình
class Feedback : public BaseEntity {
private:
    std::string customerId; // Mã khách hàng thực hiện đánh giá (khóa ngoại liên kết với Customer, VD: KH001)
    std::string comment;    // Nội dung ý kiến nhận xét của khách hàng
    std::string date;       // Ngày tạo đánh giá theo định dạng DD/MM/YYYY
    int score;              // Điểm đánh giá mức độ hài lòng (thang điểm từ 1 đến 5 sao)

public:
    // Hàm khởi tạo có tham số mặc định cho phiếu đánh giá hài lòng
    Feedback(const std::string& customer = "",
             int score = 5,
             const std::string& comment = "",
             const std::string& date = "01/01/2026");

    // Các hàm Getter lấy thông tin phiếu đánh giá
    const std::string& getCustomerId() const;
    const std::string& getComment() const;
    const std::string& getDate() const;
    int getScore() const;

    // Các hàm Setter cập nhật thông tin kèm kiểm tra ràng buộc (điểm 1-5, ngày hợp lệ)
    void setCustomerId(const std::string& customer);
    void setComment(const std::string& comment);
    void setDate(const std::string& date);
    void setScore(int score);

    // Ghi đè phương thức thuần ảo của BaseEntity để in thông tin phiếu đánh giá
    void printInfo() const override;
};
