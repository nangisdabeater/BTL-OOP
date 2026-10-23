#pragma once

#include <string>
#include "BaseEntity.h"

// Kế thừa từ BaseEntity để quản lý thống nhất mã ID (CAxxx) cho mỗi cuộc gọi tiếp nhận
class InboundCall : public BaseEntity {
private:
    std::string phoneNumber; // Số điện thoại của khách hàng gọi đến
    std::string channelId;   // Mã kênh tiếp nhận (khóa ngoại liên kết với ReceptionChannel, VD: CN001)
    std::string callTime;    // Thời điểm diễn ra cuộc gọi
    int duration;            // Thời lượng cuộc gọi tính bằng giây (>= 0)
    std::string status;      // Trạng thái cuộc gọi: ANSWERED (Đã nghe), MISSED (Nhỡ), BUSY (Máy bận)
    std::string note;        // Ghi chú tóm tắt nội dung cuộc gọi

public:
    // Hàm khởi tạo có tham số mặc định (ID sẽ được Repository cấp tự động dạng CAxxx khi gọi add)
    InboundCall(
        const std::string& phoneNumber = "",
        const std::string& channelId = "",
        const std::string& callTime = "",
        int duration = 0,
        const std::string& status = "ANSWERED",
        const std::string& note = ""
    );

    // Các hàm Getter lấy thông tin cuộc gọi (dùng const& để tránh copy chuỗi)
    const std::string& getPhoneNumber() const;
    const std::string& getChannelId() const;
    const std::string& getCallTime() const;
    int getDuration() const;
    const std::string& getStatus() const;
    const std::string& getNote() const;

    // Các hàm Setter cập nhật thông tin kèm kiểm tra dữ liệu hợp lệ
    void setPhoneNumber(const std::string& phoneNumber);
    void setChannelId(const std::string& channelId);
    void setCallTime(const std::string& callTime);
    void setDuration(int duration);
    void setStatus(const std::string& status);
    void setNote(const std::string& note);

    // Ghi đè phương thức thuần ảo từ BaseEntity để hiển thị thông tin cuộc gọi
    void printInfo() const override;
};
