#pragma once

/**
Định nghĩa thực thể Ticket (phiếu hỗ trợ khách hàng)
Đại diện cho 1 ticket với các thuộc tính: mã ID, tiêu đề, mô tả, mã nhóm dịch vụ, độ ưu tiên, chi phí, ngày tạo và trạng thái.
 */

#include <string>
#include "BaseEntity.h"

using namespace std;

enum class TicketStatus{
    OPEN,
    IN_PROGRESS,
    RESOLVED,
    CLOSED
};

class Ticket : public BaseEntity{
private:
    string title;            // Tieu de ticket (khong rong, toi da 100 ky tu)
    string description;      // Mo ta chi tiet
    string serviceGroupId;   // Ma dich vu SMxxx (khoa ngoai)
    int priority;            // Muc do uu tien: 1 (Thap), 2 (Trung binh), 3 (Cao), 4 (Khan cap)
    int estimatedFee;        // Chi phi xu ly uoc tinh (>= 0)
    string createdDate;      // Ngay tao (DD/MM/YYYY)
    TicketStatus status;     // Trang thai ticket

public:
    // Constructor (ID se duoc Repository cap tu dong dang TKxxx)
    Ticket(
        const string& title = "",
        const string& description = "",
        const string& serviceGroupId = "",
        int priority = 2,
        int estimatedFee = 0,
        const string& createdDate = "01/01/2026"
    );

    // Getter
    const string& getTitle() const;
    const string& getDescription() const;
    const string& getServiceGroupId() const;
    int getPriority() const;
    int getEstimatedFee() const;
    const string& getCreatedDate() const;
    TicketStatus getStatus() const;

    // Setter (kiem tra rang buoc)
    void setTitle(const string& title);
    void setDescription(const string& description);
    void setServiceGroupId(const string& serviceGroupId);
    void setPriority(int priority);
    void setEstimatedFee(int fee);
    void setCreatedDate(const string& date);

    // Implement pure virtual method
    void printInfo() const override;

    // Business method
    void changeStatus(TicketStatus status);
};