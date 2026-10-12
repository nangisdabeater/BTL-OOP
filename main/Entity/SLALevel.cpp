/**
 * @file SLALevel.cpp
 * @brief Trien khai lop SLALevel
 */

#include "SLALevel.h"
#include <iostream>
#include <stdexcept>

using namespace std;

SLALevel::SLALevel(const string& name, int responseMinutes, int resolveHours, int priority)
    : BaseEntity(), name(""), responseMinutes(1), resolveHours(1), priority(5)
{
    // Di qua setter de dung chung rang buoc
    setName(name);
    setResponseMinutes(responseMinutes);
    setResolveHours(resolveHours);
    setPriority(priority);
}

const string& SLALevel::getName() const { return name; }
int SLALevel::getResponseMinutes() const { return responseMinutes; }
int SLALevel::getResolveHours() const { return resolveHours; }
int SLALevel::getPriority() const { return priority; }

void SLALevel::setName(const string& name)
{
    if (name.empty()) {
        throw invalid_argument("Ten muc SLA khong duoc de trong!");
    }
    this->name = name;
}

void SLALevel::setResponseMinutes(int minutes)
{
    if (minutes <= 0) {
        throw invalid_argument("Thoi gian phan hoi phai lon hon 0!");
    }
    this->responseMinutes = minutes;
}

void SLALevel::setResolveHours(int hours)
{
    if (hours <= 0) {
        throw invalid_argument("Thoi gian xu ly phai lon hon 0!");
    }
    this->resolveHours = hours;
}

void SLALevel::setPriority(int priority)
{
    if (priority < 1 || priority > 5) {
        throw invalid_argument("Muc do uu tien phai tu 1 den 5!");
    }
    this->priority = priority;
}

void SLALevel::printInfo() const
{
    cout << "SLALevel ID: " << id
         << ", Name: " << name
         << ", Response: " << responseMinutes << " phut"
         << ", Resolve: " << resolveHours << " gio"
         << ", Priority: " << priority << endl;
}
