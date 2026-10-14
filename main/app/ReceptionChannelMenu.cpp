/**
 * @file ReceptionChannelMenu.cpp
 * @brief Triển khai các chức năng điều khiển Menu cho Kênh tiếp nhận (ReceptionChannel)
 * [PHẦN THÊM MỚI]
 */

#include "ReceptionChannelMenu.h"
#include <iostream>
#include <iomanip>
#include <Utilities.h>

using namespace std;

/**
 * @brief Khởi tạo đối tượng ReceptionChannelMenu
 * @param chRepo Kho lưu trữ Kênh tiếp nhận
 * @param cRepo Kho lưu trữ Cuộc gọi tiếp nhận (dùng để kiểm tra ràng buộc trước khi xóa)
 */
ReceptionChannelMenu::ReceptionChannelMenu(Repository<ReceptionChannel>& chRepo, Repository<InboundCall>& cRepo)
    : channelRepository(chRepo), callRepository(cRepo)
{
}

/**
 * @brief Hiển thị bảng danh mục chức năng quản lý Kênh tiếp nhận
 */
void ReceptionChannelMenu::showMenu()
{
    cout << "========================================" << endl;
    cout << "        QUAN LY KENH TIEP NHAN" << endl;
    cout << "========================================" << endl;
    cout << "1. Hien thi danh sach kenh tiep nhan" << endl;
    cout << "2. Tim kiem kenh tiep nhan theo ID" << endl;
    cout << "3. Them kenh tiep nhan moi" << endl;
    cout << "4. Cap nhat thong tin kenh tiep nhan" << endl;
    cout << "5. Thay doi trang thai (Bat/Tat hoat dong)" << endl;
    cout << "6. Xoa kenh tiep nhan" << endl;
    cout << "0. Quay lai menu chinh" << endl;
    cout << "========================================" << endl;
}

/**
 * @brief Điều phối vòng lặp các lựa chọn trong menu Kênh tiếp nhận
 */
void ReceptionChannelMenu::run()
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
            toggleStatus();
            break;
        case 6:
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
 * @brief Hiển thị tất cả kênh tiếp nhận (Read All)
 */
void ReceptionChannelMenu::displayAll()
{
    const auto& list = channelRepository.getAll();
    if (list.empty())
    {
        cout << "Danh sach kenh tiep nhan dang trong!" << endl;
        Utils::backChoice();
        return;
    }

    cout << left
         << setw(8)  << "Ma ID"
         << setw(25) << "Ten kenh tiep nhan"
         << setw(35) << "Mo ta"
         << setw(15) << "Trang thai" << endl;
    cout << string(83, '-') << endl;

    for (const auto& ch : list)
    {
        cout << left
             << setw(8)  << ch.getId()
             << setw(25) << ch.getName()
             << setw(35) << ch.getDescription()
             << setw(15) << (ch.isActive() ? "Hoat dong" : "Ngung hoat dong") << endl;
    }
    Utils::backChoice();
}

/**
 * @brief Tìm kiếm kênh tiếp nhận theo ID (CNxxx)
 */
void ReceptionChannelMenu::findById()
{
    string id = Utils::inputNonEmptyString("Nhap ma kenh tiep nhan can tim (vi du CN001, nhap 0 de quay lai): ");
    if (id == "0")
    {
        return;
    }
    ReceptionChannel* ch = channelRepository.findById(id);

    if (ch == nullptr)
    {
        cout << "Khong tim thay kenh tiep nhan voi ma: " << id << endl;
        Utils::backChoice();
        return;
    }

    cout << "--- Thong tin kenh tiep nhan ---" << endl;
    cout << "Ma ID      : " << ch->getId() << endl;
    cout << "Ten kenh   : " << ch->getName() << endl;
    cout << "Mo ta      : " << ch->getDescription() << endl;
    cout << "Trang thai : " << (ch->isActive() ? "Hoat dong" : "Ngung hoat dong") << endl;
    Utils::backChoice();
}

/**
 * @brief Thêm kênh tiếp nhận mới (Create)
 * Ràng buộc: Tên và mô tả không rỗng, Tên kênh không được trùng lặp.
 * Tự động cấp mã ID định dạng CN001, CN002,...
 */
void ReceptionChannelMenu::add()
{
    cout << "--- Them kenh tiep nhan moi ---" << endl;
    string name = Utils::inputNonEmptyString("Nhap ten kenh tiep nhan (VD: Hotline 1900, Email, Zalo OA... nhap 0 de quay lai): ");
    if (name == "0")
    {
        return;
    }

    // Kiểm tra trùng lặp tên kênh bằng tiện ích isDuplicate
    bool duplicate = ValidationUtils::isDuplicate(
        channelRepository.getAll(),
        [&name](const ReceptionChannel& existing) {
            return existing.getName() == name;
        }
    );

    if (duplicate)
    {
        cout << "Loi: Ten kenh tiep nhan '" << name << "' da ton tai tren he thong!" << endl;
        Utils::backChoice();
        return;
    }

    string description = Utils::inputNonEmptyString("Nhap mo ta kenh tiep nhan: ");

    ReceptionChannel newChannel(name, description);
    try
    {
        // Repository sẽ tự sinh mã ID với tiền tố "CN", VD: CN001
        channelRepository.add(newChannel);
        const auto& list = channelRepository.getAll();
        cout << "Them kenh tiep nhan thanh cong! Ma ID duoc cap: " << list.back().getId() << endl;
    }
    catch (const exception& e)
    {
        cout << "Loi khi them kenh tiep nhan: " << e.what() << endl;
    }
    Utils::backChoice();
}

/**
 * @brief Cập nhật thông tin kênh tiếp nhận (Update)
 */
void ReceptionChannelMenu::update()
{
    cout << "--- Cap nhat thong tin kenh tiep nhan ---" << endl;
    string id = Utils::inputNonEmptyString("Nhap ma kenh tiep nhan can cap nhat (vi du CN001, nhap 0 de quay lai): ");
    if (id == "0")
    {
        return;
    }

    ReceptionChannel* ch = channelRepository.findById(id);
    if (ch == nullptr)
    {
        cout << "Khong tim thay kenh tiep nhan voi ma: " << id << endl;
        Utils::backChoice();
        return;
    }

    ReceptionChannel updated = *ch;

    cout << "Thong tin hien tai:" << endl;
    updated.printInfo();

    cout << "\nNhap ten moi (de trong neu giu nguyen): ";
    string newName;
    getline(cin, newName);
    newName = Utils::trim(newName);

    if (!newName.empty())
    {
        // Kiểm tra trùng lặp với các kênh khác
        bool duplicate = ValidationUtils::isDuplicate(
            channelRepository.getAll(),
            [&id, &newName](const ReceptionChannel& other) {
                return other.getId() != id && other.getName() == newName;
            }
        );

        if (duplicate)
        {
            cout << "Loi: Ten kenh tiep nhan '" << newName << "' da ton tai!" << endl;
            Utils::backChoice();
            return;
        }
        updated.setName(newName);
    }

    cout << "Nhap mo ta moi (de trong neu giu nguyen): ";
    string newDesc;
    getline(cin, newDesc);
    newDesc = Utils::trim(newDesc);

    if (!newDesc.empty())
    {
        updated.setDescription(newDesc);
    }

    try
    {
        if (channelRepository.update(updated))
        {
            cout << "Cap nhat kenh tiep nhan thanh cong!" << endl;
        }
        else
        {
            cout << "Loi khi cap nhat kenh tiep nhan!" << endl;
        }
    }
    catch (const exception& e)
    {
        cout << "Loi khi cap nhat kenh tiep nhan: " << e.what() << endl;
    }
    Utils::backChoice();
}

/**
 * @brief Bật hoặc tắt trạng thái hoạt động của kênh tiếp nhận
 */
void ReceptionChannelMenu::toggleStatus()
{
    cout << "--- Thay doi trang thai kenh tiep nhan ---" << endl;
    string id = Utils::inputNonEmptyString("Nhap ma kenh tiep nhan (vi du CN001, nhap 0 de quay lai): ");
    if (id == "0")
    {
        return;
    }

    ReceptionChannel* ch = channelRepository.findById(id);
    if (ch == nullptr)
    {
        cout << "Khong tim thay kenh tiep nhan voi ma: " << id << endl;
        Utils::backChoice();
        return;
    }

    if (ch->isActive())
    {
        ch->deactivate();
        cout << "Da chuyen kenh " << id << " sang trang thai: NGUNG HOAT DONG." << endl;
    }
    else
    {
        ch->activate();
        cout << "Da chuyen kenh " << id << " sang trang thai: HOAT DONG." << endl;
    }

    try
    {
        channelRepository.update(*ch);
    }
    catch (const exception& e)
    {
        cout << "Loi khi luu trang thai: " << e.what() << endl;
    }
    Utils::backChoice();
}

/**
 * @brief Xóa kênh tiếp nhận (Delete)
 * Ràng buộc toàn vẹn: Không cho xóa nếu kênh này đã có Cuộc gọi tiếp nhận liên kết.
 */
void ReceptionChannelMenu::remove()
{
    cout << "--- Xoa kenh tiep nhan ---" << endl;
    string id = Utils::inputNonEmptyString("Nhap ma kenh tiep nhan can xoa (vi du CN001, nhap 0 de quay lai): ");
    if (id == "0")
    {
        return;
    }

    ReceptionChannel* ch = channelRepository.findById(id);
    if (ch == nullptr)
    {
        cout << "Khong tim thay kenh tiep nhan voi ma: " << id << endl;
        Utils::backChoice();
        return;
    }

    // KIỂM TRA TOÀN VẸN DỮ LIỆU: Kênh tiếp nhận có đang được cuộc gọi nào sử dụng không?
    for (const auto& call : callRepository.getAll())
    {
        if (call.getChannelId() == id)
        {
            cout << "Loi rang buoc: Kenh '" << id << "' dang co cuoc goi tiep nhan lien ket (VD ma " 
                 << call.getId() << ")! Khong the xoa." << endl;
            Utils::backChoice();
            return;
        }
    }

    cout << "Ban co chac chan muon xoa kenh " << id << " (" << ch->getName() << ")? (y/n): ";
    string confirm;
    getline(cin, confirm);
    confirm = Utils::trim(confirm);

    if (confirm == "y" || confirm == "Y")
    {
        try
        {
            if (channelRepository.remove(id))
            {
                cout << "Xoa kenh tiep nhan " << id << " thanh cong!" << endl;
            }
            else
            {
                cout << "Xoa that bai!" << endl;
            }
        }
        catch (const exception& e)
        {
            cout << "Loi khi xoa kenh tiep nhan: " << e.what() << endl;
        }
    }
    else
    {
        cout << "Da huy thao tac xoa." << endl;
    }
    Utils::backChoice();
}
