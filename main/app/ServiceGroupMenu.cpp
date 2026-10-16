#include "ServiceGroupMenu.h"
#include <iostream>
#include <iomanip>
#include <Utilities.h>

using namespace std;

/**
 * @brief Khởi tạo đối tượng ServiceGroupMenu (Quản lý Dịch vụ / ServiceManager)
 * @param sgRepo Tham chiếu tới kho lưu trữ Dịch vụ
 * @param tRepo Tham chiếu tới kho lưu trữ Ticket (để kiểm tra ràng buộc toàn vẹn)
 */
ServiceGroupMenu::ServiceGroupMenu(Repository<ServiceGroup>& sgRepo, Repository<Ticket>& tRepo)
    : serviceGroupRepository(sgRepo), ticketRepository(tRepo)
{
}

/**
 * @brief Hiển thị bảng danh mục các chức năng quản lý Dịch vụ
 */
void ServiceGroupMenu::showMenu()
{
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

/**
 * @brief Điều phối vòng lặp xử lý các lựa chọn trong menu Dịch vụ
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
 * @brief Chức năng hiển thị danh sách tất cả dịch vụ (Read All)
 */
void ServiceGroupMenu::displayAll()
{
    const auto& list = serviceGroupRepository.getAll();
    if (list.empty())
    {
        cout << "Danh sach dich vu dang trong!" << endl;
        Utils::backChoice();
        return;
    }
    cout << left
         << setw(8)  << "Ma ID"
         << setw(24) << "Ten dich vu"
         << setw(18) << "Phong ban"
         << setw(10) << "SLA(gio)"
         << setw(14) << "Phi co ban"
         << setw(15) << "Ngay ban hanh"
         << setw(14) << "Trang thai" << endl;
    cout << string(105, '-') << endl;

    for (const auto& sg : list)
    {
        cout << left
             << setw(8)  << sg.getId()
             << setw(24) << sg.getName()
             << setw(18) << sg.getDepartment()
             << setw(10) << sg.getSlaHours()
             << setw(14) << fixed << setprecision(2) << sg.getBaseFee()
             << setw(15) << sg.getCreatedDate()
             << setw(14) << (sg.isActive() ? "Hoat dong" : "Tam dung") << endl;
    }
    Utils::backChoice();
}

/**
 * @brief Chức năng tìm kiếm dịch vụ theo mã ID (Read by ID: SMxxx)
 */
void ServiceGroupMenu::findById()
{
    string id = Utils::inputNonEmptyString("Nhap ma dich vu can tim (vi du SM001, nhap 0 de quay lai): ");
    if (id == "0")
    {
        return;
    }
    ServiceGroup* sg = serviceGroupRepository.findById(id);

    if (sg == nullptr)
    {
        cout << "Khong tim thay dich vu voi ma: " << id << endl;
        Utils::backChoice();
        return;
    }

    Utils::printEntityDetails(*sg);
    Utils::backChoice();
}

/**
 * @brief Chức năng thêm mới dịch vụ (Create)
 * Ràng buộc:
 * - ID sinh tự động dạng SMxxx
 * - Tên không rỗng, tối đa 100 ký tự, không trùng lặp
 * - Mô tả, Phòng ban không rỗng
 * - SLA > 0 gio
 * - Phí cơ bản >= 0
 * - Ngày ban hành định dạng DD/MM/YYYY
 */
void ServiceGroupMenu::add()
{
    cout << "--- Them dich vu moi ---" << endl;
    string name = Utils::inputNonEmptyString("Nhap ten dich vu (nhap 0 de quay lai): ");
    if (name == "0")
    {
        return;
    }

    // Ràng buộc: Tên dịch vụ không được trùng lặp trong hệ thống
    for (const auto& existing : serviceGroupRepository.getAll())
    {
        if (existing.getName() == name)
        {
            cout << "Loi rang buoc: Ten dich vu '" << name << "' da ton tai tren he thong!" << endl;
            Utils::backChoice();
            return;
        }
    }

    string description = Utils::inputNonEmptyString("Nhap mo ta dich vu: ");
    string department = Utils::inputNonEmptyString("Nhap phong ban phu trach: ");
    int slaHours = Utils::inputInt("Nhap thoi gian xu ly tieu chuan SLA (gio, >0): ", 1, 10000);
    double baseFee = Utils::inputDouble("Nhap phi dich vu co ban (VND, >= 0): ", 0.0);
    string createdDate = Utils::inputDateString("Nhap ngay ban hanh (DD/MM/YYYY): ");

    ServiceGroup newSg(name, description, department, slaHours, baseFee, createdDate);
    try
    {
        serviceGroupRepository.add(newSg);
        const auto& list = serviceGroupRepository.getAll();
        cout << "Them dich vu thanh cong! Ma ID duoc cap: " << list.back().getId() << endl;
    }
    catch (const exception& e)
    {
        cout << "Loi khi them dich vu: " << e.what() << endl;
    }
    Utils::backChoice();
}

/**
 * @brief Chức năng cập nhật thông tin dịch vụ (Update)
 * Ràng buộc: ID phải tồn tại, tên mới không được trùng với các dịch vụ khác
 */
void ServiceGroupMenu::update()
{
    cout << "--- Cap nhat thong tin dich vu ---" << endl;
    string id = Utils::inputNonEmptyString("Nhap ma dich vu can cap nhat (vi du SM001, nhap 0 de quay lai): ");
    if (id == "0")
    {
        return;
    }

    ServiceGroup* sg = serviceGroupRepository.findById(id);
    if (sg == nullptr)
    {
        cout << "Loi: Khong tim thay dich vu voi ma: " << id << endl;
        Utils::backChoice();
        return;
    }

    ServiceGroup updated = *sg;

    cout << "Thong tin hien tai:" << endl;
    updated.printInfo();

    // Cập nhật tên
    cout << "Nhap ten moi (nhan Enter de giu nguyen): ";
    string newName;
    getline(cin, newName);
    newName = Utils::trim(newName);
    if (!newName.empty())
    {
        for (const auto& other : serviceGroupRepository.getAll())
        {
            if (other.getId() != id && other.getName() == newName)
            {
                cout << "Loi rang buoc: Ten dich vu '" << newName << "' da ton tai!" << endl;
                return;
            }
        }
        updated.setName(newName);
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

    // Cập nhật phòng ban
    cout << "Nhap phong ban moi (nhan Enter de giu nguyen): ";
    string newDept;
    getline(cin, newDept);
    newDept = Utils::trim(newDept);
    if (!newDept.empty())
    {
        updated.setDepartment(newDept);
    }

    // Cập nhật SLA
    cout << "Nhap SLA moi tinh bang gio (nhan Enter de giu nguyen): ";
    string slaInput;
    getline(cin, slaInput);
    slaInput = Utils::trim(slaInput);
    if (!slaInput.empty())
    {
        try {
            int newSla = stoi(slaInput);
            if (newSla > 0) updated.setSlaHours(newSla);
            else cout << "Gia tri SLA khong hop le, giu nguyen gia tri cu." << endl;
        } catch (...) {
            cout << "Gia tri SLA khong hop le, giu nguyen gia tri cu." << endl;
        }
    }

    // Cập nhật phí cơ bản
    cout << "Nhap phi co ban moi (nhan Enter de giu nguyen): ";
    string feeInput;
    getline(cin, feeInput);
    feeInput = Utils::trim(feeInput);
    if (!feeInput.empty())
    {
        try {
            double newFee = stod(feeInput);
            if (newFee >= 0.0) updated.setBaseFee(newFee);
            else cout << "Phi khong hop le, giu nguyen gia tri cu." << endl;
        } catch (...) {
            cout << "Phi khong hop le, giu nguyen gia tri cu." << endl;
        }
    }

    // Cập nhật trạng thái hoạt động
    cout << "Doi trang thai (1: Hoat dong, 0: Tam dung, Enter: Giu nguyen): ";
    string statusChoice;
    getline(cin, statusChoice);
    statusChoice = Utils::trim(statusChoice);
    if (statusChoice == "1")
    {
        updated.activate();
    }
    else if (statusChoice == "0")
    {
        updated.deactivate();
    }

    try
    {
        if (serviceGroupRepository.update(updated))
        {
            cout << "Cap nhat dich vu thanh cong!" << endl;
        }
        else
        {
            cout << "Loi khi cap nhat dich vu!" << endl;
        }
    }
    catch (const exception& e)
    {
        cout << "Loi khi cap nhat dich vu: " << e.what() << endl;
    }
    Utils::backChoice();
}

/**
 * @brief Chức năng xóa dịch vụ (Delete)
 * Ràng buộc: ID phải tồn tại, kiểm tra toàn vẹn dữ liệu (không xóa khi còn ticket liên kết)
 */
void ServiceGroupMenu::remove()
{
    cout << "--- Xoa dich vu ---" << endl;
    string id = Utils::inputNonEmptyString("Nhap ma dich vu can xoa (vi du SM001, nhap 0 de quay lai): ");
    if (id == "0")
    {
        return;
    }

    ServiceGroup* sg = serviceGroupRepository.findById(id);
    if (sg == nullptr)
    {
        cout << "Loi: Khong tim thay dich vu voi ma: " << id << endl;
        Utils::backChoice();
        return;
    }

    // Ràng buộc toàn vẹn dữ liệu (Khóa ngoại): Từ chối xóa nếu có Ticket đang liên kết
    for (const auto& ticket : ticketRepository.getAll())
    {
        if (ticket.getServiceGroupId() == id)
        {
            cout << "Loi rang buoc toan ven: Khong the xoa dich vu '" << id
                 << "' vi dang co ticket lien ket (Ma ticket: " << ticket.getId() << ")!" << endl;
            cout << "Vui long xu ly hoac xoa cac ticket lien quan truoc khi xoa dich vu nay." << endl;
            Utils::backChoice();
            return;
        }
    }

    cout << "Ban co chac chan muon xoa dich vu " << id << " (" << sg->getName() << ")? (y/n): ";
    string confirm;
    getline(cin, confirm);
    confirm = Utils::trim(confirm);

    if (confirm == "y" || confirm == "Y")
    {
        try
        {
            if (serviceGroupRepository.remove(id))
            {
                cout << "Da xoa dich vu thanh cong!" << endl;
            }
            else
            {
                cout << "Loi: Khong the xoa dich vu!" << endl;
            }
        }
        catch (const exception& e)
        {
            cout << "Loi khi xoa dich vu: " << e.what() << endl;
        }
    }
    else
    {
        cout << "Da huy thao tac xoa." << endl;
    }
    Utils::backChoice();
}
