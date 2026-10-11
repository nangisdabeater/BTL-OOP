#pragma once

/**
 * @file InboundCall.h
 * @brief Định nghĩa thực thể Cuộc gọi tiếp nhận (InboundCall)
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

using namespace std;

class InboundCall : public BaseEntity
{
private:
    string phoneNumber;  // Số điện thoại người gọi
    string channelId;    // Mã kênh tiếp nhận (VD: CN001)
    string agentId;      // Mã điện thoại viên tiếp nhận (VD: AG001)
    string callTime;     // Thời điểm cuộc gọi (VD: "2026-10-11 14:30")
    int duration;        // Thời lượng cuộc gọi (giây)
    string status;       // Trạng thái cuộc gọi: ANSWERED (Thành công), MISSED (Nhỡ), BUSY (Bận máy)
    string note;         // Ghi chú nội dung cuộc gọi

public:
    // Constructor (ID sẽ được Repository cấp tự động dạng CAxxx khi gọi add)
    InboundCall(
        const string& phoneNumber = "",
        const string& channelId = "",
        const string& agentId = "",
        const string& callTime = "",
        int duration = 0,
        const string& status = "ANSWERED",
        const string& note = ""
    );

    // Getter
    const string& getPhoneNumber() const;
    const string& getChannelId() const;
    const string& getAgentId() const;
    const string& getCallTime() const;
    int getDuration() const;
    const string& getStatus() const;
    const string& getNote() const;

    // Setter (có kiểm tra dữ liệu hợp lệ)
    void setPhoneNumber(const string& phoneNumber);
    void setChannelId(const string& channelId);
    void setAgentId(const string& agentId);
    void setCallTime(const string& callTime);
    void setDuration(int duration);
    void setStatus(const string& status);
    void setNote(const string& note);

    // Triển khai phương thức thuần ảo từ BaseEntity
    void printInfo() const override;
};
