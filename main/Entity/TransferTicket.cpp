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
    // Khởi tạo trực tiếp các thuộc tính qua danh sách khởi tạo, mặc định trạng thái là PENDING (Chờ nhận)
    : BaseEntity(),
      slaId(slaId),
      fromDept(fromDept),
      toDept(toDept),
      reason(reason),
      status(TransferStatus::PENDING) {
    // Kiểm tra các trường bắt buộc và chặn trường hợp bộ phận chuyển trùng với bộ phận nhận
    if (!slaId.empty() || !fromDept.empty() || !toDept.empty() || !reason.empty()) {
        if (slaId.empty())    throw invalid_argument("Ma muc SLA khong duoc de trong!");
        if (fromDept.empty()) throw invalid_argument("Bo phan chuyen khong duoc de trong!");
        if (toDept.empty())   throw invalid_argument("Bo phan nhan khong duoc de trong!");
        if (reason.empty())   throw invalid_argument("Ly do chuyen khong duoc de trong!");
        if (fromDept == toDept) {
            throw invalid_argument("Bo phan chuyen va bo phan nhan khong duoc trung nhau!");
        }
    }
}

const string& TransferTicket::getSlaId() const { return slaId; }          // Trả về mã mức SLA áp dụng
const string& TransferTicket::getFromDept() const { return fromDept; }    // Trả về bộ phận chuyển
const string& TransferTicket::getToDept() const { return toDept; }        // Trả về bộ phận nhận
const string& TransferTicket::getReason() const { return reason; }        // Trả về lý do chuyển tuyến
TransferStatus TransferTicket::getStatus() const { return status; }       // Trả về trạng thái hiện tại

void TransferTicket::setSlaId(const string& slaId) {
    if (slaId.empty()) {
        throw invalid_argument("Ma muc SLA khong duoc de trong!");
    }
    this->slaId = slaId;
}

void TransferTicket::setFromDept(const string& fromDept) {
    if (fromDept.empty()) {
        throw invalid_argument("Bo phan chuyen khong duoc de trong!");
    }
    this->fromDept = fromDept;
}

void TransferTicket::setToDept(const string& toDept) {
    if (toDept.empty()) {
        throw invalid_argument("Bo phan nhan khong duoc de trong!");
    }
    this->toDept = toDept;
}

void TransferTicket::setReason(const string& reason) {
    if (reason.empty()) {
        throw invalid_argument("Ly do chuyen khong duoc de trong!");
    }
    this->reason = reason;
}

void TransferTicket::printInfo() const {
    // Chuyển kiểu enum class TransferStatus sang chuỗi mô tả tiếng Việt
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

void TransferTicket::changeStatus(TransferStatus newStatus) {
    // Dùng static_cast<int> để so sánh thứ tự trạng thái: chặn không cho lùi trạng thái ngược quy trình
    if (static_cast<int>(newStatus) < static_cast<int>(status)) {
        throw invalid_argument("Khong the chuyen trang thai lui lai!");
    }
    status = newStatus;
}

void TransferTicket::restoreStatus(TransferStatus s) {
    status = s; // Gán lại trạng thái khi đọc dữ liệu từ file lên bộ nhớ
}
