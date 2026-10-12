#pragma once

#include <string>

/**
 * @file DateTimeUtils.h
 * @brief Tiện ích xử lý và kiểm tra dữ liệu ngày tháng định dạng dd/mm/yyyy
 */
namespace DateTimeUtils {

    /**
     * @brief Kiểm tra tính hợp lệ của chuỗi ngày theo định dạng chuẩn dd/mm/yyyy
     * @param date Chuỗi ngày cần kiểm tra
     * @return true nếu ngày hợp lệ (đúng cú pháp, ngày từ 1-31, tháng từ 1-12, năm > 1900)
     */
    bool isValidDate(const std::string& date);

    /**
     * @brief Lấy ngày hiện tại của hệ thống theo định dạng dd/mm/yyyy
     * @return std::string Chuỗi ngày hiện tại (VD: "12/10/2026")
     */
    std::string today();

} // namespace DateTimeUtils
