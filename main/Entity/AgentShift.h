#pragma once 

/**
 * @file AgentShift.h
 * @brief Định nghĩa thực thể Ca trực (Agent Shift)
 * Đại diện cho ca trực với các thuộc tính: mã ID, tên ca trực, 
 * ngày trực, giờ bắt đầu, giờ kết thúc, danh sách ID nhân viên trực.
 */

#include <string>
#include <vector>
#include "BaseEntity.h"

class AgentShift : public BaseEntity {
  private: 
    std::string shiftName;
    std::string shiftDate;
    std::string shiftStart;
    std::string shiftEnd;
    std::vector<std::string> agentIds;
    bool status;
  public:
    AgentShift(
      const std::string& shiftName,
      const std::string& shiftDate,
      const std::string& shiftStart,
      const std::string& shiftEnd
    );
    // Getter
    const std::string& getShiftName() const;
    const std::string& getShiftDate() const;
    const std::string& getShiftStart() const;
    const std::string& getShiftEnd() const;
    const std::vector<std::string>& getAgentIds() const;
    bool isActive() const;

    // Setter
    void setShiftName(const std::string& shiftName);
    void setShiftDate(const std::string& shiftDate);
    void setShiftStart(const std::string& shiftStart);
    void setShiftEnd(const std::string& shiftEnd);
    void setAgentIds(const std::vector<std::string>& agentIds);
    void addAgentId(const std::string& agentId);
    void removeAgentId(const std::string& agentId);

    void printInfo() const override;

    // Business methods
    void activate();
    void deactivate();
};