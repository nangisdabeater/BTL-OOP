#include "SLALevelMenu.h"
#include <iostream>
#include <iomanip>
#include <Utilities.h>

using namespace std;

SLALevelMenu::SLALevelMenu(Repository<SLALevel>& repo, Repository<TransferTicket>& tfRepo)
    : slaRepository(repo), transferRepository(tfRepo) {}

void SLALevelMenu::showMenu()
{
    cout << "========================================" << endl;
    cout << "           QUAN LY MUC SLA" << endl;
    cout << "========================================" << endl;
    cout << "1. Hien thi danh sach muc SLA" << endl;
    cout << "2. Tim kiem muc SLA theo ID" << endl;
    cout << "3. Them muc SLA moi" << endl;
    cout << "4. Cap nhat muc SLA" << endl;
    cout << "5. Xoa muc SLA" << endl;
    cout << "0. Quay lai menu chinh" << endl;
    cout << "========================================" << endl;
}

// Nhap 1 so nguyen hop le, su dung chung ham tien ich Utils::inputInt
int SLALevelMenu::inputInt(const string& prompt, int minValue, int maxValue)
{
    return Utils::inputInt(prompt, minValue, maxValue);
}

void SLALevelMenu::run()
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
        cout << endl;
    }
}

void SLALevelMenu::displayAll()
{
    const auto& list = slaRepository.getAll();
    if (list.empty())
    {
        cout << "Danh sach muc SLA dang trong!" << endl;
        Utils::backChoice();
        return;
    }
    cout << left
         << setw(8)  << "Ma ID"
         << setw(22) << "Ten muc SLA"
         << setw(16) << "Phan hoi(phut)"
         << setw(14) << "Xu ly(gio)"
         << setw(8)  << "Uu tien" << endl;
    cout << string(68, '-') << endl;

    for (const auto& s : list)
    {
        cout << left
             << setw(8)  << s.getId()
             << setw(22) << s.getName()
             << setw(16) << s.getResponseMinutes()
             << setw(14) << s.getResolveHours()
             << setw(8)  << s.getPriority() << endl;
    }
    Utils::backChoice();
}

void SLALevelMenu::findById()
{
    string id = Utils::inputNonEmptyString("Nhap ma muc SLA can tim (vi du SL001, nhap 0 de quay lai): ");
    if (id == "0")
    {
        return;
    }
    SLALevel* s = slaRepository.findById(id);
    if (s == nullptr)
    {
        cout << "Khong tim thay muc SLA voi ma: " << id << endl;
        Utils::backChoice();
        return;
    }
    Utils::printEntityDetails(*s);
    Utils::backChoice();
}

/**
 * Them moi. Rang buoc: ten khong rong, khong trung ten; thoi gian > 0; uu tien 1-5.
 */
void SLALevelMenu::add()
{
    cout << "--- Them muc SLA moi ---" << endl;
    string name = Utils::inputNonEmptyString("Nhap ten muc SLA (nhap 0 de quay lai): ");
    if (name == "0")
    {
        return;
    }

    for (const auto& existing : slaRepository.getAll())
    {
        if (existing.getName() == name)
        {
            cout << "Loi rang buoc: Ten muc SLA '" << name << "' da ton tai!" << endl;
            Utils::backChoice();
            return;
        }
    }

    int resp = inputInt("Nhap thoi gian phan hoi toi da (phut, >0): ", 1, 100000);
    int resolve = inputInt("Nhap thoi gian xu ly toi da (gio, >0): ", 1, 100000);
    int prio = inputInt("Nhap muc do uu tien (1 = cao nhat ... 5 = thap nhat): ", 1, 5);

    try
    {
        SLALevel s(name, resp, resolve, prio);
        slaRepository.add(s);
        cout << "Them muc SLA thanh cong! Ma duoc cap: " << slaRepository.getAll().back().getId() << endl;
    }
    catch (const exception& e)
    {
        cout << "Loi khi them muc SLA: " << e.what() << endl;
    }
    Utils::backChoice();
}

/**
 * Cap nhat. Rang buoc: ID phai ton tai; ten moi khong trung muc khac; gia tri moi hop le.
 * Nhan Enter de giu nguyen tung truong.
 */
void SLALevelMenu::update()
{
    cout << "--- Cap nhat muc SLA ---" << endl;
    string id = Utils::inputNonEmptyString("Nhap ma muc SLA can cap nhat (vi du SL001, nhap 0 de quay lai): ");
    if (id == "0")
    {
        return;
    }

    SLALevel* s = slaRepository.findById(id);
    if (s == nullptr)
    {
        cout << "Loi: Khong tim thay muc SLA voi ma: " << id << endl;
        Utils::backChoice();
        return;
    }

    SLALevel updated = *s;
    cout << "Thong tin hien tai:" << endl;
    updated.printInfo();

    try
    {
        string line;

        cout << "Ten moi (Enter de giu nguyen): ";
        getline(cin, line);
        line = Utils::trim(line);
        if (!line.empty())
        {
            for (const auto& other : slaRepository.getAll())
            {
                if (other.getId() != id && other.getName() == line)
                {
                    cout << "Loi rang buoc: Ten muc SLA '" << line << "' da ton tai!" << endl;
                    Utils::backChoice();
                    return;
                }
            }
            updated.setName(line);
        }

        cout << "TG phan hoi moi - phut (Enter de giu nguyen): ";
        getline(cin, line);
        line = Utils::trim(line);
        if (!line.empty()) updated.setResponseMinutes(stoi(line));

        cout << "TG xu ly moi - gio (Enter de giu nguyen): ";
        getline(cin, line);
        line = Utils::trim(line);
        if (!line.empty()) updated.setResolveHours(stoi(line));

        cout << "Muc uu tien moi 1-5 (Enter de giu nguyen): ";
        getline(cin, line);
        line = Utils::trim(line);
        if (!line.empty()) updated.setPriority(stoi(line));

        if (slaRepository.update(updated))
        {
            cout << "Cap nhat muc SLA thanh cong!" << endl;
        }
        else
        {
            cout << "Loi khi cap nhat muc SLA!" << endl;
        }
    }
    catch (const exception& e)
    {
        // Gom ca loi rang buoc (invalid_argument) va loi nhap so (stoi)
        cout << "Cap nhat that bai, gia tri khong hop le: " << e.what() << endl;
    }
    Utils::backChoice();
}

/**
 * Xoa. Rang buoc: ID phai ton tai, can xac nhan.
 * TODO (cho nhom thong nhat): khi Ticket (hoac ServiceGroup) co truong slaLevelId,
 * them kiem tra "khong xoa muc SLA dang duoc tham chieu" tai day.
 */
void SLALevelMenu::remove()
{
    cout << "--- Xoa muc SLA ---" << endl;
    string id = Utils::inputNonEmptyString("Nhap ma muc SLA can xoa (vi du SL001, nhap 0 de quay lai): ");
    if (id == "0")
    {
        return;
    }

    SLALevel* s = slaRepository.findById(id);
    if (s == nullptr)
    {
        cout << "Loi: Khong tim thay muc SLA voi ma: " << id << endl;
        Utils::backChoice();
        return;
    }

    // Kiểm tra ràng buộc toàn vẹn dữ liệu với Phiếu chuyển xử lý (TransferTicket)
    for (const auto& tf : transferRepository.getAll())
    {
        if (tf.getSlaId() == id)
        {
            cout << "Loi rang buoc toan ven: Khong the xoa muc SLA '" << id
                 << "' vi dang duoc ap dung cho phieu chuyen xu ly (Ma phieu: " << tf.getId() << ")!" << endl;
            Utils::backChoice();
            return;
        }
    }

    cout << "Thong tin muc SLA can xoa:" << endl;
    s->printInfo();
    cout << "Ban co chac chan muon xoa muc SLA " << id << " (" << s->getName() << ")? (y/n): ";
    string confirm;
    getline(cin, confirm);
    confirm = Utils::trim(confirm);

    if (confirm == "y" || confirm == "Y")
    {
        try
        {
            if (slaRepository.remove(id))
            {
                cout << "Da xoa muc SLA thanh cong!" << endl;
            }
            else
            {
                cout << "Loi: Khong the xoa muc SLA!" << endl;
            }
        }
        catch (const exception& e)
        {
            cout << "Loi khi xoa muc SLA: " << e.what() << endl;
        }
    }
    else
    {
        cout << "Da huy thao tac xoa." << endl;
    }
    Utils::backChoice();
}
