#pragma once

/**
 * @file Ticket.h
 * @brief Định nghĩa thực thể Ticket (phiếu hỗ trợ khách hàng)
 * Đại diện cho 1 ticket với các thuộc tính: mã ID, tiêu đề, mô tả, mã khách hàng, mã nhóm dịch vụ và trạng thái.
 */

#include <string>
#include "BaseEntity.h"

using namespace std;

enum class TicketStatus
{
    OPEN,
    IN_PROGRESS,
    RESOLVED,
    CLOSED
};

class Ticket : public BaseEntity
{
private:
    string title;
    string description;

    string customerId;
    string serviceGroupId;

    TicketStatus status;

public:
    // Constructor (id se duoc Repository cap khi add)
    Ticket(
        const string& title,
        const string& description,
        const string& customerId,
        const string& serviceGroupId
    );

    // Getter
    const string& getTitle() const;
    const string& getDescription() const;
    const string& getCustomerId() const;
    const string& getServiceGroupId() const;
    TicketStatus getStatus() const;

    // Setter
    void setTitle(const string& title);
    void setDescription(const string& description);
    void setCustomerId(const string& customerId);
    void setServiceGroupId(const string& serviceGroupId);

    // Implement pure virtual method
    void printInfo() const override;

    // Business method
    void changeStatus(TicketStatus status);
};