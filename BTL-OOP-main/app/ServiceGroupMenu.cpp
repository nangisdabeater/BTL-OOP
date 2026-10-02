#include "ServiceGroupMenu.h"
#include <iostream>
#include <iomanip>

using namespace std;

/**
 * @brief Khởi tạo đối tượng ServiceGroupMenu
 * @param sgRepo Tham chiếu tới kho lưu trữ Nhóm dịch vụ
 * @param tRepo Tham chiếu tới kho lưu trữ Ticket (để kiểm tra ràng buộc toàn vẹn)
 */
ServiceGroupMenu::ServiceGroupMenu(Repository<ServiceGroup>& sgRepo, Repository<Ticket>& tRepo)
    : serviceGroupRepository(sgRepo), ticketRepository(tRepo)
{
}

/**
 * @brief Cắt bỏ khoảng trắng thừa ở hai đầu chuỗi ký tự
 */
string ServiceGroupMenu::trim(const string& str)
{
    size_t first = str.find_first_not_of(" \t\r\n");
    if (first == string::npos) return "";
    size_t last = str.find_last_not_of(" \t\r\n");
    return str.substr(first, (last - first + 1));
}

/**
 * @brief Nhập chuỗi ký tự từ bàn phím và đảm bảo không được để trống
 * @param prompt Lời nhắc nhập dữ liệu
 * @return Chuỗi ký tự hợp lệ đã được cắt khoảng trắng
 */
string ServiceGroupMenu::inputNonEmptyString(const string& prompt)
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
 * @brief Hiển thị bảng danh mục các chức năng quản lý Nhóm dịch vụ
 */
void ServiceGroupMenu::showMenu()
{
    cout << "========================================" << endl;
    cout << "        QUAN LY NHOM DICH VU" << endl;
    cout << "========================================" << endl;
    cout << "1. Hien thi danh sach nhom dich vu" << endl;
    cout << "2. Tim kiem nhom dich vu theo ID" << endl;
    cout << "3. Them nhom dich vu moi" << endl;
    cout << "4. Cap nhat thong tin nhom dich vu" << endl;
    cout << "5. Xoa nhom dich vu" << endl;
    cout << "0. Quay lai menu chinh" << endl;
    cout << "========================================" << endl;
}

/**
 * @brief Điều phối vòng lặp xử lý các lựa chọn trong menu Nhóm dịch vụ
 */
void ServiceGroupMenu::run()
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
 * @brief Chức năng hiển thị danh sách tất cả nhóm dịch vụ (Read All)
 * Hiển thị đầy đủ 4 thuộc tính: Mã ID, Tên, Mô tả, Trạng thái hoạt động
 */
void ServiceGroupMenu::displayAll()
{
    const auto& list = serviceGroupRepository.getAll();
    if (list.empty())
    {
        cout << "Danh sach nhom dich vu dang trong!" << endl;
        return;
    }

    cout << left
         << setw(8)  << "Ma ID"
         << setw(25) << "Ten nhom dich vu"
         << setw(35) << "Mo ta"
         << setw(15) << "Trang thai" << endl;
    cout << string(83, '-') << endl;

    for (const auto& sg : list)
    {
        cout << left
             << setw(8)  << sg.getId()
             << setw(25) << sg.getName()
             << setw(35) << sg.getDescription()
             << setw(15) << (sg.isActive() ? "Hoat dong" : "Tam dung") << endl;
    }
}

/**
 * @brief Chức năng tìm kiếm nhóm dịch vụ theo mã ID (Read by ID)
 */
void ServiceGroupMenu::findById()
{
    string id = inputNonEmptyString("Nhap ma nhom dich vu can tim (vi du NP001): ");
    ServiceGroup* sg = serviceGroupRepository.findById(id);

    if (sg == nullptr)
    {
        cout << "Khong tim thay nhom dich vu voi ma: " << id << endl;
        return;
    }

    cout << "--- Thong tin nhom dich vu ---" << endl;
    cout << "Ma ID      : " << sg->getId() << endl;
    cout << "Ten nhom   : " << sg->getName() << endl;
    cout << "Mo ta      : " << sg->getDescription() << endl;
    cout << "Trang thai : " << (sg->isActive() ? "Hoat dong" : "Tam dung") << endl;
}

/**
 * @brief Chức năng thêm mới nhóm dịch vụ (Create)
 * Ràng buộc: Tên và mô tả không được rỗng, Tên nhóm dịch vụ không được trùng lặp
 */
void ServiceGroupMenu::add()
{
    cout << "--- Them nhom dich vu moi ---" << endl;
    string name = inputNonEmptyString("Nhap ten nhom dich vu: ");

    // Ràng buộc: Tên nhóm dịch vụ không được trùng lặp trong hệ thống
    for (const auto& existing : serviceGroupRepository.getAll())
    {
        if (existing.getName() == name)
        {
            cout << "Loi rang buoc: Ten nhom dich vu '" << name << "' da ton tai tren he thong!" << endl;
            return;
        }
    }

    string description = inputNonEmptyString("Nhap mo ta nhom dich vu: ");

    ServiceGroup newSg(name, description);
    try
    {
        serviceGroupRepository.add(newSg);
        const auto& list = serviceGroupRepository.getAll();
        cout << "Them nhom dich vu thanh cong! Ma duoc cap: " << list.back().getId() << endl;
    }
    catch (const exception& e)
    {
        cout << "Loi khi them nhom dich vu: " << e.what() << endl;
    }
}

/**
 * @brief Chức năng cập nhật thông tin nhóm dịch vụ (Update)
 * Ràng buộc: ID phải tồn tại, tên mới không được trùng với các nhóm khác
 */
void ServiceGroupMenu::update()
{
    cout << "--- Cap nhat thong tin nhom dich vu ---" << endl;
    string id = inputNonEmptyString("Nhap ma nhom dich vu can cap nhat: ");

    ServiceGroup* sg = serviceGroupRepository.findById(id);
    if (sg == nullptr)
    {
        cout << "Loi: Khong tim thay nhom dich vu voi ma: " << id << endl;
        return;
    }

    cout << "Thong tin hien tai: [" << sg->getId() << "] " << sg->getName()
         << " | " << sg->getDescription()
         << " | " << (sg->isActive() ? "Hoat dong" : "Tam dung") << endl;

    // Cập nhật tên nhóm
    cout << "Nhap ten moi (nhan Enter de giu nguyen): ";
    string newName;
    getline(cin, newName);
    newName = trim(newName);
    if (!newName.empty())
    {
        // Ràng buộc: Tên mới không được trùng với tên của nhóm dịch vụ khác
        for (const auto& other : serviceGroupRepository.getAll())
        {
            if (other.getId() != id && other.getName() == newName)
            {
                cout << "Loi rang buoc: Ten nhom dich vu '" << newName << "' da ton tai!" << endl;
                return;
            }
        }
        sg->setName(newName);
    }

    // Cập nhật mô tả
    cout << "Nhap mo ta moi (nhan Enter de giu nguyen): ";
    string newDesc;
    getline(cin, newDesc);
    newDesc = trim(newDesc);
    if (!newDesc.empty())
    {
        sg->setDescription(newDesc);
    }

    // Cập nhật trạng thái hoạt động
    cout << "Doi trang thai (1: Hoat dong, 0: Tam dung, Enter: Giu nguyen): ";
    string statusChoice;
    getline(cin, statusChoice);
    statusChoice = trim(statusChoice);
    if (statusChoice == "1")
    {
        sg->activate();
    }
    else if (statusChoice == "0")
    {
        sg->deactivate();
    }

    if (serviceGroupRepository.update(*sg))
    {
        cout << "Cap nhat nhom dich vu thanh cong!" << endl;
    }
    else
    {
        cout << "Loi khi cap nhat nhom dich vu!" << endl;
    }
}

/**
 * @brief Chức năng xóa nhóm dịch vụ (Delete)
 * Ràng buộc: ID phải tồn tại, kiểm tra toàn vẹn dữ liệu (không xóa khi còn ticket liên kết)
 */
void ServiceGroupMenu::remove()
{
    cout << "--- Xoa nhom dich vu ---" << endl;
    string id = inputNonEmptyString("Nhap ma nhom dich vu can xoa: ");

    ServiceGroup* sg = serviceGroupRepository.findById(id);
    if (sg == nullptr)
    {
        cout << "Loi: Khong tim thay nhom dich vu voi ma: " << id << endl;
        return;
    }

    // Ràng buộc toàn vẹn dữ liệu (Khóa ngoại): Từ chối xóa nếu có Ticket đang liên kết
    for (const auto& ticket : ticketRepository.getAll())
    {
        if (ticket.getServiceGroupId() == id)
        {
            cout << "Loi rang buoc toan ven: Khong the xoa nhom dich vu '" << id
                 << "' vi dang co ticket lien ket (Ma ticket: " << ticket.getId() << ")!" << endl;
            cout << "Vui long xu ly hoac xoa cac ticket lien quan truoc khi xoa nhom dich vu nay." << endl;
            return;
        }
    }

    cout << "Ban co chac chan muon xoa nhom dich vu " << id << " (" << sg->getName() << ")? (y/n): ";
    string confirm;
    getline(cin, confirm);
    confirm = trim(confirm);

    if (confirm == "y" || confirm == "Y")
    {
        if (serviceGroupRepository.remove(id))
        {
            cout << "Da xoa nhom dich vu thanh cong!" << endl;
        }
        else
        {
            cout << "Loi: Khong the xoa nhom dich vu!" << endl;
        }
    }
    else
    {
        cout << "Da huy thao tac xoa." << endl;
    }
}
