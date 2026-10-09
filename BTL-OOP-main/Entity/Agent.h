#pragma once 

/**
 * @file ServiceGroup.h
 * @brief Định nghĩa thực thể Nhóm dịch vụ (Agent)
 * Đại diện cho 1 nhóm dịch vụ với các thuộc tính: mã ID, tên, điện thoại
 * email, số điện thoại để tư vấn, trạng thái.
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
      const string &fullName,
      const string &phoneNumber,
      const string &email,
      const string &extensionPhone
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