/**
 Triển khai các phương thức của lớp thực thể Kênh tiếp nhận (ReceptionChannel)
 * [PHẦN THÊM MỚI]
 */

#include "ReceptionChannel.h"
#include <iostream>
#include <stdexcept>

using namespace std;

// Khởi tạo kênh tiếp nhận mặc định trạng thái hoạt động là true
ReceptionChannel::ReceptionChannel(
    const string& name,
    const string& description)
    : BaseEntity(),
      name(name),
      description(description),
      active(true){}

// GETTER

const string& ReceptionChannel::getName() const{
    return name;
}

const string& ReceptionChannel::getDescription() const{
    return description;
}

bool ReceptionChannel::isActive() const{
    return active;
}

// SETTER (Có ràng buộc không rỗng)

void ReceptionChannel::setName(const string& name){
    if (name.empty()) {
        throw invalid_argument("Ten kenh tiep nhan khong duoc de trong!");
    }
    this->name = name;
}

void ReceptionChannel::setDescription(const string& description){
    if (description.empty()) {
        throw invalid_argument("Mo ta kenh tiep nhan khong duoc de trong!");
    }
    this->description = description;
}

// PHƯƠNG THỨC HIỂN THỊ THÔNG TIN

void ReceptionChannel::printInfo() const
{
    cout << "Kenh tiep nhan ID: " << id 
         << " | Ten: " << name 
         << " | Mo ta: " << description 
         << " | Trang thai: " << (active ? "Hoat dong" : "Ngung hoat dong") << endl;
}

// CÁC HÀM NGHIỆP VỤ BẬT / TẮT

void ReceptionChannel::activate(){
    active = true;
}

void ReceptionChannel::deactivate(){
    active = false;
}