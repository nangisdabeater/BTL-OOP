/**
 * @file Agent.cpp
 * @brief Triển khai các phương thức của lớp thực thể Điện thoại viên (Agent)
 */
#include "Agent.h"
#include <iostream>
#include <stdexcept>

Agent::Agent(
  const string& fullName,
  const string& phoneNumber,
  const string& email,
  const string& extensionPhone
)
  : BaseEntity(),
  fullName(fullName),
  phoneNumber(phoneNumber),
  email(email),
  extensionPhone(extensionPhone),
  status(true)
{}
  
// Getter
const string& Agent::getFullName() const {
  return fullName;
}

const string& Agent::getPhoneNumber() const {
  return phoneNumber;
}

const string& Agent::getEmail() const {
  return email;
}

const string& Agent::getExtensionPhone() const {
  return extensionPhone;
}

bool Agent::isActive() const {
  return status;
}

// Setter
void Agent::setFullName(const string& name) {
  if (name.empty()) {
    throw invalid_argument("Ten nhan vien khong duoc de trong!");
  }
  this->fullName = name;
}

void Agent::setEmail(const string& email) {
  if (email.empty()) {
    throw invalid_argument("Ten nhan vien khong duoc de trong!");
  }
  this->email = email;
}

void Agent::setPhoneNumber(const string& phoneNumber) {
  if (phoneNumber.empty()) {
    throw invalid_argument("Ten nhan vien khong duoc de trong!");
  }
  this->phoneNumber = phoneNumber;
}

void Agent::setExtensionPhone(const string& extensionPhone) {
  if (extensionPhone.empty()) {
    throw invalid_argument("Ten nhan vien khong duoc de trong!");
  }
  this->extensionPhone = extensionPhone;
}

void Agent::printInfo() const {
  cout << "Agent ID:" << id << ",fullName: " << fullName << ", Active" << (status ? "Yes" : "No") << endl;
}

// Business methods
void Agent::activate() {
  status = true;  
}

void Agent::deactivate() {
  status = false;
}