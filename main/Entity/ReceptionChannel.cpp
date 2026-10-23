#include "ReceptionChannel.h"
#include <iostream>
#include <stdexcept>

using namespace std;

// Khởi tạo kênh tiếp nhận bằng danh sách khởi tạo, mặc định trạng thái hoạt động là true
ReceptionChannel::ReceptionChannel(
    const string& name,
    const string& description)
    : BaseEntity(),
      name(name),
      description(description),
      active(true) {}

const string& ReceptionChannel::getName() const {
    return name; // Trả về tên kênh tiếp nhận
}

const string& ReceptionChannel::getDescription() const {
    return description; // Trả về mô tả kênh tiếp nhận
}

bool ReceptionChannel::isActive() const {
    return active; // Trả về trạng thái hoạt động
}

void ReceptionChannel::setName(const string& name) {
    // Tên kênh tiếp nhận không được phép để trống
    if (name.empty()) {
        throw invalid_argument("Ten kenh tiep nhan khong duoc de trong!");
    }
    this->name = name;
}

void ReceptionChannel::setDescription(const string& description) {
    if (description.empty()) {
        throw invalid_argument("Mo ta kenh tiep nhan khong duoc de trong!");
    }
    this->description = description;
}

void ReceptionChannel::printInfo() const {
    // Hiển thị thông tin chi tiết của kênh tiếp nhận ra màn hình
    cout << "Kenh tiep nhan ID: " << id 
         << " | Ten: " << name 
         << " | Mo ta: " << description 
         << " | Trang thai: " << (active ? "Hoat dong" : "Ngung hoat dong") << endl;
}

void ReceptionChannel::activate() {
    active = true; // Kích hoạt kênh tiếp nhận
}

void ReceptionChannel::deactivate() {
    active = false; // Tạm ngưng kênh tiếp nhận
}
