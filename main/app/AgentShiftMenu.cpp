#include "AgentShiftMenu.h"
#include <iostream>
#include <iomanip>
#include "../Utilities/Utilities.h"

// Chỉ dùng using namespace std ở file .cpp để tránh xung đột tên ở các file header
using namespace std;

// Khởi tạo đối tượng AgentShiftMenu với tham chiếu tới kho Ca trực và kho Điện thoại viên
AgentShiftMenu::AgentShiftMenu(Repository<AgentShift>& asRepo, Repository<Agent>& agRepo)
  : agentRepository(agRepo), agentShiftRepository(asRepo) {}

// Hiển thị bảng danh mục các chức năng quản lý Ca trực
void AgentShiftMenu::showMenu() {
    cout << "========================================" << endl;
    cout << "        QUAN LY CA TRUC" << endl;
    cout << "========================================" << endl;
    cout << "1. Hien thi danh sach cac ca truc" << endl;
    cout << "2. Tim kiem ca truc theo ID" << endl;
    cout << "3. Them ca truc moi" << endl;
    cout << "4. Cap nhat thong tin ca truc" << endl;
    cout << "5. Xoa thong tin ca truc" << endl;
    cout << "6. Quan ly phan cong nhan vien cho ca truc" << endl;
    cout << "0. Quay lai menu chinh" << endl;
    cout << "========================================" << endl;
}

// Điều phối vòng lặp xử lý các lựa chọn trong menu Ca trực
void AgentShiftMenu::run() {
  bool inMenu = true;
  while (inMenu) {
    showMenu();

    int choice;
    cout << "Nhap lua chon: ";
    // Kiểm tra nếu nhập sai định dạng số nguyên thì xóa cờ lỗi của cin
    if (!(cin >> choice)) {
      cin.clear();
      cin.ignore(1000, '\n');
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
    case 6:
      manageAgentShift();
      break;
    case 0:
      inMenu = false;
      break;
    default:
      cout << "Lua chon khong hop le!" << endl;
    }
  }
}

// Hiển thị danh sách tất cả ca trực kèm danh sách mã nhân viên đảm nhận
void AgentShiftMenu::displayAll() {
  const auto& list = agentShiftRepository.getAll();
  if (list.empty()) {
    cout << "Danh sach ca truc dang trong!" << endl;
  } else {
    cout << left << setw(8) << "Ma ID" 
       << setw(25) << "Ca truc"
       << setw(25) << "Ngay"
       << setw(25) << "Bat dau"
       << setw(25) << "Ket thuc"
       << setw(25) << "Nhan vien dam nhan"
       << setw(15) << "Trang thai" << endl;
    cout << string(140, '-') << endl;

    for (const auto& as : list) {
    cout << left << setw(8) << as.getId() 
        << setw(25) << as.getShiftName()
        << setw(25) << as.getShiftDate()
        << setw(25) << as.getShiftStart()
        << setw(25) << as.getShiftEnd()
        // Nối mảng mã nhân viên thành chuỗi ngăn cách bằng dấu phẩy để in gọn trong 1 cột
        << setw(25) << Utils::join(as.getAgentIds())
        << setw(15) << (as.isActive() ? "Hoat dong" : "Tam dung") << endl;
    }
  }
  Utils::backChoice();
}

// Tìm kiếm ca trực theo mã định danh ID
void AgentShiftMenu::findById() {
  string id = Utils::inputNonEmptyString("Nhap ma ca truc can tim (vi du AS001, nhap 0 de quay lai): ");
  if (id == "0") {
    return;
  }
  AgentShift* as = agentShiftRepository.findById(id);

  if (as == nullptr) {
    cout << "Ca truc voi id ban nhap khong ton tai!" << endl;
    Utils::backChoice();
    return;
  }

  Utils::printEntityDetails(*as);
  Utils::backChoice();
}

// Thêm mới ca trực: dùng hàm kiểm tra trùng lặp với biểu thức lambda để chặn trùng khung giờ trong cùng ngày
void AgentShiftMenu::add() {
    cout << "--- Them ca truc moi ---" << endl;
    const auto& list = agentShiftRepository.getAll();

    string shiftName = Utils::inputNonEmptyString("Nhap ten ca truc (nhap 0 de quay lai): ");
    if (shiftName == "0") {
      return;
    }
    string shiftDate = Utils::inputNonEmptyString("Nhap ngay truc: ");
    
    string shiftStart;
    while (true) {
      shiftStart = Utils::inputNonEmptyString("Nhap gio bat dau: ");
      // Dùng biểu thức lambda truyền điều kiện so sánh ngày và giờ bắt đầu vào hàm mẫu isDuplicate
      bool isShiftStartExisted = ValidationUtils::isDuplicate(list, [&](const AgentShift& as) {
      return as.getShiftStart() == shiftStart && as.getShiftDate() == shiftDate;
      });
      if (isShiftStartExisted) {
        cout << "Gio bat dau luc '" << shiftStart << "' da duoc tao! Vui long nhap gio truc khac!" << endl;
      } else {
        break;
      }
    }

    string shiftEnd;
    while (true) {
      shiftEnd = Utils::inputNonEmptyString("Nhap gio ket thuc: ");
      bool isShiftEndExisted = ValidationUtils::isDuplicate(list, [&](const AgentShift& as) {
      return as.getShiftEnd() == shiftEnd && as.getShiftDate() == shiftDate;
      });
      if (isShiftEndExisted) {
        cout << "Gio ket thuc luc '" << shiftEnd << "' da duoc tao! Vui long nhap gio truc khac!" << endl;
      } else {
        break;
      }
    }
    
    AgentShift newAs(shiftName, shiftDate, shiftStart, shiftEnd);

    try {
        agentShiftRepository.add(newAs);
        const auto& list = agentShiftRepository.getAll();
        cout << "Tao ca truc thanh cong! Ma duoc cap: " << list.back().getId() << endl;
    }
    catch (const exception& e) {
        cout << "Loi khi them ca truc: " << e.what() << endl;
    }
    Utils::backChoice();
}

// Cập nhật thông tin ca trực: cho phép nhấn Enter để giữ nguyên từng trường
void AgentShiftMenu::update() {
  cout << "--- Cap nhat thong tin ca truc ---" << endl;
  string id = Utils::inputNonEmptyString("Nhap ma ca truc can cap nhat (nhap 0 de quay lai): ");
  if (id == "0") {
    return;
  }

  AgentShift* as = agentShiftRepository.findById(id);
  if (as == nullptr) {
      cout << "Loi: Khong tim thay ca truc voi ma: " << id << endl;
      Utils::backChoice();
      return;
  }

  AgentShift updated = *as;
  const auto& list = agentShiftRepository.getAll();

  cout << "Thong tin hien tai:" << endl;
  updated.printInfo();

  // Cập nhật tên ca trực
  cout << "Nhap ten moi (nhan Enter de giu nguyen): ";
  string newName;
  getline(cin, newName);
  newName = Utils::trim(newName);
  if (!newName.empty()) {
    updated.setShiftName(newName);
  }

  // Cập nhật ngày trực
  cout << "Nhap ngay truc (nhan Enter de giu nguyen): ";
  string newShiftDate;
  getline(cin, newShiftDate);
  newShiftDate = Utils::trim(newShiftDate);
  if (!newShiftDate.empty()) {
    updated.setShiftDate(newShiftDate);
  }

  // Cập nhật giờ bắt đầu và kiểm tra không trùng với ca trực khác cùng ngày
  string newShiftStart;
  while (true) {
    cout << "Nhap gio bat dau moi (nhan Enter de giu nguyen): ";
    getline(cin, newShiftStart);
    newShiftStart = Utils::trim(newShiftStart); 

    if (newShiftStart.empty()) {
      break;
    }
    bool isShiftStartExisted = ValidationUtils::isDuplicate(list, [&](const AgentShift& as) {
    return as.getId() != updated.getId() && as.getShiftDate() == updated.getShiftDate() && as.getShiftStart() == newShiftStart;
    });
    if (isShiftStartExisted) {
      cout << "Gio bat dau truc '" << newShiftStart << "' da ton tai! Vui long nhap gio truc khac!" << endl;
    } else {
      updated.setShiftStart(newShiftStart);
      break;
    }
  }

  // Cập nhật giờ kết thúc và kiểm tra không trùng với ca trực khác cùng ngày
  string newShiftEnd;
  while (true) {
    cout << "Nhap gio ket thuc moi (nhan Enter de giu nguyen): ";
    getline(cin, newShiftEnd);
    newShiftEnd = Utils::trim(newShiftEnd); 

    if (newShiftEnd.empty()) {
      break;
    }
    bool isShiftEndExisted = ValidationUtils::isDuplicate(list, [&](const AgentShift& as) {
    return as.getId() != updated.getId() && as.getShiftDate() == updated.getShiftDate() && as.getShiftEnd() == newShiftEnd;
    });
    if (isShiftEndExisted) {
      cout << "Gio ket thuc truc '" << newShiftEnd << "' da ton tai! Vui long nhap gio truc khac!" << endl;
    } else {
      updated.setShiftEnd(newShiftEnd);
      break;
    }
  }

  // Cập nhật trạng thái hoạt động
  cout << "Doi trang thai (1: Hoat dong, 0: Tam dung, Enter: Giu nguyen): ";
  string statusChoice;
  getline(cin, statusChoice);
  statusChoice = Utils::trim(statusChoice);
  if (statusChoice == "1") {
      updated.activate();
  }
  else if (statusChoice == "0") {
      updated.deactivate();
  }

  try {
    if (agentShiftRepository.update(updated)) {
      cout << "Cap nhat ca truc thanh cong!" << endl;
    }
    else {
      cout << "Loi khi cap nhat ca truc!" << endl;
    }
  }
  catch (const exception& e) {
    cout << "Loi khi cap nhat ca truc: " << e.what() << endl;
  }
  Utils::backChoice();
}

// Xóa ca trực: chặn xóa nếu trong ca trực vẫn còn nhân viên đang được phân công
void AgentShiftMenu::remove() {
  cout << "--- Xoa ca truc ---" << endl;
  string id = Utils::inputNonEmptyString("Nhap ma ca truc can xoa (nhap 0 de quay lai): ");
  if (id == "0") {
    return;
  }

  AgentShift* as = agentShiftRepository.findById(id);
  if (as == nullptr) {
    cout << "Loi: Khong tim thay ca truc voi ma: " << id << endl;
    Utils::backChoice();
    return;
  }

  const vector<string>& agentIds = as->getAgentIds();

  if (!(agentIds.empty())) {
    cout << "Loi rang buoc toan ven: Khong the xoa ca truc " << as->getShiftName() << "!" << endl;
    cout << "Ly do: Ca truc da co nhan vien dang ky!" << endl;
    cout << "Vui long vao menu Ca Truc -> Xoa nhan vien khoi ca truoc khi xoa ho so ho!" << endl;
    Utils::backChoice();
    return;
  }

  cout << "Thong tin ca truc can xoa:" << endl;
  as->printInfo();
  cout << "Ban co chac chan muon xoa ca truc: " << id  << " (" << as->getShiftName() << ")? (Y/N): ";
  string confirm;
  getline(cin, confirm);
  confirm = Utils::trim(confirm);

  if (confirm == "y" || confirm == "Y") {
    try {
      if (agentShiftRepository.remove(id)) {
        cout << "Da xoa ca truc thanh cong!" << endl;
      } else {
        cout << "Loi: Khong the xoa ca truc!" << endl;
      }
    }
    catch (const exception& e) {
      cout << "Loi khi xoa ca truc: " << e.what() << endl;
    }
  } else {
    cout << "Da huy thao tac xoa." << endl;
  }

  Utils::backChoice();
}

// Quản lý phân công nhân viên vào ca trực (thêm hoặc gỡ mã nhân viên khỏi ca)
void AgentShiftMenu::manageAgentShift() {
  cout << "--- Quan ly phan cong nhan vien trong ca truc ---" << endl;
  string shiftId = Utils::inputNonEmptyString("Nhap ma ca truc can quan ly (VD: AS001, nhap 0 de quay lai): ");
  if (shiftId == "0") {
    return;
  }

  AgentShift* as = agentShiftRepository.findById(shiftId);

  if (as == nullptr) {
    cout << "Loi: Khong tim thay ca truc voi ma: " << shiftId << endl;
    Utils::backChoice();
    return;
  }

  bool inSubMenu = true;
  while (inSubMenu) {
    AgentShift* currentShift = agentShiftRepository.findById(shiftId);

    cout << "Ca truc: " << currentShift->getShiftName() << " | Ngay: " << currentShift->getShiftDate() << endl;
    cout << "Nhan vien hien tai: ";

    const vector<string>& agentIds = currentShift->getAgentIds();
    if (agentIds.empty()) {
      cout << "Chua phan cong";
    } else {
      for (size_t i = 0; i < agentIds.size(); ++i) {
        Agent* ag = agentRepository.findById(agentIds[i]);
        if (ag != nullptr) {
          cout << agentIds[i] << " (" << ag->getFullName() << ")";
        } else {
          cout << agentIds[i];
        }
        if (i < agentIds.size() - 1) cout << ", ";
      }
    }
    cout << "\n========================================" << endl;
    cout << "1. Them nhan vien vao ca" << endl;
    cout << "2. Xoa nhan vien khoi ca" << endl;
    cout << "0. Quay lai" << endl;
    cout << "========================================" << endl;

    int choice;
    cout << "Nhap lua chon: ";
    if (!(cin >> choice)) {
      cin.clear();
      cin.ignore(10000, '\n');
      cout << "Lua chon khong hop le!" << endl;
      continue;
    }
    cin.ignore(10000, '\n');

    switch (choice) {
    case 1: {
        string agentId = Utils::inputNonEmptyString("Nhap ma nhan vien can them (VD: AG001, nhap 0 de quay lai): ");
        if (agentId == "0") break;

        // Kiểm tra khóa ngoại: mã nhân viên phải tồn tại và đang hoạt động mới được phân công
        Agent* ag = agentRepository.findById(agentId);
        if (ag == nullptr) {
          cout << "Loi: Nhan vien " << agentId << " khong ton tai trong he thong!" << endl;
          break;
        }
        if (!ag->isActive()) {
          cout << "Loi rang buoc: Dien thoai vien '" << ag->getFullName() << "' (" << agentId << ") dang tam dung hoat dong, khong the phan cong vao ca truc!" << endl;
          break;
        }

        try {
          AgentShift copyShift = *currentShift;
          copyShift.addAgentId(agentId);

          if (agentShiftRepository.update(copyShift)) {
            cout << "-> Da them thanh cong!" << endl;
          }
        }
        catch(const std::exception& e) {
          std::cerr << e.what() << '\n';
        }
        break;
      }
    case 2: {
        string agentId = Utils::inputNonEmptyString("Nhap ma nhan vien can xoa (VD: AG001, nhap 0 de quay lai): ");
        if (agentId == "0") break;

        try {
          AgentShift copyShift = *currentShift;
          copyShift.removeAgentId(agentId);

          if (agentShiftRepository.update(copyShift)) {
            cout << "-> Da xoa nhan vien khoi ca!" << endl;
          }
        }
        catch(const std::exception& e) {
          std::cerr << e.what() << '\n';
        }
        break;
      }
    case 0: 
      inSubMenu = false;
      break;
    default:
      cout << "Lua chon khong hop le!" << endl;
    }
  }
}
