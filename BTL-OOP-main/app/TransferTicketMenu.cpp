#include "TransferTicketMenu.h"
#include <iostream>
#include <iomanip>
#include <Utilities.h>

using namespace std;

TransferTicketMenu::TransferTicketMenu(
    Repository<TransferTicket>& tfRepo,
    Repository<Ticket>& tRepo,
    Repository<Agent>& aRepo
)
    : transferRepository(tfRepo), ticketRepository(tRepo), agentRepository(aRepo)
{
}

string TransferTicketMenu::statusToString(TransferStatus s)
{
    switch (s)
    {
    case TransferStatus::PENDING:   return "Cho nhan";
    case TransferStatus::ACCEPTED:  return "Da nhan";
    case TransferStatus::COMPLETED: return "Hoan tat";
    }
    return "Khong ro";
}

// Tra ten agent de hien thi de doc (neu agent con ton tai)
string TransferTicketMenu::agentNameOf(const string& agentId)
{
    Agent* a = agentRepository.findById(agentId);
    if (a == nullptr)
    {
        return "(khong xac dinh)";
    }
    return a->getFullName();
}

void TransferTicketMenu::showMenu()
{
    cout << "========================================" << endl;
    cout << "       QUAN LY PHIEU CHUYEN XU LY" << endl;
    cout << "========================================" << endl;
    cout << "1. Hien thi danh sach phieu chuyen" << endl;
    cout << "2. Tim kiem phieu chuyen theo ID" << endl;
    cout << "3. Tao phieu chuyen moi" << endl;
    cout << "4. Cap nhat phieu chuyen" << endl;
    cout << "5. Xoa phieu chuyen" << endl;
    cout << "0. Quay lai menu chinh" << endl;
    cout << "========================================" << endl;
}

void TransferTicketMenu::run()
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

void TransferTicketMenu::displayAll()
{
    const auto& list = transferRepository.getAll();
    if (list.empty())
    {
        cout << "Danh sach phieu chuyen xu ly dang trong!" << endl;
        return;
    }
    cout << left
         << setw(8)  << "Ma ID"
         << setw(10) << "Ticket"
         << setw(14) << "Agent chuyen"
         << setw(12) << "Agent nhan"
         << setw(28) << "Ly do"
         << setw(12) << "Trang thai" << endl;
    cout << string(84, '-') << endl;

    for (const auto& t : list)
    {
        cout << left
             << setw(8)  << t.getId()
             << setw(10) << t.getTicketId()
             << setw(14) << t.getFromAgentId()
             << setw(12) << t.getToAgentId()
             << setw(28) << t.getReason()
             << setw(12) << statusToString(t.getStatus()) << endl;
    }
}

void TransferTicketMenu::findById()
{
    string id = Utils::inputNonEmtpyString("Nhap ma phieu chuyen can tim (vi du TF001): ");
    TransferTicket* t = transferRepository.findById(id);
    if (t == nullptr)
    {
        cout << "Khong tim thay phieu chuyen voi ma: " << id << endl;
        return;
    }

    cout << "--- Thong tin phieu chuyen xu ly ---" << endl;
    cout << "Ma ID        : " << t->getId() << endl;
    cout << "Ticket       : " << t->getTicketId() << endl;
    cout << "Agent chuyen : [" << t->getFromAgentId() << "] " << agentNameOf(t->getFromAgentId()) << endl;
    cout << "Agent nhan   : [" << t->getToAgentId() << "] " << agentNameOf(t->getToAgentId()) << endl;
    cout << "Ly do        : " << t->getReason() << endl;
    cout << "Trang thai   : " << statusToString(t->getStatus()) << endl;
}

/**
 * Tao phieu chuyen. Rang buoc:
 * 1. Cac truong khong rong.
 * 2. Khoa ngoai: Ticket, Agent chuyen, Agent nhan phai ton tai.
 * 3. Agent chuyen va Agent nhan phai dang hoat dong.
 * 4. Agent chuyen khac Agent nhan.
 * 5. Khong chuyen Ticket da RESOLVED/CLOSED.
 */
void TransferTicketMenu::add()
{
    cout << "--- Tao phieu chuyen xu ly ---" << endl;
    string ticketId = Utils::inputNonEmtpyString("Nhap ma ticket can chuyen: ");

    Ticket* ticket = ticketRepository.findById(ticketId);
    if (ticket == nullptr)
    {
        cout << "Loi rang buoc: Ticket '" << ticketId << "' khong ton tai!" << endl;
        return;
    }
    if (ticket->getStatus() == TicketStatus::RESOLVED || ticket->getStatus() == TicketStatus::CLOSED)
    {
        cout << "Loi rang buoc: Ticket '" << ticketId << "' da xu ly xong/da dong, khong the chuyen!" << endl;
        return;
    }

    string fromId = Utils::inputNonEmtpyString("Nhap ma agent chuyen: ");
    Agent* from = agentRepository.findById(fromId);
    if (from == nullptr)
    {
        cout << "Loi rang buoc: Agent chuyen '" << fromId << "' khong ton tai!" << endl;
        return;
    }

    string toId = Utils::inputNonEmtpyString("Nhap ma agent nhan: ");
    Agent* to = agentRepository.findById(toId);
    if (to == nullptr)
    {
        cout << "Loi rang buoc: Agent nhan '" << toId << "' khong ton tai!" << endl;
        return;
    }

    if (fromId == toId)
    {
        cout << "Loi rang buoc: Agent chuyen va agent nhan khong duoc trung nhau!" << endl;
        return;
    }
    if (!from->isActive() || !to->isActive())
    {
        cout << "Loi rang buoc: Ca hai agent phai dang hoat dong!" << endl;
        return;
    }

    string reason = Utils::inputNonEmtpyString("Nhap ly do chuyen: ");

    try
    {
        TransferTicket tf(ticketId, fromId, toId, reason);
        transferRepository.add(tf);
        cout << "Tao phieu chuyen thanh cong! Ma duoc cap: "
             << transferRepository.getAll().back().getId() << endl;
    }
    catch (const exception& e)
    {
        cout << "Loi khi tao phieu chuyen: " << e.what() << endl;
    }
}

/**
 * Cap nhat. Rang buoc: ID ton tai; phieu Hoan tat bi khoa;
 * trang thai chi di toi (Cho nhan -> Da nhan -> Hoan tat).
 */
void TransferTicketMenu::update()
{
    cout << "--- Cap nhat phieu chuyen xu ly ---" << endl;
    string id = Utils::inputNonEmtpyString("Nhap ma phieu chuyen can cap nhat: ");

    TransferTicket* t = transferRepository.findById(id);
    if (t == nullptr)
    {
        cout << "Loi: Khong tim thay phieu chuyen voi ma: " << id << endl;
        return;
    }
    if (t->getStatus() == TransferStatus::COMPLETED)
    {
        cout << "Loi rang buoc: Phieu '" << id << "' da Hoan tat, khong duoc chinh sua!" << endl;
        return;
    }

    TransferTicket updated = *t;
    cout << "Thong tin hien tai: [" << updated.getId() << "] ly do: " << updated.getReason()
         << " | trang thai: " << statusToString(updated.getStatus()) << endl;

    try
    {
        string line;

        cout << "Ly do moi (Enter de giu nguyen): ";
        getline(cin, line);
        line = Utils::trim(line);
        if (!line.empty()) updated.setReason(line);

        cout << "Trang thai moi (0: Cho nhan, 1: Da nhan, 2: Hoan tat, Enter: Giu nguyen): ";
        getline(cin, line);
        line = Utils::trim(line);
        if (!line.empty())
        {
            int st = stoi(line);
            if (st < 0 || st > 2)
            {
                cout << "Loi: Trang thai khong hop le!" << endl;
                return;
            }
            updated.changeStatus(static_cast<TransferStatus>(st));  // nem loi neu di lui
        }

        if (transferRepository.update(updated))
        {
            cout << "Cap nhat phieu chuyen thanh cong!" << endl;
        }
        else
        {
            cout << "Loi khi cap nhat phieu chuyen!" << endl;
        }
    }
    catch (const exception& e)
    {
        cout << "Cap nhat that bai: " << e.what() << endl;
    }
}

/**
 * Xoa. Rang buoc: ID ton tai; khong xoa phieu da Hoan tat (giu lai de truy vet).
 */
void TransferTicketMenu::remove()
{
    cout << "--- Xoa phieu chuyen xu ly ---" << endl;
    string id = Utils::inputNonEmtpyString("Nhap ma phieu chuyen can xoa: ");

    TransferTicket* t = transferRepository.findById(id);
    if (t == nullptr)
    {
        cout << "Loi: Khong tim thay phieu chuyen voi ma: " << id << endl;
        return;
    }
    if (t->getStatus() == TransferStatus::COMPLETED)
    {
        cout << "Loi rang buoc: Khong the xoa phieu '" << id
             << "' vi da Hoan tat (giu lai de truy vet)!" << endl;
        return;
    }

    cout << "Ban co chac chan muon xoa phieu chuyen " << id << "? (y/n): ";
    string confirm;
    getline(cin, confirm);
    confirm = Utils::trim(confirm);

    if (confirm == "y" || confirm == "Y")
    {
        try
        {
            if (transferRepository.remove(id))
            {
                cout << "Da xoa phieu chuyen thanh cong!" << endl;
            }
            else
            {
                cout << "Loi: Khong the xoa phieu chuyen!" << endl;
            }
        }
        catch (const exception& e)
        {
            cout << "Loi khi xoa phieu chuyen: " << e.what() << endl;
        }
    }
    else
    {
        cout << "Da huy thao tac xoa." << endl;
    }
}
