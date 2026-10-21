#include "AgentMenu.h"
#include <iostream>
#include <iomanip>
#include <Utilities.h>

using namespace std;

/**
 * @brief Khởi tạo đối tượng AgentMenu
 * @param agRepo Tham chiếu tới kho lưu trữ Điện thoại viên
 * @param asRepo Tham chiếu tới kho lưu trữ Ca trực (để kiểm tra ràng buộc toàn vẹn trong cùng phân hệ)
 */
AgentMenu::AgentMenu(Repository<Agent>& agRepo, Repository<AgentShift>& asRepo)
  : agentRepository(agRepo), agentShiftRepository(asRepo)
{}

/**
 * @brief Hiển thị bảng danh mục các chức năng quản lý Điện thoại viên.
 */
void AgentMenu::showMenu()
{
    cout << "========================================" << endl;
    cout << "        QUAN LY DIEN THOAI VIEN" << endl;
    cout << "========================================" << endl;
    cout << "1. Hien thi danh sach dien thoai vien" << endl;
    cout << "2. Tim kiem dien thoai vien theo ID" << endl;
    cout << "3. Them dien thoai vien moi" << endl;
    cout << "4. Cap nhat thong tin dien thoai vien" << endl;
    cout << "5. Xoa thong tin dien thoai vien" << endl;
    cout << "0. Quay lai menu chinh" << endl;
    cout << "========================================" << endl;
}

/**
 * @brief Điều phối vòng lặp xử lý các lựa chọn trong menu Điện thoại viên.
 */
void AgentMenu::run() {
  bool inMenu = true;
  while (inMenu) {
    showMenu();

    int choice;
    cout << "Nhap lua chon: ";
    if (!(cin >> choice)) {
      cin.clear();
      cin.ignore(10000, '\n');
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
    cout << endl;
  }
}

/**
 * @brief Chức năng hiển thị danh sách tất cả điện thoại viên (Read All)
 */
void AgentMenu::displayAll() {
  const auto& list = agentRepository.getAll();
  if (list.empty()) {
    cout << "Danh sach dien thoai vien dang trong!" << endl;
    Utils::backChoice();
    return;
  }
  
  cout << left << setw(8) << "Ma ID" 
       << setw(25) << "Ho va ten"
       << setw(16) << "So dien thoai"
       << setw(26) << "Email"
       << setw(14) << "May le"
       << setw(15) << "Trang thai" << endl;
  cout << string(95, '-') << endl;

  for (const auto& ag : list) {
    cout << left << setw(8) << ag.getId() 
        << setw(25) << ag.getFullName()
        << setw(16) << ag.getPhoneNumber()
        << setw(26) << ag.getEmail()
        << setw(14) << ag.getExtensionPhone()
        << setw(15) << (ag.isActive() ? "Hoat dong" : "Tam dung") << endl;
  }
  Utils::backChoice();
}

/**
 * @brief Chức năng tìm kiếm điện thoại viên theo mã ID (Read by ID)
 */
void AgentMenu::findById() {
  string id = Utils::inputNonEmptyString("Nhap ma so dien thoai vien can tim (vi du AG001, nhap 0 de quay lai): ");
  if (id == "0") {
    return;
  }
  Agent* ag = agentRepository.findById(id);

  if (ag == nullptr) {
    cout << "Khong tim thay dien thoai vien voi ma: " << id << endl;
    Utils::backChoice();
    return;
  }

  Utils::printEntityDetails(*ag);
  Utils::backChoice();
}

/**
 * @brief Chức năng thêm mới điện thoại viên (Create)
 * Ràng buộc: Họ tên, SĐT, Email, Máy lẻ không rỗng; SĐT và Email không được trùng lặp
 */
void AgentMenu::add()
{
    cout << "--- Them dien thoai vien moi ---" << endl;
    const auto& list = agentRepository.getAll();

    string fullName = Utils::inputNonEmptyString("Nhap ho va ten dien thoai vien (nhap 0 de quay lai): ");
    if (fullName == "0") {
        return;
    }

    string phoneNumber;
    while (true)
    {
      phoneNumber = Utils::inputNonEmptyString("Nhap so dien thoai: ");
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
      email = Utils::inputNonEmptyString("Nhap email: ");
      bool isEmailExisted = ValidationUtils::isDuplicate(list, [&](const Agent& ag) {
        return ag.getEmail() == email;
      });
      if (isEmailExisted) {
        cout << "Email '" << email << "' da ton tai! Vui long nhap email khac!" << endl;
      } else {
        break;
      }
    }

    string extensionPhone = Utils::inputNonEmptyString("Nhap so tong dai / may le duoc cap: ");
    
    Agent newAg(fullName, phoneNumber, email, extensionPhone);

    try
    {
        agentRepository.add(newAg);
        const auto& updatedList = agentRepository.getAll();
        cout << "Them dien thoai vien thanh cong! Ma duoc cap: " << updatedList.back().getId() << endl;
    }
    catch (const exception& e)
    {
        cout << "Loi khi them dien thoai vien: " << e.what() << endl;
    }
    Utils::backChoice();
}

/**
 * @brief Chức năng cập nhật thông tin điện thoại viên (Update)
 * Ràng buộc: ID phải tồn tại, SĐT và Email mới không trùng với người khác
 */
void AgentMenu::update()
{
  cout << "--- Cap nhat thong tin dien thoai vien ---" << endl;
  string id = Utils::inputNonEmptyString("Nhap ma dien thoai vien can cap nhat (vi du AG001, nhap 0 de quay lai): ");
  if (id == "0") {
    return;
  }

  Agent* ag = agentRepository.findById(id);
  if (ag == nullptr)
  {
      cout << "Loi: Khong tim thay dien thoai vien voi ma: " << id << endl;
      Utils::backChoice();
      return;
  }

  Agent updated = *ag;
  const auto& list = agentRepository.getAll();

  cout << "Thong tin hien tai:" << endl;
  updated.printInfo();

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
  while (true) {
    cout << "Nhap so dien thoai moi (nhan Enter de giu nguyen): ";
    string newPhoneNum;
    getline(cin, newPhoneNum);
    newPhoneNum = Utils::trim(newPhoneNum); 
    if (newPhoneNum.empty()) {
      break; // Giữ nguyên số cũ
    }
    bool isPhoneNumberExisted = ValidationUtils::isDuplicate(list, [&](const Agent& other) {
      return other.getId() != id && other.getPhoneNumber() == newPhoneNum;
    });
    if (isPhoneNumberExisted) {
      cout << "So dien thoai '" << newPhoneNum << "' da ton tai! Vui long nhap so dien thoai khac!" << endl;
    } else {
      updated.setPhoneNumber(newPhoneNum);
      break;
    }
  }

  // Cập nhật email
  while (true) {
    cout << "Nhap email moi (nhan Enter de giu nguyen): ";
    string newEmail;
    getline(cin, newEmail);
    newEmail = Utils::trim(newEmail); 
    if (newEmail.empty()) {
      break; // Giữ nguyên email cũ
    }
    bool isEmailExisted = ValidationUtils::isDuplicate(list, [&](const Agent& other) {
      return other.getId() != id && other.getEmail() == newEmail;
    });
    if (isEmailExisted) {
      cout << "Email '" << newEmail << "' da ton tai! Vui long nhap email khac!" << endl;
    } else {
      updated.setEmail(newEmail);
      break;
    }
  }

  // Cập nhật số điện thoại tổng đài
  cout << "Nhap so may le tong dai moi (nhan Enter de giu nguyen): ";
  string newExtensionPhone;
  getline(cin, newExtensionPhone);
  newExtensionPhone = Utils::trim(newExtensionPhone);
  if (!newExtensionPhone.empty())
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
      cout << "Cap nhat dien thoai vien thanh cong!" << endl;
    }
    else
    {
      cout << "Loi khi cap nhat dien thoai vien!" << endl;
    }
  }
  catch (const exception& e)
  {
    cout << "Loi khi cap nhat dien thoai vien: " << e.what() << endl;
  }
  Utils::backChoice();
}

/**
 * @brief Chức năng xóa điện thoại viên (Delete)
 * Ràng buộc: ID phải tồn tại, kiểm tra toàn vẹn dữ liệu (không xóa khi còn cuộc gọi liên kết)
 */
void AgentMenu::remove() {
  cout << "--- Xoa dien thoai vien ---" << endl;
  string id = Utils::inputNonEmptyString("Nhap ma dien thoai vien can xoa (vi du AG001, nhap 0 de quay lai): ");
  if (id == "0") {
    return;
  }

  Agent* ag = agentRepository.findById(id);
  if (ag == nullptr) {
    cout << "Loi: Khong tim thay dien thoai vien voi ma: " << id << endl;
    Utils::backChoice();
    return;
  }

  // Kiểm tra ràng buộc toàn vẹn dữ liệu với Ca trực (AgentShift - thuộc phân hệ của Thành)
  for (const auto& shift : agentShiftRepository.getAll()) {
    for (const auto& assignedId : shift.getAgentIds()) {
      if (assignedId == id) {
        cout << "Loi rang buoc toan ven: Khong the xoa dien thoai vien '" << id
             << "' vi dang duoc phan cong trong ca truc '" << shift.getShiftName()
             << "' (Ma ca: " << shift.getId() << ")!" << endl;
        cout << "Vui long vao menu Ca Truc -> Xoa nhan vien khoi ca truoc khi xoa ho so!" << endl;
        Utils::backChoice();
        return;
      }
    }
  }

  cout << "Thong tin dien thoai vien can xoa:" << endl;
  ag->printInfo();
  cout << "Ban co chac chan muon xoa dien thoai vien " << id << " (" << ag->getFullName() << ")? (y/n): ";
  string confirm;
  getline(cin, confirm);
  confirm = Utils::trim(confirm);

  if (confirm == "y" || confirm == "Y") {
    try {
      if (agentRepository.remove(id)) {
        cout << "Da xoa dien thoai vien thanh cong!" << endl;
      } else {
        cout << "Loi: Khong the xoa dien thoai vien!" << endl;
      }
    } catch (const exception& e) {
      cout << "Loi khi xoa dien thoai vien: " << e.what() << endl;
    }
  } else {
    cout << "Da huy thao tac xoa." << endl;
  }
  Utils::backChoice();
}
