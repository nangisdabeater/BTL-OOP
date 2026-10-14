#include "TicketMenu.h"
#include <iostream>
#include <iomanip>
#include <Utilities.h>

using namespace std;

/**
 * @brief Khởi tạo đối tượng TicketMenu
 * @param tRepo Tham chiếu tới kho lưu trữ Ticket
 * @param sgRepo Tham chiếu tới kho lưu trữ Dịch vụ (để kiểm tra khóa ngoại)
 */
TicketMenu::TicketMenu(Repository<Ticket>& tRepo, Repository<ServiceGroup>& sgRepo, Repository<Customer>& cRepo)
    : ticketRepository(tRepo), serviceGroupRepository(sgRepo), customerRepository(cRepo)
{
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
 * @brief Chuyển đổi mã mức độ ưu tiên sang chuỗi hiển thị
 */
string TicketMenu::priorityToString(int priority)
{
    switch (priority)
    {
    case 1: return "Thap";
    case 2: return "Trung binh";
    case 3: return "Cao";
    case 4: return "Khan cap";
    default: return to_string(priority);
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
 */
void TicketMenu::displayAll()
{
    const auto& list = ticketRepository.getAll();
    if (list.empty())
    {
        cout << "Danh sach ticket dang trong!" << endl;
        Utils::backChoice();
        return;
    }

    cout << left
         << setw(8)  << "Ma ID"
         << setw(22) << "Tieu de"
         << setw(10) << "Ma KH"
         << setw(10) << "Ma DV"
         << setw(12) << "Uu tien"
         << setw(12) << "Chi phi"
         << setw(12) << "Ngay tao"
         << setw(14) << "Trang thai" << endl;
    cout << string(100, '-') << endl;

    for (const auto& t : list)
    {
        cout << left
             << setw(8)  << t.getId()
             << setw(22) << t.getTitle()
             << setw(10) << t.getCustomerId()
             << setw(10) << t.getServiceGroupId()
             << setw(12) << priorityToString(t.getPriority())
             << setw(12) << fixed << setprecision(2) << t.getEstimatedFee()
             << setw(12) << t.getCreatedDate()
             << setw(14) << ticketStatusToString(t.getStatus()) << endl;
    }
    Utils::backChoice();
}

/**
 * @brief Tìm kiếm ticket theo mã ID (Read by ID: TKxxx)
 */
void TicketMenu::findById()
{
    string id = Utils::inputNonEmptyString("Nhap ma ticket can tim (vi du TK001, nhap 0 de quay lai): ");
    if (id == "0")
    {
        return;
    }
    Ticket* t = ticketRepository.findById(id);

    if (t == nullptr)
    {
        cout << "Khong tim thay ticket voi ma: " << id << endl;
        Utils::backChoice();
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
    cout << "Dich vu        : [" << t->getServiceGroupId() << "] " << sgName << endl;
    cout << "Muc do uu tien : " << priorityToString(t->getPriority()) << " (" << t->getPriority() << ")" << endl;
    cout << "Chi phi uoc tinh: " << fixed << setprecision(2) << t->getEstimatedFee() << " VND" << endl;
    cout << "Ngay tao       : " << t->getCreatedDate() << endl;
    cout << "Trang thai     : " << ticketStatusToString(t->getStatus()) << endl;
    Utils::backChoice();
}

/**
 * @brief Tạo mới ticket (Create)
 * Ràng buộc:
 * 1. Mã ticket sinh tự động dạng TKxxx.
 * 2. Tiêu đề không rỗng, tối đa 100 ký tự.
 * 3. Khóa ngoại: Mã dịch vụ (ServiceGroupId: SMxxx) phải tồn tại và đang hoạt động.
 * 4. Mức ưu tiên 1 - 4. Chi phí >= 0. Ngày tạo hợp lệ (DD/MM/YYYY).
 */
void TicketMenu::add()
{
    cout << "--- Tao ticket moi ---" << endl;
    string title = Utils::inputNonEmptyString("Nhap tieu de ticket (nhap 0 de quay lai): ");
    if (title == "0")
    {
        return;
    }
    string description = Utils::inputNonEmptyString("Nhap mo ta chi tiet van de: ");
    
    // Ràng buộc khóa ngoại: Kiểm tra tồn tại của khách hàng trong CustomerRepository
    string customerId;
    while (true)
    {
        customerId = Utils::inputNonEmptyString("Nhap ma khach hang (VD: KH001, nhap 0 de quay lai): ");
        if (customerId == "0")
        {
            cout << "Da huy thao tac tao ticket." << endl;
            Utils::backChoice();
            return;
        }
        Customer* cust = customerRepository.findById(customerId);
        if (cust == nullptr)
        {
            cout << "Loi rang buoc: Khach hang voi ma '" << customerId << "' khong ton tai! Vui long nhap lai." << endl;
        }
        else
        {
            break;
        }
    }
    
    // Ràng buộc khóa ngoại: Kiểm tra tồn tại và hoạt động của dịch vụ
    string serviceGroupId;
    while (true)
    {
        serviceGroupId = Utils::inputNonEmptyString("Nhap ma dich vu (vi du SM001, nhap 0 de quay lai): ");
        if (serviceGroupId == "0")
        {
            cout << "Da huy thao tac tao ticket." << endl;
            Utils::backChoice();
            return;
        }
        ServiceGroup* sg = serviceGroupRepository.findById(serviceGroupId);
        if (sg == nullptr)
        {
            cout << "Loi rang buoc: Dich vu voi ma '" << serviceGroupId << "' khong ton tai! Vui long nhap lai." << endl;
        }
        else if (!sg->isActive())
        {
            cout << "Loi rang buoc: Dich vu '" << sg->getName()
                 << "' dang TAM NGUNG hoat dong! Khong the tao ticket moi." << endl;
        }
        else
        {
            break;
        }
    }

    int priority = Utils::inputInt("Nhap muc uu tien (1: Thap, 2: Trung binh, 3: Cao, 4: Khan cap): ", 1, 4);
    double estimatedFee = Utils::inputDouble("Nhap chi phi xu ly uoc tinh (VND, >= 0): ", 0.0);
    string createdDate = Utils::inputDateString("Nhap ngay tao ticket (DD/MM/YYYY): ");

    Ticket newTicket(title, description, customerId, serviceGroupId, priority, estimatedFee, createdDate);
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
    Utils::backChoice();
}

/**
 * @brief Cập nhật thông tin và trạng thái ticket (Update)
 */
void TicketMenu::update()
{
    cout << "--- Cap nhat thong tin ticket ---" << endl;
    string id = Utils::inputNonEmptyString("Nhap ma ticket can cap nhat (vi du TK001, nhap 0 de quay lai): ");
    if (id == "0")
    {
        return;
    }

    Ticket* t = ticketRepository.findById(id);
    if (t == nullptr)
    {
        cout << "Loi: Khong tim thay ticket voi ma: " << id << endl;
        Utils::backChoice();
        return;
    }

    Ticket updated = *t;

    cout << "Thong tin hien tai: [" << updated.getId() << "] " << updated.getTitle()
         << " | Uu tien: " << priorityToString(updated.getPriority())
         << " | Chi phi: " << fixed << setprecision(2) << updated.getEstimatedFee()
         << " | Trang thai: " << ticketStatusToString(updated.getStatus()) << endl;

    // Cập nhật tiêu đề
    cout << "Nhap tieu de moi (nhan Enter de giu nguyen): ";
    string newTitle;
    getline(cin, newTitle);
    newTitle = Utils::trim(newTitle);
    if (!newTitle.empty())
    {
        updated.setTitle(newTitle);
    }

    // Cập nhật mô tả
    cout << "Nhap mo ta moi (nhan Enter de giu nguyen): ";
    string newDesc;
    getline(cin, newDesc);
    newDesc = Utils::trim(newDesc);
    if (!newDesc.empty())
    {
        updated.setDescription(newDesc);
    }

    // Cập nhật mức ưu tiên
    cout << "Nhap muc uu tien moi (1-4, Enter de giu nguyen): ";
    string prioInput;
    getline(cin, prioInput);
    prioInput = Utils::trim(prioInput);
    if (!prioInput.empty())
    {
        try {
            int p = stoi(prioInput);
            if (p >= 1 && p <= 4) updated.setPriority(p);
            else cout << "Muc uu tien khong hop le, giu nguyen cu." << endl;
        } catch (...) {
            cout << "Muc ưu tiên khong hop le, giu nguyen cu." << endl;
        }
    }

    // Cập nhật chi phí
    cout << "Nhap chi phi moi (Enter de giu nguyen): ";
    string feeInput;
    getline(cin, feeInput);
    feeInput = Utils::trim(feeInput);
    if (!feeInput.empty())
    {
        try {
            double fee = stod(feeInput);
            if (fee >= 0.0) updated.setEstimatedFee(fee);
            else cout << "Chi phi khong hop le, giu nguyen cu." << endl;
        } catch (...) {
            cout << "Chi phi khong hop le, giu nguyen cu." << endl;
        }
    }

    // Cập nhật trạng thái
    cout << "Chon trang thai moi (0: OPEN, 1: IN_PROGRESS, 2: RESOLVED, 3: CLOSED, Enter: Giu nguyen): ";
    string statusChoice;
    getline(cin, statusChoice);
    statusChoice = Utils::trim(statusChoice);
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
    Utils::backChoice();
}

/**
 * @brief Xóa ticket (Delete)
 */
void TicketMenu::remove()
{
    cout << "--- Xoa ticket ---" << endl;
    string id = Utils::inputNonEmptyString("Nhap ma ticket can xoa (vi du TK001, nhap 0 de quay lai): ");
    if (id == "0")
    {
        return;
    }

    Ticket* t = ticketRepository.findById(id);
    if (t == nullptr)
    {
        cout << "Loi: Khong tim thay ticket voi ma: " << id << endl;
        Utils::backChoice();
        return;
    }

    if (t->getStatus() == TicketStatus::IN_PROGRESS)
    {
        cout << "Canh bao nghiep vu: Ticket nay dang trong tien trinh xu ly (IN_PROGRESS)!" << endl;
    }

    cout << "Ban co chac chan muon xoa ticket " << id << " (" << t->getTitle() << ")? (y/n): ";
    string confirm;
    getline(cin, confirm);
    confirm = Utils::trim(confirm);

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
    Utils::backChoice();
}
