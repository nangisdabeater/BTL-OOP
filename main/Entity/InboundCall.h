#pragma once
/**
    Định nghĩa thực thể Cuộc gọi tiếp nhận (InboundCall)
 * [PHẦN THÊM MỚI]
 * Đại diện cho 1 cuộc gọi đến từ khách hàng tới trung tâm CSKH.
 * Thuộc tính:
 *   - Mã ID: Kế thừa BaseEntity, định dạng tự động CA001, CA002,...
 *   - Số điện thoại khách gọi (phoneNumber)
 *   - Mã kênh tiếp nhận (channelId - khóa ngoại liên kết ReceptionChannel, VD: CN001)
 *   - Mã nhân viên trực tiếp nhận (agentId - khóa ngoại liên kết Agent, VD: AG001)
 *   - Thời gian cuộc gọi (callTime)
 *   - Thời lượng cuộc gọi (duration: tính theo giây)
 *   - Trạng thái cuộc gọi (status: ANSWERED / MISSED / BUSY)
 *   - Ghi chú tóm tắt (note)
 */

#include <string>
#include "BaseEntity.h"


class InboundCall : public BaseEntity{
private:
    std::string phoneNumber;  
    std::string channelId;    
    std::string callTime;     
    int duration;        
    std::string status;       // Trạng thái cuộc gọi: ANSWERED (Thành công), MISSED (Nhỡ), BUSY (Bận máy)
    std::string note;         

public:
    // Constructor (ID sẽ được Repository cấp tự động dạng CAxxx khi gọi add)
    InboundCall(
        const std::string& phoneNumber = "",
        const std::string& channelId = "",
        const std::string& callTime = "",
        int duration = 0,
        const std::string& status = "ANSWERED",
        const std::string& note = ""
    );

    // Getter
    const std::string& getPhoneNumber() const;
    const std::string& getChannelId() const;
    const std::string& getCallTime() const;
    int getDuration() const;
    const std::string& getStatus() const;
    const std::string& getNote() const;

    // Setter (có kiểm tra dữ liệu hợp lệ)
    void setPhoneNumber(const std::string& phoneNumber);
    void setChannelId(const std::string& channelId);
    void setCallTime(const std::string& callTime);
    void setDuration(int duration);
    void setStatus(const std::string& status);
    void setNote(const std::string& note);

    // Triển khai phương thức thuần ảo từ BaseEntity
    void printInfo() const override;
};
