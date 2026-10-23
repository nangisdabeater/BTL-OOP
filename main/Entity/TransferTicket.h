#pragma once

/**
 * @file TransferTicket.h
 * @brief Thuc the Phieu chuyen xu ly: ghi nhan phieu chuyen tuyen xu ly theo muc cam ket SLA.
 * Thuoc tinh: ma ID (TFxxx), ma muc SLA (SLxxx), bo phan chuyen, bo phan nhan, ly do, trang thai.
 */

#include <string>
#include "BaseEntity.h"


enum class TransferStatus
{
    PENDING,     // Cho nhan
    ACCEPTED,    // Da nhan
    COMPLETED    // Hoan tat
};

class TransferTicket : public BaseEntity
{
private:
    std::string slaId;       // Ma muc SLA ap dung (SLxxx - khoa ngoai lien ket SLALevel)
    std::string fromDept;    // Bo phan / tuyen chuyen
    std::string toDept;      // Bo phan / tuyen nhan
    std::string reason;      // Ly do chuyen xu ly
    TransferStatus status;

public:
    // id se duoc Repository cap khi add (TFxxx)
    TransferTicket(
        const std::string& slaId = "",
        const std::string& fromDept = "",
        const std::string& toDept = "",
        const std::string& reason = ""
    );

    // Getter
    const std::string& getSlaId() const;
    const std::string& getFromDept() const;
    const std::string& getToDept() const;
    const std::string& getReason() const;
    TransferStatus getStatus() const;

    // Setter
    void setSlaId(const std::string& slaId);
    void setFromDept(const std::string& fromDept);
    void setToDept(const std::string& toDept);
    void setReason(const std::string& reason);

    void printInfo() const override;

    // Business method: chi cho phep chuyen trang thai theo chieu PENDING -> ACCEPTED -> COMPLETED
    void changeStatus(TransferStatus newStatus);

    // Dung khi doc tu file (khong kiem tra chieu chuyen)
    void restoreStatus(TransferStatus s);
};
