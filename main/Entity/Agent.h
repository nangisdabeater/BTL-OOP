#pragma once 

/**
 * @file Agent.h
 * @brief Định nghĩa thực thể Điện thoại viên (Agent)
 * Đại diện cho 1 nhân viên tư vấn với các thuộc tính: mã ID (AGxxx), họ tên, số điện thoại,
 * email, số máy lẻ (extensionPhone), trạng thái hoạt động.
 */

#include <string>
#include "BaseEntity.h"

class Agent : public BaseEntity {
  private:
    string fullName;
    string phoneNumber;
    string email;
    string extensionPhone;
    bool status;
  public: 
    Agent(
      const string &fullName = "",
      const string &phoneNumber = "",
      const string &email = "",
      const string &extensionPhone = ""
    );

    // Getter 
    const string& getFullName() const;
    const string& getPhoneNumber() const;
    const string& getEmail() const;
    const string& getExtensionPhone() const;
    bool isActive() const;

    // Setter
    void setFullName(const string& fullName);
    void setPhoneNumber(const string& phoneNumber);
    void setEmail(const string& email);
    void setExtensionPhone(const string& extensionPhone);

    // Implement pure virtual method
    void printInfo() const override;

    // Business methods
    void activate();
    void deactivate();
};