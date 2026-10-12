#pragma once

#include "BaseEntity.h"
#include <string>

/**
 * @file Feedback.h
 * @brief Lớp thực thể đại diện cho Phiếu đánh giá hài lòng của khách hàng (Feedback)
 * Kế thừa từ lớp cơ sở trừu tượng BaseEntity
 */
class Feedback : public BaseEntity
{
private:
    std::string customerId; ///< Mã khách hàng thực hiện đánh giá (VD: KH001)
    std::string ticketId;   ///< Mã yêu cầu/ticket được đánh giá (VD: TK001)
    std::string comment;    ///< Nội dung ý kiến hoặc nhận xét của khách hàng
    std::string date;       ///< Ngày tạo đánh giá theo định dạng dd/mm/yyyy
    int score;              ///< Điểm đánh giá mức độ hài lòng (thang điểm 1 đến 5 sao)

public:
    /**
     * @brief Khởi tạo đối tượng Phiếu đánh giá hài lòng
     * @param customer Mã khách hàng
     * @param ticket Mã ticket
     * @param score Điểm hài lòng (1-5)
     * @param comment Nội dung nhận xét
     * @param date Ngày ban hành phiếu đánh giá (dd/mm/yyyy)
     */
    Feedback(const std::string& customer = "",
             const std::string& ticket = "",
             int score = 5,
             const std::string& comment = "",
             const std::string& date = "01/01/2026");

    // --- Các phương thức lấy dữ liệu (Getters) ---
    const std::string& getCustomerId() const;
    const std::string& getTicketId() const;
    const std::string& getComment() const;
    const std::string& getDate() const;
    int getScore() const;

    // --- Các phương thức cập nhật dữ liệu (Setters với kiểm tra ràng buộc) ---
    void setCustomerId(const std::string& customer);
    void setTicketId(const std::string& ticket);
    void setComment(const std::string& comment);
    void setDate(const std::string& date);
    void setScore(int score);

    /**
     * @brief In thông tin phiếu đánh giá ra màn hình (Ghi đè phương thức thuần ảo)
     */
    void printInfo() const override;
};
