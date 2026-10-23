#include "FeedbackMenu.h"
#include "../Utilities/Utilities.h"
#include <iostream>
#include <iomanip>

// Chỉ dùng using namespace std ở file .cpp để không làm bẩn không gian tên ở file .h
using namespace std;

// Khởi tạo menu đánh giá với tham chiếu tới kho Phiếu đánh giá và kho Khách hàng
FeedbackMenu::FeedbackMenu(Repository<Feedback>& r, Repository<Customer>& c)
    : repo(r), customers(c) {}

// Hiển thị bảng menu các chức năng quản lý phiếu đánh giá hài lòng
void FeedbackMenu::showMenu() {
    cout << "========================================" << endl;
    cout << "   QUAN LY PHIEU DANH GIA HAI LONG" << endl;
    cout << "========================================" << endl;
    cout << "1. Hien thi danh sach phieu danh gia" << endl;
    cout << "2. Tim kiem phieu danh gia theo ma ID" << endl;
    cout << "3. Them phieu danh gia moi" << endl;
    cout << "4. Cap nhat diem / noi dung danh gia" << endl;
    cout << "5. Xoa phieu danh gia" << endl;
    cout << "0. Quay lai menu chinh" << endl;
    cout << "========================================" << endl;
}

// Hiển thị toàn bộ danh sách phiếu đánh giá dưới dạng bảng căn lề trái
void FeedbackMenu::displayAll() {
    const auto& list = repo.getAll();
    if (list.empty()) {
        cout << "Danh sach phieu danh gia dang trong!" << endl;
        Utils::backChoice();
        return;
    }

    cout << left
         << setw(8)  << "Ma ID"
         << setw(10) << "Ma KH"
         << setw(8)  << "Diem"
         << setw(14) << "Ngay"
         << "Noi dung danh gia" << endl;
    cout << string(75, '-') << endl;

    for (const auto& x : list) {
        cout << left
             << setw(8)  << x.getId()
             << setw(10) << x.getCustomerId()
             << setw(8)  << x.getScore()
             << setw(14) << x.getDate()
             << x.getComment() << endl;
    }
    Utils::backChoice();
}

// Tìm kiếm và hiển thị thông tin phiếu đánh giá theo mã định danh ID
void FeedbackMenu::findById() {
    string id = Utils::inputNonEmptyString("Nhap ma danh gia can tim (VD: DG001, nhap 0 de quay lai): ");
    if (id == "0") {
        return;
    }
    auto* x = repo.findById(id);
    if (x) {
        Utils::printEntityDetails(*x);
    }
    else {
        cout << "Khong tim thay phieu danh gia voi ma: " << id << endl;
    }
    Utils::backChoice();
}

// Thêm phiếu đánh giá mới: kiểm tra tồn tại mã khách hàng (khóa ngoại) và định dạng ngày DD/MM/YYYY
void FeedbackMenu::add() {
    cout << "--- Them phieu danh gia moi ---" << endl;
    string kh = Utils::inputNonEmptyString("Nhap ma khach hang (VD: KH001, nhap 0 de quay lai): ");
    if (kh == "0") {
        return;
    }
    // Kiểm tra khóa ngoại: mã khách hàng phải tồn tại trong kho khách hàng
    if (!customers.findById(kh)) {
        cout << "Loi rang buoc: Khach hang voi ma '" << kh << "' khong ton tai!" << endl;
        Utils::backChoice();
        return;
    }

    int score = Utils::inputInt("Nhap diem danh gia (1-5): ", 1, 5);
    string date = Utils::inputDateString("Nhap ngay danh gia (DD/MM/YYYY): ");
    cout << "Nhap noi dung danh gia (co the de trong, nhan Enter de bo qua): ";
    string msg;
    getline(cin, msg);
    msg = Utils::trim(msg);

    Feedback x(kh, score, msg, date);
    try {
        repo.add(x);
        const auto& list = repo.getAll();
        cout << "Them phieu danh gia thanh cong! Ma duoc cap: " << list.back().getId() << endl;
    }
    catch (const exception& ex) {
        cout << "Loi khi them danh gia: " << ex.what() << endl;
    }
    Utils::backChoice();
}

// Cập nhật điểm số và nhận xét cho phiếu đánh giá hiện có
void FeedbackMenu::update() {
    cout << "--- Cap nhat phieu danh gia ---" << endl;
    string id = Utils::inputNonEmptyString("Nhap ma danh gia can sua (nhap 0 de quay lai): ");
    if (id == "0") {
        return;
    }
    auto* x = repo.findById(id);
    if (!x) {
        cout << "Khong tim thay phieu danh gia voi ma: " << id << endl;
        Utils::backChoice();
        return;
    }

    // Tạo bản sao tạm để chỉnh sửa trước khi lưu chính thức vào kho
    Feedback updated = *x;

    cout << "Thong tin hien tai:" << endl;
    updated.printInfo();

    cout << "Diem hien tai: " << updated.getScore() << ". Nhap diem moi (1-5, Enter de giu nguyen): ";
    string v;
    getline(cin, v);
    v = Utils::trim(v);
    if (!v.empty()) {
        try {
            int sc = stoi(v);
            if (sc >= 1 && sc <= 5) updated.setScore(sc);
            else cout << "Diem khong hop le, giu nguyen diem cu." << endl;
        } catch (...) {
            cout << "Diem khong hop le, giu nguyen diem cu." << endl;
        }
    }

    cout << "Noi dung hien tai: " << updated.getComment() << endl;
    cout << "Nhap noi dung moi (Enter de giu nguyen): ";
    getline(cin, v);
    v = Utils::trim(v);
    if (!v.empty()) {
        updated.setComment(v);
    }

    try {
        if (repo.update(updated)) {
            cout << "Cap nhat phieu danh gia thanh cong!" << endl;
        }
        else {
            cout << "Loi khi cap nhat phieu danh gia!" << endl;
        }
    }
    catch (const exception& ex) {
        cout << "Loi khi luu: " << ex.what() << endl;
    }
    Utils::backChoice();
}

// Xóa phiếu đánh giá khỏi hệ thống
void FeedbackMenu::remove() {
    cout << "--- Xoa phieu danh gia ---" << endl;
    string id = Utils::inputNonEmptyString("Nhap ma danh gia can xoa (nhap 0 de quay lai): ");
    if (id == "0") {
        return;
    }
    auto* x = repo.findById(id);
    if (!x) {
        cout << "Khong tim thay phieu danh gia voi ma: " << id << endl;
        Utils::backChoice();
        return;
    }

    cout << "Thong tin phieu danh gia can xoa:" << endl;
    x->printInfo();
    cout << "Ban co chac chan muon xoa phieu danh gia " << id << "? (y/n): ";
    char y;
    cin >> y;
    cin.ignore(10000, '\n');
    if (y == 'y' || y == 'Y') {
        if (repo.remove(id)) {
            cout << "Da xoa phieu danh gia thanh cong!" << endl;
        }
        else {
            cout << "Khong the xoa phieu danh gia!" << endl;
        }
    }
    else {
        cout << "Da huy thao tac xoa." << endl;
    }
    Utils::backChoice();
}

// Điều phối các lựa chọn của người dùng trong menu đánh giá hài lòng
void FeedbackMenu::run() {
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

        try {
            switch (choice) {
            case 1: displayAll(); break;
            case 2: findById();   break;
            case 3: add();        break;
            case 4: update();     break;
            case 5: remove();     break;
            case 0: inMenu = false; break;
            default: cout << "Lua chon khong hop le!" << endl;
            }
        }
        catch (const exception& e) {
            cout << "Loi: " << e.what() << endl;
        }

        cout << endl;
    }
}
