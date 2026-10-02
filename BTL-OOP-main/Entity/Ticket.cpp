/**
 * @file Ticket.cpp
 * @brief Triển khai các phương thức của lớp thực thể Ticket
 */

#include "Ticket.h"
#include <iostream>
#include <stdexcept>

using namespace std;

Ticket::Ticket(
    const string& title,
    const string& description,
    const string& customerId,
    const string& serviceGroupId
)
    : BaseEntity(),
      title(title),
      description(description),
      customerId(customerId),
      serviceGroupId(serviceGroupId),
      status(TicketStatus::OPEN)
{
}

// Getter

const string& Ticket::getTitle() const
{
    return title;
}

const string& Ticket::getDescription() const
{
    return description;
}

const string& Ticket::getCustomerId() const
{
    return customerId;
}

const string& Ticket::getServiceGroupId() const
{
    return serviceGroupId;
}

TicketStatus Ticket::getStatus() const
{
    return status;
}

// Setter

void Ticket::setTitle(const string& title)
{
    if (title.empty()) {
        throw invalid_argument("Tieu de ticket khong duoc de trong!");
    }
    this->title = title;
}

void Ticket::setDescription(const string& description)
{
    if (description.empty()) {
        throw invalid_argument("Mo ta ticket khong duoc de trong!");
    }
    this->description = description;
}

void Ticket::setCustomerId(const string& customerId)
{
    if (customerId.empty()) {
        throw invalid_argument("Ma khach hang khong duoc de trong!");
    }
    this->customerId = customerId;
}

void Ticket::setServiceGroupId(const string& serviceGroupId)
{
    if (serviceGroupId.empty()) {
        throw invalid_argument("Ma nhom dich vu khong duoc de trong!");
    }
    this->serviceGroupId = serviceGroupId;
}

void Ticket::printInfo() const
{
    cout << "Ticket ID: " << id 
         << ", Title: " << title 
         << ", Customer ID: " << customerId 
         << ", Service Group ID: " << serviceGroupId << endl;
}

// Business method

void Ticket::changeStatus(TicketStatus status)
{
    this->status = status;
}