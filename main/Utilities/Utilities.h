#pragma once 

#include <string>
#include <vector>

class BaseEntity;

namespace Utils {
  std::string trim(const std::string& str);

  std::string inputNonEmptyString(const std::string& prompt);

  std::string formatId(const std::string& prefix, int number, int padding = 3);
  
  int extractIdNumber(const std::string& id, const std::string& prefix);

  bool isValidDate(const std::string& dateStr);
  std::string today();
  std::string inputDateString(const std::string& prompt);
  double inputDouble(const std::string& prompt, double minVal = 0.0);
  int inputInt(const std::string& prompt, int minVal, int maxVal);

  std::string join(const std::vector<std::string>& vec, const std::string& delemiter = ", ");

  void backChoice();

  void printEntityDetails(const BaseEntity& entity);

  std::string resolveDataPath(const std::string& relativePath);
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