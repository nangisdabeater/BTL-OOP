#include "Utilities.h"
#include <iostream>
#include <iomanip>
#include <sstream>
#include <cctype>
#include <ctime>
#include <filesystem>
#if defined(_WIN32)
#ifndef NOMINMAX
#define NOMINMAX
#endif
#include <windows.h>
#endif
#include "../Entity/BaseEntity.h"

namespace Utils {
    std::string trim(const std::string& str) {
        size_t first = str.find_first_not_of(" \t\r\n");
        if (first == std::string::npos) {
            return "";
        }
        size_t last = str.find_last_not_of(" \t\r\n");
        return str.substr(first, last - first + 1);
    }

    std::string inputNonEmptyString(const std::string& prompt) {
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

    bool isValidDate(const std::string& dateStr) {
        // Định dạng DD/MM/YYYY (10 ký tự)
        if (dateStr.length() != 10) return false;
        if (dateStr[2] != '/' || dateStr[5] != '/') return false;
        for (int i = 0; i < 10; ++i) {
            if (i == 2 || i == 5) continue;
            if (!isdigit(static_cast<unsigned char>(dateStr[i]))) return false;
        }
        try {
            int d = std::stoi(dateStr.substr(0, 2));
            int m = std::stoi(dateStr.substr(3, 2));
            int y = std::stoi(dateStr.substr(6, 4));
            if (y < 1900 || y > 2100) return false;
            if (m < 1 || m > 12) return false;
            int maxDays[] = { 31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31 };
            if ((y % 4 == 0 && y % 100 != 0) || (y % 400 == 0)) {
                maxDays[1] = 29;
            }
            if (d < 1 || d > maxDays[m - 1]) return false;
            return true;
        } catch (...) {
            return false;
        }
    }

    std::string today() {
        std::time_t t = std::time(nullptr);
        std::tm* p = std::localtime(&t);
        std::ostringstream o;
        o << std::setfill('0') << std::setw(2) << p->tm_mday << '/'
          << std::setw(2) << p->tm_mon + 1 << '/'
          << p->tm_year + 1900;
        return o.str();
    }

    std::string inputDateString(const std::string& prompt) {
        while (true) {
            std::string date = inputNonEmptyString(prompt);
            if (isValidDate(date)) {
                return date;
            }
            std::cout << "Dinh dang ngay khong hop le! Vui long nhap theo dinh dang DD/MM/YYYY (VD: 12/10/2026)." << std::endl;
        }
    }

    double inputDouble(const std::string& prompt, double minVal) {
        while (true) {
            std::string s = inputNonEmptyString(prompt);
            try {
                size_t pos = 0;
                double val = std::stod(s, &pos);
                if (pos == s.length() && val >= minVal) {
                    return val;
                }
            } catch (...) {}
            std::cout << "Gia tri khong hop le! Vui long nhap so thuc >= " << minVal << "." << std::endl;
        }
    }

    int inputInt(const std::string& prompt, int minVal, int maxVal) {
        while (true) {
            std::string s = inputNonEmptyString(prompt);
            try {
                size_t pos = 0;
                int val = std::stoi(s, &pos);
                if (pos == s.length() && val >= minVal && val <= maxVal) {
                    return val;
                }
            } catch (...) {}
            std::cout << "Gia tri khong hop le! Vui long nhap so nguyen trong khoang [" 
                      << minVal << " - " << maxVal << "]." << std::endl;
        }
    }

    std::string join(const std::vector<std::string>& vec, const std::string& delemiter) {
    std::string result = "";
    for (size_t i = 0; i < vec.size(); i++) {
        result += vec[i];
        if (i < vec.size() - 1) result += delemiter;
    }

    return result.empty() ? "Chua phan cong" : result;
    }

    void backChoice() {
        int backChoice;
        while (true) {
            std::cout << "\n0. Quay lai menu" << std::endl;
            std::cout << "Moi nhap lua chon: ";

            if (!(std::cin >> backChoice)) {
                std::cin.clear();
                std::cin.ignore(10000, '\n');
                std::cout << "Loi: Vui long nhap so 0!" << std::endl;
                continue;
            }
            std::cin.ignore(10000, '\n');

            if (backChoice == 0) {
                break;
            } else {
                std::cout << "Loi: Chuc nang khong ton tai! Vui long nhap 0 de quay lai." << std::endl;
            }
        }
    }

    void printEntityDetails(const BaseEntity& entity) {
        std::cout << "\n--- Thong tin chi tiet thuc the ---" << std::endl;
        entity.printInfo();
        std::cout << "-----------------------------------" << std::endl;
    }

    std::string resolveDataPath(const std::string& relativePath) {
        if (relativePath.empty()) return "";

        std::filesystem::path p(relativePath);
        if (p.is_absolute()) {
            return p.lexically_normal().string();
        }

#if defined(_WIN32)
        // 1. Uu tien tim thu muc goc du an tu duong dan file thuc thi (.exe)
        char exePathBuf[MAX_PATH];
        if (GetModuleFileNameA(NULL, exePathBuf, MAX_PATH) > 0) {
            std::filesystem::path currExe = std::filesystem::path(exePathBuf).parent_path();
            for (int i = 0; i < 6; ++i) {
                std::filesystem::path dataDir = currExe / "data";
                if (std::filesystem::exists(dataDir) && std::filesystem::is_directory(dataDir) &&
                    (std::filesystem::exists(currExe / "CMakeLists.txt") || std::filesystem::exists(currExe / "main"))) {
                    return (currExe / relativePath).lexically_normal().string();
                }
                if (!currExe.has_parent_path() || currExe == currExe.parent_path()) break;
                currExe = currExe.parent_path();
            }
        }
#endif

        // 2. Tim thu muc goc tu thu muc lam viec hien tai (current_path)
        std::filesystem::path curr = std::filesystem::current_path();
        for (int i = 0; i < 6; ++i) {
            std::filesystem::path dataDir = curr / "data";
            if (std::filesystem::exists(dataDir) && std::filesystem::is_directory(dataDir) &&
                (std::filesystem::exists(curr / "CMakeLists.txt") || std::filesystem::exists(curr / "main"))) {
                return (curr / relativePath).lexically_normal().string();
            }
            if (!curr.has_parent_path() || curr == curr.parent_path()) break;
            curr = curr.parent_path();
        }

        // 3. Fallback: kiem tra truc tiep o thu muc cha
        if (std::filesystem::exists("../data") && std::filesystem::is_directory("../data")) {
            return std::filesystem::absolute("../" + relativePath).lexically_normal().string();
        }

        return std::filesystem::absolute(relativePath).lexically_normal().string();
    }
}