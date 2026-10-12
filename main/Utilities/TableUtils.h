#pragma once

#include <vector>
#include <algorithm>
#include <string>
#include <iostream>
#include <iomanip>

/**
 * @file TableUtils.h
 * @brief Bộ công cụ tiện ích bảng: hỗ trợ sắp xếp, tìm kiếm và in bảng ra console.
 */
namespace TableUtils {

    /**
     * @brief Sắp xếp danh sách phần tử theo khóa (key) tùy chọn.
     * 
     * @tparam T Kiểu dữ liệu của phần tử trong mảng (vector)
     * @tparam Key Hàm hoặc biểu thức lambda trích xuất giá trị so sánh từ phần tử T
     * @param rows Danh sách tham chiếu các phần tử cần sắp xếp
     * @param key Hàm hoặc lambda trích xuất giá trị khóa (VD: [](const Customer& c) { return c.getFullName(); })
     * @param ascending true để sắp xếp tăng dần, false để sắp xếp giảm dần (mặc định: true)
     */
    template <class T, class Key>
    void sortBy(std::vector<T>& rows, Key key, bool ascending = true) {
        std::sort(rows.begin(), rows.end(), [&](const T& a, const T& b) {
            return ascending ? (key(a) < key(b)) : (key(b) < key(a));
        });
    }

    /**
     * @brief Tìm kiếm và lọc các phần tử thỏa mãn điều kiện logic (predicate).
     * 
     * @tparam T Kiểu dữ liệu của phần tử trong mảng
     * @tparam Predicate Hàm hoặc biểu thức lambda kiểm tra điều kiện logic
     * @param rows Danh sách các phần tử đầu vào
     * @param match Hàm/lambda trả về true nếu phần tử khớp điều kiện lọc
     * @return std::vector<T> Danh sách các phần tử thỏa mãn điều kiện
     */
    template <class T, class Predicate>
    std::vector<T> search(const std::vector<T>& rows, Predicate match) {
        std::vector<T> result;
        for (const auto& row : rows) {
            if (match(row)) {
                result.push_back(row);
            }
        }
        return result;
    }

    /**
     * @brief In một dòng dữ liệu dạng bảng căn lề trái với độ rộng cột đồng nhất.
     * 
     * Sử dụng kỹ thuật variadic template và C++17 fold expression.
     * 
     * @tparam Values Danh sách các kiểu dữ liệu của từng giá trị cột
     * @param width Độ rộng cố định cho mỗi cột khi hiển thị
     * @param values Các giá trị cần in trên cùng một dòng
     */
    template <class... Values>
    void printRow(int width, const Values&... values) {
        ((std::cout << std::left << std::setw(width) << values), ...);
        std::cout << '\n';
    }

} // namespace TableUtils
