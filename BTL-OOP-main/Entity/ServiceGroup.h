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
    string name;
    string description;
    bool active;

public:
    // Constructor (id se duoc Repository cap khi add)
    ServiceGroup(
        const string& name,
        const string& description
    );

    // Getter
    const string& getName() const;
    const string& getDescription() const;
    bool isActive() const;

    // Setter
    void setName(const string& name);
    void setDescription(const string& description);

    // Implement pure virtual method
    void printInfo() const override;

    // Business methods
    void activate();
    void deactivate();
};