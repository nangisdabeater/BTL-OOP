#include "SLALevel.h"
#include <iostream>
#include <stdexcept>

using namespace std;

SLALevel::SLALevel(const string& name, int responseMinutes, int resolveHours, int priority)
    : BaseEntity(), name(""), responseMinutes(1), resolveHours(1), priority(5) {
    // Gọi qua các hàm setter ngay trong hàm khởi tạo để tái sử dụng chung logic kiểm tra điều kiện hợp lệ
    if (!name.empty()) {
        setName(name);
        setResponseMinutes(responseMinutes);
        setResolveHours(resolveHours);
        setPriority(priority);
    }
}

const string& SLALevel::getName() const { return name; }                 // Trả về tên mức SLA
int SLALevel::getResponseMinutes() const { return responseMinutes; }     // Trả về thời gian phản hồi (phút)
int SLALevel::getResolveHours() const { return resolveHours; }           // Trả về thời gian xử lý (giờ)
int SLALevel::getPriority() const { return priority; }                   // Trả về mức ưu tiên (1 -> 5)

void SLALevel::setName(const string& name) {
    if (name.empty()) {
        throw invalid_argument("Ten muc SLA khong duoc de trong!");
    }
    this->name = name;
}

void SLALevel::setResponseMinutes(int minutes) {
    // Thời gian cam kết phản hồi bắt buộc phải lớn hơn 0 phút
    if (minutes <= 0) {
        throw invalid_argument("Thoi gian phan hoi phai lon hon 0!");
    }
    this->responseMinutes = minutes;
}

void SLALevel::setResolveHours(int hours) {
    // Thời gian cam kết xử lý bắt buộc phải lớn hơn 0 giờ
    if (hours <= 0) {
        throw invalid_argument("Thoi gian xu ly phai lon hon 0!");
    }
    this->resolveHours = hours;
}

void SLALevel::setPriority(int priority) {
    // Mức độ ưu tiên chỉ nhận giá trị từ 1 đến 5
    if (priority < 1 || priority > 5) {
        throw invalid_argument("Muc do uu tien phai tu 1 den 5!");
    }
    this->priority = priority;
}

void SLALevel::printInfo() const {
    // Hiển thị thông tin chi tiết của mức cam kết SLA ra màn hình
    cout << "SLALevel ID: " << id
         << " | Ten: " << name
         << " | TG phan hoi: " << responseMinutes << " phut"
         << " | TG xu ly: " << resolveHours << " gio"
         << " | Uu tien: " << priority << endl;
}
