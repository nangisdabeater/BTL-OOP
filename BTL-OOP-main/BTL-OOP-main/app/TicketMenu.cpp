#include "TicketMenu.h"
#include <iostream>
#include <iomanip>

using namespace std;

/**
 * @brief Khởi tạo đối tượng TicketMenu
 * @param tRepo Tham chiếu tới kho lưu trữ Ticket
 * @param sgRepo Tham chiếu tới kho lưu trữ Nhóm dịch vụ (để kiểm tra khóa ngoại)
 */
TicketMenu::TicketMenu(Repository<Ticket>& tRepo, Repository<ServiceGroup>& sgRepo)
    : ticketRepository(tRepo), serviceGroupRepository(sgRepo)
{
}

/**
 * @brief Cắt bỏ khoảng trắng thừa ở hai đầu chuỗi ký tự
 */
string TicketMenu::trim(const string& str)
{
    size_t first = str.find_first_not_of(" \t\r\n");
    if (first == string::npos) return "";
    size_t last = str.find_last_not_of(" \t\r\n");
    return str.substr(first, (last - first + 1));
}

/**
 * @brief Nhập chuỗi ký tự từ bàn phím và đảm bảo không được để trống
 */
string TicketMenu::inputNonEmptyString(const string& prompt)
{
    string input;
    while (true)
    {
        cout << prompt;
        getline(cin, input);
        input = trim(input);
        if (!input.empty())
        {
            return input;
        }
        cout << "Gia tri khong duoc de trong! Vui long nhap lai." << endl;
    }
}

/**
 * @brief Chuyển đổi giá trị enum TicketStatus sang dạng chuỗi hiển thị
 */
string TicketMenu::ticketStatusToString(TicketStatus status)
{
    switch (status)
    {
    case TicketStatus::OPEN:
        return "OPEN";
    case TicketStatus::IN_PROGRESS:
        return "IN_PROGRESS";
    case TicketStatus::RESOLVED:
        return "RESOLVED";
    case TicketStatus::CLOSED:
        return "CLOSED";
    default:
        return "UNKNOWN";
    }
}

/**
 * @brief Hiển thị bảng danh mục các chức năng quản lý Ticket
 */
void TicketMenu::showMenu()
{
    cout << "========================================" << endl;
    cout << "             QUAN LY TICKET" << endl;
    cout << "========================================" << endl;
    cout << "1. Hien thi danh sach ticket" << endl;
    cout << "2. Tim kiem ticket theo ID" << endl;
    cout << "3. Tao ticket moi" << endl;
    cout << "4. Cap nhat thong tin ticket" << endl;
    cout << "5. Xoa ticket" << endl;
    cout << "0. Quay lai menu chinh" << endl;
    cout << "========================================" << endl;
}

/**
 * @brief Điều phối vòng lặp xử lý các lựa chọn trong menu Ticket
 */
void TicketMenu::run()
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
        cin.ignore(10000, '\n'); // Xóa ký tự xuống dòng còn sót trong bộ đệm

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
 * @brief Hiển thị danh sách tất cả các ticket (Read All)
 * Hiển thị 6 thuộc tính: Mã ID, Tiêu đề, Mã KH, Mã nhóm DV, Tên nhóm DV, Trạng thái
 */
void TicketMenu::displayAll()
{
    const auto& list = ticketRepository.getAll();
    if (list.empty())
    {
        cout << "Danh sach ticket dang trong!" << endl;
        return;
    }

    cout << left
         << setw(8)  << "Ma ID"
         << setw(22) << "Tieu de"
         << setw(10) << "Ma KH"
         << setw(12) << "Ma Nhom DV"
         << setw(20) << "Ten Nhom DV"
         << setw(14) << "Trang thai" << endl;
    cout << string(86, '-') << endl;

    for (const auto& t : list)
    {
        string sgName = "(Chua xac dinh)";
        ServiceGroup* sg = serviceGroupRepository.findById(t.getServiceGroupId());
        if (sg != nullptr)
        {
            sgName = sg->getName();
        }

        cout << left
             << setw(8)  << t.getId()
             << setw(22) << t.getTitle()
             << setw(10) << t.getCustomerId()
             << setw(12) << t.getServiceGroupId()
             << setw(20) << sgName
             << setw(14) << ticketStatusToString(t.getStatus()) << endl;
    }
}

/**
 * @brief Tìm kiếm ticket theo mã ID (Read by ID)
 */
void TicketMenu::findById()
{
    string id = inputNonEmptyString("Nhap ma ticket can tim (vi du NP001): ");
    Ticket* t = ticketRepository.findById(id);

    if (t == nullptr)
    {
        cout << "Khong tim thay ticket voi ma: " << id << endl;
        return;
    }

    string sgName = "(Chua xac dinh)";
    ServiceGroup* sg = serviceGroupRepository.findById(t->getServiceGroupId());
    if (sg != nullptr)
    {
        sgName = sg->getName();
    }

    cout << "--- Thong tin chi tiet Ticket ---" << endl;
    cout << "Ma Ticket      : " << t->getId() << endl;
    cout << "Tieu de        : " << t->getTitle() << endl;
    cout << "Mo ta chi tiet : " << t->getDescription() << endl;
    cout << "Ma khach hang  : " << t->getCustomerId() << endl;
    cout << "Nhom dich vu   : [" << t->getServiceGroupId() << "] " << sgName << endl;
    cout << "Trang thai     : " << ticketStatusToString(t->getStatus()) << endl;
}

/**
 * @brief Tạo mới ticket (Create)
 * Ràng buộc:
 * 1. Các trường dữ liệu không được rỗng.
 * 2. Khóa ngoại: Nhóm dịch vụ (ServiceGroupId) phải tồn tại.
 * 3. Trạng thái nhóm dịch vụ: Nhóm dịch vụ phải đang hoạt động (active == true).
 */
void TicketMenu::add()
{
    cout << "--- Tao ticket moi ---" << endl;
    string title = inputNonEmptyString("Nhap tieu de ticket: ");
    string description = inputNonEmptyString("Nhap mo ta chi tiet van de: ");
    string customerId = inputNonEmptyString("Nhap ma khach hang: ");
    string serviceGroupId = inputNonEmptyString("Nhap ma nhom dich vu (vi du SG001): ");

    // Ràng buộc khóa ngoại: Kiểm tra tồn tại của nhóm dịch vụ
    ServiceGroup* sg = serviceGroupRepository.findById(serviceGroupId);
    if (sg == nullptr)
    {
        cout << "Loi rang buoc: Nhom dich vu voi ma '" << serviceGroupId << "' khong ton tai!" << endl;
        cout << "Vui long kiem tra danh sach nhom dich vu truoc khi tao ticket." << endl;
        return;
    }

    // Ràng buộc trạng thái: Nhóm dịch vụ phải đang hoạt động
    if (!sg->isActive())
    {
        cout << "Loi rang buoc: Nhom dich vu '" << sg->getName()
             << "' dang TAM DUNG hoat dong! Khong the tiep nhan ticket moi." << endl;
        return;
    }

    Ticket newTicket(title, description, customerId, serviceGroupId);
    try
    {
        ticketRepository.add(newTicket);
        const auto& list = ticketRepository.getAll();
        cout << "Tao ticket thanh cong! Ma ticket duoc cap: " << list.back().getId() << endl;
    }
    catch (const exception& e)
    {
        cout << "Loi khi tao ticket: " << e.what() << endl;
    }
}

/**
 * @brief Cập nhật thông tin và trạng thái ticket (Update)
 */
void TicketMenu::update()
{
    cout << "--- Cap nhat thong tin ticket ---" << endl;
    string id = inputNonEmptyString("Nhap ma ticket can cap nhat: ");

    Ticket* t = ticketRepository.findById(id);
    if (t == nullptr)
    {
        cout << "Loi: Khong tim thay ticket voi ma: " << id << endl;
        return;
    }

    Ticket updated = *t;

    cout << "Thong tin hien tai: [" << updated.getId() << "] " << updated.getTitle()
         << " | Trang thai: " << ticketStatusToString(updated.getStatus()) << endl;

    // Cập nhật tiêu đề
    cout << "Nhap tieu de moi (nhan Enter de giu nguyen): ";
    string newTitle;
    getline(cin, newTitle);
    newTitle = trim(newTitle);
    if (!newTitle.empty())
    {
        updated.setTitle(newTitle);
    }

    // Cập nhật mô tả
    cout << "Nhap mo ta moi (nhan Enter de giu nguyen): ";
    string newDesc;
    getline(cin, newDesc);
    newDesc = trim(newDesc);
    if (!newDesc.empty())
    {
        updated.setDescription(newDesc);
    }

    // Cập nhật trạng thái
    cout << "Chon trang thai moi (0: OPEN, 1: IN_PROGRESS, 2: RESOLVED, 3: CLOSED, Enter: Giu nguyen): ";
    string statusChoice;
    getline(cin, statusChoice);
    statusChoice = trim(statusChoice);
    if (!statusChoice.empty())
    {
        try
        {
            int st = stoi(statusChoice);
            if (st >= 0 && st <= 3)
            {
                updated.changeStatus(static_cast<TicketStatus>(st));
            }
            else
            {
                cout << "Loi: Ma trang thai khong hop le (chi chap nhan tu 0 den 3)! Giu nguyen trang thai cu." << endl;
            }
        }
        catch (...)
        {
            cout << "Loi: Dinh dang trang thai khong hop le! Giu nguyen trang thai cu." << endl;
        }
    }

    try
    {
        if (ticketRepository.update(updated))
        {
            cout << "Cap nhat ticket thanh cong!" << endl;
        }
        else
        {
            cout << "Loi khi cap nhat ticket!" << endl;
        }
    }
    catch (const exception& e)
    {
        cout << "Loi khi cap nhat ticket: " << e.what() << endl;
    }
}

/**
 * @brief Xóa ticket (Delete)
 * Cảnh báo nếu ticket đang trong tiến trình xử lý
 */
void TicketMenu::remove()
{
    cout << "--- Xoa ticket ---" << endl;
    string id = inputNonEmptyString("Nhap ma ticket can xoa: ");

    Ticket* t = ticketRepository.findById(id);
    if (t == nullptr)
    {
        cout << "Loi: Khong tim thay ticket voi ma: " << id << endl;
        return;
    }

    // Cảnh báo nghiệp vụ nếu ticket đang trong quá trình xử lý
    if (t->getStatus() == TicketStatus::IN_PROGRESS)
    {
        cout << "Canh bao nghiep vu: Ticket nay dang trong tien trinh xu ly (IN_PROGRESS)!" << endl;
    }

    cout << "Ban co chac chan muon xoa ticket " << id << " (" << t->getTitle() << ")? (y/n): ";
    string confirm;
    getline(cin, confirm);
    confirm = trim(confirm);

    if (confirm == "y" || confirm == "Y")
    {
        try
        {
            if (ticketRepository.remove(id))
            {
                cout << "Da xoa ticket thanh cong!" << endl;
            }
            else
            {
                cout << "Loi: Khong the xoa ticket!" << endl;
            }
        }
        catch (const exception& e)
        {
            cout << "Loi khi xoa ticket: " << e.what() << endl;
        }
    }
    else
    {
        cout << "Da huy thao tac xoa." << endl;
    }
}
