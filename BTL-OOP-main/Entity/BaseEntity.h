#pragma once

#include <string>
#include <stdexcept>

using namespace std;

class BaseEntity
{
protected:
    string id;

public:
    BaseEntity() : id("") {}
    virtual ~BaseEntity() = default;

    virtual const string& getId() const {
        return id;
    }

    virtual void setId(const string& newId) {
        if (newId.empty()) {
            throw invalid_argument("ID khong duoc de trong!");
        }
        id = newId;
    }

    // Phuong thuc thuan ao (pure virtual) de dam bao BaseEntity la lop truu tuong
    virtual void printInfo() const = 0;
};
