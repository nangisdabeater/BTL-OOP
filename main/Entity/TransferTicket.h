#pragma once

#include <string>
#include "BaseEntity.h"

// Dùng enum class cho trạng thái phiếu chuyển tuyến để tránh xung đột tên toàn cục và đảm bảo an toàn kiểu
enum class TransferStatus {
    PENDING,     // Đang chờ bộ phận mới tiếp nhận
    ACCEPTED,    // Bộ phận mới đã tiếp nhận xử lý
    COMPLETED    // Đã hoàn tất xử lý
};

class TransferTicket : public BaseEntity {
private:
    std::string slaId;       // Mã mức cam kết SLA áp dụng (khóa ngoại liên kết với SLALevel, VD: SL001)
    std::string fromDept;    // Bộ phận chuyển đi
    std::string toDept;      // Bộ phận tiếp nhận xử lý
    std::string reason;      // Lý do chuyển tuyến xử lý
    TransferStatus status;   // Trạng thái hiện tại của phiếu chuyển

public:
    // Hàm khởi tạo có tham số mặc định (ID sẽ được Repository tự động cấp dạng TFxxx khi thêm mới)
    TransferTicket(
        const std::string& slaId = "",
        const std::string& fromDept = "",
        const std::string& toDept = "",
        const std::string& reason = ""
    );

    // Các hàm Getter lấy thông tin phiếu chuyển
    const std::string& getSlaId() const;
    const std::string& getFromDept() const;
    const std::string& getToDept() const;
    const std::string& getReason() const;
    TransferStatus getStatus() const;

    // Các hàm Setter cập nhật thông tin kèm kiểm tra không được để trống
    void setSlaId(const std::string& slaId);
    void setFromDept(const std::string& fromDept);
    void setToDept(const std::string& toDept);
    void setReason(const std::string& reason);

    // Ghi đè phương thức thuần ảo từ BaseEntity để hiển thị thông tin phiếu chuyển
    void printInfo() const override;

    // Chuyển trạng thái theo quy trình một chiều: PENDING -> ACCEPTED -> COMPLETED (không cho phép lùi trạng thái)
    void changeStatus(TransferStatus newStatus);

    // Khôi phục trạng thái trực tiếp khi đọc dữ liệu từ file (không kiểm tra chiều tiến trình)
    void restoreStatus(TransferStatus s);
};
