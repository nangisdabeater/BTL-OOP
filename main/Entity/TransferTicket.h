#pragma once

/**
 * @file TransferTicket.h
 * @brief Thuc the Phieu chuyen xu ly: ghi nhan phieu chuyen tuyen xu ly theo muc cam ket SLA.
 * Thuoc tinh: ma ID (TFxxx), ma muc SLA (SLxxx), bo phan chuyen, bo phan nhan, ly do, trang thai.
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
    string slaId;       // Ma muc SLA ap dung (SLxxx - khoa ngoai lien ket SLALevel)
    string fromDept;    // Bo phan / tuyen chuyen
    string toDept;      // Bo phan / tuyen nhan
    string reason;      // Ly do chuyen xu ly
    TransferStatus status;

public:
    // id se duoc Repository cap khi add (TFxxx)
    TransferTicket(
        const string& slaId = "",
        const string& fromDept = "",
        const string& toDept = "",
        const string& reason = ""
    );

    // Getter
    const string& getSlaId() const;
    const string& getFromDept() const;
    const string& getToDept() const;
    const string& getReason() const;
    TransferStatus getStatus() const;

    // Setter
    void setSlaId(const string& slaId);
    void setFromDept(const string& fromDept);
    void setToDept(const string& toDept);
    void setReason(const string& reason);

    void printInfo() const override;

    // Business method: chi cho phep chuyen trang thai theo chieu PENDING -> ACCEPTED -> COMPLETED
    void changeStatus(TransferStatus newStatus);

    // Dung khi doc tu file (khong kiem tra chieu chuyen)
    void restoreStatus(TransferStatus s);
};
