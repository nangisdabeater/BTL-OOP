#pragma once 

#include <string>
#include <vector>

namespace Utils {
  std::string trim(const std::string& str);

  std::string inputNonEmtpyString(const std::string& prompt);

  std::string formatId(const std::string& prefix, int number, int padding = 3);
  
  int extractIdNumber(const std::string& id, const std::string& prefix);
}

namespace ValidationUtils 
{
    /**
     * Hàm kiểm tra trùng lặp tổng quát (Generic)
     * @param list: Danh sách bất kỳ (vector<ServiceGroup>, vector<User>, vector<Ticket>...)
     * @param condition: Điều kiện kiểm tra trùng lặp (Lambda function)
     */
    template <typename T, typename Predicate>
    bool isDuplicate(const std::vector<T>& list, Predicate condition)
    {
        for (const auto& item : list)
        {
            if (condition(item))
            {
                return true; // Tìm thấy phần tử trùng lặp
            }
        }
        return false; // Không trùng lặp
    }
}