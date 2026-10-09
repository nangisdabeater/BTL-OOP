#include "AgentMenu.h"
#include <iostream>
#include <iomanip>
#include <Utilities.h>

using namespace std;

/**
 * @brief Khởi tạo đối tượng ServiceGroupMenu
 * @param agRepo Tham chiếu tới kho lưu trữ Điện thoại viên
 * @param sRepo Tham chiếu tới kho lưu trữ Shift (để kiểm tra ràng buộc toàn vẹn)
 */

AgentMenu::AgentMenu(Repository<Agent>& agRepo)
  : agentRepository(agRepo)
{}

/**
 * @brief Hiển thị bảng danh mục các chức năng quản lý Dien thoai vien.
 */

void AgentMenu::showMenu()
{
    cout << "========================================" << endl;
    cout << "        QUAN LY DIEN THOAI VIEN" << endl;
    cout << "========================================" << endl;
    cout << "1. Hien thi danh sach ten cac dien thoai vien" << endl;
    cout << "2. Tim kiem dien thoai vien theo ID" << endl;
    cout << "3. Them dien thoai vien moi" << endl;
    cout << "4. Cap nhat thong tin dien thoai vien" << endl;
    cout << "5. Xoa thong tin dien thoai vien" << endl;
    cout << "0. Quay lai menu chinh" << endl;
    cout << "========================================" << endl;
}

/**
 * @brief Điều phối vòng lặp xử lý các lựa chọn trong menu Dien thoai vien.
 */
void AgentMenu::run() {
  bool inMenu = true;
  while (inMenu) {
    showMenu();

    int choice;
    cout << "Nhap lua chon: ";
    if (!(cin >> choice)) {
      cin.clear();
      cin.ignore(1000, '\n');
      cout << "Lua chon khong hop le!" << endl << endl;
      continue;
    }
    cin.ignore(10000, '\n');
    
    switch (choice)
    {
    case 1:
      displayAll();
      break;
    case 2:
      findById();
      break;
    case 3:
      add();
      break;
    case 4:
      update();
      break;
    case 5:
      remove();
      break;
    case 0:
      inMenu = false;
      break;
    default:
      cout << "Lua chon khong hop le!" << endl;
    }
  }
}

/**
 * @brief Chức năng hiển thị danh sách tất cả điện thoại viên (Read All)
 * Hiển thị đầy đủ 4 thuộc tính: Mã ID, Tên, Mô tả, Trạng thái hoạt động
 */
void AgentMenu::displayAll() {
  const auto& list = agentRepository.getAll();
  if (list.empty()) {
    cout << "Danh sach nhan vien ca truc dang trong!" << endl;
  } else {
    cout << left << setw(8) << "Ma ID" 
       << setw(25) << "Ho va ten"
       << setw(25) << "So dien thoai"
       << setw(25) << "Email"
       << setw(25) << "So dien thoai duoc cap"
       << setw(15) << "Trang thai" << endl;
    cout << string(95, '-') << endl;

    for (const auto& ag : list) {
    cout << left << setw(8) << ag.getId() 
        << setw(25) << ag.getFullName()
        << setw(25) << ag.getPhoneNumber()
        << setw(25) << ag.getEmail()
        << setw(25) << ag.getExtensionPhone()
        << setw(15) << (ag.isActive() ? "Hoat dong" : "Tam dung") << endl;
    }
  }
  int backChoice;
  while (true) {
    cout << "0. Quay lai menu chinh" << endl;
    if (!(cin >> backChoice)) {
      cin.clear();
      cin.ignore(10000, '\n');
      continue;
    }

    cin.ignore(10000, '\n');

    if (backChoice == 0) {
      break;
    }
  }
}

/**
 * @brief Chức năng tìm kiếm điện thoại viên theo mã ID (Read by ID)
 */
void AgentMenu::findById() {
  string id = Utils:: inputNonEmtpyString("Nhap ma so dien thoai vien can tim (vi du AG001): ");
  Agent* ag = agentRepository.findById(id);

  if (ag == nullptr) {
    cout << "Khong tim thay dien thoai vien voi ma: " << id << endl;
    return;
  }

  cout << "--- Thong tin dien thoai vien ---" << endl;
  cout << "Ma dinh danh      : " << ag->getId() << endl;
  cout << "Ho va ten   : " << ag->getFullName() << endl;
  cout << "So dien thoai     : " << ag->getPhoneNumber() << endl;
  cout << "Email     : " << ag->getEmail() << endl;
  cout << "So dien thoai duoc cap     : " << ag->getExtensionPhone() << endl;
  cout << "Trang thai : " << (ag->isActive() ? "Hoat dong" : "Tam dung") << endl;
}

/**
 * @brief Chức năng thêm mới điện thoại viên (Create)
 * Ràng buộc: Tên và mô tả không được rỗng, Tên nhóm dịch vụ không được trùng lặp
 */
void AgentMenu::add()
{
    cout << "--- Them dien thoai vien moi ---" << endl;
    const auto& list = agentRepository.getAll();

    string fullName = Utils::inputNonEmtpyString("Nhap ho va ten dien thoai vien: ");

    string phoneNumber;
    while (true)
    {
      phoneNumber = Utils::inputNonEmtpyString("Nhap so dien thoai: ");
      bool isPhoneNumberExisted = ValidationUtils::isDuplicate(list, [&](const Agent& ag) {
      return ag.getPhoneNumber() == phoneNumber;
      });
      if (isPhoneNumberExisted) {
        cout << "So dien thoai '" << phoneNumber << "' da ton tai! Vui long nhap so dien thoai khac!" << endl;
      } else {
        break;
      }
    }
    
    string email;
    while (true)
    {
      email = Utils::inputNonEmtpyString("Nhap email: ");
      bool isEmailExisted = ValidationUtils::isDuplicate(list, [&](const Agent& ag) {
      return ag.getEmail() == email;
      });
      if (isEmailExisted) {
        cout << "Email '" << email << "' da ton tai! Vui long nhap email khac!" << endl;
      } else {
        break;
      }
    }

    string extensionPhone = Utils::inputNonEmtpyString("Nhap so tong dai duoc cap: ");
    
    Agent newAg(fullName, phoneNumber, email,  extensionPhone);

    try
    {
        agentRepository.add(newAg);
        const auto& list = agentRepository.getAll();
        cout << "Dien thoai vien thanh cong! Ma duoc cap: " << list.back().getId() << endl;
    }
    catch (const exception& e)
    {
        cout << "Loi khi them nhom dich vu: " << e.what() << endl;
    }
}

/**
 * @brief Chức năng cập nhật thông tin nhóm dịch vụ (Update)
 * Ràng buộc: ID phải tồn tại, tên mới không được trùng với các nhóm khác
 */
void AgentMenu::update()
{
  cout << "--- Cap nhat thong tin nhom dich vu ---" << endl;
  string id = Utils::inputNonEmtpyString("Nhap ma dien thoai vien can cap nhat: ");

  Agent* ag = agentRepository.findById(id);
  if (ag == nullptr)
  {
      cout << "Loi: Khong tim thay dien thoai vien voi ma: " << id << endl;
      return;
  }

  Agent updated = *ag;
  const auto& list = agentRepository.getAll();

  cout << "Thong tin hien tai: [" << updated.getId() << "] " << updated.getFullName()
        << " | " << updated.getPhoneNumber()
        << " | " << updated.getEmail()
        << " | " << updated.getExtensionPhone()
        << " | " << (updated.isActive() ? "Hoat dong" : "Tam dung") << endl;

  // Cập nhật họ và tên điện thoại viên
  cout << "Nhap ten moi (nhan Enter de giu nguyen): ";
  string newName;
  getline(cin, newName);
  newName = Utils::trim(newName);
  if (!newName.empty())
  {
      updated.setFullName(newName);
  }

  // Cập nhật số điện thoại
  string newPhoneNum;
  while (true) {
    cout << "Nhap so dien thoai moi (nhan Enter de giu nguyen): ";
    getline(cin, newPhoneNum);
    newPhoneNum = Utils::trim(newPhoneNum); 
    bool isPhoneNumberExisted = ValidationUtils::isDuplicate(list, [&](const Agent& ag) {
    return ag.getPhoneNumber() == newPhoneNum;
    });
    if (isPhoneNumberExisted) {
      cout << "So dien thoai '" << newPhoneNum << "' da ton tai! Vui long nhap so dien thoai khac!" << endl;
    } else {
      updated.setPhoneNumber(newPhoneNum);
      break;
    }
  }
  // Cập nhật email
  string newEmail;
  while (true) {
    cout << "Nhap so email moi (nhan Enter de giu nguyen): ";
    getline(cin, newEmail);
    newEmail = Utils::trim(newEmail); 
    bool isEmailExisted = ValidationUtils::isDuplicate(list, [&](const Agent& ag) {
    return ag.getEmail() == newEmail;
    });
    if (isEmailExisted) {
      cout << "Email '" << newEmail << "' da ton tai! Vui long nhap email khac!" << endl;
    } else {
      updated.setEmail(newEmail);
      break;
    }
  }
  // Cập nhật số điện thoại tổng đài
  cout << "Nhap so dien thoai tong dai moi (nhan Enter de giu nguyen): ";
  string newExtensionPhone;
  getline(cin, newExtensionPhone);
  newExtensionPhone = Utils::trim(newExtensionPhone);
  if (!newName.empty())
  {
      updated.setExtensionPhone(newExtensionPhone);
  }

  // Cập nhật trạng thái hoạt động
  cout << "Doi trang thai (1: Hoat dong, 0: Tam dung, Enter: Giu nguyen): ";
  string statusChoice;
  getline(cin, statusChoice);
  statusChoice = Utils::trim(statusChoice);
  if (statusChoice == "1")
  {
      updated.activate();
  }
  else if (statusChoice == "0")
  {
      updated.deactivate();
  }

  try
  {
    if (agentRepository.update(updated))
    {
      cout << "Cap nhat nhom dich vu thanh cong!" << endl;
    }
    else
    {
      cout << "Loi khi cap nhat nhom dich vu!" << endl;
    }
  }
  catch (const exception& e)
  {
    cout << "Loi khi cap nhat nhom dich vu: " << e.what() << endl;
  }
}

/**
 * @brief Chức năng xóa điện thoại viên (Delete)
 * Ràng buộc: ID phải tồn tại, kiểm tra toàn vẹn dữ liệu (không xóa khi còn ticket liên kết)
 */
void AgentMenu::remove() {
  cout << "--- Xoa dien thoai vien ---" << endl;
  string id = Utils::inputNonEmtpyString("Nhap ma dien thoai vien can xoa: ");

  Agent* ag = agentRepository.findById(id);
  if (ag == nullptr) {
    cout << "Loi: Khong tim thay dien thoai vien voi ma: " << id << endl;
    return;
  }
}
