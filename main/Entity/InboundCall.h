#pragma once
#include <string>
#include "BaseEntity.h"


class InboundCall : public BaseEntity{
private:
    std::string phoneNumber;  
    std::string channelId;    
    std::string agentId;      
    std::string callTime;     
    int duration;        
    std::string status;       // Trạng thái cuộc gọi: ANSWERED (Thành công), MISSED (Nhỡ), BUSY (Bận máy)
    std::string note;         

public:
    // Constructor (ID sẽ được Repository cấp tự động dạng CAxxx khi gọi add)
    InboundCall(
        const std::string& phoneNumber = "",
        const std::string& channelId = "",
        const std::string& agentId = "",
        const std::string& callTime = "",
        int duration = 0,
        const std::string& status = "ANSWERED",
        const std::string& note = ""
    );

    // Getter
    const std::string& getPhoneNumber() const;
    const std::string& getChannelId() const;
    const std::string& getAgentId() const;
    const std::string& getCallTime() const;
    int getDuration() const;
    const std::string& getStatus() const;
    const std::string& getNote() const;

    // Setter (có kiểm tra dữ liệu hợp lệ)
    void setPhoneNumber(const std::string& phoneNumber);
    void setChannelId(const std::string& channelId);
    void setAgentId(const std::string& agentId);
    void setCallTime(const std::string& callTime);
    void setDuration(int duration);
    void setStatus(const std::string& status);
    void setNote(const std::string& note);

    // Triển khai phương thức thuần ảo từ BaseEntity
    void printInfo() const override;
};
