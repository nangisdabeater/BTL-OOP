/**
 * @file InboundCall.cpp
 * @brief Triển khai các phương thức của lớp thực thể Cuộc gọi tiếp nhận (InboundCall)
 * [PHẦN THÊM MỚI]
 */

#include "InboundCall.h"
#include <iostream>
#include <stdexcept>

InboundCall::InboundCall(
    const string& phoneNumber,
    const string& channelId,
    const string& agentId,
    const string& callTime,
    int duration,
    const string& status,
    const string& note
)
    : BaseEntity(),
      phoneNumber(phoneNumber),
      channelId(channelId),
      agentId(agentId),
      callTime(callTime),
      duration(duration),
      status(status),
      note(note)
{
}

// ==============================
// GETTER
// ==============================
const string& InboundCall::getPhoneNumber() const
{
    return phoneNumber;
}

const string& InboundCall::getChannelId() const
{
    return channelId;
}

const string& InboundCall::getAgentId() const
{
    return agentId;
}

const string& InboundCall::getCallTime() const
{
    return callTime;
}

int InboundCall::getDuration() const
{
    return duration;
}

const string& InboundCall::getStatus() const
{
    return status;
}

const string& InboundCall::getNote() const
{
    return note;
}

// ==============================
// SETTER (Có kiểm tra tính hợp lệ)
// ==============================
void InboundCall::setPhoneNumber(const string& phoneNumber)
{
    if (phoneNumber.empty()) {
        throw invalid_argument("So dien thoai khong duoc de trong!");
    }
    this->phoneNumber = phoneNumber;
}

void InboundCall::setChannelId(const string& channelId)
{
    if (channelId.empty()) {
        throw invalid_argument("Ma kenh tiep nhan khong duoc de trong!");
    }
    this->channelId = channelId;
}

void InboundCall::setAgentId(const string& agentId)
{
    this->agentId = agentId;
}

void InboundCall::setCallTime(const string& callTime)
{
    if (callTime.empty()) {
        throw invalid_argument("Thoi gian cuoc goi khong duoc de trong!");
    }
    this->callTime = callTime;
}

void InboundCall::setDuration(int duration)
{
    if (duration < 0) {
        throw invalid_argument("Thoi luong cuoc goi khong duoc am!");
    }
    this->duration = duration;
}

void InboundCall::setStatus(const string& status)
{
    if (status.empty()) {
        throw invalid_argument("Trang thai cuoc goi khong duoc de trong!");
    }
    this->status = status;
}

void InboundCall::setNote(const string& note)
{
    this->note = note;
}

// ==============================
// PHƯƠNG THỨC HIỂN THỊ THÔNG TIN
// ==============================
void InboundCall::printInfo() const
{
    cout << "Cuoc goi ID: " << id
         << " | SDT: " << phoneNumber
         << " | Kenh: " << channelId
         << " | DTV: " << (agentId.empty() ? "(Chua gan)" : agentId)
         << " | Thoi gian: " << callTime
         << " | Thoi luong: " << duration << "s"
         << " | Trang thai: " << status
         << " | Ghi chu: " << note << endl;
}
