#include "InboundCall.h"
#include <iostream>
#include <stdexcept>

using namespace std;

// Khởi tạo đối tượng InboundCall bằng danh sách khởi tạo thành viên
InboundCall::InboundCall(
    const string& phoneNumber,
    const string& channelId,
    const string& callTime,
    int duration,
    const string& status,
    const string& note
)
    : BaseEntity(),
      phoneNumber(phoneNumber),
      channelId(channelId),
      callTime(callTime),
      duration(duration),
      status(status),
      note(note) {}

const string& InboundCall::getPhoneNumber() const {
    return phoneNumber; // Trả về số điện thoại người gọi
}

const string& InboundCall::getChannelId() const {
    return channelId; // Trả về mã kênh tiếp nhận
}

const string& InboundCall::getCallTime() const {
    return callTime; // Trả về thời điểm gọi
}

int InboundCall::getDuration() const {
    return duration; // Trả về thời lượng cuộc gọi (giây)
}

const string& InboundCall::getStatus() const {
    return status; // Trả về trạng thái cuộc gọi
}

const string& InboundCall::getNote() const {
    return note; // Trả về ghi chú cuộc gọi
}

void InboundCall::setPhoneNumber(const string& phoneNumber) {
    if (phoneNumber.empty()) {
        throw invalid_argument("So dien thoai khong duoc de trong!");
    }
    this->phoneNumber = phoneNumber;
}

void InboundCall::setChannelId(const string& channelId) {
    if (channelId.empty()) {
        throw invalid_argument("Ma kenh tiep nhan khong duoc de trong!");
    }
    this->channelId = channelId;
}

void InboundCall::setCallTime(const string& callTime) {
    if (callTime.empty()) {
        throw invalid_argument("Thoi gian cuoc goi khong duoc de trong!");
    }
    this->callTime = callTime;
}

void InboundCall::setDuration(int duration) {
    // Thời lượng cuộc gọi tính bằng giây không được phép là số âm
    if (duration < 0) {
        throw invalid_argument("Thoi luong cuoc goi khong duoc am!");
    }
    this->duration = duration;
}

void InboundCall::setStatus(const string& status) {
    if (status.empty()) {
        throw invalid_argument("Trang thai cuoc goi khong duoc de trong!");
    }
    this->status = status;
}

void InboundCall::setNote(const string& note) {
    this->note = note; // Ghi chú có thể để trống
}

void InboundCall::printInfo() const {
    // Hiển thị thông tin chi tiết cuộc gọi ra màn hình
    cout << "Cuoc goi ID: " << id
         << " | SDT: " << phoneNumber
         << " | Ma kenh: " << channelId
         << " | Thoi gian: " << callTime
         << " | Thoi luong: " << duration << "s"
         << " | Trang thai: " << status
         << " | Ghi chu: " << note << endl;
}
