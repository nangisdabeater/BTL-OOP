#include "Customer.h"
#include <stdexcept>
#include <iostream>

using namespace std;

// Khởi tạo đối tượng Customer bằng danh sách khởi tạo thành viên để tối ưu tốc độ
Customer::Customer(const string& n, const string& p, const string& e, const string& a)
    : BaseEntity(), fullName(n), phone(p), email(e), address(a) {}

const string& Customer::getFullName() const {
    return fullName; // Trả về họ tên khách hàng
}

const string& Customer::getPhone() const {
    return phone; // Trả về số điện thoại
}

const string& Customer::getEmail() const {
    return email; // Trả về địa chỉ email
}

const string& Customer::getAddress() const {
    return address; // Trả về địa chỉ cư trú
}

void Customer::setFullName(const string& v) {
    // Kiểm tra họ tên không được để trống, nếu trống thì ném ngoại lệ
    if (v.empty()) {
        throw invalid_argument("Ten khach hang khong duoc rong");
    }
    fullName = v;
}

void Customer::setPhone(const string& v) {
    if (v.empty()) {
        throw invalid_argument("So dien thoai khong duoc rong");
    }
    phone = v;
}

void Customer::setEmail(const string& v) {
    // Kiểm tra chuỗi email phải hợp lệ (nếu hàm find trả về string::npos tức là không tìm thấy)
    if (v.find('@') == string::npos) {
        throw invalid_argument("Email khong hop le");
    }
    email = v;
}

void Customer::setAddress(const string& v) {
    if (v.empty()) {
        throw invalid_argument("Dia chi khong duoc rong");
    }
    address = v;
}

void Customer::printInfo() const {
    // Hiển thị thông tin chi tiết khách hàng ra màn hình
    cout << "Customer ID: " << id
         << " | Ho ten: " << fullName
         << " | SDT: " << phone
         << " | Email: " << email
         << " | Dia chi: " << address << endl;
}
