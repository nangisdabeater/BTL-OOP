#pragma once

#include <string>
#include <stdexcept>

class BaseEntity {
protected:
    // Dùng protected để vừa che giấu dữ liệu với bên ngoài, vừa cho phép các lớp con sử dụng trực tiếp.
    std::string id;

public:
// Dùng danh sách khởi tạo : id("")" để khởi tạo trực tiếp vùng nhớ cho id,tối ưu hiệu năng hơn so với việc gán "id = "";" bên trong thân hàm.
    BaseEntity() : id("") {}

/** Dùng hàm hủy ảo (virtual destructor) thay vì hàm hủy thường:
Khi xóa đối tượng lớp con thông qua con trỏ lớp cha (VD: BaseEntity* p = new Ticket(); delete p;),
nếu không có "virtual" thì chương trình chỉ gọi hàm hủy của lớp cha mà bỏ qua hàm hủy của lớp con,
dẫn đến rò rỉ bộ nhớ (Memory Leak). Dùng "= default" để trình biên dịch tự sinh hàm hủy chuẩn.
*/
    virtual ~BaseEntity() = default;
    //Trả về tham chiếu hằng "const std::string&" thay vì trả về giá trị "std::string":
    //Từ khóa "const" ở cuối cho phép nhập 1 chuỗi các kí tự tạm thời (VD:entity.setId(Utils::inputNonEmptyString("Nhap ma ID: "));).
    virtual const std::string& getId() const {
        return id;
    }

    // Truyền tham số kiểu "const std::string&" để không tốn bộ nhớ copy chuỗi đầu vào.
    virtual void setId(const std::string& newId) {
        if (newId.empty()) {
//Dùng throw ngoại lệ thay vì chỉ in ra cout rồi bỏ qua, nhằm ép buộc nơi gọi phải xử lý lỗi và ngăn chặn triệt để việc tạo ra đối tượng có ID rỗng.
            throw std::invalid_argument("ID khong duoc de trong!");
        }
        id = newId;
    }
    // Biến BaseEntity thành lớp trừu tượng, ngăn việc tạo đối tượng BaseEntity vô nghĩa.
    // Bắt buộc mọi lớp con kế thừa đều phải tự định nghĩa hàm printInfo() riêng (Tính đa hình).
    virtual void printInfo() const = 0;
};