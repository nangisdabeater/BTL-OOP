/**
 * @file TransferTicket.cpp
 * @brief Trien khai lop TransferTicket
 */

#include "TransferTicket.h"
#include <iostream>
#include <stdexcept>

using namespace std;

TransferTicket::TransferTicket(
    const string& slaId,
    const string& fromDept,
    const string& toDept,
    const string& reason
)
    : BaseEntity(),
      slaId(slaId),
      fromDept(fromDept),
      toDept(toDept),
      reason(reason),
      status(TransferStatus::PENDING)
{
    if (slaId.empty())    throw invalid_argument("Ma muc SLA khong duoc de trong!");
    if (fromDept.empty()) throw invalid_argument("Bo phan chuyen khong duoc de trong!");
    if (toDept.empty())   throw invalid_argument("Bo phan nhan khong duoc de trong!");
    if (reason.empty())   throw invalid_argument("Ly do chuyen khong duoc de trong!");
    if (fromDept == toDept) {
        throw invalid_argument("Bo phan chuyen va bo phan nhan khong duoc trung nhau!");
    }
}

const string& TransferTicket::getSlaId() const { return slaId; }
const string& TransferTicket::getFromDept() const { return fromDept; }
const string& TransferTicket::getToDept() const { return toDept; }
const string& TransferTicket::getReason() const { return reason; }
TransferStatus TransferTicket::getStatus() const { return status; }

void TransferTicket::setSlaId(const string& slaId)
{
    if (slaId.empty()) {
        throw invalid_argument("Ma muc SLA khong duoc de trong!");
    }
    this->slaId = slaId;
}

void TransferTicket::setFromDept(const string& fromDept)
{
    if (fromDept.empty()) {
        throw invalid_argument("Bo phan chuyen khong duoc de trong!");
    }
    this->fromDept = fromDept;
}

void TransferTicket::setToDept(const string& toDept)
{
    if (toDept.empty()) {
        throw invalid_argument("Bo phan nhan khong duoc de trong!");
    }
    this->toDept = toDept;
}

void TransferTicket::setReason(const string& reason)
{
    if (reason.empty()) {
        throw invalid_argument("Ly do chuyen khong duoc de trong!");
    }
    this->reason = reason;
}

void TransferTicket::printInfo() const
{
    string stStr;
    switch (status) {
        case TransferStatus::PENDING: stStr = "Cho nhan (PENDING)"; break;
        case TransferStatus::ACCEPTED: stStr = "Da nhan (ACCEPTED)"; break;
        case TransferStatus::COMPLETED: stStr = "Hoan tat (COMPLETED)"; break;
    }
    cout << "TransferTicket ID: " << id
         << " | Ma SLA: " << slaId
         << " | Tu BP: " << fromDept
         << " | Sang BP: " << toDept
         << " | Ly do: " << reason
         << " | Trang thai: " << stStr << endl;
}

void TransferTicket::changeStatus(TransferStatus newStatus)
{
    if (static_cast<int>(newStatus) < static_cast<int>(status)) {
        throw invalid_argument("Khong the chuyen trang thai lui lai!");
    }
    status = newStatus;
}

void TransferTicket::restoreStatus(TransferStatus s)
{
    status = s;
}
