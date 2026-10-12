# BTL-OOP: Hệ Thống Quản Lý Trung Tâm Chăm Sóc Khách Hàng (CSKH)

Dự án Bài Tập Lớn môn Lập trình Hướng đối tượng (OOP).

## 🚀 Tính năng chính

Hệ thống quản lý trung tâm CSKH gồm 9 phân hệ chức năng hoàn chỉnh:
1. **Quản lý dịch vụ (Service Manager)**: Nhóm dịch vụ, SLA, phí cơ bản, ngày ban hành (`SMxxx`).
2. **Quản lý ticket**: Phiếu hỗ trợ khách hàng, liên kết dịch vụ, khách hàng, mức ưu tiên, ngày tạo (`TKxxx`).
3. **Quản lý điện thoại viên (Agent)**: Thông tin điện thoại viên, số máy lẻ, trạng thái hoạt động (`AGxxx`).
4. **Quản lý kênh tiếp nhận (Reception Channel)**: Hotline, Email, Fanpage, Trực tiếp (`CNxxx`).
5. **Quản lý cuộc gọi tiếp nhận (Inbound Call)**: Thời gian, thời lượng, trạng thái nghe máy/cuộc gọi nhỡ (`CAxxx`).
6. **Quản lý mức cam kết SLA**: Mức độ phản hồi và thời gian xử lý cam kết theo độ ưu tiên (`SLAxxx`).
7. **Quản lý phiếu chuyển xử lý (Transfer Ticket)**: Điều phối ticket giữa các nhân viên hỗ trợ (`TFxxx`).
8. **Quản lý khách hàng (Customer)**: Thông tin cá nhân, liên hệ, địa chỉ khách hàng (`KHxxx`).
9. **Quản lý đánh giá hài lòng (Feedback)**: Chấm điểm hài lòng (1-5 sao), ngày đánh giá, ý kiến phản hồi (`DGxxx`).

## 🛠️ Công nghệ & Kiến trúc
- **Ngôn ngữ**: C++17
- **Mô hình lập trình**: Hướng đối tượng (OOP - Đóng gói, Kế thừa, Đa hình, Trừu tượng).
- **Kiến trúc phân tầng**:
  - `Entity/`: Lớp thực thể kế thừa từ lớp cơ sở trừu tượng `BaseEntity`.
  - `Repository/`: Generic repository template `Repository<T>` quản lý dữ liệu trong bộ nhớ.
  - `FileManager/`: Template specialization `FileManager<T>` đọc/ghi dữ liệu dạng bảng căn lề (`data/*.txt`).
  - `app/`: Các lớp điều khiển Menu và giao diện người dùng console.
  - `Utilities/`: Bộ công cụ tiện ích xử lý chuỗi, định dạng bảng (`TableUtils`), kiểm tra ngày tháng (`DateTimeUtils`).

## 📁 Cấu trúc thư mục
```
├── CMakeLists.txt
├── .gitignore
├── README.md
├── data/                    # Thư mục chứa các tệp dữ liệu bảng (.txt)
│   ├── agents.txt
│   ├── channels.txt
│   ├── customers.txt
│   ├── feedback.txt
│   ├── inbound_calls.txt
│   ├── service_groups.txt
│   ├── sla_levels.txt
│   ├── tickets.txt
│   └── transfer_tickets.txt
└── main/                    # Mã nguồn chính của dự án
    ├── CMakeLists.txt
    ├── main.cpp
    ├── Entity/
    ├── FileManager/
    ├── Repository/
    ├── Utilities/
    └── app/
```

## 🔨 Hướng dẫn biên dịch và chạy
```bash
# Biên dịch chương trình
cmake --build build

# Chạy chương trình
./build/myprogram.exe
```
