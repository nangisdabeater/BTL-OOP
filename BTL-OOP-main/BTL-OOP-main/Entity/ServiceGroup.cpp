/**
 * @file ServiceGroup.cpp
 * @brief Triển khai các phương thức của lớp thực thể Nhóm dịch vụ (ServiceGroup)
 */

#include "ServiceGroup.h"
#include <iostream>
#include <stdexcept>

ServiceGroup::ServiceGroup(
    const string& name,
    const string& description
)
    : BaseEntity(),
      name(name),
      description(description),
      active(true)
{
}

// Getter
const string& ServiceGroup::getName() const
{
    return name;
}

const string& ServiceGroup::getDescription() const
{
    return description;
}

bool ServiceGroup::isActive() const
{
    return active;
}

// Setter
void ServiceGroup::setName(const string& name)
{
    if (name.empty()) {
        throw invalid_argument("Ten nhom dich vu khong duoc de trong!");
    }
    this->name = name;
}

void ServiceGroup::setDescription(const string& description)
{
    if (description.empty()) {
        throw invalid_argument("Mo ta nhom dich vu khong duoc de trong!");
    }
    this->description = description;
}

void ServiceGroup::printInfo() const
{
    cout << "ServiceGroup ID: " << id 
         << ", Name: " << name 
         << ", Active: " << (active ? "Yes" : "No") << endl;
}

// Business methods
void ServiceGroup::activate()
{
    active = true;
}

void ServiceGroup::deactivate()
{
    active = false;
}