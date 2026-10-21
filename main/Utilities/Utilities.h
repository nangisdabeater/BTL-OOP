#pragma once 

#include <string>
#include <vector>
#include <climits>

class BaseEntity;

namespace Utils {
  std::string trim(const std::string& str);

  std::string inputNonEmptyString(const std::string& prompt);

  std::string formatId(const std::string& prefix, int number, int padding = 3);
  
  int extractIdNumber(const std::string& id, const std::string& prefix);

  bool isValidDate(const std::string& dateStr);
  std::string inputDateString(const std::string& prompt);
  int inputInt(const std::string& prompt, int minVal = 0, int maxVal = INT_MAX);

  std::string join(const std::vector<std::string>& vec, const std::string& delemiter = ", ");

  void backChoice();

  void printEntityDetails(const BaseEntity& entity);


}

namespace ValidationUtils {
    /**
     * Hàm kiểm tra trùng lặp
     * list: Danh sách bất kỳ (vector<ServiceGroup>, vector<User>, vector<Ticket>...)
     * Điều kiện kiểm tra trùng lặp : hàm Lambda
     */
    template <typename T, typename Predicate>
    bool isDuplicate(const std::vector<T>& list, Predicate condition){
        for (const auto& item : list){
            if (condition(item)){
                return true; // Tìm thấy phần tử trùng lặp
            }
        }
        return false; // Không trùng lặp
    }
}