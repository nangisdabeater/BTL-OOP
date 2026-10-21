/**
Triển khai các phương thức của lớp thực thể Nhóm dịch vụ (ServiceGroup)
 */

#include "ServiceGroup.h"
#include <iostream>
#include <iomanip>
#include <stdexcept>

ServiceGroup::ServiceGroup(
    const string& name,
    const string& description,
    const string& department,
    int slaHours,
    int baseFee,
    const string& createdDate
)
    : BaseEntity(),
      name(name),
      description(description),
      department(department),
      slaHours(slaHours),
      baseFee(baseFee),
      createdDate(createdDate),
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

const string& ServiceGroup::getDepartment() const
{
    return department;
}

int ServiceGroup::getSlaHours() const
{
    return slaHours;
}

int ServiceGroup::getBaseFee() const
{
    return baseFee;
}

const string& ServiceGroup::getCreatedDate() const
{
    return createdDate;
}

bool ServiceGroup::isActive() const
{
    return active;
}

// Setter
void ServiceGroup::setName(const string& name)
{
    if (name.empty()) {
        throw invalid_argument("Ten dich vu khong duoc de trong!");
    }
    if (name.length() > 100) {
        throw invalid_argument("Ten dich vu khong duoc vuot qua 100 ky tu!");
    }
    this->name = name;
}

void ServiceGroup::setDescription(const string& description)
{
    if (description.empty()) {
        throw invalid_argument("Mo ta dich vu khong duoc de trong!");
    }
    this->description = description;
}

void ServiceGroup::setDepartment(const string& department)
{
    if (department.empty()) {
        throw invalid_argument("Phong ban phu trach khong duoc de trong!");
    }
    this->department = department;
}

void ServiceGroup::setSlaHours(int slaHours)
{
    if (slaHours <= 0) {
        throw invalid_argument("Thoi gian xu ly SLA phai lon hon 0 gio!");
    }
    this->slaHours = slaHours;
}

void ServiceGroup::setBaseFee(int baseFee)
{
    if (baseFee < 0) {
        throw invalid_argument("Phi dich vu co ban phai lon hon hoac bang 0!");
    }
    this->baseFee = baseFee;
}

void ServiceGroup::setCreatedDate(const string& createdDate)
{
    if (createdDate.empty()) {
        throw invalid_argument("Ngay ban hanh khong duoc de trong!");
    }
    this->createdDate = createdDate;
}

void ServiceGroup::printInfo() const
{
    cout << "Service ID: " << id 
         << " | Ten: " << name 
         << " | Phong ban: " << department
         << " | SLA: " << slaHours << "h"
         << " | Phi co ban: " << baseFee
         << " | Ngay ban hanh: " << createdDate
         << " | Trang thai: " << (active ? "Hoat dong" : "Tam ngung") << endl;
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