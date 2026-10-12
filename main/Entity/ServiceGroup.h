#pragma once

/**
 * @file ServiceGroup.h
 * @brief Định nghĩa thực thể Nhóm dịch vụ (ServiceGroup)
 * Đại diện cho 1 nhóm dịch vụ với các thuộc tính: mã ID, tên, mô tả và trạng thái hoạt động.
 */

#include <string>
#include "BaseEntity.h"

using namespace std;

class ServiceGroup : public BaseEntity
{
private:
    string name;            // Ten dich vu (khong rong, toi da 100 ky tu)
    string description;     // Mo ta chi tiet
    string department;      // Phong ban phu trach (khong rong, toi da 50 ky tu)
    int slaHours;           // Thoi gian xu ly tieu chuan (gio, > 0)
    double baseFee;         // Phi dich vu co ban (>= 0)
    string createdDate;     // Ngay ban hanh (DD/MM/YYYY)
    bool active;            // Trang thai hoat dong (true: Hoat dong, false: Tam ngung)

public:
    // Constructor (ID se duoc Repository cap tu dong dang SMxxx)
    ServiceGroup(
        const string& name = "",
        const string& description = "",
        const string& department = "Phong CSKH",
        int slaHours = 24,
        double baseFee = 0.0,
        const string& createdDate = "01/01/2026"
    );

    // Getter
    const string& getName() const;
    const string& getDescription() const;
    const string& getDepartment() const;
    int getSlaHours() const;
    double getBaseFee() const;
    const string& getCreatedDate() const;
    bool isActive() const;

    // Setter (rang buoc du lieu hop le)
    void setName(const string& name);
    void setDescription(const string& description);
    void setDepartment(const string& department);
    void setSlaHours(int slaHours);
    void setBaseFee(double baseFee);
    void setCreatedDate(const string& createdDate);

    // Implement pure virtual method
    void printInfo() const override;

    // Business methods
    void activate();
    void deactivate();
};

// Bí danh tương thích cho use case ServiceManager
using ServiceManager = ServiceGroup;