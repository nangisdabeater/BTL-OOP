#include "FileManager.h"
#include <iostream>
#include <fstream>
#include <sstream>
#include <filesystem>
#include <iomanip>
#include <stdexcept>
#include "Utilities.h"

using namespace std;

/**
 * @brief Nạp danh sách Điện thoại viên từ file văn bản có kẻ hàng cột
 * @param filePath Đường dẫn file (data/agent.txt)
 * @return Danh sách vector các đối tượng Agent đã đọc được
 */

vector<Agent> FileManager<Agent>::load(const string& filePath) {
  vector<Agent> result;
  ifstream inFile(filePath);
  if (!inFile.is_open()) {
    return result;
  }

  string line;
  while (getline(inFile, line)) {
    if (line.empty()) continue;
    size_t firstChar = line.find_first_not_of(" \t\r\n");
    if (firstChar == string::npos || line[firstChar] == '-') continue;

    stringstream ss(line);
    string id, fullName, phoneNumber, email,extensionPhone, status;

    if (getline(ss, id, '|') &&
        getline(ss, fullName, '|') &&
        getline(ss, phoneNumber, '|') &&
        getline(ss, email, '|') && 
        getline(ss, extensionPhone, '|') &&
        getline(ss, status, '|')) {
          id = Utils::trim(id);
          fullName = Utils::trim(fullName);
          phoneNumber = Utils::trim(phoneNumber);
          email = Utils::trim(email);
          extensionPhone = Utils::trim(extensionPhone);
          status = Utils::trim(status);

          if (id == "Ma ID") {
            continue;
          }

          if (Utils::extractIdNumber(id, "AG") == -1) {
            continue;
          }

          Agent ag(fullName, phoneNumber, email, extensionPhone);
          ag.setId(id);
          if (status == "0") {
            ag.deactivate();
          } else {
            ag.activate();
          }

          result.push_back(ag);
        }
  }

  return result;
}

/**
 * @brief Ghi toàn bộ danh sách Điện thoại viên ra file văn bản kẻ hàng cột ngay ngắn
 * @param filePath Đường dẫn file (data/agents.txt)
 * @param data Dữ liệu cần ghi
 */
void FileManager<Agent>::save(const string& filePath, const vector<Agent>& data) {
  filesystem::path p(filePath);
  if (p.has_parent_path()) {
    filesystem::create_directories(p.parent_path());
  }

  ofstream outFile(filePath);
  if (!outFile.is_open()) {
    throw runtime_error("Khong the mo file de ghi: " + filePath);
  }

  outFile << left
          << setw(8) << "Ma ID" << " | "
          << setw(25) << "Ho va ten" << " | "
          << setw(25) << "So dien thoai" << " | "
          << setw(25) << "Email" << " | " 
          << setw(25) << "So tong dai" << " | "
          << "Trang thai" << "\n";
  outFile << string(95, '-') << "\n";

  for (const auto& ag: data) {
    outFile << left
        << setw(8) << ag.getId() << " | "
        << setw(25) << ag.getFullName() << " | "
        << setw(25) << ag.getPhoneNumber() << " | "
        << setw(25) << ag.getEmail() << " | " 
        << setw(25) << ag.getExtensionPhone() << " | "
        << (ag.isActive() ? 1 : 0) << "\n";
  }
  outFile.flush();
  if (!outFile) {
    throw runtime_error("Khong the ghi du lieu vao file: " + filePath);
  }
}