/**
 * @file AgentShift.cpp
 * @brief Triển khai các phương thức của lớp thực thể Ca trực (AgentShift)
 */

#include "AgentShift.h"
#include <iostream>
#include <stdexcept>
#include <algorithm>

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
{} 

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
    throw invalid_argument("Ngay ca truc khong duoc de trong!");
  }
  this->shiftDate = shiftDate;
}

void AgentShift::setShiftStart(const string& shiftStart) {
  if (shiftStart.empty()) {
    throw invalid_argument("Gio bat dau ca truc khong duoc de trong!");
  }
  this->shiftStart = shiftStart;
}

void AgentShift::setShiftEnd(const string& shiftEnd) {
  if (shiftEnd.empty()) {
    throw invalid_argument("Gio ket thuc ca truc khong duoc de trong!");
  }
  this->shiftEnd = shiftEnd;
}

void AgentShift::setAgentIds(const vector<string>& agentIds) {
  this->agentIds = agentIds;
}

void AgentShift::addAgentId(const string& agentId) 
{
    if (agentId.empty()) {
        throw invalid_argument("ID nhan vien khong duoc de trong!");
    }
    for (const auto& existingId : agentIds) {
        if (existingId == agentId) {
            throw invalid_argument("Nhan vien nay da co trong ca truc!");
        }
    }
    agentIds.push_back(agentId);
}

void AgentShift::removeAgentId(const string& agentId) {
  auto it = std::remove(agentIds.begin(), agentIds.end(), agentId);

  if (it != agentIds.end()) {
    agentIds.erase(it, agentIds.end());
  } else {
    throw invalid_argument("Nhan vien nay khong co trong ca truc!");
  }
}

void AgentShift::printInfo() const {
  cout << "Ca truc ID: " << id
       << " | Ten ca: " << shiftName
       << " | Ngay: " << shiftDate
       << " | Bat dau: " << shiftStart
       << " | Ket thuc: " << shiftEnd
       << " | Trang thai: " << (status ? "Hoat dong" : "Tam ngung")
       << " | So DTV: " << agentIds.size() << endl;
  if (!agentIds.empty()) {
    cout << "  -> DS ma nhan vien: ";
    for (size_t i = 0; i < agentIds.size(); ++i) {
      cout << agentIds[i] << (i + 1 < agentIds.size() ? ", " : "");
    }
    cout << endl;
  }
}

// Business methods
void AgentShift::activate() {
  status = true;
}

void AgentShift::deactivate() {
  status = false;
}