#include "TransferTicketMenu.h"
#include <iostream>
#include <iomanip>
#include <Utilities.h>

// Chỉ dùng using namespace std ở file .cpp để giữ sạch không gian tên toàn cục ở file .h
using namespace std;

// Khởi tạo menu bằng danh sách khởi tạo cho 2 biến tham chiếu Repository
TransferTicketMenu::TransferTicketMenu(
    Repository<TransferTicket>& tfRepo,
    Repository<SLALevel>& sRepo
)
    : transferRepository(tfRepo), slaRepository(sRepo)
{
}

// Chuyển đổi giá trị enum TransferStatus sang chuỗi mô tả để hiển thị trên bảng
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

// Tra cứu tên mức SLA từ mã slaId để hiển thị kèm khi xem chi tiết phiếu chuyển
string TransferTicketMenu::slaNameOf(const string& slaId)
{
    SLALevel* s = slaRepository.findById(slaId);
    if (s == nullptr)
    {
        return "(khong xac dinh)";
    }
    return s->getName();
}

// Hiển thị bảng tùy chọn quản lý phiếu chuyển xử lý
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

// Vòng lặp điều phối các chức năng trong menu phiếu chuyển xử lý
void TransferTicketMenu::run()
{
    bool inMenu = true;
    while (inMenu)
    {
        showMenu();

        int choice;
        cout << "Nhap lua chon: ";
        // Kiểm tra nếu người dùng nhập chữ thay vì số để xóa cờ lỗi cin, tránh lặp vô hạn
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

// Hiển thị danh sách phiếu chuyển dưới dạng bảng căn lề trái
void TransferTicketMenu::displayAll()
{
    const auto& list = transferRepository.getAll();
    if (list.empty())
    {
        cout << "Danh sach phieu chuyen xu ly dang trong!" << endl;
        Utils::backChoice();
        return;
    }
    cout << left
         << setw(8)  << "Ma ID"
         << setw(10) << "Ma SLA"
         << setw(22) << "Bo phan chuyen"
         << setw(24) << "Bo phan nhan"
         << setw(36) << "Ly do"
         << setw(12) << "Trang thai" << endl;
    cout << string(112, '-') << endl;

    for (const auto& t : list)
    {
        cout << left
             << setw(8)  << t.getId()
             << setw(10) << t.getSlaId()
             << setw(22) << t.getFromDept()
             << setw(24) << t.getToDept()
             << setw(36) << t.getReason()
             << setw(12) << statusToString(t.getStatus()) << endl;
    }
    Utils::backChoice();
}

// Tìm kiếm phiếu chuyển theo mã định danh và hiển thị kèm tên mức SLA
void TransferTicketMenu::findById()
{
    string id = Utils::inputNonEmptyString("Nhap ma phieu chuyen can tim (vi du TF001, nhap 0 de quay lai): ");
    if (id == "0")
    {
        return;
    }
    TransferTicket* t = transferRepository.findById(id);
    if (t == nullptr)
    {
        cout << "Khong tim thay phieu chuyen voi ma: " << id << endl;
        Utils::backChoice();
        return;
    }

    Utils::printEntityDetails(*t);
    cout << "  -> Muc cam ket SLA: [" << t->getSlaId() << "] " << slaNameOf(t->getSlaId()) << endl;
    Utils::backChoice();
}

// Tạo phiếu chuyển mới: kiểm tra khóa ngoại slaId tồn tại và bộ phận chuyển khác bộ phận nhận
void TransferTicketMenu::add()
{
    cout << "--- Tao phieu chuyen xu ly ---" << endl;
    if (slaRepository.getAll().empty())
    {
        cout << "Loi: Chua co muc SLA nao trong he thong! Vui long tao muc SLA truoc." << endl;
        Utils::backChoice();
        return;
    }

    string slaId;
    while (true)
    {
        slaId = Utils::inputNonEmptyString("Nhap ma muc SLA ap dung (VD: SL001, nhap 0 de quay lai): ");
        if (slaId == "0")
        {
            return;
        }
        // Kiểm tra mã SLA phải tồn tại trong slaRepository trước khi cho phép gán
        SLALevel* sla = slaRepository.findById(slaId);
        if (sla == nullptr)
        {
            cout << "Loi rang buoc: Muc SLA '" << slaId << "' khong ton tai! Vui long nhap lai." << endl;
        }
        else
        {
            break;
        }
    }

    string fromDept = Utils::inputNonEmptyString("Nhap bo phan chuyen (VD: Tuyen 1 - CSKH, nhap 0 de quay lai): ");
    if (fromDept == "0")
    {
        return;
    }

    string toDept = Utils::inputNonEmptyString("Nhap bo phan nhan (VD: Tuyen 2 - Ky thuat, nhap 0 de quay lai): ");
    if (toDept == "0")
    {
        return;
    }

    // Kiểm tra ràng buộc nghiệp vụ: không được chuyển phiếu cho chính bộ phận đang giữ
    if (fromDept == toDept)
    {
        cout << "Loi rang buoc: Bo phan chuyen va bo phan nhan khong duoc trung nhau!" << endl;
        Utils::backChoice();
        return;
    }

    string reason = Utils::inputNonEmptyString("Nhap ly do chuyen: ");

    try
    {
        TransferTicket tf(slaId, fromDept, toDept, reason);
        transferRepository.add(tf);
        cout << "Tao phieu chuyen thanh cong! Ma duoc cap: "
             << transferRepository.getAll().back().getId() << endl;
    }
    catch (const exception& e)
    {
        cout << "Loi khi tao phieu chuyen: " << e.what() << endl;
    }
    Utils::backChoice();
}

// Cập nhật phiếu chuyển: khóa sửa nếu đã Hoàn tất và chỉ cho phép trạng thái tiến lên một chiều
void TransferTicketMenu::update()
{
    cout << "--- Cap nhat phieu chuyen xu ly ---" << endl;
    string id = Utils::inputNonEmptyString("Nhap ma phieu chuyen can cap nhat (nhap 0 de quay lai): ");
    if (id == "0")
    {
        return;
    }

    TransferTicket* t = transferRepository.findById(id);
    if (t == nullptr)
    {
        cout << "Loi: Khong tim thay phieu chuyen voi ma: " << id << endl;
        Utils::backChoice();
        return;
    }
    // Phiếu đã hoàn tất thì bị khóa chỉnh sửa để đảm bảo tính toàn vẹn lịch sử xử lý
    if (t->getStatus() == TransferStatus::COMPLETED)
    {
        cout << "Loi rang buoc: Phieu '" << id << "' da Hoan tat, khong duoc chinh sua!" << endl;
        Utils::backChoice();
        return;
    }

    TransferTicket updated = *t;
    cout << "Thong tin hien tai:" << endl;
    updated.printInfo();

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
                Utils::backChoice();
                return;
            }
            // Dùng hàm changeStatus thay vì gán trực tiếp để kiểm tra quy tắc không được lùi trạng thái
            updated.changeStatus(static_cast<TransferStatus>(st));
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
    Utils::backChoice();
}

// Xóa phiếu chuyển: chặn xóa nếu phiếu đã Hoàn tất để giữ lại dữ liệu đối soát
void TransferTicketMenu::remove()
{
    cout << "--- Xoa phieu chuyen xu ly ---" << endl;
    string id = Utils::inputNonEmptyString("Nhap ma phieu chuyen can xoa (nhap 0 de quay lai): ");
    if (id == "0")
    {
        return;
    }

    TransferTicket* t = transferRepository.findById(id);
    if (t == nullptr)
    {
        cout << "Loi: Khong tim thay phieu chuyen voi ma: " << id << endl;
        Utils::backChoice();
        return;
    }
    if (t->getStatus() == TransferStatus::COMPLETED)
    {
        cout << "Loi rang buoc: Khong the xoa phieu '" << id
             << "' vi da Hoan tat (giu lai de truy vet)!" << endl;
        Utils::backChoice();
        return;
    }

    cout << "Thong tin phieu chuyen can xoa:" << endl;
    t->printInfo();
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
    Utils::backChoice();
}
