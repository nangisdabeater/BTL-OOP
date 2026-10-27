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
         << setw(10) << "Ma NV"
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
             << setw(10) << call.getAgentId()
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

    Utils::printEntityDetails(*call);
    ReceptionChannel* ch = channelRepository.findById(call->getChannelId());
    if (ch != nullptr)
    {
        cout << "  -> Kenh tiep nhan: " << ch->getName()
             << " (" << (ch->isActive() ? "Hoat dong" : "Ngung hoat dong") << ")" << endl;
    }
    Agent* ag = agentRepository.findById(call->getAgentId());
    if (ag != nullptr)
    {
        cout << "  -> Nhan vien tiep nhan: " << ag->getFullName()
             << " (SDT: " << ag->getPhoneNumber() << " | May le: " << ag->getExtensionPhone() << ")" << endl;
    }
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
    //kiem tra so dien thoai hop le
    string phone;
    while (true)
    {
        phone = Utils::inputNonEmptyString("Nhap so dien thoai nguoi goi (nhap 0 de quay lai): ");
        if (phone == "0")
        {
            return;
        }
        if (phone.length() != 10)
        {
            cout << "Loi : So dien thoai phai co dung 10 chu so!" << endl;
            continue;
        }
        if (phone[0] != '0')
        {
            cout << "Loi: So dien thoai phai bat dau bang so 0!" << endl;
            continue;
        }
        bool allDigits = true;
        for (char c : phone)
        {
            if (!isdigit(c))
            {
                allDigits = false;
                break;
            }
        }
        if (!allDigits)
        {
            cout << "Loi: So dien thoai chi duoc chua chu so (0-9)!" << endl;
            continue;
        }
        break; 
    }

    // Nhập và kiểm tra mã kênh tiếp nhận
    string chId;
    while (true)
    {
        chId = Utils::inputNonEmptyString("Nhap ma kenh tiep nhan (VD: CN001, nhap 0 de quay lai): ");
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

    // Nhập và kiểm tra mã nhân viên tiếp nhận (ràng buộc tiền tố AG và phải tồn tại)
    string agId;
    while (true)
    {
        agId = Utils::inputNonEmptyString("Nhap ma nhan vien tiep nhan (VD: AG001, nhap 0 de quay lai): ");
        if (agId == "0")
        {
            return;
        }
        if (agId.length() < 3 || agId.substr(0, 2) != "AG")
        {
            cout << "Loi: Ma nhan vien phai bat dau bang 'AG' (VD: AG001)! Vui long nhap lai." << endl;
            continue;
        }
        Agent* ag = agentRepository.findById(agId);
        if (ag == nullptr)
        {
            cout << "Loi: Khong ton tai nhan vien voi ma " << agId << " trong danh sach Agents! Vui long nhap lai." << endl;
            continue;
        }
        cout << "  -> Nhan vien tiep nhan: " << ag->getFullName() << " (" << ag->getPhoneNumber() << ")" << endl;
        break;
    }

    string callDate = Utils::inputDateString("Nhap ngay cuoc goi (DD/MM/YYYY): ");
    string callHour;
    while (true)
    {
        callHour = Utils::inputNonEmptyString("Nhap gio cuoc goi (HH:MM, VD: 14:30): ");
        if (callHour.length() == 5 && callHour[2] == ':' &&
            isdigit(callHour[0]) && isdigit(callHour[1]) &&
            isdigit(callHour[3]) && isdigit(callHour[4]))
        {
            int h = stoi(callHour.substr(0, 2));
            int m = stoi(callHour.substr(3, 2));
            if (h >= 0 && h <= 23 && m >= 0 && m <= 59)
            {
                break;
            }
        }
        cout << "Gio khong hop le! Vui long nhap dung dinh dang HH:MM (tu 00:00 den 23:59)." << endl;
    }
    string callTime = callDate + " " + callHour;

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
    cout << "  3. BUSY (May ban)" << endl;
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

    // cap nhat sdt
    while (true)
    {
        cout << "Nhap so dien thoai moi (de trong de giu nguyen - hien tai: "
             << updated.getPhoneNumber() << "): ";
        string newPhone;
        getline(cin, newPhone);
        newPhone = Utils::trim(newPhone);
        if (newPhone.empty() || newPhone == "0")
        {
            break; // Giữ nguyên
        }
        if (newPhone.length() != 10 || newPhone[0] != '0')
        {
            cout << "Loi: So dien thoai phai co dung 10 chu so va bat dau bang so 0!" << endl;
            continue;
        }
        bool allDigits = true;
        for (char c : newPhone)
        {
            if (!isdigit(static_cast<unsigned char>(c)))
            {
                allDigits = false;
                break;
            }
        }
        if (!allDigits)
        {
            cout << "Loi: So dien thoai chi duoc chua chu so (0-9)!" << endl;
            continue;
        }
        updated.setPhoneNumber(newPhone);
        cout << "  -> Da cap nhat so dien thoai: " << newPhone << endl;
        break;
    }

    // cap nhat ma kenh
    while (true)
    {
        cout << "Nhap ma kenh tiep nhan moi (VD: CN001, de trong de giu nguyen - hien tai: "
             << updated.getChannelId() << "): ";
        string newChId;
        getline(cin, newChId);
        newChId = Utils::trim(newChId);
        if (newChId.empty() || newChId == "0")
        {
            break; 
        }
        ReceptionChannel* ch = channelRepository.findById(newChId);
        if (ch == nullptr)
        {
            cout << "Loi: Khong ton tai kenh tiep nhan voi ma " << newChId << "! Vui long nhap lai." << endl;
            continue;
        }
        if (!ch->isActive())
        {
            cout << "Canh bao: Kenh " << newChId << " dang tam dung hoat dong. Ban van muon chon? (y/n): ";
            string cf;
            getline(cin, cf);
            if (Utils::trim(cf) != "y" && Utils::trim(cf) != "Y")
            {
                continue;
            }
        }
        updated.setChannelId(newChId);
        cout << "  -> Da cap nhat kenh: " << ch->getName() << endl;
        break;
    }

    //cap nhat nhan vien
    cout << "Nhap ma nhan vien tiep nhan moi (VD: AG001, de trong de giu nguyen - hien tai: "
         << updated.getAgentId() << "): ";
    string newAgId;
    getline(cin, newAgId);
    newAgId = Utils::trim(newAgId);
    if (!newAgId.empty())
    {
        while (true)
        {
            if (newAgId.length() < 3 || newAgId.substr(0, 2) != "AG")
            {
                cout << "Loi: Ma nhan vien phai bat dau bang 'AG' (VD: AG001)!" << endl;
            }
            else
            {
                Agent* ag = agentRepository.findById(newAgId);
                if (ag == nullptr)
                {
                    cout << "Loi: Khong ton tai nhan vien voi ma " << newAgId << "!" << endl;
                }
                else
                {
                    updated.setAgentId(newAgId);
                    cout << "  -> Da cap nhat nhan vien: " << ag->getFullName() << endl;
                    break;
                }
            }
            newAgId = Utils::inputNonEmptyString("Nhap lai ma nhan vien (hoac 0 de bo qua khong doi): ");
            if (newAgId == "0") break;
        }
    }

    //cap nhat thoi gian
    cout << "Ban co muon doi thoi gian cuoc goi khong? (y/n, de trong de giu nguyen - hien tai: "
         << updated.getCallTime() << "): ";
    string changeTimeChoice;
    getline(cin, changeTimeChoice);
    changeTimeChoice = Utils::trim(changeTimeChoice);
    if (changeTimeChoice == "y" || changeTimeChoice == "Y")
    {
        string newDate = Utils::inputDateString("Nhap ngay cuoc goi moi (DD/MM/YYYY): ");
        string newHour;
        while (true)
        {
            newHour = Utils::inputNonEmptyString("Nhap gio cuoc goi moi (HH:MM, VD: 14:30): ");
            if (newHour.length() == 5 && newHour[2] == ':' &&
                isdigit(static_cast<unsigned char>(newHour[0])) && isdigit(static_cast<unsigned char>(newHour[1])) &&
                isdigit(static_cast<unsigned char>(newHour[3])) && isdigit(static_cast<unsigned char>(newHour[4])))
            {
                int h = stoi(newHour.substr(0, 2));
                int m = stoi(newHour.substr(3, 2));
                if (h >= 0 && h <= 23 && m >= 0 && m <= 59)
                {
                    break;
                }
            }
            cout << "Gio khong hop le! Vui long nhap dung dinh dang HH:MM (tu 00:00 den 23:59)." << endl;
        }
        updated.setCallTime(newDate + " " + newHour);
        cout << "  -> Da cap nhat thoi gian: " << updated.getCallTime() << endl;
    }

    // cap nhat thoi luong
    while (true)
    {
        cout << "Nhap thoi luong moi (giay, de trong de giu nguyen - hien tai: "
             << updated.getDuration() << "s): ";
        string durInput;
        getline(cin, durInput);
        durInput = Utils::trim(durInput);
        if (durInput.empty())
        {
            break; 
        }
        try
        {
            int d = stoi(durInput);
            if (d < 0)
            {
                cout << "Loi: Thoi luong khong duoc am! Vui long nhap lai." << endl;
                continue;
            }
            updated.setDuration(d);
            cout << "  -> Da cap nhat thoi luong: " << d << "s" << endl;
            break;
        }
        catch (...)
        {
            cout << "Loi: Thoi luong phai la so nguyen hop le!" << endl;
        }
    }

    // cap nhat trang thai
    cout << "Chon trang thai moi (1: ANSWERED, 2: MISSED, 3: BUSY, de trong de giu nguyen - hien tai: "
         << updated.getStatus() << "): ";
    string stInput;
    getline(cin, stInput);
    stInput = Utils::trim(stInput);
    if (stInput == "1") updated.setStatus("ANSWERED");
    else if (stInput == "2") updated.setStatus("MISSED");
    else if (stInput == "3") updated.setStatus("BUSY");

    // cap nhat ghi chu
    cout << "Nhap ghi chu moi (de trong de giu nguyen - hien tai: \""
         << updated.getNote() << "\"): ";
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

    ReceptionChannel* ch = channelRepository.findById(chId);
    if (ch == nullptr)
    {
        cout << "Loi: Kenh tiep nhan voi ma '" << chId << "' khong ton tai!" << endl;
        Utils::backChoice();
        return;
    }

    int count = 0;
    cout << "\n--- Danh sach cuoc goi thuoc kenh: " << chId << " (" << ch->getName() << ") ---" << endl;
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

    cout << "Thong tin cuoc goi can xoa:" << endl;
    call->printInfo();
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
