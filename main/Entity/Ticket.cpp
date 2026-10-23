#include "Ticket.h"
#include <iostream>
#include <stdexcept>

using namespace std;

Ticket::Ticket(
    const string& title,
    const string& description,
    const string& serviceGroupId,
    int priority,
    int estimatedFee,
    const string& createdDate
)
    // Dùng danh sách khởi tạo (Member Initializer List) để gán trực tiếp các thuộc tính ngay lúc cấp phát bộ nhớ,
    // tránh việc phải tạo chuỗi rỗng rồi mới gán lại trong thân hàm. Mặc định Ticket mới tạo có trạng thái là OPEN.
    : BaseEntity(),
      title(title),
      description(description),
      serviceGroupId(serviceGroupId),
      priority(priority),
      estimatedFee(estimatedFee),
      createdDate(createdDate),
      status(TicketStatus::OPEN) {}

const string& Ticket::getTitle() const {
    return title; // Trả về tham chiếu hằng tới tiêu đề ticket
}

const string& Ticket::getDescription() const {
    return description; // Trả về tham chiếu hằng tới mô tả chi tiết
}

const string& Ticket::getServiceGroupId() const {
    return serviceGroupId; // Trả về mã nhóm dịch vụ (khóa ngoại)
}

int Ticket::getPriority() const {
    return priority; // Trả về mức độ ưu tiên (1 -> 4)
}

int Ticket::getEstimatedFee() const {
    return estimatedFee; // Trả về chi phí xử lý ước tính
}

const string& Ticket::getCreatedDate() const {
    return createdDate; // Trả về ngày tạo ticket
}

TicketStatus Ticket::getStatus() const {
    return status; // Trả về trạng thái hiện tại của ticket
}

void Ticket::setTitle(const string& title) {
    // Chặn lỗi tiêu đề rỗng hoặc dài quá 100 ký tự bằng cách ném ngoại lệ invalid_argument
    if (title.empty()) {
        throw invalid_argument("Tieu de ticket khong duoc de trong!");
    }
    if (title.length() > 100) {
        throw invalid_argument("Tieu de ticket khong duoc vuot qua 100 ky tu!");
    }
    this->title = title;
}

void Ticket::setDescription(const string& description) {
    if (description.empty()) {
        throw invalid_argument("Mo ta ticket khong duoc de trong!");
    }
    this->description = description;
}

void Ticket::setServiceGroupId(const string& serviceGroupId) {
    if (serviceGroupId.empty()) {
        throw invalid_argument("Ma dich vu khong duoc de trong!");
    }
    this->serviceGroupId = serviceGroupId;
}

void Ticket::setPriority(int priority) {
    // Chỉ cho phép mức độ ưu tiên nằm trong khoảng từ 1 (Thấp) đến 4 (Khẩn cấp)
    if (priority < 1 || priority > 4) {
        throw invalid_argument("Muc do uu tien phai tu 1 (Thap) den 4 (Khan cap)!");
    }
    this->priority = priority;
}

void Ticket::setEstimatedFee(int fee) {
    // Chi phí ước tính phải lớn hơn hoặc bằng 0
    if (fee < 0) {
        throw invalid_argument("Chi phi xu ly uoc tinh phai lon hon hoac bang 0!");
    }
    this->estimatedFee = fee;
}

void Ticket::setCreatedDate(const string& date) {
    if (date.empty()) {
        throw invalid_argument("Ngay tao ticket khong duoc de trong!");
    }
    this->createdDate = date;
}

void Ticket::printInfo() const {
    // Chuyển số mức ưu tiên (1-4) sang chữ để người dùng dễ đọc trên màn hình
    string prioStr;
    switch (priority) {
        case 1: prioStr = "Thap"; break;
        case 2: prioStr = "Trung binh"; break;
        case 3: prioStr = "Cao"; break;
        case 4: prioStr = "Khan cap"; break;
        default: prioStr = to_string(priority); break;
    }

    // Chuyển giá trị enum class TicketStatus sang chuỗi tương ứng để in ra màn hình
    string stStr;
    switch (status) {
        case TicketStatus::OPEN: stStr = "OPEN"; break;
        case TicketStatus::IN_PROGRESS: stStr = "IN_PROGRESS"; break;
        case TicketStatus::RESOLVED: stStr = "RESOLVED"; break;
        case TicketStatus::CLOSED: stStr = "CLOSED"; break;
    }

    cout << "Ticket ID: " << id 
         << " | Tieu de: " << title 
         << " | Ma DV: " << serviceGroupId
         << " | Uu tien: " << prioStr
         << " | Chi phi: " << estimatedFee
         << " | Ngay tao: " << createdDate
         << " | Trang thai: " << stStr << endl;
}

void Ticket::changeStatus(TicketStatus status) {
    this->status = status; // Cập nhật trạng thái mới cho ticket
}
