#pragma once

/**
 * @file SLALevel.h
 * @brief Thuc the Muc SLA (cam ket thoi gian phuc vu)
 * Thuoc tinh: ma ID, ten muc, thoi gian phan hoi (phut), thoi gian xu ly (gio), muc uu tien (1-5).
 */

#include <string>
#include "BaseEntity.h"

using namespace std;

class SLALevel : public BaseEntity
{
private:
    string name;
    int responseMinutes;   // > 0
    int resolveHours;      // > 0
    int priority;          // 1 (cao nhat) -> 5 (thap nhat)

public:
    // id se duoc Repository cap khi add
    SLALevel(const string& name, int responseMinutes, int resolveHours, int priority);

    // Getter
    const string& getName() const;
    int getResponseMinutes() const;
    int getResolveHours() const;
    int getPriority() const;

    // Setter (nem invalid_argument neu sai rang buoc)
    void setName(const string& name);
    void setResponseMinutes(int minutes);
    void setResolveHours(int hours);
    void setPriority(int priority);

    void printInfo() const override;
};
