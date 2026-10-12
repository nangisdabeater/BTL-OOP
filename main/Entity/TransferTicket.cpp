/**
 * @file TransferTicket.cpp
 * @brief Trien khai lop TransferTicket
 */

#include "TransferTicket.h"
#include <iostream>
#include <stdexcept>

using namespace std;

TransferTicket::TransferTicket(
    const string& ticketId,
    const string& fromAgentId,
    const string& toAgentId,
    const string& reason
)
    : BaseEntity(),
      ticketId(ticketId),
      fromAgentId(fromAgentId),
      toAgentId(toAgentId),
      reason(reason),
      status(TransferStatus::PENDING)
{
    if (ticketId.empty())    throw invalid_argument("Ma ticket khong duoc de trong!");
    if (fromAgentId.empty()) throw invalid_argument("Ma agent chuyen khong duoc de trong!");
    if (toAgentId.empty())   throw invalid_argument("Ma agent nhan khong duoc de trong!");
    if (reason.empty())      throw invalid_argument("Ly do chuyen khong duoc de trong!");
    if (fromAgentId == toAgentId) {
        throw invalid_argument("Agent chuyen va agent nhan khong duoc trung nhau!");
    }
}

const string& TransferTicket::getTicketId() const { return ticketId; }
const string& TransferTicket::getFromAgentId() const { return fromAgentId; }
const string& TransferTicket::getToAgentId() const { return toAgentId; }
const string& TransferTicket::getReason() const { return reason; }
TransferStatus TransferTicket::getStatus() const { return status; }

void TransferTicket::setReason(const string& reason)
{
    if (reason.empty()) {
        throw invalid_argument("Ly do chuyen khong duoc de trong!");
    }
    this->reason = reason;
}

void TransferTicket::printInfo() const
{
    cout << "TransferTicket ID: " << id
         << ", Ticket: " << ticketId
         << ", From: " << fromAgentId
         << ", To: " << toAgentId << endl;
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
