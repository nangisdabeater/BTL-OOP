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
    std::string fullName;
    std::string phoneNumber;
    std::string email;
    std::string extensionPhone;
    bool status;
  public: 
    Agent(
      const std::string &fullName = "",
      const std::string &phoneNumber = "",
      const std::string &email = "",
      const std::string &extensionPhone = ""
    );

    // Getter 
    const std::string& getFullName() const;
    const std::string& getPhoneNumber() const;
    const std::string& getEmail() const;
    const std::string& getExtensionPhone() const;
    bool isActive() const;

    // Setter
    void setFullName(const std::string& fullName);
    void setPhoneNumber(const std::string& phoneNumber);
    void setEmail(const std::string& email);
    void setExtensionPhone(const std::string& extensionPhone);

    // Implement pure virtual method
    void printInfo() const override;

    // Business methods
    void activate();
    void deactivate();
};