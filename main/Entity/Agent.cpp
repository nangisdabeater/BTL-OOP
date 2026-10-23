#include "Agent.h"
#include <iostream>
#include <stdexcept>

using namespace std;

Agent::Agent(
    const string& fullName,
    const string& phoneNumber,
    const string& email,
    const string& extensionPhone
)
    // Dùng danh sách khởi tạo để gán trực tiếp các thuộc tính ngay khi tạo đối tượng, tối ưu hiệu năng
    : BaseEntity(),
      fullName(fullName),
      phoneNumber(phoneNumber),
      email(email),
      extensionPhone(extensionPhone),
      status(true) {} // Mặc định điện thoại viên mới tạo ở trạng thái đang hoạt động
  
const string& Agent::getFullName() const {
    return fullName; // Trả về tham chiếu hằng tới họ tên nhân viên
}

const string& Agent::getPhoneNumber() const {
    return phoneNumber; // Trả về tham chiếu hằng tới số điện thoại
}

const string& Agent::getEmail() const {
    return email; // Trả về tham chiếu hằng tới địa chỉ email
}

const string& Agent::getExtensionPhone() const {
    return extensionPhone; // Trả về tham chiếu hằng tới số máy lẻ
}

bool Agent::isActive() const {
    return status; // Trả về trạng thái hoạt động hiện tại
}

void Agent::setFullName(const string& name) {
    // Kiểm tra tên không được để trống, nếu trống thì ném ngoại lệ để chặn dữ liệu lỗi
    if (name.empty()) {
        throw invalid_argument("Ten nhan vien khong duoc de trong!");
    }
    this->fullName = name;
}

void Agent::setEmail(const string& email) {
    if (email.empty()) {
        throw invalid_argument("Email nhan vien khong duoc de trong!");
    }
    this->email = email;
}

void Agent::setPhoneNumber(const string& phoneNumber) {
    if (phoneNumber.empty()) {
        throw invalid_argument("So dien thoai nhan vien khong duoc de trong!");
    }
    this->phoneNumber = phoneNumber;
}

void Agent::setExtensionPhone(const string& extensionPhone) {
    if (extensionPhone.empty()) {
        throw invalid_argument("So may le tong dai khong duoc de trong!");
    }
    this->extensionPhone = extensionPhone;
}

void Agent::printInfo() const {
    // In thông tin chi tiết của điện thoại viên ra màn hình
    cout << "Agent ID: " << id 
         << " | Ho ten: " << fullName 
         << " | SDT: " << phoneNumber 
         << " | Email: " << email 
         << " | May le: " << extensionPhone 
         << " | Trang thai: " << (status ? "Hoat dong" : "Tam ngung") << endl;
}

void Agent::activate() {
    status = true; // Bật trạng thái hoạt động
}

void Agent::deactivate() {
    status = false; // Tạm ngưng hoạt động
}
