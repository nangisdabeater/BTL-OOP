#pragma once

/**
Định nghĩa thực thể Nhóm dịch vụ (ServiceGroup)
Đại diện cho 1 nhóm dịch vụ với các thuộc tính: mã ID, tên, mô tả và trạng thái hoạt động.
 */

#include <string>
#include "BaseEntity.h"


class ServiceGroup : public BaseEntity{
private:
    std::string name;            // Ten dich vu (khong rong, toi da 100 ky tu)
    std::string description;     // Mo ta chi tiet
    std::string department;      // Phong ban phu trach (khong rong, toi da 50 ky tu)
    int slaHours;           // Thoi gian xu ly tieu chuan (gio, > 0)
    int baseFee;            // Phi dich vu co ban (>= 0)
    std::string createdDate;     // Ngay ban hanh (DD/MM/YYYY)
    bool active;            // Trang thai hoat dong (true: Hoat dong, false: Tam ngung)

public:
    // Constructor (ID se duoc Repository cap tu dong dang SMxxx)
    ServiceGroup(
        const std::string& name = "",
        const std::string& description = "",
        const std::string& department = "Phong CSKH",
        int slaHours = 24,
        int baseFee = 0,
        const std::string& createdDate = "01/01/2026"
    );

    // Getter
    const std::string& getName() const;
    const std::string& getDescription() const;
    const std::string& getDepartment() const;
    int getSlaHours() const;
    int getBaseFee() const;
    const std::string& getCreatedDate() const;
    bool isActive() const;

    // Setter (rang buoc du lieu hop le)
    void setName(const std::string& name);
    void setDescription(const std::string& description);
    void setDepartment(const std::string& department);
    void setSlaHours(int slaHours);
    void setBaseFee(int baseFee);
    void setCreatedDate(const std::string& createdDate);

    // Implement pure virtual method
    void printInfo() const override;

    // Business methods
    void activate();
    void deactivate();
};

// Bí danh tương thích cho use case ServiceManager
using ServiceManager = ServiceGroup;