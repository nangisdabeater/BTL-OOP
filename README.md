# BÀI TẬP LỚN LẬP TRÌNH HƯỚNG ĐỐI TƯỢNG (BTL OOP)
## ĐỀ TÀI: HỆ THỐNG QUẢN LÝ TRUNG TÂM CHĂM SÓC KHÁCH HÀNG (CSKH)
---

## 1. GIỚI THIỆU DỰ ÁN
Dự án phần mềm quản lý trung tâm chăm sóc khách hàng được xây dựng bằng ngôn ngữ C++ (chuẩn C++11 trở lên), áp dụng đầy đủ 4 tính chất lập trình (OOP)**:
- **Đóng gói:** Toàn bộ thuộc tính khai báo `protected`/`private`, truy cập và biến đổi qua getter/setter có kiểm tra tính hợp lệ dữ liệu.
- **Kế thừa:** Tất cả các thực thể nghiệp vụ đều kế thừa từ lớp cơ sở chung `BaseEntity`.
- **Trừu tượng:** Lớp `BaseEntity` chứa phương thức thuần ảo `virtual void printInfo() const = 0`, ép buộc các lớp dẫn xuất phải triển khai chi tiết thông tin thực thể.
- **Đa hình:** Sử dụng đa hình qua con trỏ/tham chiếu `BaseEntity` để hiển thị thông tin (`Utils::printEntityDetails`).

Hệ thống cung cấp cơ chế lưu trữ tự động (Auto-save) và nạp dữ liệu từ thư mục `data/`, đảm bảo tính toàn vẹn và độc lập với vị trí thực thi chương trình.

---

## 2. PHÂN CHIA NHIỆM VỤ THÀNH VIÊN

### Bảng 1: Use Cases

| STT | Thành viên | Use Case 1 | Use Case 2 | Vai trò chính |
| :---: | :--- | :--- | :--- | :--- |
| 1 | **Năng** *(Trưởng nhóm)* | **Nhóm dịch vụ** (*ServiceGroup*) | **Ticket yêu cầu** (*Ticket*) | Kiến trúc hệ thống + Tích hợp |
| 2 | **Quang Hiếu** | **Kênh tiếp nhận** (*ReceptionChannel*) | **Cuộc gọi tiếp nhận** (*InboundCall*) | Nghiệp vụ tiếp nhận & Cuộc gọi |
| 3 | **Cường** | **Mức cam kết SLA** (*SLALevel*) | **Phiếu chuyển xử lý** (*TransferTicket*) | Quản lý SLA & Luân chuyển xử lý ticket |
| 4 | **Chí Linh** | **Khách hàng** (*Customer*) | **Phiếu đánh giá hài lòng** (*Feedback*) | Quản lý khách hàng & Đánh giá dịch vụ |
| 5 | **Thành** | **Điện thoại viên** (*Agent*) | **Ca trực** (*AgentShift*) | Quản lý nhân sự & Lịch trực |

---

### Bảng 2: Chi tiết nhiệm vụ thiết kế chung

#### 1. Năng
- **Xây dựng kiến trúc OOP cốt lõi:**
  - Thiết kế lớp cơ sở trừu tượng `BaseEntity` và phương thức thuần ảo `printInfo()`.
  - Thiết kế và cài đặt lớp khuôn mẫu Generic `Repository<T>` hỗ trợ đầy đủ thao tác CRUD (Create, Read, Update, Delete) và tự động sinh mã ID theo tiền tố (`SM`, `TK`,...).
- **Khung ứng dụng & Điều phối:**
  - Thiết kế lớp điều phối trung tâm `Application` (`App.h`, `App.cpp`) và `main.cpp`.
  - Xây dựng giao diện menu chính (`showMainMenu`) và liên kết điều hướng 10 phân hệ nghiệp vụ.
- **Tích hợp & Quản trị Git:**
  - Thiết lập dự án với CMake, quản trị nhánh Git, giải quyết xung đột mã nguồn và tích hợp toàn hệ thống.

#### 2. Quang Hiếu - Nghiệp vụ Tiếp nhận, Cuộc gọi & Bộ tiện ích Kiểm tra dữ liệu
- **Nghiệp vụ Kênh tiếp nhận (`ReceptionChannel`) - Tiền tố ID: `CN`:**
  - Thiết kế lớp thực thể `ReceptionChannel` kế thừa `BaseEntity` với các thuộc tính: mã kênh (`CNxxx`), tên kênh tiếp nhận, mô tả, trạng thái hoạt động (`active`).
  - Cài đặt các phương thức nghiệp vụ: kích hoạt (`activate`), tạm ngưng (`deactivate`).
  - Xây dựng giao diện menu `ReceptionChannelMenu` hỗ trợ toàn diện chu trình CRUD:
    - Hiển thị danh sách kênh tiếp nhận dạng bảng căn lề chuẩn hóa.
    - Tìm kiếm kênh tiếp nhận theo mã ID (`CNxxx`).
    - Thêm kênh tiếp nhận mới (kiểm tra không rỗng, kiểm tra chống trùng lặp tên kênh bằng `ValidationUtils::isDuplicate`, tự động cấp phát ID tiền tố `CN`).
    - Cập nhật thông tin kênh (tên kênh, mô tả; kiểm tra tính hợp lệ và chống trùng lặp tên mới).
    - Chuyển đổi trạng thái hoạt động (`toggleStatus`).
    - Xóa kênh tiếp nhận kèm **kiểm tra ràng buộc toàn vẹn dữ liệu**: ngăn chặn xóa nếu kênh đang có cuộc gọi tiếp nhận liên kết.
- **Nghiệp vụ Cuộc gọi tiếp nhận (`InboundCall`) - Tiền tố ID: `CA`:**
  - Thiết kế lớp thực thể `InboundCall` kế thừa `BaseEntity` với các thuộc tính: mã cuộc gọi (`CAxxx`), số điện thoại, mã kênh tiếp nhận (`channelId` - khóa ngoại), mã điện thoại viên tiếp nhận (`agentId` - khóa ngoại), thời gian gọi (`callTime`), thời lượng (`duration` tính bằng giây), trạng thái (`status`: `ANSWERED`, `MISSED`, `BUSY`), ghi chú (`note`).
  - Cài đặt kiểm tra hợp lệ dữ liệu trong các phương thức setter (thời lượng không âm, các mã và thông tin bắt buộc không để trống).
  - Triển khai chuyên biệt hóa `FileManager<InboundCall>` (`InboundCallFileManager.cpp`) đọc/ghi file `data/inbound_calls.txt` với 8 trường dữ liệu (đồng bộ bổ sung cột `Ma NV`).
  - Xây dựng giao diện menu `InboundCallMenu` đầy đủ chức năng:
    - Hiển thị danh sách cuộc gọi dưới dạng bảng chi tiết 8 cột.
    - Tìm kiếm cuộc gọi theo mã ID (`CAxxx`), hiển thị chi tiết cuộc gọi kết hợp tra cứu Tên kênh tiếp nhận và Họ tên điện thoại viên tiếp nhận.
    - Ghi nhận cuộc gọi mới: kiểm tra số điện thoại hợp lệ, kiểm tra ràng buộc khóa ngoại kênh tiếp nhận (phải tồn tại và đang hoạt động `active == true`), kiểm tra ràng buộc khóa ngoại điện thoại viên (phải tồn tại trong danh sách `Agent`), kiểm tra thời gian theo định dạng `DD/MM/YYYY HH:MM`, tự động sinh mã ID tiền tố `CA`.
    - Cập nhật cuộc gọi (cho phép sửa kênh tiếp nhận, nhân viên phụ trách, thời lượng, trạng thái, ghi chú).
    - Lọc và thống kê danh sách cuộc gọi theo kênh tiếp nhận.
    - Xóa cuộc gọi với xác nhận an toàn (y/n).
- **Bộ công cụ tiện ích nhập liệu an toàn (`Utilities`):**
  - Hàm nhập chuỗi không rỗng (`Utils::inputNonEmptyString`).
  - Hàm nhập số nguyên (`Utils::inputInt`) có kiểm tra khoảng giá trị hợp lệ.
  - Kiểm tra và định dạng mã định danh (`Utils::formatId`, `Utils::extractIdNumber`).
  - Xử lý cắt tỉa khoảng trắng chuỗi (`Utils::trim`).
  - Hàm mẫu kiểm tra trùng lặp tổng quát (`ValidationUtils::isDuplicate`).
  - Xử lý ngoại lệ nhập liệu: Sử dụng `try/catch/throw` để bắt lỗi nhập liệu sai kiểu từ người dùng, làm sạch bộ đệm `cin` tránh lặp vô tận.

#### 3. Cường - Quản lý File
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
  - Hàm nhập ngày tháng có kiểm tra hợp lệ (`Utils::inputDateString`).
- **Định dạng hiển thị giao diện bảng:**
  - Sử dụng thư viện `<iomanip>` (`setw`, `left`, `setfill`, `setprecision`) để căn chỉnh các cột dữ liệu thẳng hàng, phân cách rõ ràng.
  - Hàm nối danh sách chuỗi (`Utils::join`).

#### 5. Thành - Dữ liệu mẫu, Ràng buộc & Kiểm thử
- **Xây dựng bộ dữ liệu mẫu:**
  - Chuẩn bị đầy đủ 10 file dữ liệu chuẩn định dạng ban đầu trong thư mục `data/` (`agents.txt`, `customers.txt`, `tickets.txt`,...).
  - Đảm bảo mỗi file có ít nhất 10 bản ghi dữ liệu hợp lệ.
- **Kiểm tra ràng buộc và quan hệ giữa các thực thể:**
  - Xây dựng cơ chế kiểm tra ràng buộc khóa ngoại và toàn vẹn tham chiếu giữa các cặp thực thể của từng thành viên trước khi thực hiện xóa (Delete cascade/restrict).
  - Kiểm tra tính tồn tại và hợp lệ của thực thể cha khi tạo thực thể con phụ thuộc (ví dụ: `AgentShift` phụ thuộc `Agent`, `TransferTicket` phụ thuộc `SLALevel`, `Ticket` phụ thuộc `ServiceGroup`, `InboundCall` phụ thuộc `ReceptionChannel` và `Agent`, `Feedback` phụ thuộc `Customer`).
- **Kiểm thử tổng hợp & Hỗ trợ tích hợp:**
  - Kiểm thử toàn bộ các kịch bản CRUD trên cả 10 phân hệ menu.
  - Đảm bảo cơ chế tự động đồng bộ dữ liệu giữa bộ nhớ và file văn bản hoạt động chính xác.

---

## 3. ĐẶC TẢ CHI TIẾT CÁC USE CASE & RÀNG BUỘC NGHIỆP VỤ

### 3.1. Kênh tiếp nhận (`ReceptionChannel`) - Quang Hiếu
- **Mã tiền tố:** `CN` (VD: `CN001`, `CN002`,...)
- **File lưu trữ:** `data/channels.txt`
- **Cấu trúc trường dữ liệu:**
  `Ma ID | Ten kenh tiep nhan | Mo ta | Trang thai`
- **Các quy tắc & ràng buộc nghiệp vụ:**
  - `Ten kenh tiep nhan`: Bắt buộc, không được để trống, không được trùng lặp với tên kênh đã tồn tại trong hệ thống (kiểm tra qua `ValidationUtils::isDuplicate`).
  - `Mo ta`: Bắt buộc, không được để trống.
  - `Trang thai`: `1` (Đang hoạt động / Active) hoặc `0` (Tạm ngưng / Inactive).
  - **Ràng buộc toàn vẹn khi xóa (Referential Integrity):** Không cho phép xóa kênh tiếp nhận nếu kênh đó đang có cuộc gọi tiếp nhận (`InboundCall`) liên kết trong hệ thống.
  - **Quản lý trạng thái:** Cung cấp chức năng chuyển đổi nhanh trạng thái hoạt động (`toggleStatus`) kích hoạt hoặc tạm ngưng kênh.

### 3.2. Cuộc gọi tiếp nhận (`InboundCall`) - Quang Hiếu
- **Mã tiền tố:** `CA` (VD: `CA001`, `CA002`,...)
- **File lưu trữ:** `data/inbound_calls.txt`
- **Cấu trúc trường dữ liệu:**
  `Ma ID | So dien thoai | Ma kenh | Ma NV | Thoi gian | Thoi luong | Trang thai | Ghi chu`
- **Các quy tắc & ràng buộc nghiệp vụ:**
  - `So dien thoai`: Bắt buộc, không để trống, kiểm tra định dạng số điện thoại hợp lệ.
  - `Ma kenh` (*Khóa ngoại liên kết `ReceptionChannel`*): Bắt buộc phải tồn tại trong danh sách kênh VÀ kênh phải đang ở trạng thái **Hoạt động** (`active == true`). Hệ thống sẽ từ chối ghi nhận cuộc gọi vào kênh đang tạm ngưng.
  - `Ma NV` (*Khóa ngoại liên kết `Agent`*): Bắt buộc phải tồn tại trong danh sách Điện thoại viên (`agents.txt`).
  - `Thoi gian`: Định dạng chuẩn ngày giờ `DD/MM/YYYY HH:MM`.
  - `Thoi luong`: Số nguyên không âm (`>= 0`, tính bằng giây). Nếu cuộc gọi nhỡ (`MISSED`), thời lượng là `0`.
  - `Trang thai`: Chuẩn hóa 3 trạng thái hợp lệ: `ANSWERED` (Đã nghe), `MISSED` (Nhỡ), `BUSY` (Máy bận).
  - `Ghi chu`: Tóm tắt nội dung trao đổi của cuộc gọi.
  - **Truy vấn liên kết:** Khi tìm kiếm cuộc gọi theo mã ID, hệ thống tự động kết nối và hiển thị kèm Tên kênh tiếp nhận và Họ tên điện thoại viên tiếp nhận.
  - **Thống kê:** Hỗ trợ lọc danh sách cuộc gọi theo từng kênh tiếp nhận và thống kê tổng số lượng cuộc gọi của kênh.

---

## 4. CẤU TRÚC THƯ MỤC DỰ ÁN

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
│   ├── inbound_calls.txt  # Dữ liệu cuộc gọi tiếp nhận (bổ sung Ma NV)
│   ├── service_groups.txt
│   ├── sla_levels.txt
│   ├── tickets.txt
│   └── transfer_tickets.txt
├── main/                  # Mã nguồn chính của chương trình
│   ├── app/               # Lớp Application và các Menu giao diện
│   │   ├── App.h / App.cpp
│   │   ├── AgentMenu.h / AgentMenu.cpp
│   │   ├── AgentShiftMenu.h / AgentShiftMenu.cpp
│   │   ├── CustomerMenu.h / CustomerMenu.cpp
│   │   ├── FeedbackMenu.h / FeedbackMenu.cpp
│   │   ├── InboundCallMenu.h / InboundCallMenu.cpp           # Menu quản lý Cuộc gọi tiếp nhận
│   │   ├── ReceptionChannelMenu.h / ReceptionChannelMenu.cpp # Menu quản lý Kênh tiếp nhận
│   │   ├── ServiceGroupMenu.h / ServiceGroupMenu.cpp
│   │   ├── SLALevelMenu.h / SLALevelMenu.cpp
│   │   ├── TicketMenu.h / TicketMenu.cpp
│   │   └── TransferTicketMenu.h / TransferTicketMenu.cpp
│   ├── Entity/            # Các lớp thực thể (Kế thừa BaseEntity)
│   │   ├── BaseEntity.h
│   │   ├── Agent.h / Agent.cpp
│   │   ├── AgentShift.h / AgentShift.cpp
│   │   ├── Customer.h / Customer.cpp
│   │   ├── Feedback.h / Feedback.cpp
│   │   ├── InboundCall.h / InboundCall.cpp                   # Thực thể Cuộc gọi tiếp nhận
│   │   ├── ReceptionChannel.h / ReceptionChannel.cpp         # Thực thể Kênh tiếp nhận
│   │   ├── ServiceGroup.h / ServiceGroup.cpp
│   │   ├── SLALevel.h / SLALevel.cpp
│   │   ├── Ticket.h / Ticket.cpp
│   │   └── TransferTicket.h / TransferTicket.cpp
│   ├── FileManager/       # Đọc và ghi file dữ liệu
│   │   ├── FileManager.h
│   │   ├── AgentFileManager.cpp
│   │   ├── AgentShiftFileManager.cpp
│   │   ├── CustomerFileManager.cpp
│   │   ├── FeedbackFileManager.cpp
│   │   ├── InboundCallFileManager.cpp                        # Đọc/ghi file cuộc gọi tiếp nhận
│   │   ├── ReceptionChannelFileManager.cpp                   # Đọc/ghi file kênh tiếp nhận
│   │   ├── ServiceGroupFileManager.cpp
│   │   ├── SLALevelFileManager.cpp
│   │   ├── TicketFileManager.cpp
│   │   └── TransferTicketFileManager.cpp
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

## 5. HƯỚNG DẪN BIÊN DỊCH VÀ CHẠY CHƯƠNG TRÌNH

### Yêu cầu môi trường
- Trình biên dịch C++ hỗ trợ chuẩn **C++11** trở lên (GCC / MinGW-w64, Clang hoặc MSVC).
- Công cụ **CMake** (phiên bản >= 3.15).

---

### Cách 1: Chạy nhanh bằng VS Code (Khuyên dùng)
1. Tải code về và mở thư mục gốc `BTL-OOP` bằng **Visual Studio Code**.
2. Đảm bảo đã cài đặt extension **CMake Tools** và **C/C++**.
3. Nhấn vào **nút tam giác Play ▶️** (hoặc phím tắt `Shift + F5`) trên thanh trạng thái dưới cùng của VS Code để chạy chương trình ngay lập tức.

---

### Cách 2: Chạy qua dòng lệnh (Terminal)
Nếu không dùng VS Code, bạn có thể chạy bằng dòng lệnh từ thư mục gốc:

1. **Biên dịch dự án:**
   ```bash
   cmake -B build
   cmake --build build
   ```
2. **Chạy chương trình:**
   - **Windows:**
     ```powershell
     .\build\myprogram.exe
     ```
   - **Linux / macOS:**
     ```bash
     ./build/myprogram
     ```
