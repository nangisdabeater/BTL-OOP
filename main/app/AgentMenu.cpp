#include "AgentMenu.h"
#include <iostream>
#include <iomanip>
#include <Utilities.h>

// Chỉ dùng using namespace std ở file .cpp để không làm ô nhiễm không gian tên ở file .h
using namespace std;

// Khởi tạo đối tượng AgentMenu bằng danh sách khởi tạo cho 2 tham chiếu Repository
AgentMenu::AgentMenu(Repository<Agent>& agRepo, Repository<AgentShift>& asRepo)
  : agentRepository(agRepo), agentShiftRepository(asRepo) {}

// Hiển thị bảng danh mục các chức năng quản lý Điện thoại viên
void AgentMenu::showMenu() {
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

// Điều phối vòng lặp xử lý các lựa chọn trong menu Điện thoại viên
void AgentMenu::run() {
  bool inMenu = true;
  while (inMenu) {
    showMenu();

    int choice;
    cout << "Nhap lua chon: ";
    // Kiểm tra nếu nhập sai kiểu số nguyên thì xóa cờ lỗi cin để tránh treo chương trình
    if (!(cin >> choice)) {
      cin.clear();
      cin.ignore(10000, '\n');
      cout << "Lua chon khong hop le!" << endl << endl;
      continue;
    }
    cin.ignore(10000, '\n');

    switch (choice) {
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

// Hiển thị danh sách tất cả điện thoại viên dưới dạng bảng
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

// Tìm kiếm điện thoại viên theo mã định danh ID
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

// Thêm mới điện thoại viên: kiểm tra tính duy nhất của số điện thoại, email và số máy lẻ
void AgentMenu::add() {
    cout << "--- Them dien thoai vien moi ---" << endl;
    string fullName = Utils::inputNonEmptyString("Nhap ho va ten dien thoai vien (nhap 0 de quay lai): ");
    if (fullName == "0") {
      return;
    }

    string phoneNumber = Utils::inputNonEmptyString("Nhap so dien thoai di dong: ");
    // Kiểm tra trùng số điện thoại với các điện thoại viên hiện có
    for (const auto& existing : agentRepository.getAll()) {
      if (existing.getPhoneNumber() == phoneNumber) {
        cout << "Loi rang buoc: So dien thoai '" << phoneNumber << "' da ton tai tren he thong!" << endl;
        Utils::backChoice();
        return;
      }
    }

    string email = Utils::inputNonEmptyString("Nhap dia chi email: ");
    // Kiểm tra trùng địa chỉ email
    for (const auto& existing : agentRepository.getAll()) {
      if (existing.getEmail() == email) {
        cout << "Loi rang buoc: Email '" << email << "' da ton tai tren he thong!" << endl;
        Utils::backChoice();
        return;
      }
    }

    string extensionPhone = Utils::inputNonEmptyString("Nhap so may le: ");
    // Kiểm tra trùng số máy lẻ nội bộ
    for (const auto& existing : agentRepository.getAll()) {
      if (existing.getExtensionPhone() == extensionPhone) {
        cout << "Loi rang buoc: So may le '" << extensionPhone << "' da ton tai tren he thong!" << endl;
        Utils::backChoice();
        return;
      }
    }

    Agent newAg(fullName, phoneNumber, email, extensionPhone);
    try {
      agentRepository.add(newAg);
      const auto& list = agentRepository.getAll();
      cout << "Them dien thoai vien thanh cong! Ma ID duoc cap: " << list.back().getId() << endl;
    } catch (const exception& e) {
      cout << "Loi khi them dien thoai vien: " << e.what() << endl;
    }
    Utils::backChoice();
}

// Cập nhật thông tin điện thoại viên: nhấn Enter để giữ nguyên từng trường không đổi
void AgentMenu::update() {
  cout << "--- Cap nhat thong tin dien thoai vien ---" << endl;
  string id = Utils::inputNonEmptyString("Nhap ma dien thoai vien can cap nhat (vi du AG001, nhap 0 de quay lai): ");
  if (id == "0") {
    return;
  }

  Agent* ag = agentRepository.findById(id);
  if (ag == nullptr) {
    cout << "Loi: Khong tim thay dien thoai vien voi ma: " << id << endl;
    Utils::backChoice();
    return;
  }

  // Tạo bản sao để chỉnh sửa tạm trước khi ghi đè vào Repository
  Agent updated = *ag;

  cout << "Thong tin hien tai:" << endl;
  updated.printInfo();

  cout << "Nhap ho va ten moi (nhan Enter de giu nguyen): ";
  string newFullName;
  getline(cin, newFullName);
  newFullName = Utils::trim(newFullName);
  if (!newFullName.empty()) {
    updated.setFullName(newFullName);
  }

  cout << "Nhap so dien thoai moi (nhan Enter de giu nguyen): ";
  string newPhoneNumber;
  getline(cin, newPhoneNumber);
  newPhoneNumber = Utils::trim(newPhoneNumber);
  if (!newPhoneNumber.empty()) {
    for (const auto& other : agentRepository.getAll()) {
      if (other.getId() != id && other.getPhoneNumber() == newPhoneNumber) {
        cout << "Loi rang buoc: So dien thoai '" << newPhoneNumber << "' da ton tai!" << endl;
        Utils::backChoice();
        return;
      }
    }
    updated.setPhoneNumber(newPhoneNumber);
  }

  cout << "Nhap email moi (nhan Enter de giu nguyen): ";
  string newEmail;
  getline(cin, newEmail);
  newEmail = Utils::trim(newEmail);
  if (!newEmail.empty()) {
    for (const auto& other : agentRepository.getAll()) {
      if (other.getId() != id && other.getEmail() == newEmail) {
        cout << "Loi rang buoc: Email '" << newEmail << "' da ton tai!" << endl;
        Utils::backChoice();
        return;
      }
    }
    updated.setEmail(newEmail);
  }

  cout << "Nhap so may le moi (nhan Enter de giu nguyen): ";
  string newExtensionPhone;
  getline(cin, newExtensionPhone);
  newExtensionPhone = Utils::trim(newExtensionPhone);
  if (!newExtensionPhone.empty()) {
    for (const auto& other : agentRepository.getAll()) {
      if (other.getId() != id && other.getExtensionPhone() == newExtensionPhone) {
        cout << "Loi rang buoc: So may le '" << newExtensionPhone << "' da ton tai!" << endl;
        Utils::backChoice();
        return;
      }
    }
    updated.setExtensionPhone(newExtensionPhone);
  }

  cout << "Doi trang thai (1: Hoat dong, 0: Tam dung, Enter: Giu nguyen): ";
  string statusChoice;
  getline(cin, statusChoice);
  statusChoice = Utils::trim(statusChoice);
  if (statusChoice == "1") {
    updated.activate();
  } else if (statusChoice == "0") {
    updated.deactivate();
  }

  try {
    if (agentRepository.update(updated)) {
      cout << "Cap nhat dien thoai vien thanh cong!" << endl;
    } else {
      cout << "Loi khi cap nhat dien thoai vien!" << endl;
    }
  } catch (const exception& e) {
    cout << "Loi khi cap nhat dien thoai vien: " << e.what() << endl;
  }
  Utils::backChoice();
}

// Xóa điện thoại viên: kiểm tra ràng buộc toàn vẹn với Ca trực trước khi xóa
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

  // Kiểm tra ràng buộc toàn vẹn dữ liệu: chặn xóa nếu nhân viên đang được phân công trong Ca trực (AgentShift)
  for (const auto& shift : agentShiftRepository.getAll()) {
    for (const auto& assignedId : shift.getAgentIds()) {
      if (assignedId == id) {
        cout << "Loi rang buoc toan ven: Khong the xoa dien thoai vien '" << id
             << "' vi dang duoc phan cong trong ca truc '" << shift.getShiftName()
             << "' (Ma ca: " << shift.getId() << ")!" << endl;
        cout << "Vui long go bo nhan vien khoi ca truc truoc khi xoa." << endl;
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
