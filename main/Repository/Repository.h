#pragma once

#include <vector>
#include <algorithm>
#include <string>
#include <stdexcept>
#include <iostream>
#include "Utilities.h"
#include "FileManager.h"
// Không dùng "using namespace std;" trong file này vì lớp có thuộc tính tên là "count",
// nếu mở namespace std sẽ bị xung đột trực tiếp với hàm "std::count" trong thư viện <algorithm>.

// Dùng khuôn mẫu lớp (template <typename T>) thay vì viết 10 lớp Repository riêng cho 10 thực thể:
// Giúp tái sử dụng chung 1 bộ code CRUD (Thêm, Sửa, Xóa, Tìm kiếm, Lưu file) cho mọi thực thể,
// tránh lặp code và dễ bảo trì.
template <typename T>
class Repository {
private:
    // Dùng std::vector thay vì mảng tĩnh hay danh sách liên kết (std::list):
    // Mảng tĩnh bị giới hạn kích thước cố định, còn std::list tốn thêm bộ nhớ lưu con trỏ và truy cập chậm.
    // std::vector tự động co giãn kích thước và lưu trữ liên tiếp trong RAM giúp duyệt dữ liệu nhanh nhất.
    std::vector<T> data;
    int count = 1;            // Bộ đếm số thứ tự ID tự động tăng từ 1 đến 999
    std::string idPrefix;     // Tiền tố mã ID riêng cho từng thực thể (VD: "TK" cho Ticket, "SM" cho ServiceGroup)
    std::string filePath;     // Đường dẫn file lưu trữ dữ liệu trên ổ đĩa

    // Hàm nội bộ tự động ghép tiền tố và số đếm để tạo ID mới
    std::string generateId();

    // Hàm lưu file an toàn: nếu ghi file thất bại thì tự động khôi phục lại dữ liệu cũ trong RAM
    void saveOrRollback(const std::vector<T>& backup, int backupCounter);

public:
    // Khởi tạo kho lưu trữ với tiền tố ID và đường dẫn file tương ứng
    Repository(const std::string& prefix, const std::string& filePath = "") {
        this->idPrefix = prefix;
        this->filePath = filePath;
    }

    void load();                         // Đọc dữ liệu từ file vào vector data và đồng bộ lại bộ đếm count
    void save();                         // Ghi toàn bộ dữ liệu trong vector data ra file
    void add(const T& entity);           // Thêm thực thể mới vào kho và cấp phát ID tự động
    bool remove(const std::string& id);  // Xóa thực thể theo ID, trả về true nếu xóa thành công

    // Dùng con trỏ T* làm kiểu trả về thay vì tham chiếu T& hay giá trị T:
    // - Không dùng T (trả về giá trị): vì sẽ tạo ra bản sao tốn bộ nhớ và không sửa trực tiếp được.
    // - Không dùng T& (tham chiếu): vì tham chiếu không thể mang giá trị null, nếu không tìm thấy ID
    //   thì bắt buộc phải quăng ngoại lệ (throw), làm code ở Menu phải bọc try-catch rất phức tạp.
    // - Dùng con trỏ T*: tìm thấy thì trả về địa chỉ thực thể (&entity), không thấy thì trả về nullptr
    //   giúp kiểm tra bằng lệnh if (ptr == nullptr) cực kỳ gọn nhẹ.
    T* findById(const std::string& id);

    // Trả về tham chiếu hằng (const std::vector<T>&) thay vì trả về bản sao vector
    // để tránh việc copy toàn bộ danh sách gây tốn RAM và giảm hiệu năng.
    const std::vector<T>& getAll() const;

    bool update(const T& entity);        // Cập nhật thông tin thực thể đã có trong kho
};

template <typename T>
std::string Repository<T>::generateId() {
    if (count > 999) {
        // Quăng lỗi tràn số nếu vượt quá giới hạn định dạng 3 chữ số (001 -> 999)
        throw std::overflow_error("Da vuot qua gioi han ma ID cho phep (toi da 999)!");
    }
    // Gọi hàm formatId để định dạng số thứ tự thành chuỗi 3 chữ số (VD: 1 -> "001") rồi tăng count lên 1
    return Utils::formatId(idPrefix, count++);
}

template <typename T>
void Repository<T>::load() {
    data.clear(); // Xóa sạch dữ liệu cũ trên RAM trước khi nạp lại từ file
    count = 1;    // Đặt lại bộ đếm về 1

    // Gọi lớp chuyên biệt hóa FileManager<T> để đọc danh sách đối tượng từ file
    std::vector<T> loaded = FileManager<T>::load(filePath);

    for (const T& entity : loaded) {
        // Tách lấy phần số trong mã ID (VD: "TK005" -> lấy số 5) để kiểm tra định dạng
        int number = Utils::extractIdNumber(entity.getId(), idPrefix);

        // Nếu mã ID sai định dạng (-1) hoặc bị trùng lặp với bản ghi đã nạp trước đó thì bỏ qua
        if (number == -1 || findById(entity.getId()) != nullptr) {
            std::cout << "Canh bao: Bo qua ban ghi co ma khong hop le hoac bi trung: "
                      << entity.getId() << std::endl;
            continue;
        }

        data.push_back(entity); // Đưa bản ghi hợp lệ vào kho lưu trữ trong RAM

        // Cập nhật bộ đếm count luôn lớn hơn số ID lớn nhất đã có để khi thêm mới không bị trùng ID cũ
        if (number >= count) {
            count = number + 1;
        }
    }
}

template <typename T>
void Repository<T>::save() {
    // Ủy quyền việc ghi dữ liệu xuống file cho lớp FileManager<T> tương ứng
    FileManager<T>::save(filePath, data);
}

template <typename T>
void Repository<T>::saveOrRollback(const std::vector<T>& backup, int backupCounter) {
    try {
        save(); // Thử ghi dữ liệu mới xuống ổ đĩa
    }
    catch (...) {
        // Tại sao phải có bước khôi phục (Rollback) này?
        // Nếu quá trình ghi file bị lỗi (ổ cứng đầy, file bị khóa,...) mà không khôi phục lại,
        // dữ liệu trên RAM sẽ bị lệch so với dữ liệu trong file.
        data = backup;
        count = backupCounter;
        throw; // Ném tiếp lỗi ra ngoài để tầng Menu bắt được và in thông báo cho người dùng
    }
}

template <typename T>
void Repository<T>::add(const T& entity) {
    // Tạo bản sao lưu trạng thái trước khi thêm để phòng trường hợp ghi file lỗi
    std::vector<T> backup = data;
    int backupCounter = count;

    T newEntity = entity;            // Tạo bản sao của thực thể truyền vào để gán ID mới
    newEntity.setId(generateId());   // Cấp phát mã ID tự động cho thực thể mới

    data.push_back(newEntity);       // Thêm vào cuối vector

    saveOrRollback(backup, backupCounter); // Lưu xuống file, nếu lỗi thì hoàn tác
}

template <typename T>
T* Repository<T>::findById(const std::string& id) {
    // Duyệt qua từng phần tử bằng tham chiếu (T&) để lấy đúng địa chỉ gốc trong vector
    for (T& entity : data) {
        if (entity.getId() == id) {
            return &entity; // Trả về địa chỉ của phần tử tìm thấy
        }
    }
    return nullptr; // Không tìm thấy thì trả về con trỏ rỗng
}

template <typename T>
bool Repository<T>::remove(const std::string& id) {
    std::vector<T> backup = data; // Sao lưu trạng thái trước khi xóa

    // Tại sao dùng kết hợp std::remove_if và data.erase thay vì dùng vòng lặp for rồi xóa?
    // - Nếu dùng vòng lặp for rồi gọi erase: mỗi lần xóa 1 phần tử, vector phải dịch chuyển toàn bộ
    //   các phần tử phía sau lên trước, đồng thời làm hỏng con trỏ lặp (iterator invalidation) dễ gây sập chương trình.
    // - Dùng std::remove_if: chỉ duyệt mảng đúng 1 lần để dồn các phần tử cần xóa về cuối vector,
    //   sau đó gọi data.erase cắt bỏ phần đuôi đúng 1 lần duy nhất (tối ưu hiệu năng O(N)).
    //
    // Tại sao dùng biểu thức Lambda "[id](const T& entity)"?
    // - Giúp viết trực tiếp điều kiện so sánh ID ngay tại chỗ mà không cần phải viết thêm một hàm rời bên ngoài.
    auto it = std::remove_if(
        data.begin(),
        data.end(),
        [id](const T& entity) {
            return entity.getId() == id;
        }
    );

    if (it == data.end()) {
        return false; // Không có phần tử nào khớp với ID cần xóa
    }

    data.erase(it, data.end()); // Thực sự xóa bỏ các phần tử đã bị dồn về cuối vector

    saveOrRollback(backup, count); // Lưu danh sách mới xuống file, nếu lỗi thì khôi phục lại

    return true;
}

template <typename T>
const std::vector<T>& Repository<T>::getAll() const {
    return data; // Trả về tham chiếu hằng tới danh sách dữ liệu hiện tại
}

template <typename T>
bool Repository<T>::update(const T& entity) {
    // Tìm con trỏ trỏ tới đối tượng đang nằm trong vector dựa theo ID
    T* existingEntity = findById(entity.getId());

    if (existingEntity == nullptr) {
        return false; // Không tìm thấy đối tượng cần cập nhật
    }

    std::vector<T> backup = data; // Sao lưu dữ liệu trước khi ghi đè

    *existingEntity = entity;     // Ghi đè thông tin mới trực tiếp vào vùng nhớ của đối tượng trong vector

    saveOrRollback(backup, count); // Lưu thay đổi xuống file

    return true;
}
