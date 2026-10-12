#include "FeedbackMenu.h"
#include "../Utilities/Utilities.h"
#include "../Utilities/DateTimeUtils.h"
#include <iostream>
#include <iomanip>

using namespace std;

FeedbackMenu::FeedbackMenu(Repository<Feedback>& r, Repository<Customer>& c)
    : repo(r), customers(c)
{
}

/**
 * @brief Hiển thị menu chức năng quản lý phiếu đánh giá hài lòng
 */
void FeedbackMenu::showMenu()
{
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

/**
 * @brief Hiển thị toàn bộ danh sách phiếu đánh giá dưới dạng bảng
 */
void FeedbackMenu::displayAll()
{
    const auto& list = repo.getAll();
    if (list.empty())
    {
        cout << "Danh sach phieu danh gia dang trong!" << endl;
        return;
    }

    cout << left
         << setw(8)  << "Ma ID"
         << setw(10) << "Ma KH"
         << setw(12) << "Ma Ticket"
         << setw(8)  << "Diem"
         << setw(14) << "Ngay"
         << "Noi dung danh gia" << endl;
    cout << string(85, '-') << endl;

    for (const auto& x : list)
    {
        cout << left
             << setw(8)  << x.getId()
             << setw(10) << x.getCustomerId()
             << setw(12) << x.getTicketId()
             << setw(8)  << x.getScore()
             << setw(14) << x.getDate()
             << x.getComment() << endl;
    }
}

/**
 * @brief Tìm kiếm và hiển thị thông tin phiếu đánh giá theo mã ID
 */
void FeedbackMenu::findById()
{
    string id = Utils::inputNonEmptyString("Nhap ma danh gia can tim (VD: DG001): ");
    auto* x = repo.findById(id);
    if (x)
    {
        cout << "--- Thong tin phieu danh gia ---" << endl;
        cout << "Ma ID        : " << x->getId() << endl;
        cout << "Ma khach hang: " << x->getCustomerId() << endl;
        cout << "Ma ticket    : " << x->getTicketId() << endl;
        cout << "Diem danh gia: " << x->getScore() << " / 5" << endl;
        cout << "Ngay danh gia: " << x->getDate() << endl;
        cout << "Noi dung     : " << x->getComment() << endl;
    }
    else
    {
        cout << "Khong tim thay phieu danh gia voi ma: " << id << endl;
    }
}

/**
 * @brief Thêm phiếu đánh giá mới, có kiểm tra tồn tại mã khách hàng và định dạng ngày dd/mm/yyyy
 */
void FeedbackMenu::add()
{
    cout << "--- Them phieu danh gia moi ---" << endl;
    string kh = Utils::inputNonEmptyString("Nhap ma khach hang (VD: KH001): ");
    if (!customers.findById(kh))
    {
        cout << "Loi rang buoc: Khach hang voi ma '" << kh << "' khong ton tai!" << endl;
        return;
    }

    string tk = Utils::inputNonEmptyString("Nhap ma ticket (VD: TK001): ");
    int score = Utils::inputInt("Nhap diem danh gia (1-5): ", 1, 5);
    string date = Utils::inputDateString("Nhap ngay danh gia (DD/MM/YYYY): ");
    string msg = Utils::inputNonEmptyString("Nhap noi dung danh gia: ");

    Feedback x(kh, tk, score, msg, date);
    try
    {
        repo.add(x);
        const auto& list = repo.getAll();
        cout << "Them phieu danh gia thanh cong! Ma duoc cap: " << list.back().getId() << endl;
    }
    catch (const exception& ex)
    {
        cout << "Loi khi them danh gia: " << ex.what() << endl;
    }
}

/**
 * @brief Cập nhật điểm và nhận xét cho phiếu đánh giá hiện có
 */
void FeedbackMenu::update()
{
    cout << "--- Cap nhat phieu danh gia ---" << endl;
    string id = Utils::inputNonEmptyString("Nhap ma danh gia can sua: ");
    auto* x = repo.findById(id);
    if (!x)
    {
        cout << "Khong tim thay phieu danh gia voi ma: " << id << endl;
        return;
    }

    cout << "Diem hien tai: " << x->getScore() << ". Nhap diem moi (1-5, Enter de giu nguyen): ";
    string v;
    getline(cin, v);
    v = Utils::trim(v);
    if (!v.empty())
    {
        try {
            int sc = stoi(v);
            if (sc >= 1 && sc <= 5) x->setScore(sc);
            else cout << "Diem khong hop le, giu nguyen diem cu." << endl;
        } catch (...) {
            cout << "Diem khong hop le, giu nguyen diem cu." << endl;
        }
    }

    cout << "Noi dung hien tai: " << x->getComment() << endl;
    cout << "Nhap noi dung moi (Enter de giu nguyen): ";
    getline(cin, v);
    v = Utils::trim(v);
    if (!v.empty())
    {
        x->setComment(v);
    }

    try
    {
        repo.save();
        cout << "Cap nhat phieu danh gia thanh cong!" << endl;
    }
    catch (const exception& ex)
    {
        cout << "Loi khi luu: " << ex.what() << endl;
    }
}

/**
 * @brief Xóa phiếu đánh giá khỏi hệ thống
 */
void FeedbackMenu::remove()
{
    cout << "--- Xoa phieu danh gia ---" << endl;
    string id = Utils::inputNonEmptyString("Nhap ma danh gia can xoa: ");
    auto* x = repo.findById(id);
    if (!x)
    {
        cout << "Khong tim thay phieu danh gia voi ma: " << id << endl;
        return;
    }

    cout << "Ban co chac chan muon xoa phieu danh gia " << id << "? (y/n): ";
    char y;
    cin >> y;
    cin.ignore(10000, '\n');
    if (y == 'y' || y == 'Y')
    {
        if (repo.remove(id))
        {
            cout << "Da xoa phieu danh gia thanh cong!" << endl;
        }
        else
        {
            cout << "Khong the xoa phieu danh gia!" << endl;
        }
    }
    else
    {
        cout << "Da huy thao tac xoa." << endl;
    }
}

/**
 * @brief Điều phối các lựa chọn người dùng trong menu đánh giá hài lòng
 */
void FeedbackMenu::run()
{
    bool inMenu = true;
    while (inMenu)
    {
        showMenu();
        int choice;
        cout << "Nhap lua chon: ";
        if (!(cin >> choice))
        {
            cin.clear();
            cin.ignore(10000, '\n');
            cout << "Lua chon khong hop le!" << endl << endl;
            continue;
        }
        cin.ignore(10000, '\n');

        try
        {
            switch (choice)
            {
            case 1: displayAll(); break;
            case 2: findById();   break;
            case 3: add();        break;
            case 4: update();     break;
            case 5: remove();     break;
            case 0: inMenu = false; break;
            default: cout << "Lua chon khong hop le!" << endl;
            }
        }
        catch (const exception& e)
        {
            cout << "Loi: " << e.what() << endl;
        }

        cout << endl;
    }
}
