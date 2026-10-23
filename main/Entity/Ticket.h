#pragma once

/**
Định nghĩa thực thể Ticket (phiếu hỗ trợ khách hàng)
Đại diện cho 1 ticket với các thuộc tính: mã ID, tiêu đề, mô tả, mã nhóm dịch vụ, độ ưu tiên, chi phí, ngày tạo và trạng thái.
 */

#include <string>
#include "BaseEntity.h"


enum class TicketStatus{
    OPEN,
    IN_PROGRESS,
    RESOLVED,
    CLOSED
};

class Ticket : public BaseEntity{
private:
    std::string title;            // Tieu de ticket (khong rong, toi da 100 ky tu)
    std::string description;      // Mo ta chi tiet
    std::string serviceGroupId;   // Ma dich vu SMxxx (khoa ngoai)
    int priority;            // Muc do uu tien: 1 (Thap), 2 (Trung binh), 3 (Cao), 4 (Khan cap)
    int estimatedFee;        // Chi phi xu ly uoc tinh (>= 0)
    std::string createdDate;      // Ngay tao (DD/MM/YYYY)
    TicketStatus status;     // Trang thai ticket

public:
    // Constructor (ID se duoc Repository cap tu dong dang TKxxx)
    Ticket(
        const std::string& title = "",
        const std::string& description = "",
        const std::string& serviceGroupId = "",
        int priority = 2,
        int estimatedFee = 0,
        const std::string& createdDate = "01/01/2026"
    );

    // Getter
    const std::string& getTitle() const;
    const std::string& getDescription() const;
    const std::string& getServiceGroupId() const;
    int getPriority() const;
    int getEstimatedFee() const;
    const std::string& getCreatedDate() const;
    TicketStatus getStatus() const;

    // Setter (kiem tra rang buoc)
    void setTitle(const std::string& title);
    void setDescription(const std::string& description);
    void setServiceGroupId(const std::string& serviceGroupId);
    void setPriority(int priority);
    void setEstimatedFee(int fee);
    void setCreatedDate(const std::string& date);

    // Implement pure virtual method
    void printInfo() const override;

    // Business method
    void changeStatus(TicketStatus status);
};