//Triển khai các phương thức của lớp thực thể Ticket

#include "Ticket.h"
#include <iostream>
#include <stdexcept>

using namespace std;

Ticket::Ticket(
    const string& title,
    const string& description,
    const string& serviceGroupId,
    int priority,
    double estimatedFee,
    const string& createdDate
)
    : BaseEntity(),
      title(title),
      description(description),
      serviceGroupId(serviceGroupId),
      priority(priority),
      estimatedFee(estimatedFee),
      createdDate(createdDate),
      status(TicketStatus::OPEN){}

// Getter

const string& Ticket::getTitle() const{
    return title;
}

const string& Ticket::getDescription() const{
    return description;
}

const string& Ticket::getServiceGroupId() const{
    return serviceGroupId;
}

int Ticket::getPriority() const{
    return priority;
}

double Ticket::getEstimatedFee() const{
    return estimatedFee;
}

const string& Ticket::getCreatedDate() const{
    return createdDate;
}

TicketStatus Ticket::getStatus() const{
    return status;
}

// Setter

void Ticket::setTitle(const string& title){
    if (title.empty()) {
        throw invalid_argument("Tieu de ticket khong duoc de trong!");
    }
    if (title.length() > 100) {
        throw invalid_argument("Tieu de ticket khong duoc vuot qua 100 ky tu!");
    }
    this->title = title;
}

void Ticket::setDescription(const string& description){
    if (description.empty()) {
        throw invalid_argument("Mo ta ticket khong duoc de trong!");
    }
    this->description = description;
}

void Ticket::setServiceGroupId(const string& serviceGroupId){
    if (serviceGroupId.empty()) {
        throw invalid_argument("Ma dich vu khong duoc de trong!");
    }
    this->serviceGroupId = serviceGroupId;
}

void Ticket::setPriority(int priority){
    if (priority < 1 || priority > 4) {
        throw invalid_argument("Muc do uu tien phai tu 1 (Thap) den 4 (Khan cap)!");
    }
    this->priority = priority;
}

void Ticket::setEstimatedFee(double fee){
    if (fee < 0.0) {
        throw invalid_argument("Chi phi xu ly uoc tinh phai lon hon hoac bang 0!");
    }
    this->estimatedFee = fee;
}

void Ticket::setCreatedDate(const string& date){
    if (date.empty()) {
        throw invalid_argument("Ngay tao ticket khong duoc de trong!");
    }
    this->createdDate = date;
}

void Ticket::printInfo() const{
    string prioStr;
    switch (priority) {
        case 1: prioStr = "Thap"; break;
        case 2: prioStr = "Trung binh"; break;
        case 3: prioStr = "Cao"; break;
        case 4: prioStr = "Khan cap"; break;
        default: prioStr = to_string(priority); break;
    }

    string stStr;
    switch (status) {
        case TicketStatus::OPEN: stStr = "OPEN"; break;
        case TicketStatus::IN_PROGRESS: stStr = "IN_PROGRESS"; break;
        case TicketStatus::RESOLVED: stStr = "RESOLVED"; break;
        case TicketStatus::CLOSED: stStr = "CLOSED"; break;
    }

    cout << "Ticket ID: " << id 
         << " | Tieu de: " << title 
         << " | Ma DV: " << serviceGroupId
         << " | Uu tien: " << prioStr
         << " | Chi phi: " << estimatedFee
         << " | Ngay tao: " << createdDate
         << " | Trang thai: " << stStr << endl;
}

// Business method

void Ticket::changeStatus(TicketStatus status){
    this->status = status;
}