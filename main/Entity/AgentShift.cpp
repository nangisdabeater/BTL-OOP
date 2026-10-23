#include "AgentShift.h"
#include <iostream>
#include <stdexcept>
#include <algorithm>

using namespace std;

AgentShift::AgentShift(
    const string& shiftName,
    const string& shiftDate,
    const string& shiftStart,
    const string& shiftEnd
)
    // Dùng danh sách khởi tạo để gán trực tiếp các thuộc tính ngay lúc tạo đối tượng
    : BaseEntity(),
      shiftName(shiftName),
      shiftDate(shiftDate),
      shiftStart(shiftStart),
      shiftEnd(shiftEnd),
      status(true) {}

const string& AgentShift::getShiftName() const {
    return shiftName; // Trả về tên ca trực
}

const string& AgentShift::getShiftDate() const {
    return shiftDate; // Trả về ngày trực
}

const string& AgentShift::getShiftStart() const {
    return shiftStart; // Trả về giờ bắt đầu ca
}

const string& AgentShift::getShiftEnd() const {
    return shiftEnd; // Trả về giờ kết thúc ca
}

const vector<string>& AgentShift::getAgentIds() const {
    return agentIds; // Trả về tham chiếu hằng tới danh sách mã nhân viên trong ca
}

bool AgentShift::isActive() const {
    return status; // Trả về trạng thái hoạt động của ca trực
}

void AgentShift::setShiftName(const string& shiftName) {
    if (shiftName.empty()) {
        throw invalid_argument("Ten ca truc khong duoc de trong!");
    }
    this->shiftName = shiftName;
}

void AgentShift::setShiftDate(const string& shiftDate) {
    if (shiftDate.empty()) {
        throw invalid_argument("Ngay ca truc khong duoc de trong!");
    }
    this->shiftDate = shiftDate;
}

void AgentShift::setShiftStart(const string& shiftStart) {
    if (shiftStart.empty()) {
        throw invalid_argument("Gio bat dau ca truc khong duoc de trong!");
    }
    this->shiftStart = shiftStart;
}

void AgentShift::setShiftEnd(const string& shiftEnd) {
    if (shiftEnd.empty()) {
        throw invalid_argument("Gio ket thuc ca truc khong duoc de trong!");
    }
    this->shiftEnd = shiftEnd;
}

void AgentShift::setAgentIds(const vector<string>& agentIds) {
    this->agentIds = agentIds; // Gán danh sách mã nhân viên khi nạp từ file
}

void AgentShift::addAgentId(const string& agentId) {
    if (agentId.empty()) {
        throw invalid_argument("ID nhan vien khong duoc de trong!");
    }
    // Duyệt kiểm tra xem mã nhân viên đã có trong ca trực chưa để tránh phân công trùng lặp
    for (const auto& existingId : agentIds) {
        if (existingId == agentId) {
            throw invalid_argument("Nhan vien nay da co trong ca truc!");
        }
    }
    agentIds.push_back(agentId); // Thêm mã nhân viên vào cuối danh sách ca trực
}

void AgentShift::removeAgentId(const string& agentId) {
    // Dùng thuật toán std::remove kết hợp erase để dồn phần tử cần xóa về cuối vector rồi cắt bỏ trong 1 lần duyệt
    auto it = std::remove(agentIds.begin(), agentIds.end(), agentId);

    if (it != agentIds.end()) {
        agentIds.erase(it, agentIds.end()); // Xóa bỏ mã nhân viên khỏi vector
    } else {
        throw invalid_argument("Nhan vien nay khong co trong ca truc!");
    }
}

void AgentShift::printInfo() const {
    cout << "Ca truc ID: " << id
         << " | Ten ca: " << shiftName
         << " | Ngay: " << shiftDate
         << " | Bat dau: " << shiftStart
         << " | Ket thuc: " << shiftEnd
         << " | Trang thai: " << (status ? "Hoat dong" : "Tam ngung")
         << " | So DTV: " << agentIds.size() << endl;
    // Nếu ca trực đã có nhân viên phân công thì in thêm danh sách mã nhân viên ở dòng dưới
    if (!agentIds.empty()) {
        cout << "  -> DS ma nhan vien: ";
        for (size_t i = 0; i < agentIds.size(); ++i) {
            cout << agentIds[i] << (i + 1 < agentIds.size() ? ", " : "");
        }
        cout << endl;
    }
}

void AgentShift::activate() {
    status = true; // Bật trạng thái hoạt động
}

void AgentShift::deactivate() {
    status = false; // Tạm ngưng ca trực
}
