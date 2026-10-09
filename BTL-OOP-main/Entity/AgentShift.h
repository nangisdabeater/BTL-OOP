#pragma once 

/**
 * @file ServiceGroup.h
 * @brief Định nghĩa thực thể Ca trực (Agent Shift)
 * Đại diện cho 1 nhóm dịch vụ với các thuộc tính: mã ID, tên ca trực, 
 * ngày trực, giờ bắt đầu, giờ kết thúc, id nhân viên trực.
 */

#include <string>
#include <vector>
#include "BaseEntity.h"

class AgentShift : public BaseEntity {
  private: 
    string shiftName;
    string shiftDate;
    string shiftStart;
    string shiftEnd;
    vector<string> agentIds;
    bool status;
  public:
    AgentShift(
      const string& shiftName,
      const string& shiftDate,
      const string& shiftStart,
      const string& shiftEnd
    );
    // Getter
    const string& getShiftName() const;
    const string& getShiftDate() const;
    const string& getShiftStart() const;
    const string& getShiftEnd() const;
    const vector<string>& getAgentIds() const;

    // Setter
    void setShiftName(const string& shiftName);
    void setShiftDate(const string& shiftDate);
    void setShiftStart(const string& shiftStart);
    void setShiftEnd(const string& shiftEnd);

    void printInfo() const override;

    // Business methods
    void activate();
    void deactivate();
};