#pragma once

#include <string>
#include <stdexcept>

class BaseEntity{
protected:
    std::string id;

public:
    BaseEntity() : id("") {}
    virtual ~BaseEntity() = default;

    virtual const std::string& getId() const {
        return id;
    }

    virtual void setId(const std::string& newId) {
        if (newId.empty()) {
            throw std::invalid_argument("ID khong duoc de trong!");
        }
        id = newId;
    }

    // Phuong thuc thuan ao (pure virtual) de dam bao BaseEntity la lop truu tuong
    virtual void printInfo() const = 0;
};
