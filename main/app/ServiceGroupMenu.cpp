#include "ServiceGroupMenu.h"
#include <iostream>
#include <iomanip>
#include <Utilities.h>

using namespace std;

// dùng Danh sách khởi tạo ": serviceGroupRepository(sgRepo)..." vì biến thành viên là kiểu tham chiếu (&),
// mà tham chiếu trong C++ bắt buộc phải được gắn đối tượng ngay lúc khởi tạo chứ không thể gán trong thân hàm {}.
ServiceGroupMenu::ServiceGroupMenu(Repository<ServiceGroup>& sgRepo, Repository<Ticket>& tRepo)
    : serviceGroupRepository(sgRepo), ticketRepository(tRepo) {}

void ServiceGroupMenu::showMenu() {
    cout << "========================================" << endl;
    cout << "    QUAN LY DICH VU (SERVICE MANAGER)" << endl;
    cout << "========================================" << endl;
    cout << "1. Hien thi danh sach dich vu" << endl;
    cout << "2. Tim kiem dich vu theo ID" << endl;
    cout << "3. Them dich vu moi" << endl;
    cout << "4. Cap nhat thong tin dich vu" << endl;
    cout << "5. Xoa dich vu" << endl;
    cout << "0. Quay lai menu chinh" << endl;
    cout << "========================================" << endl;
}

void ServiceGroupMenu::run() {
    bool inMenu = true;
    while (inMenu) {
        showMenu();

        int choice;
        cout << "Nhap lua chon: ";
        // Kiểm tra nếu người dùng nhập chữ cái vào biến int làm luồng cin bị lỗi
        if (!(cin >> choice)) {
            cin.clear();             // Xóa cờ báo lỗi của cin để có thể tiếp tục nhập ở vòng lặp sau
            cin.ignore(10000, '\n'); // Xóa toàn bộ các ký tự chữ bị kẹt trong bộ đệm bàn phím tránh lặp vô tận
            cout << "Lua chon khong hop le!" << endl << endl;
            continue;
        }
        cin.ignore(10000, '\n'); // Xóa ký tự xuống dòng (\n) còn sót lại sau khi dùng cin >> choice

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
            inMenu = false; // Đặt cờ false để thoát vòng lặp quay về menu chính
            break;
        default:
            cout << "Lua chon khong hop le!" << endl;
        }

        cout << endl;
    }
}

void ServiceGroupMenu::displayAll() {
    // Dùng "const auto&" để mượn trực tiếp danh sách từ kho dữ liệu mà không phải copy sang biến mới
    const auto& list = serviceGroupRepository.getAll();
    if (list.empty()) {
        cout << "Danh sach dich vu dang trong!" << endl;
        Utils::backChoice();
        return;
    }

    // Dùng left và setw() của thư viện <iomanip> để căn lề trái và chia độ rộng từng cột đều nhau
    cout << left
         << setw(8)  << "Ma ID"
         << setw(24) << "Ten dich vu"
         << setw(18) << "Phong ban"
         << setw(10) << "SLA(gio)"
         << setw(14) << "Phi co ban"
         << setw(15) << "Ngay ban hanh"
         << setw(14) << "Trang thai" << endl;
    cout << string(105, '-') << endl; // In đường kẻ ngang gồm 105 dấu gạch ngang '-'

    for (const auto& sg : list) {
        cout << left
             << setw(8)  << sg.getId()
             << setw(24) << sg.getName()
             << setw(18) << sg.getDepartment()
             << setw(10) << sg.getSlaHours()
             << setw(14) << sg.getBaseFee()
             << setw(15) << sg.getCreatedDate()
             << setw(14) << (sg.isActive() ? "Hoat dong" : "Tam dung") << endl;
    }
    Utils::backChoice();
}

void ServiceGroupMenu::findById() {
    string id = Utils::inputNonEmptyString("Nhap ma dich vu can tim (vi du SM001, nhap 0 de quay lai): ");
    if (id == "0") {
        return; // Người dùng nhập 0 thì thoát hàm quay lại menu ngay
    }

    // Nhận con trỏ từ hàm findById: nếu không tìm thấy sẽ trả về nullptr
    ServiceGroup* sg = serviceGroupRepository.findById(id);

    if (sg == nullptr) {
        cout << "Khong tim thay dich vu voi ma: " << id << endl;
        Utils::backChoice();
        return;
    }

    // Giải tham chiếu (*sg) để truyền đối tượng vào hàm in chi tiết đa hình
    Utils::printEntityDetails(*sg);
    Utils::backChoice();
}

void ServiceGroupMenu::add() {
    cout << "--- Them dich vu moi ---" << endl;
    string name = Utils::inputNonEmptyString("Nhap ten dich vu (nhap 0 de quay lai): ");
    if (name == "0") {
        return;
    }

    // Duyệt qua toàn bộ danh sách hiện có để kiểm tra trùng tên dịch vụ trước khi cho phép thêm mới
    for (const auto& existing : serviceGroupRepository.getAll()) {
        if (existing.getName() == name) {
            cout << "Loi rang buoc: Ten dich vu '" << name << "' da ton tai tren he thong!" << endl;
            Utils::backChoice();
            return;
        }
    }

    string description = Utils::inputNonEmptyString("Nhap mo ta dich vu: ");
    string department = Utils::inputNonEmptyString("Nhap phong ban phu trach: ");
    int slaHours = Utils::inputInt("Nhap thoi gian xu ly tieu chuan SLA (gio, >0): ", 1, 10000);
    int baseFee = Utils::inputInt("Nhap phi dich vu co ban (VND, >= 0): ", 0);
    string createdDate = Utils::inputDateString("Nhap ngay ban hanh (DD/MM/YYYY): ");

    ServiceGroup newSg(name, description, department, slaHours, baseFee, createdDate);
    try {
        // Khi gọi add(), Repository sẽ tự động sinh mã ID (SM001, SM002...) và lưu xuống file
        serviceGroupRepository.add(newSg);
        const auto& list = serviceGroupRepository.getAll();
        // Lấy phần tử cuối cùng vừa thêm vào (list.back()) để in ra mã ID vừa được cấp
        cout << "Them dich vu thanh cong! Ma ID duoc cap: " << list.back().getId() << endl;
    }
    catch (const exception& e) {
        cout << "Loi khi them dich vu: " << e.what() << endl;
    }
    Utils::backChoice();
}

void ServiceGroupMenu::update() {
    cout << "--- Cap nhat thong tin dich vu ---" << endl;
    string id = Utils::inputNonEmptyString("Nhap ma dich vu can cap nhat (vi du SM001, nhap 0 de quay lai): ");
    if (id == "0") {
        return;
    }

    ServiceGroup* sg = serviceGroupRepository.findById(id);
    if (sg == nullptr) {
        cout << "Loi: Khong tim thay dich vu voi ma: " << id << endl;
        Utils::backChoice();
        return;
    }

    // Tạo bản sao "updated" từ đối tượng gốc (*sg): mọi chỉnh sửa sẽ làm trên bản sao này trước,
    // đến cuối hàm mới gọi update(updated) để nếu người dùng nhập lỗi giữa chừng thì dữ liệu gốc không bị hỏng.
    ServiceGroup updated = *sg;

    cout << "Thong tin hien tai:" << endl;
    updated.printInfo();

    // Dùng getline kết hợp kiểm tra chuỗi rỗng để cho phép người dùng nhấn phím Enter nếu muốn giữ nguyên thông tin cũ
    cout << "Nhap ten moi (nhan Enter de giu nguyen): ";
    string newName;
    getline(cin, newName);
    newName = Utils::trim(newName);
    if (!newName.empty()) {
        // Kiểm tra tên mới không được trùng với tên của các dịch vụ khác (trừ chính nó: other.getId() != id)
        for (const auto& other : serviceGroupRepository.getAll()) {
            if (other.getId() != id && other.getName() == newName) {
                cout << "Loi rang buoc: Ten dich vu '" << newName << "' da ton tai!" << endl;
                Utils::backChoice();
                return;
            }
        }
        updated.setName(newName);
    }

    cout << "Nhap mo ta moi (nhan Enter de giu nguyen): ";
    string newDesc;
    getline(cin, newDesc);
    newDesc = Utils::trim(newDesc);
    if (!newDesc.empty()) {
        updated.setDescription(newDesc);
    }

    cout << "Nhap phong ban moi (nhan Enter de giu nguyen): ";
    string newDept;
    getline(cin, newDept);
    newDept = Utils::trim(newDept);
    if (!newDept.empty()) {
        updated.setDepartment(newDept);
    }

    cout << "Nhap SLA moi tinh bang gio (nhan Enter de giu nguyen): ";
    string slaInput;
    getline(cin, slaInput);
    slaInput = Utils::trim(slaInput);
    if (!slaInput.empty()) {
        try {
            int newSla = stoi(slaInput); // Chuyển chuỗi sang số nguyên, nếu nhập chữ sẽ nhảy vào catch
            if (newSla > 0) updated.setSlaHours(newSla);
            else cout << "Gia tri SLA khong hop le, giu nguyen gia tri cu." << endl;
        } catch (...) {
            cout << "Gia tri SLA khong hop le, giu nguyen gia tri cu." << endl;
        }
    }

    cout << "Nhap phi co ban moi (nhan Enter de giu nguyen): ";
    string feeInput;
    getline(cin, feeInput);
    feeInput = Utils::trim(feeInput);
    if (!feeInput.empty()) {
        try {
            int newFee = stoi(feeInput);
            if (newFee >= 0) updated.setBaseFee(newFee);
            else cout << "Phi khong hop le, giu nguyen gia tri cu." << endl;
        } catch (...) {
            cout << "Phi khong hop le, giu nguyen gia tri cu." << endl;
        }
    }

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
        // Ghi đè đối tượng đã chỉnh sửa vào kho lưu trữ và cập nhật xuống file
        if (serviceGroupRepository.update(updated)) {
            cout << "Cap nhat dich vu thanh cong!" << endl;
        }
        else {
            cout << "Loi khi cap nhat dich vu!" << endl;
        }
    }
    catch (const exception& e) {
        cout << "Loi khi cap nhat dich vu: " << e.what() << endl;
    }
    Utils::backChoice();
}

void ServiceGroupMenu::remove() {
    cout << "--- Xoa dich vu ---" << endl;
    string id = Utils::inputNonEmptyString("Nhap ma dich vu can xoa (vi du SM001, nhap 0 de quay lai): ");
    if (id == "0") {
        return;
    }

    ServiceGroup* sg = serviceGroupRepository.findById(id);
    if (sg == nullptr) {
        cout << "Loi: Khong tim thay dich vu voi ma: " << id << endl;
        Utils::backChoice();
        return;
    }

    // Tại sao phải duyệt qua ticketRepository trước khi xóa dịch vụ?
    // Để đảm bảo toàn vẹn khóa ngoại: nếu dịch vụ này đang có Ticket tham chiếu tới mà bị xóa mất,
    // thì các Ticket đó sẽ bị lỗi mất thông tin dịch vụ. Do đó phải chặn lại không cho xóa.
    for (const auto& ticket : ticketRepository.getAll()) {
        if (ticket.getServiceGroupId() == id) {
            cout << "Loi rang buoc toan ven: Khong the xoa dich vu '" << id
                 << "' vi dang co ticket lien ket (Ma ticket: " << ticket.getId() << ")!" << endl;
            cout << "Vui long xu ly hoac xoa cac ticket lien quan truoc khi xoa dich vu nay." << endl;
            Utils::backChoice();
            return;
        }
    }

    cout << "Thong tin dich vu can xoa:" << endl;
    sg->printInfo();
    cout << "Ban co chac chan muon xoa dich vu " << id << " (" << sg->getName() << ")? (y/n): ";
    string confirm;
    getline(cin, confirm);
    confirm = Utils::trim(confirm);

    if (confirm == "y" || confirm == "Y") {
        try {
            if (serviceGroupRepository.remove(id)) {
                cout << "Da xoa dich vu thanh cong!" << endl;
            }
            else {
                cout << "Loi: Khong the xoa dich vu!" << endl;
            }
        }
        catch (const exception& e) {
            cout << "Loi khi xoa dich vu: " << e.what() << endl;
        }
    }
    else {
        cout << "Da huy thao tac xoa." << endl;
    }
    Utils::backChoice();
}
