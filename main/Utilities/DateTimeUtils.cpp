#include "DateTimeUtils.h"
#include "Utilities.h"
#include <ctime>
#include <iomanip>
#include <sstream>

/**
 * @brief Kiểm tra ngày hợp lệ thông qua Utils::isValidDate
 */
bool DateTimeUtils::isValidDate(const std::string& s) {
    return Utils::isValidDate(s);
}

/**
 * @brief Lấy ngày hiện tại của hệ thống theo định dạng dd/mm/yyyy
 */
std::string DateTimeUtils::today() {
    std::time_t t = std::time(nullptr);
    std::tm* p = std::localtime(&t);
    std::ostringstream o;
    o << std::setfill('0') << std::setw(2) << p->tm_mday << '/'
      << std::setw(2) << p->tm_mon + 1 << '/'
      << p->tm_year + 1900;
    return o.str();
}
