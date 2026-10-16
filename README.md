# BÀI TẬP LỚN LẬP TRÌNH HƯỚNG ĐỐI TƯỢNG (BTL OOP)
## ĐỀ TÀI: HỆ THỐNG QUẢN LÝ TRUNG TÂM CHĂM SÓC KHÁCH HÀNG (CSKH)
(tạo ra bởi Gehihi)
---

## 1. GIỚI THIỆU DỰ ÁN
Dự án phần mềm quản lý trung tâm chăm sóc khách hàng được xây dựng bằng ngôn ngữ C++ (chuẩn C++17), áp dụng đầy đủ 4 tính chất cốt lõi của **Lập trình Hướng đối tượng (OOP)**:
- **Đóng gói (Encapsulation):** Toàn bộ thuộc tính khai báo `protected`/`private`, truy cập và biến đổi qua getter/setter có kiểm tra tính hợp lệ dữ liệu.
- **Kế thừa (Inheritance):** Tất cả các thực thể nghiệp vụ đều kế thừa từ lớp cơ sở chung `BaseEntity`.
- **Trừu tượng (Abstraction):** Lớp `BaseEntity` chứa phương thức thuần ảo `virtual void printInfo() const = 0`, ép buộc các lớp dẫn xuất phải triển khai chi tiết thông tin thực thể.
- **Đa hình (Polymorphism):** Sử dụng đa hình qua con trỏ/tham chiếu `BaseEntity` để hiển thị thông tin (`Utils::printEntityDetails`).

Hệ thống cung cấp cơ chế lưu trữ tự động (Auto-save) và nạp dữ liệu từ thư mục `data/`, đảm bảo tính toàn vẹn và độc lập với vị trí thực thi chương trình.

---

## 2. PHÂN CHIA NHIỆM VỤ THÀNH VIÊN

### Bảng 1: Phân chia nghiệp vụ (Use Cases)

| STT | Thành viên | Nghiệp vụ phụ trách (Use Case 1) | Nghiệp vụ phụ trách (Use Case 2) | Vai trò chính |
| :---: | :--- | :--- | :--- | :--- |
| 1 | **Năng** *(Trưởng nhóm)* | **Nhóm dịch vụ** (*ServiceGroup*) | **Ticket yêu cầu** (*Ticket*) | Kiến trúc hệ thống + Tích hợp |
| 2 | **Quang Hiếu** | **Kênh tiếp nhận** (*ReceptionChannel*) | **Cuộc gọi tiếp nhận** (*InboundCall*) | Nghiệp vụ tiếp nhận & Cuộc gọi |
| 3 | **Cường** | **Mức cam kết SLA** (*SLALevel*) | **Phiếu chuyển xử lý** (*TransferTicket*) | Quản lý SLA & Luân chuyển xử lý ticket |
| 4 | **Chí Linh** | **Khách hàng** (*Customer*) | **Phiếu đánh giá hài lòng** (*Feedback*) | Quản lý khách hàng & Đánh giá dịch vụ |
| 5 | **Thành** | **Điện thoại viên** (*Agent*) | **Ca trực** (*AgentShift*) | Quản lý nhân sự & Lịch trực |

---

### Bảng 2: Chi tiết nhiệm vụ kỹ thuật và Module phụ trách

#### 1. Năng (Trưởng nhóm) - Kiến trúc + Tích hợp
- **Xây dựng kiến trúc OOP cốt lõi:**
  - Thiết kế lớp cơ sở trừu tượng `BaseEntity` và phương thức thuần ảo `printInfo()`.
  - Thiết kế và cài đặt lớp khuôn mẫu Generic `Repository<T>` hỗ trợ đầy đủ thao tác CRUD (Create, Read, Update, Delete) và tự động sinh mã ID theo tiền tố (`SM`, `TK`,...).
- **Khung ứng dụng & Điều phối:**
  - Thiết kế lớp điều phối trung tâm `Application` (`App.h`, `App.cpp`) và `main.cpp`.
  - Xây dựng giao diện menu chính (`showMainMenu`) và liên kết điều hướng 10 phân hệ nghiệp vụ.
- **Tích hợp & Quản trị Git:**
  - Thiết lập dự án với CMake, quản trị nhánh Git, giải quyết xung đột mã nguồn và tích hợp toàn hệ thống.

#### 2. Quang Hiếu - Tiện ích nhập liệu & Kiểm tra dữ liệu (Validator)
- **Xây dựng bộ công cụ nhập liệu an toàn (`Utilities`):**
  - Hàm nhập chuỗi không rỗng (`Utils::inputNonEmptyString`).
  - Hàm nhập số nguyên (`Utils::inputInt`) và số thực (`Utils::inputDouble`) có giới hạn khoảng giá trị.
- **Bộ kiểm tra hợp lệ dữ liệu (Validator):**
  - Kiểm tra và định dạng mã định danh (`Utils::formatId`, `Utils::extractIdNumber`).
  - Xử lý cắt tỉa khoảng trắng chuỗi (`Utils::trim`).
  - Hàm mẫu kiểm tra trùng lặp tổng quát (`ValidationUtils::isDuplicate`).
- **Xử lý ngoại lệ nhập liệu (Exception Handling):**
  - Sử dụng `try/catch/throw` để bắt lỗi nhập liệu sai kiểu từ người dùng, làm sạch bộ đệm `cin` tránh lặp vô tận.

#### 3. Cường - Quản lý File (FileManager & Data Parsing)
- **Thiết kế hệ thống lưu trữ dữ liệu (`FileManager`):**
  - Chuyên biệt hóa lớp mẫu `FileManager<T>` (Template Specialization) cho từng thực thể.
  - Xây dựng các hàm nạp dữ liệu (`load`) và lưu dữ liệu (`save`).
- **Xử lý phân tích và định dạng dữ liệu:**
  - Đọc và bóc tách từng dòng file văn bản phân cách bằng ký tự `|`.
  - Tự động tạo file và thư mục cha nếu file chưa tồn tại.
  - Bắt lỗi và bỏ qua an toàn các dòng dữ liệu bị sai định dạng mà không làm gián đoạn chương trình.

#### 4. Chí Linh - Tiện ích Ngày tháng & Định dạng Hiển thị
- **Xử lý Ngày tháng (`Date/Time Utilities`):**
  - Kiểm tra tính hợp lệ của ngày tháng theo định dạng `DD/MM/YYYY` (`Utils::isValidDate`), xử lý năm nhuận và số ngày chuẩn của từng tháng.
  - Lấy ngày tháng hiện tại của hệ thống (`Utils::today`).
  - Hàm nhập ngày tháng có kiểm tra hợp lệ (`Utils::inputDateString`).
- **Định dạng hiển thị giao diện bảng:**
  - Sử dụng thư viện `<iomanip>` (`setw`, `left`, `setfill`, `setprecision`) để căn chỉnh các cột dữ liệu thẳng hàng, phân cách rõ ràng.
  - Hàm nối danh sách chuỗi (`Utils::join`).

#### 5. Thành - Dữ liệu mẫu, Ràng buộc nghiệp vụ & Kiểm thử
- **Xây dựng bộ dữ liệu mẫu:**
  - Chuẩn bị đầy đủ 10 file dữ liệu chuẩn định dạng ban đầu trong thư mục `data/` (`agents.txt`, `customers.txt`, `tickets.txt`,...).
- **Kiểm tra ràng buộc và quan hệ giữa các thực thể:**
  - Kiểm tra tính tồn tại của Dịch vụ (`ServiceGroup`) và Khách hàng (`Customer`) khi tạo Ticket.
  - Kiểm tra sự tồn tại của Điện thoại viên (`Agent`) khi tiếp nhận cuộc gọi, phân công xử lý ticket hoặc xếp ca trực.
- **Kiểm thử tổng hợp & Hỗ trợ tích hợp:**
  - Kiểm thử toàn bộ các kịch bản CRUD trên cả 10 phân hệ menu.
  - Đảm bảo cơ chế tự động đồng bộ dữ liệu giữa bộ nhớ và file văn bản hoạt động chính xác.

---

## 3. CẤU TRÚC THƯ MỤC DỰ ÁN

```text
BTL-OOP/
├── .vscode/               # Cấu hình biên dịch và môi trường VS Code
│   └── settings.json
├── data/                  # Thư mục lưu trữ dữ liệu văn bản (.txt)
│   ├── agents.txt
│   ├── agent_shifts.txt
│   ├── channels.txt
│   ├── customers.txt
│   ├── feedback.txt
│   ├── inbound_calls.txt
│   ├── service_groups.txt
│   ├── sla_levels.txt
│   ├── tickets.txt
│   └── transfer_tickets.txt
├── main/                  # Mã nguồn chính của chương trình
│   ├── app/               # Lớp Application và các Menu giao diện
│   │   ├── App.h / App.cpp
│   │   ├── AgentMenu.h / AgentMenu.cpp
│   │   ├── ServiceGroupMenu.h / ServiceGroupMenu.cpp
│   │   ├── TicketMenu.h / TicketMenu.cpp
│   │   └── ... (các Menu khác)
│   ├── Entity/            # Các lớp thực thể (Kế thừa BaseEntity)
│   │   ├── BaseEntity.h
│   │   ├── Agent.h / Agent.cpp
│   │   ├── Customer.h / Customer.cpp
│   │   ├── ServiceGroup.h / ServiceGroup.cpp
│   │   ├── Ticket.h / Ticket.cpp
│   │   └── ... (các Entity khác)
│   ├── FileManager/       # Đọc và ghi file dữ liệu
│   │   ├── FileManager.h
│   │   ├── AgentFileManager.cpp
│   │   └── ... (các FileManager chuyên biệt)
│   ├── Repository/        # Khuôn mẫu Repository CRUD
│   │   └── Repository.h
│   ├── Utilities/         # Các hàm tiện ích dùng chung
│   │   ├── Utilities.h / Utilities.cpp
│   ├── CMakeLists.txt     # Cấu hình build cho thư mục main
│   └── main.cpp           # Điểm khởi đầu chương trình
├── .gitignore             # Khai báo các file bỏ qua không commit lên Git
├── CMakeLists.txt         # Cấu hình CMake gốc
└── README.md              # Tài liệu dự án
```

---

## 4. HƯỚNG DẪN BIÊN DỊCH VÀ CHẠY CHƯƠNG TRÌNH

### Yêu cầu môi trường
- Trình biên dịch C++ hỗ trợ C++17 (GCC / MinGW-w64, Clang hoặc MSVC).
- Công cụ CMake (phiên bản >= 3.15).

### Các bước thực hiện qua dòng lệnh (Terminal):

1. **Tạo cấu hình build với CMake:**
   ```bash
   cmake -S main -B build -G "MinGW Makefiles"
   ```
2. **Biên dịch dự án:**
   ```bash
   cmake --build build
   ```
3. **Chạy chương trình:**
   ```bash
   ./build/myprogram.exe
   ```
*(Lưu ý: Nhờ cơ chế tự động tìm gốc dự án, bạn có thể thực thi file từ bất cứ thư mục nào mà dữ liệu vẫn được đọc/ghi chính xác vào `data/`).*
