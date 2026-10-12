#pragma once

/**
 * @file TransferTicket.h
 * @brief Thuc the Phieu chuyen xu ly: ghi nhan 1 Ticket duoc chuyen tu Agent nay sang Agent khac.
 * Thuoc tinh: ma ID, ma ticket, ma agent chuyen, ma agent nhan, ly do, trang thai.
 */

#include <string>
#include "BaseEntity.h"

using namespace std;

enum class TransferStatus
{
    PENDING,     // Cho nhan
    ACCEPTED,    // Da nhan
    COMPLETED    // Hoan tat
};

class TransferTicket : public BaseEntity
{
private:
    string ticketId;
    string fromAgentId;
    string toAgentId;
    string reason;
    TransferStatus status;

public:
    // id se duoc Repository cap khi add
    TransferTicket(
        const string& ticketId,
        const string& fromAgentId,
        const string& toAgentId,
        const string& reason
    );

    // Getter
    const string& getTicketId() const;
    const string& getFromAgentId() const;
    const string& getToAgentId() const;
    const string& getReason() const;
    TransferStatus getStatus() const;

    // Setter
    void setReason(const string& reason);

    void printInfo() const override;

    // Business method: chi cho phep chuyen trang thai theo chieu PENDING -> ACCEPTED -> COMPLETED
    void changeStatus(TransferStatus newStatus);

    // Dung khi doc tu file (khong kiem tra chieu chuyen)
    void restoreStatus(TransferStatus s);
};
