/**
 * @file InboundCallMenu.cpp
 * @brief Triển khai các chức năng điều khiển Menu cho Cuộc gọi tiếp nhận (InboundCall)
 * [PHẦN THÊM MỚI]
 */

#include "InboundCallMenu.h"
#include <iostream>
#include <iomanip>
#include <Utilities.h>

using namespace std;

/**
 * @brief Khởi tạo đối tượng InboundCallMenu
 * @param cRepo Kho lưu trữ Cuộc gọi tiếp nhận
 * @param chRepo Kho lưu trữ Kênh tiếp nhận (khóa ngoại)
 * @param agRepo Kho lưu trữ Điện thoại viên (khóa ngoại)
 */
InboundCallMenu::InboundCallMenu(
    Repository<InboundCall>& cRepo,
    Repository<ReceptionChannel>& chRepo,
    Repository<Agent>& agRepo
)
    : callRepository(cRepo), channelRepository(chRepo), agentRepository(agRepo)
{
}

/**
 * @brief Hiển thị bảng danh mục chức năng quản lý Cuộc gọi tiếp nhận
 */
void InboundCallMenu::showMenu()
{
    cout << "========================================" << endl;
    cout << "      QUAN LY CUOC GOI TIEP NHAN" << endl;
    cout << "========================================" << endl;
    cout << "1. Hien thi danh sach cuoc goi tiep nhan" << endl;
    cout << "2. Tim kiem cuoc goi theo ID" << endl;
    cout << "3. Ghi nhan cuoc goi den moi" << endl;
    cout << "4. Cap nhat thong tin cuoc goi" << endl;
    cout << "5. Loc cuoc goi theo Kenh tiep nhan" << endl;
    cout << "6. Xoa thong tin cuoc goi" << endl;
    cout << "0. Quay lai menu chinh" << endl;
    cout << "========================================" << endl;
}

/**
 * @brief Điều phối vòng lặp xử lý các lựa chọn trong menu Cuộc gọi tiếp nhận
 */
void InboundCallMenu::run()
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
            filterByChannel();
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
 * @brief Hiển thị toàn bộ danh sách cuộc gọi tiếp nhận (Read All)
 */
void InboundCallMenu::displayAll()
{
    const auto& list = callRepository.getAll();
    if (list.empty())
    {
        cout << "Danh sach cuoc goi tiep nhan dang trong!" << endl;
        Utils::backChoice();
        return;
    }

    cout << left
         << setw(8)  << "Ma ID"
         << setw(14) << "So dien thoai"
         << setw(10) << "Ma kenh"
         << setw(10) << "Ma DTV"
         << setw(18) << "Thoi gian"
         << setw(12) << "T.Luong(s)"
         << setw(12) << "Trang thai"
         << "Ghi chu" << endl;
    cout << string(95, '-') << endl;

    for (const auto& call : list)
    {
        cout << left
             << setw(8)  << call.getId()
             << setw(14) << call.getPhoneNumber()
             << setw(10) << call.getChannelId()
             << setw(10) << (call.getAgentId().empty() ? "-" : call.getAgentId())
             << setw(18) << call.getCallTime()
             << setw(12) << call.getDuration()
             << setw(12) << call.getStatus()
             << call.getNote() << endl;
    }
    Utils::backChoice();
}

/**
 * @brief Tìm kiếm cuộc gọi theo mã ID (CAxxx)
 */
void InboundCallMenu::findById()
{
    string id = Utils::inputNonEmptyString("Nhap ma cuoc goi can tim (vi du CA001, nhap 0 de quay lai): ");
    if (id == "0")
    {
        return;
    }
    InboundCall* call = callRepository.findById(id);

    if (call == nullptr)
    {
        cout << "Khong tim thay cuoc goi voi ma: " << id << endl;
        Utils::backChoice();
        return;
    }

    cout << "--- Thong tin chi tiet cuoc goi ---" << endl;
    cout << "Ma ID         : " << call->getId() << endl;
    cout << "So dien thoai : " << call->getPhoneNumber() << endl;
    cout << "Ma kenh       : " << call->getChannelId();

    // Hiển thị tên kênh tương ứng nếu tìm thấy
    ReceptionChannel* ch = channelRepository.findById(call->getChannelId());
    if (ch != nullptr)
    {
        cout << " (" << ch->getName() << ")";
    }
    cout << endl;

    cout << "DTV tiep nhan : " << (call->getAgentId().empty() ? "(Chua gan)" : call->getAgentId());
    Agent* ag = agentRepository.findById(call->getAgentId());
    if (ag != nullptr)
    {
        cout << " (" << ag->getFullName() << ")";
    }
    cout << endl;

    cout << "Thoi gian     : " << call->getCallTime() << endl;
    cout << "Thoi luong    : " << call->getDuration() << " giay" << endl;
    cout << "Trang thai    : " << call->getStatus() << endl;
    cout << "Ghi chu       : " << call->getNote() << endl;
    Utils::backChoice();
}

/**
 * @brief Ghi nhận cuộc gọi đến mới (Create)
 * Kiểm tra khóa ngoại kênh tiếp nhận (channelId) và điện thoại viên (agentId).
 * Tự động sinh ID định dạng CA001, CA002,...
 */
void InboundCallMenu::add()
{
    cout << "--- Ghi nhan cuoc goi tiep nhan moi ---" << endl;

    // Kiểm tra xem đã có kênh tiếp nhận nào tồn tại chưa
    if (channelRepository.getAll().empty())
    {
        cout << "Loi: Chua co kenh tiep nhan nao trong he thong! Vui long them kenh tiep nhan truoc." << endl;
        Utils::backChoice();
        return;
    }

    string phone = Utils::inputNonEmptyString("Nhap so dien thoai nguoi goi (nhap 0 de quay lai): ");
    if (phone == "0")
    {
        return;
    }

    // Nhập và kiểm tra mã kênh tiếp nhận
    string chId;
    while (true)
    {
        chId = Utils::inputNonEmptyString("Nhap ma kenh tiếp nhận (VD: CN001, nhap 0 de quay lai): ");
        if (chId == "0")
        {
            return;
        }
        ReceptionChannel* ch = channelRepository.findById(chId);
        if (ch == nullptr)
        {
            cout << "Loi: Khong ton tai kenh tiep nhan voi ma " << chId << "! Vui long nhap lai." << endl;
        }
        else if (!ch->isActive())
        {
            cout << "Canh bao: Kenh " << chId << " dang tam dung hoat dong. Ban van muon chon? (y/n): ";
            string cf;
            getline(cin, cf);
            if (Utils::trim(cf) == "y" || Utils::trim(cf) == "Y") break;
        }
        else
        {
            break;
        }
    }

    // Nhập mã điện thoại viên tiếp nhận (tùy chọn, có thể để trống)
    string agId;
    while (true)
    {
        cout << "Nhap ma dien thoai vien tiep nhan (VD: AG001, de trong neu chua gan): ";
        getline(cin, agId);
        agId = Utils::trim(agId);
        if (agId.empty())
        {
            break; // Cho phép để trống
        }
        Agent* ag = agentRepository.findById(agId);
        if (ag == nullptr)
        {
            cout << "Loi rang buoc: Ma DTV '" << agId << "' khong ton tai tren he thong! Vui long nhap lai." << endl;
        }
        else if (!ag->isActive())
        {
            cout << "Canh bao: DTV '" << ag->getFullName() << "' dang tam dung hoat dong. Ban van muon gan? (y/n): ";
            string cf;
            getline(cin, cf);
            if (Utils::trim(cf) == "y" || Utils::trim(cf) == "Y") break;
        }
        else
        {
            break;
        }
    }

    string callTime = Utils::inputNonEmptyString("Nhap thoi gian cuoc goi (VD: 2026-10-11 14:30): ");

    int duration = 0;
    cout << "Nhap thoi luong cuoc goi (tinh theo giay): ";
    if (!(cin >> duration) || duration < 0)
    {
        cin.clear();
        cin.ignore(10000, '\n');
        duration = 0;
        cout << "Thoi luong khong hop le, mac dinh dat la 0 giay." << endl;
    }
    else
    {
        cin.ignore(10000, '\n');
    }

    cout << "Chon trang thai cuoc goi:" << endl;
    cout << "  1. ANSWERED (Da nghe may / Tiep nhan thanh cong)" << endl;
    cout << "  2. MISSED (Cuoc goi nho)" << endl;
    cout << "  3. BUSY (May ban / Khach cup ngang)" << endl;
    cout << "Nhap lua chon (mac dinh 1): ";
    string stChoice;
    getline(cin, stChoice);
    stChoice = Utils::trim(stChoice);

    string status = "ANSWERED";
    if (stChoice == "2") status = "MISSED";
    else if (stChoice == "3") status = "BUSY";

    cout << "Nhap ghi chu / noi dung tom tat: ";
    string note;
    getline(cin, note);
    note = Utils::trim(note);

    InboundCall newCall(phone, chId, agId, callTime, duration, status, note);
    try
    {
        // Repository sẽ tự sinh mã ID với tiền tố "CA", VD: CA001
        callRepository.add(newCall);
        const auto& list = callRepository.getAll();
        cout << "Ghi nhan cuoc goi thanh cong! Ma ID duoc cap: " << list.back().getId() << endl;
    }
    catch (const exception& e)
    {
        cout << "Loi khi them cuoc goi tiep nhan: " << e.what() << endl;
    }
    Utils::backChoice();
}

/**
 * @brief Cập nhật thông tin cuộc gọi (Update)
 */
void InboundCallMenu::update()
{
    cout << "--- Cap nhat thong tin cuoc goi ---" << endl;
    string id = Utils::inputNonEmptyString("Nhap ma cuoc goi can cap nhat (vi du CA001, nhap 0 de quay lai): ");
    if (id == "0")
    {
        return;
    }

    InboundCall* call = callRepository.findById(id);
    if (call == nullptr)
    {
        cout << "Khong tim thay cuoc goi voi ma: " << id << endl;
        Utils::backChoice();
        return;
    }

    InboundCall updated = *call;

    cout << "Thong tin hien tai:" << endl;
    updated.printInfo();

    // Cập nhật DTV
    cout << "\nNhap ma DTV moi (de trong neu giu nguyen '" << updated.getAgentId() << "'): ";
    string newAg;
    getline(cin, newAg);
    newAg = Utils::trim(newAg);
    if (!newAg.empty())
    {
        Agent* ag = agentRepository.findById(newAg);
        if (ag == nullptr)
        {
            cout << "Canh bao: Ma DTV " << newAg << " khong tim thay trong he thong." << endl;
        }
        updated.setAgentId(newAg);
    }

    // Cập nhật trạng thái
    cout << "Chon trang thai moi (1: ANSWERED, 2: MISSED, 3: BUSY, de trong de giu nguyen): ";
    string stInput;
    getline(cin, stInput);
    stInput = Utils::trim(stInput);
    if (stInput == "1") updated.setStatus("ANSWERED");
    else if (stInput == "2") updated.setStatus("MISSED");
    else if (stInput == "3") updated.setStatus("BUSY");

    // Cập nhật ghi chú
    cout << "Nhap ghi chu moi (de trong de giu nguyen): ";
    string newNote;
    getline(cin, newNote);
    newNote = Utils::trim(newNote);
    if (!newNote.empty())
    {
        updated.setNote(newNote);
    }

    try
    {
        if (callRepository.update(updated))
        {
            cout << "Cap nhat cuoc goi thanh cong!" << endl;
        }
        else
        {
            cout << "Loi khi cap nhat cuoc goi!" << endl;
        }
    }
    catch (const exception& e)
    {
        cout << "Loi khi cap nhat cuoc goi: " << e.what() << endl;
    }
    Utils::backChoice();
}

/**
 * @brief Lọc các cuộc gọi tiếp nhận theo mã kênh (CNxxx)
 */
void InboundCallMenu::filterByChannel()
{
    string chId = Utils::inputNonEmptyString("Nhap ma kenh tiep nhan can loc (VD: CN001, nhap 0 de quay lai): ");
    if (chId == "0")
    {
        return;
    }

    int count = 0;
    cout << "\n--- Danh sach cuoc goi thuoc kenh: " << chId << " ---" << endl;
    for (const auto& call : callRepository.getAll())
    {
        if (call.getChannelId() == chId)
        {
            call.printInfo();
            count++;
        }
    }

    if (count == 0)
    {
        cout << "Khong co cuoc goi nao thuoc kenh " << chId << "!" << endl;
    }
    else
    {
        cout << "Tong so cuoc goi: " << count << endl;
    }
    Utils::backChoice();
}

/**
 * @brief Xóa cuộc gọi tiếp nhận (Delete)
 */
void InboundCallMenu::remove()
{
    cout << "--- Xoa thong tin cuoc goi ---" << endl;
    string id = Utils::inputNonEmptyString("Nhap ma cuoc goi can xoa (vi du CA001, nhap 0 de quay lai): ");
    if (id == "0")
    {
        return;
    }

    InboundCall* call = callRepository.findById(id);
    if (call == nullptr)
    {
        cout << "Khong tim thay cuoc goi voi ma: " << id << endl;
        Utils::backChoice();
        return;
    }

    cout << "Ban co chac muon xoa cuoc goi " << id << "? (y/n): ";
    string confirm;
    getline(cin, confirm);
    if (Utils::trim(confirm) == "y" || Utils::trim(confirm) == "Y")
    {
        try
        {
            if (callRepository.remove(id))
            {
                cout << "Xoa cuoc goi " << id << " thanh cong!" << endl;
            }
            else
            {
                cout << "Xoa that bai!" << endl;
            }
        }
        catch (const exception& e)
        {
            cout << "Loi khi xoa cuoc goi: " << e.what() << endl;
        }
    }
    else
    {
        cout << "Da huy thao tac xoa." << endl;
    }
    Utils::backChoice();
}
