#include "Utilities.h"
#include <iostream>
#include <iomanip>
#include <sstream>
#include <cctype>

namespace Utils {
    std::string trim(const std::string& str) {
        size_t first = str.find_first_not_of(" \t\r\n");
        if (first == std::string::npos) {
            return "";
        }
        size_t last = str.find_last_not_of(" \t\r\n");
        return str.substr(first, last - first + 1);
    }

    std::string inputNonEmtpyString(const std::string& prompt) {
        std::string input;
        while (true) {
            std::cout << prompt;
            std::getline(std::cin, input);
            input = trim(input);
            if (!input.empty()) {
                return input;
            }
            std::cout << "Gia tri khong duoc de trong! Vui long nhap lai." << std::endl;
        }
    }

    std::string formatId(const std::string& prefix, int number, int padding) {
        std::stringstream ss;
        // setfill('0') và setw(3) sẽ tự động thêm các số 0 ở trước (VD: 1 -> 001)
        ss << prefix << std::setw(padding) << std::setfill('0') << number;
        return ss.str();
    }

    int extractIdNumber(const std::string& id, const std::string& prefix) {
        // Kiểm tra độ dài tối thiểu và so khớp tiền tố
        if (id.length() <= prefix.length() || id.substr(0, prefix.length()) != prefix) {
            return -1; 
        }

        std::string numberStr = id.substr(prefix.length());
        
        // Đảm bảo phần phía sau tiền tố hoàn toàn là số
        for (char c : numberStr) {
            if (!isdigit(static_cast<unsigned char>(c))) {
                return -1;
            }
        }

        try {
            int number = std::stoi(numberStr);
            if (number < 1 || number > 999) return -1; // Ràng buộc giới hạn (có thể thay đổi nếu cần)
            return number;
        }
        catch (...) {
            return -1;
        }
    }
}