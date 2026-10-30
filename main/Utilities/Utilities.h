#pragma once 

#include <string>
#include <vector>
#include <climits>

class BaseEntity;

/*Dùng "namespace Utils" thay vì tạo "class Utils" chứa các hàm static
Vì các hàm tiện ích này không có thuộc tính dữ liệu , dùng namespace là cách
để gom nhóm các hàm hỗ trợ, tránh xung đột tên mà không cần tạo class giả.
*/
namespace Utils {
    // Cắt bỏ khoảng trắng thừa (dấu cách, tab, xuống dòng) ở đầu và cuối chuỗi
    std::string trim(const std::string& str);

    // Nhập chuỗi từ bàn phím và bắt buộc không được để trống
    std::string inputNonEmptyString(const std::string& prompt);

    // Định dạng mã ID tự động thêm số 0 ở đầu (VD: prefix = "TK", number = 1 -> "TK001")
    std::string formatId(const std::string& prefix, int number, int padding = 3);
  
    // Tách lấy phần số từ mã ID (VD: "TK005" -> 5), trả về -1 nếu mã ID sai định dạng
    int extractIdNumber(const std::string& id, const std::string& prefix);

    // Kiểm tra chuỗi ngày tháng có đúng định dạng DD/MM/YYYY và hợp lệ theo lịch thực tế không
    bool isValidDate(const std::string& dateStr);

    // So sánh 2 ngày định dạng DD/MM/YYYY: trả về -1 nếu date1 < date2, 0 nếu bằng nhau, 1 nếu date1 > date2
    int compareDates(const std::string& date1, const std::string& date2);

    // Nhập chuỗi ngày tháng từ bàn phím, yêu cầu nhập lại nếu sai định dạng DD/MM/YYYY
    std::string inputDateString(const std::string& prompt);

    // Nhập số nguyên nằm trong khoảng [minVal, maxVal], chống trôi lệnh khi nhập sai chữ cái
    int inputInt(const std::string& prompt, int minVal = 0, int maxVal = INT_MAX);

    // Nối danh sách các chuỗi trong vector thành 1 chuỗi duy nhất ngăn cách bởi dấu phẩy
    std::string join(const std::vector<std::string>& vec, const std::string& delemiter = ", ");

    // Bắt buộc người dùng nhập số 0 để quay lại menu trước đó
    void backChoice();

    // Nhận tham chiếu lớp cha BaseEntity& để gọi hàm ảo printInfo() của lớp con tương ứng (Tính đa hình)
    void printEntityDetails(const BaseEntity& entity);
}

namespace ValidationUtils {
/** Dùng hàm khuôn mẫu (template) kết hợp biến điều kiện Predicate (Lambda)để
Mỗi thực thể cần kiểm tra trùng lặp trên các thuộc tính khác nhau (trùng tên dịch vụ, trùng SĐT,...).
Nhờ truyền điều kiện Lambda vào biến "condition", chỉ cần viết 1 hàm này là dùng chung được cho mọi loại danh sách.
*/
    template <typename T, typename Predicate>
    bool isDuplicate(const std::vector<T>& list, Predicate condition) {
        for (const auto& item : list) {
            if (condition(item)) {
                return true; // Tìm thấy phần tử bị trùng lặp
            }
        }
        return false; // Không có phần tử nào bị trùng
    }
}
