#include "ReceptionChannelMenu.h"
#include <iostream>
#include <iomanip>
#include <Utilities.h>

// Chỉ dùng using namespace std ở file .cpp để không làm bẩn không gian tên ở file .h
using namespace std;

// Khởi tạo menu kênh tiếp nhận với tham chiếu tới kho Kênh tiếp nhận và kho Cuộc gọi
ReceptionChannelMenu::ReceptionChannelMenu(Repository<ReceptionChannel>& chRepo, Repository<InboundCall>& cRepo)
    : channelRepository(chRepo), callRepository(cRepo) {}

// Hiển thị bảng danh mục chức năng quản lý Kênh tiếp nhận
void ReceptionChannelMenu::showMenu() {
    cout << "========================================" << endl;
    cout << "        QUAN LY KENH TIEP NHAN" << endl;
    cout << "========================================" << endl;
    cout << "1. Hien thi danh sach kenh tiep nhan" << endl;
    cout << "2. Tim kiem kenh tiep nhan theo ID" << endl;
    cout << "3. Them kenh tiep nhan moi" << endl;
    cout << "4. Cap nhat thong tin kenh tiep nhan" << endl;
    cout << "5. Thay doi trang thai (Bat/Tat hoat dong)" << endl;
    cout << "6. Xoa kenh tiep nhan" << endl;
    cout << "0. Quay lai menu chinh" << endl;
    cout << "========================================" << endl;
}

// Điều phối vòng lặp các lựa chọn trong menu Kênh tiếp nhận
void ReceptionChannelMenu::run() {
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
            toggleStatus();
            break;
        case 6:
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

// Hiển thị tất cả kênh tiếp nhận dưới dạng bảng
void ReceptionChannelMenu::displayAll() {
    const auto& list = channelRepository.getAll();
    if (list.empty()) {
        cout << "Danh sach kenh tiep nhan dang trong!" << endl;
        Utils::backChoice();
        return;
    }

    cout << left
         << setw(8)  << "Ma ID"
         << setw(25) << "Ten kenh tiep nhan"
         << setw(42) << "Mo ta"
         << setw(15) << "Trang thai" << endl;
    cout << string(90, '-') << endl;

    for (const auto& ch : list) {
        cout << left
             << setw(8)  << ch.getId()
             << setw(25) << ch.getName()
             << setw(42) << ch.getDescription()
             << setw(15) << (ch.isActive() ? "Hoat dong" : "Ngung hoat dong") << endl;
    }
    Utils::backChoice();
}

// Tìm kiếm kênh tiếp nhận theo mã ID (CNxxx)
void ReceptionChannelMenu::findById() {
    string id = Utils::inputNonEmptyString("Nhap ma kenh tiep nhan can tim (vi du CN001, nhap 0 de quay lai): ");
    if (id == "0") {
        return;
    }
    ReceptionChannel* ch = channelRepository.findById(id);

    if (ch == nullptr) {
        cout << "Khong tim thay kenh tiep nhan voi ma: " << id << endl;
        Utils::backChoice();
        return;
    }

    Utils::printEntityDetails(*ch);
    Utils::backChoice();
}

// Thêm kênh tiếp nhận mới: kiểm tra không trùng tên kênh
void ReceptionChannelMenu::add() {
    cout << "--- Them kenh tiep nhan moi ---" << endl;
    string name = Utils::inputNonEmptyString("Nhap ten kenh tiep nhan (VD: Hotline 1900, Email, Zalo OA... nhap 0 de quay lai): ");
    if (name == "0") {
        return;
    }

    // Kiểm tra trùng lặp tên kênh bằng hàm mẫu isDuplicate kết hợp biểu thức lambda
    bool duplicate = ValidationUtils::isDuplicate(
        channelRepository.getAll(),
        [&name](const ReceptionChannel& existing) {
            return existing.getName() == name;
        }
    );

    if (duplicate) {
        cout << "Loi: Ten kenh tiep nhan '" << name << "' da ton tai tren he thong!" << endl;
        Utils::backChoice();
        return;
    }

    string description = Utils::inputNonEmptyString("Nhap mo ta kenh tiep nhan: ");

    ReceptionChannel newChannel(name, description);
    try {
        channelRepository.add(newChannel);
        const auto& list = channelRepository.getAll();
        cout << "Them kenh tiep nhan thanh cong! Ma ID duoc cap: " << list.back().getId() << endl;
    }
    catch (const exception& e) {
        cout << "Loi khi them kenh tiep nhan: " << e.what() << endl;
    }
    Utils::backChoice();
}

// Cập nhật thông tin kênh tiếp nhận: cho phép nhấn Enter để giữ nguyên trường cũ
void ReceptionChannelMenu::update() {
    cout << "--- Cap nhat thong tin kenh tiep nhan ---" << endl;
    string id = Utils::inputNonEmptyString("Nhap ma kenh tiep nhan can cap nhat (vi du CN001, nhap 0 de quay lai): ");
    if (id == "0") {
        return;
    }

    ReceptionChannel* ch = channelRepository.findById(id);
    if (ch == nullptr) {
        cout << "Khong tim thay kenh tiep nhan voi ma: " << id << endl;
        Utils::backChoice();
        return;
    }

    ReceptionChannel updated = *ch;

    cout << "Thong tin hien tai:" << endl;
    updated.printInfo();

    cout << "\nNhap ten moi (de trong neu giu nguyen): ";
    string newName;
    getline(cin, newName);
    newName = Utils::trim(newName);

    if (!newName.empty()) {
        // Kiểm tra tên mới không được trùng với các kênh tiếp nhận khác
        bool duplicate = ValidationUtils::isDuplicate(
            channelRepository.getAll(),
            [&id, &newName](const ReceptionChannel& other) {
                return other.getId() != id && other.getName() == newName;
            }
        );

        if (duplicate) {
            cout << "Loi: Ten kenh tiep nhan '" << newName << "' da ton tai!" << endl;
            Utils::backChoice();
            return;
        }
        updated.setName(newName);
    }

    cout << "Nhap mo ta moi (de trong neu giu nguyen): ";
    string newDesc;
    getline(cin, newDesc);
    newDesc = Utils::trim(newDesc);

    if (!newDesc.empty()) {
        updated.setDescription(newDesc);
    }

    try {
        if (channelRepository.update(updated)) {
            cout << "Cap nhat kenh tiep nhan thanh cong!" << endl;
        }
        else {
            cout << "Loi khi cap nhat kenh tiep nhan!" << endl;
        }
    }
    catch (const exception& e) {
        cout << "Loi khi cap nhat kenh tiep nhan: " << e.what() << endl;
    }
    Utils::backChoice();
}

// Bật hoặc tắt trạng thái hoạt động của kênh tiếp nhận
void ReceptionChannelMenu::toggleStatus() {
    cout << "--- Thay doi trang thai kenh tiep nhan ---" << endl;
    string id = Utils::inputNonEmptyString("Nhap ma kenh tiep nhan (vi du CN001, nhap 0 de quay lai): ");
    if (id == "0") {
        return;
    }

    ReceptionChannel* ch = channelRepository.findById(id);
    if (ch == nullptr) {
        cout << "Khong tim thay kenh tiep nhan voi ma: " << id << endl;
        Utils::backChoice();
        return;
    }

    if (ch->isActive()) {
        ch->deactivate();
        cout << "Da chuyen kenh " << id << " sang trang thai: NGUNG HOAT DONG." << endl;
    }
    else {
        ch->activate();
        cout << "Da chuyen kenh " << id << " sang trang thai: HOAT DONG." << endl;
    }

    try {
        channelRepository.update(*ch);
    }
    catch (const exception& e) {
        cout << "Loi khi luu trang thai: " << e.what() << endl;
    }
    Utils::backChoice();
}

// Xóa kênh tiếp nhận: kiểm tra ràng buộc toàn vẹn với Cuộc gọi tiếp nhận trước khi xóa
void ReceptionChannelMenu::remove() {
    cout << "--- Xoa kenh tiep nhan ---" << endl;
    string id = Utils::inputNonEmptyString("Nhap ma kenh tiep nhan can xoa (vi du CN001, nhap 0 de quay lai): ");
    if (id == "0") {
        return;
    }

    ReceptionChannel* ch = channelRepository.findById(id);
    if (ch == nullptr) {
        cout << "Khong tim thay kenh tiep nhan voi ma: " << id << endl;
        Utils::backChoice();
        return;
    }

    // Kiểm tra toàn vẹn dữ liệu: chặn xóa nếu kênh này đang được tham chiếu bởi cuộc gọi tiếp nhận (InboundCall)
    for (const auto& call : callRepository.getAll()) {
        if (call.getChannelId() == id) {
            cout << "Loi rang buoc: Kenh '" << id << "' dang co cuoc goi tiep nhan lien ket (VD ma " 
                 << call.getId() << ")! Khong the xoa." << endl;
            Utils::backChoice();
            return;
        }
    }

    cout << "Thong tin kenh tiep nhan can xoa:" << endl;
    ch->printInfo();
    cout << "Ban co chac chan muon xoa kenh " << id << " (" << ch->getName() << ")? (y/n): ";
    string confirm;
    getline(cin, confirm);
    confirm = Utils::trim(confirm);

    if (confirm == "y" || confirm == "Y") {
        try {
            if (channelRepository.remove(id)) {
                cout << "Xoa kenh tiep nhan " << id << " thanh cong!" << endl;
            }
            else {
                cout << "Xoa that bai!" << endl;
            }
        }
        catch (const exception& e) {
            cout << "Loi khi xoa kenh tiep nhan: " << e.what() << endl;
        }
    }
    else {
        cout << "Da huy thao tac xoa." << endl;
    }
    Utils::backChoice();
}
