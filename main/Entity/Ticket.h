#pragma once

#include <string>
#include "BaseEntity.h"

// Tại sao dùng "enum class TicketStatus" thay vì dùng kiểu chuỗi string hay enum thường?
// - Không dùng string: vì so sánh chuỗi chậm và dễ gõ sai chính tả (VD: "Open" khác với "OPEN").
// - Không dùng enum thường: vì các giá trị OPEN, CLOSED sẽ bị tràn ra phạm vi toàn cục dễ đụng độ tên với lớp khác,
//   và tự động bị ép kiểu ngầm sang số nguyên int gây mất an toàn.
// - Dùng enum class: bắt buộc gọi rõ "TicketStatus::OPEN", chống xung đột tên và an toàn kiểu tuyệt đối.
enum class TicketStatus {
    OPEN,        // Phiếu mới tạo, đang chờ tiếp nhận
    IN_PROGRESS, // Đang trong quá trình xử lý
    RESOLVED,    // Đã giải quyết xong sự cố
    CLOSED       // Đã đóng phiếu hoàn tất
};

class Ticket : public BaseEntity {
private:
    std::string title;          // Tiêu đề phiếu hỗ trợ (không được rỗng, tối đa 100 ký tự)
    std::string description;    // Mô tả chi tiết vấn đề của khách hàng
    // Tại sao lưu "serviceGroupId" dạng chuỗi (mã khóa ngoại) thay vì lưu cả đối tượng ServiceGroup bên trong Ticket?
    // Để giảm sự phụ thuộc chặt (Loose Coupling) và tránh việc dữ liệu bị nhân bản: nếu tên dịch vụ thay đổi,
    // chỉ cần sửa ở ServiceGroup mà không bị lệch dữ liệu trong các Ticket.
    std::string serviceGroupId; // Mã nhóm dịch vụ liên kết (VD: SM001)
    int priority;               // Mức độ ưu tiên: 1 (Thấp), 2 (Trung bình), 3 (Cao), 4 (Khẩn cấp)
    int estimatedFee;           // Chi phí xử lý ước tính (>= 0 VNĐ)
    std::string createdDate;    // Ngày tạo phiếu theo định dạng DD/MM/YYYY
    TicketStatus status;        // Trạng thái hiện tại của phiếu hỗ trợ

public:
    // Hàm khởi tạo có tham số mặc định (ID sẽ được Repository tự động cấp phát dạng TKxxx khi thêm vào kho)
    Ticket(
        const std::string& title = "",
        const std::string& description = "",
        const std::string& serviceGroupId = "",
        int priority = 2,
        int estimatedFee = 0,
        const std::string& createdDate = "01/01/2026"
    );

    // Các hàm Getter lấy thông tin thuộc tính (dùng const& cho chuỗi để không tốn bộ nhớ copy)
    const std::string& getTitle() const;
    const std::string& getDescription() const;
    const std::string& getServiceGroupId() const;
    int getPriority() const;
    int getEstimatedFee() const;
    const std::string& getCreatedDate() const;
    TicketStatus getStatus() const;

    // Các hàm Setter cập nhật thuộc tính kèm kiểm tra ràng buộc dữ liệu hợp lệ
    void setTitle(const std::string& title);
    void setDescription(const std::string& description);
    void setServiceGroupId(const std::string& serviceGroupId);
    void setPriority(int priority);
    void setEstimatedFee(int fee);
    void setCreatedDate(const std::string& date);

    // Ghi đè phương thức thuần ảo của lớp cha BaseEntity để in thông tin Ticket
    void printInfo() const override;

    // Thay đổi trạng thái xử lý của Ticket (OPEN, IN_PROGRESS, RESOLVED, CLOSED)
    void changeStatus(TicketStatus status);
};
