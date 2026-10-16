#include "CustomerMenu.h"
#include "../Utilities/Utilities.h"
#include <iostream>
#include <iomanip>

using namespace std;

CustomerMenu::CustomerMenu(Repository<Customer>& r)
    : repo(r)
{
}

/**
 * @brief Hiển thị menu chức năng quản lý khách hàng
 */
void CustomerMenu::showMenu()
{
    cout << "========================================" << endl;
    cout << "         QUAN LY KHACH HANG" << endl;
    cout << "========================================" << endl;
    cout << "1. Hien thi danh sach khach hang" << endl;
    cout << "2. Tim kiem khach hang theo ma ID" << endl;
    cout << "3. Them khach hang moi" << endl;
    cout << "4. Cap nhat thong tin khach hang" << endl;
    cout << "5. Xoa khach hang" << endl;
    cout << "0. Quay lai menu chinh" << endl;
    cout << "========================================" << endl;
}

/**
 * @brief Hiển thị toàn bộ danh sách khách hàng dưới dạng bảng
 */
void CustomerMenu::displayAll()
{
    const auto& list = repo.getAll();
    if (list.empty())
    {
        cout << "Danh sach khach hang dang trong!" << endl;
        Utils::backChoice();
        return;
    }

    cout << left
         << setw(8)  << "Ma ID"
         << setw(24) << "Ho ten"
         << setw(16) << "Dien thoai"
         << setw(28) << "Email"
         << "Dia chi" << endl;
    cout << string(90, '-') << endl;

    for (const auto& x : list)
    {
        cout << left
             << setw(8)  << x.getId()
             << setw(24) << x.getFullName()
             << setw(16) << x.getPhone()
             << setw(28) << x.getEmail()
             << x.getAddress() << endl;
    }
    Utils::backChoice();
}

/**
 * @brief Tìm kiếm thông tin khách hàng theo mã định danh (ID)
 */
void CustomerMenu::findById()
{
    string id = Utils::inputNonEmptyString("Nhap ma khach hang can tim (VD: KH001, nhap 0 de quay lai): ");
    if (id == "0")
    {
        return;
    }
    auto* x = repo.findById(id);
    if (x)
    {
        Utils::printEntityDetails(*x);
    }
    else
    {
        cout << "Khong tim thay khach hang voi ma: " << id << endl;
    }
    Utils::backChoice();
}

/**
 * @brief Thêm khách hàng mới và cấp phát mã ID tự động (KHxxx)
 */
void CustomerMenu::add()
{
    cout << "--- Them khach hang moi ---" << endl;
    string n = Utils::inputNonEmptyString("Nhap ho ten khach hang (nhap 0 de quay lai): ");
    if (n == "0")
    {
        return;
    }
    string p = Utils::inputNonEmptyString("Nhap so dien thoai: ");
    string e;
    while (true)
    {
        e = Utils::inputNonEmptyString("Nhap email: ");
        if (e.find('@') != string::npos) break;
        cout << "Email khong hop le (can chua ky tu '@')! Vui long nhap lai." << endl;
    }
    string a = Utils::inputNonEmptyString("Nhap dia chi: ");

    Customer x(n, p, e, a);
    try
    {
        repo.add(x);
        const auto& list = repo.getAll();
        cout << "Them khach hang thanh cong! Ma duoc cap: " << list.back().getId() << endl;
    }
    catch (const exception& ex)
    {
        cout << "Loi khi them khach hang: " << ex.what() << endl;
    }
    Utils::backChoice();
}

/**
 * @brief Cập nhật thông tin khách hàng, cho phép ấn Enter để giữ nguyên giá trị cũ
 */
void CustomerMenu::update()
{
    cout << "--- Cap nhat thong tin khach hang ---" << endl;
    string id = Utils::inputNonEmptyString("Nhap ma KH can sua (nhap 0 de quay lai): ");
    if (id == "0")
    {
        return;
    }
    auto* x = repo.findById(id);
    if (!x)
    {
        cout << "Khong tim thay khach hang voi ma: " << id << endl;
        Utils::backChoice();
        return;
    }

    cout << "Thong tin hien tai:" << endl;
    x->printInfo();

    string v;
    cout << "Ho ten [" << x->getFullName() << "] (Enter de giu): ";
    getline(cin, v);
    if (!Utils::trim(v).empty()) x->setFullName(Utils::trim(v));

    cout << "Dien thoai [" << x->getPhone() << "] (Enter de giu): ";
    getline(cin, v);
    if (!Utils::trim(v).empty()) x->setPhone(Utils::trim(v));

    cout << "Email [" << x->getEmail() << "] (Enter de giu): ";
    getline(cin, v);
    if (!Utils::trim(v).empty())
    {
        try { x->setEmail(Utils::trim(v)); }
        catch (const exception& ex) { cout << "Loi email: " << ex.what() << ", giu nguyen email cu." << endl; }
    }

    cout << "Dia chi [" << x->getAddress() << "] (Enter de giu): ";
    getline(cin, v);
    if (!Utils::trim(v).empty()) x->setAddress(Utils::trim(v));

    try
    {
        repo.save();
        cout << "Cap nhat thong tin khach hang thanh cong!" << endl;
    }
    catch (const exception& ex)
    {
        cout << "Loi khi luu: " << ex.what() << endl;
    }
    Utils::backChoice();
}

/**
 * @brief Xóa khách hàng khỏi hệ thống
 */
void CustomerMenu::remove()
{
    cout << "--- Xoa khach hang ---" << endl;
    string id = Utils::inputNonEmptyString("Nhap ma KH can xoa (nhap 0 de quay lai): ");
    if (id == "0")
    {
        return;
    }
    auto* x = repo.findById(id);
    if (!x)
    {
        cout << "Khong tim thay khach hang voi ma: " << id << endl;
        Utils::backChoice();
        return;
    }

    cout << "Ban co chac chan muon xoa khach hang " << id << " (" << x->getFullName() << ")? (y/n): ";
    char y;
    cin >> y;
    cin.ignore(10000, '\n');
    if (y == 'y' || y == 'Y')
    {
        if (repo.remove(id))
        {
            cout << "Da xoa khach hang thanh cong!" << endl;
        }
        else
        {
            cout << "Khong the xoa khach hang!" << endl;
        }
    }
    else
    {
        cout << "Da huy thao tac xoa." << endl;
    }
    Utils::backChoice();
}

/**
 * @brief Điều phối các lựa chọn người dùng trong menu quản lý khách hàng
 */
void CustomerMenu::run()
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
