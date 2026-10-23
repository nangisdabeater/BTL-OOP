#include "ServiceGroup.h"
#include <iostream>
#include <iomanip>
#include <stdexcept>

using namespace std;

ServiceGroup::ServiceGroup(
    const string& name,
    const string& description,
    const string& department,
    int slaHours,
    int baseFee,
    const string& createdDate
)
    // Tại sao dùng Danh sách khởi tạo thành viên (Member Initializer List) ": BaseEntity(), name(name)..."
    // thay vì gán "this->name = name;" bên trong cặp ngoặc nhọn {}?
    // Vì nếu gán trong thân hàm {}, trình biên dịch sẽ phải tạo các biến string rỗng trước rồi mới gán đè giá trị lên (tốn 2 bước).
    // Dùng danh sách khởi tạo giúp truyền thẳng giá trị ngay lúc tạo biến (chỉ mất 1 bước), tối ưu hiệu năng hơn.
    : BaseEntity(),
      name(name),
      description(description),
      department(department),
      slaHours(slaHours),
      baseFee(baseFee),
      createdDate(createdDate),
      active(true) {} // Mặc định khi mới tạo thì dịch vụ ở trạng thái đang hoạt động (true)

const string& ServiceGroup::getName() const {
    return name; // Trả về tham chiếu hằng tới tên dịch vụ
}

const string& ServiceGroup::getDescription() const {
    return description; // Trả về tham chiếu hằng tới mô tả dịch vụ
}

const string& ServiceGroup::getDepartment() const {
    return department; // Trả về tham chiếu hằng tới phòng ban phụ trách
}

int ServiceGroup::getSlaHours() const {
    return slaHours; // Với kiểu nguyên thủy int có kích thước nhỏ (4 byte), trả về trực tiếp giá trị nhanh hơn tham chiếu
}

int ServiceGroup::getBaseFee() const {
    return baseFee;
}

const string& ServiceGroup::getCreatedDate() const {
    return createdDate;
}

bool ServiceGroup::isActive() const {
    return active;
}

void ServiceGroup::setName(const string& name) {
    // Kiểm tra tên rỗng hoặc quá 100 ký tự thì ném ngoại lệ để chặn dữ liệu sai ngay lập tức
    if (name.empty()) {
        throw invalid_argument("Ten dich vu khong duoc de trong!");
    }
    if (name.length() > 100) {
        throw invalid_argument("Ten dich vu khong duoc vuot qua 100 ky tu!");
    }
    // Dùng con trỏ "this->" để phân biệt thuộc tính "name" của lớp với tham số "name" truyền vào hàm
    this->name = name;
}

void ServiceGroup::setDescription(const string& description) {
    if (description.empty()) {
        throw invalid_argument("Mo ta dich vu khong duoc de trong!");
    }
    this->description = description;
}

void ServiceGroup::setDepartment(const string& department) {
    if (department.empty()) {
        throw invalid_argument("Phong ban phu trach khong duoc de trong!");
    }
    this->department = department;
}

void ServiceGroup::setSlaHours(int slaHours) {
    // Thời gian cam kết xử lý bắt buộc phải là số nguyên dương (> 0)
    if (slaHours <= 0) {
        throw invalid_argument("Thoi gian xu ly SLA phai lon hon 0 gio!");
    }
    this->slaHours = slaHours;
}

void ServiceGroup::setBaseFee(int baseFee) {
    // Chi phí dịch vụ có thể bằng 0 (miễn phí) nhưng không được phép là số âm
    if (baseFee < 0) {
        throw invalid_argument("Phi dich vu co ban phai lon hon hoac bang 0!");
    }
    this->baseFee = baseFee;
}

void ServiceGroup::setCreatedDate(const string& createdDate) {
    if (createdDate.empty()) {
        throw invalid_argument("Ngay ban hanh khong duoc de trong!");
    }
    this->createdDate = createdDate;
}

void ServiceGroup::printInfo() const {
    // Truy cập trực tiếp thuộc tính "id" của lớp cha BaseEntity vì "id" được khai báo là protected
    cout << "Service ID: " << id 
         << " | Ten: " << name 
         << " | Phong ban: " << department
         << " | SLA: " << slaHours << "h"
         << " | Phi co ban: " << baseFee
         << " | Ngay ban hanh: " << createdDate
         << " | Trang thai: " << (active ? "Hoat dong" : "Tam ngung") << endl;
}

void ServiceGroup::activate() {
    active = true; // Chuyển trạng thái sang đang hoạt động
}

void ServiceGroup::deactivate() {
    active = false; // Chuyển trạng thái sang tạm ngưng
}
