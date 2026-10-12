/**
 * @file AgentShift.cpp
 * @brief Triển khai các phương thức của lớp thực thể Ca trực (AgentShift)
 */

#include "AgentShift.h"
#include <iostream>
#include <stdexcept>

AgentShift::AgentShift(
    const string& shiftName,
    const string& shiftDate,
    const string& shiftStart,
    const string& shiftEnd
)
    : BaseEntity(),
      shiftName(shiftName),
      shiftDate(shiftDate),
      shiftStart(shiftStart),
      shiftEnd(shiftEnd),
      status(true)
{
}

// Getter
const string& AgentShift::getShiftName() const {
    return shiftName;
}

const string& AgentShift::getShiftDate() const {
    return shiftDate;
}

const string& AgentShift::getShiftStart() const {
    return shiftStart;
}

const string& AgentShift::getShiftEnd() const {
    return shiftEnd;
}

const vector<string>& AgentShift::getAgentIds() const {
    return agentIds;
}

bool AgentShift::isActive() const {
    return status;
}

// Setter
void AgentShift::setShiftName(const string& shiftName) {
    if (shiftName.empty()) {
        throw invalid_argument("Ten ca truc khong duoc de trong!");
    }
    this->shiftName = shiftName;
}

void AgentShift::setShiftDate(const string& shiftDate) {
    if (shiftDate.empty()) {
        throw invalid_argument("Ngay truc khong duoc de trong!");
    }
    this->shiftDate = shiftDate;
}

void AgentShift::setShiftStart(const string& shiftStart) {
    if (shiftStart.empty()) {
        throw invalid_argument("Gio bat dau khong duoc de trong!");
    }
    this->shiftStart = shiftStart;
}

void AgentShift::setShiftEnd(const string& shiftEnd) {
    if (shiftEnd.empty()) {
        throw invalid_argument("Gio ket thuc khong duoc de trong!");
    }
    this->shiftEnd = shiftEnd;
}

void AgentShift::addAgentId(const string& agentId) {
    if (agentId.empty()) {
        throw invalid_argument("Ma DTV khong duoc de trong!");
    }
    for (const auto& id : agentIds) {
        if (id == agentId) return; // Da co san trong ca truc
    }
    agentIds.push_back(agentId);
}

void AgentShift::printInfo() const {
    cout << "Ca truc ID: " << id
         << " | Ten: " << shiftName
         << " | Ngay: " << shiftDate
         << " | Gio: " << shiftStart << " - " << shiftEnd
         << " | So DTV: " << agentIds.size()
         << " | Trang thai: " << (status ? "Hoat dong" : "Tam ngung") << endl;
}

// Business methods
void AgentShift::activate() {
    status = true;
}

void AgentShift::deactivate() {
    status = false;
}
