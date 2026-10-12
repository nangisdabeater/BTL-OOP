#include "Customer.h"
#include <stdexcept>
#include <iostream>

using namespace std;

/**
 * @brief Constructor khởi tạo đối tượng Customer
 */
Customer::Customer(const string& n, const string& p, const string& e, const string& a)
    : fullName(n), phone(p), email(e), address(a)
{
}

const string& Customer::getFullName() const {
    return fullName;
}

const string& Customer::getPhone() const {
    return phone;
}

const string& Customer::getEmail() const {
    return email;
}

const string& Customer::getAddress() const {
    return address;
}

/**
 * @brief Cập nhật họ tên khách hàng, yêu cầu không được để trống
 */
void Customer::setFullName(const string& v) {
    if (v.empty()) {
        throw invalid_argument("Ten khach hang khong duoc rong");
    }
    fullName = v;
}

/**
 * @brief Cập nhật số điện thoại, yêu cầu không được để trống
 */
void Customer::setPhone(const string& v) {
    if (v.empty()) {
        throw invalid_argument("So dien thoai khong duoc rong");
    }
    phone = v;
}

/**
 * @brief Cập nhật email, yêu cầu phải chứa ký tự '@'
 */
void Customer::setEmail(const string& v) {
    if (v.find('@') == string::npos) {
        throw invalid_argument("Email khong hop le");
    }
    email = v;
}

/**
 * @brief Cập nhật địa chỉ, yêu cầu không được để trống
 */
void Customer::setAddress(const string& v) {
    if (v.empty()) {
        throw invalid_argument("Dia chi khong duoc rong");
    }
    address = v;
}

/**
 * @brief Hiển thị thông tin tóm tắt khách hàng
 */
void Customer::printInfo() const {
    cout << id << " | " << fullName << " | " << phone << " | " << email << " | " << address << '\n';
}
