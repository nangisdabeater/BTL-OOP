#include "App.h"
#include "ServiceGroupMenu.h"
#include "TicketMenu.h"
#include <iostream>
#include <fstream>
#include <sstream>
#include <filesystem>
#include <iomanip>

using namespace std;

// Hàm hỗ trợ cắt khoảng trắng hai đầu của trường dữ liệu khi đọc file
static string trimToken(const string& s)
{
    size_t start = s.find_first_not_of(" \t\r\n");
    if (start == string::npos) return "";
    size_t end = s.find_last_not_of(" \t\r\n");
    return s.substr(start, end - start + 1);
}

// ============================================================================
// CHUYÊN BIỆT HÓA FILEMANAGER CHO SERVICEGROUP (LƯU TRỮ TRÊN FILE RIÊNG)
// ============================================================================

/**
 * @brief Nạp danh sách Nhóm dịch vụ từ file văn bản có kẻ hàng cột
 * @param filePath Đường dẫn file (data/service_groups.txt)
 * @return Danh sách vector các đối tượng ServiceGroup đã đọc được
 */
vector<ServiceGroup> FileManager<ServiceGroup>::load(const string& filePath)
{
    vector<ServiceGroup> result;
    ifstream inFile(filePath);
    if (!inFile.is_open())
    {
        return result; // Nếu file chưa tồn tại thì trả về danh sách rỗng
    }

    string line;
    while (getline(inFile, line))
    {
        if (line.empty()) continue;
        size_t firstChar = line.find_first_not_of(" \t\r\n");
        // Bỏ qua dòng trống hoặc dòng kẻ ngang phân cách (bắt đầu bằng '-')
        if (firstChar == string::npos || line[firstChar] == '-') continue;

        stringstream ss(line);
        string id, name, desc, activeStr;
        // Đọc các trường dữ liệu ngăn cách bằng ký tự '|'
        if (getline(ss, id, '|') &&
            getline(ss, name, '|') &&
            getline(ss, desc, '|') &&
            getline(ss, activeStr, '|'))
        {
            id = trimToken(id);
            name = trimToken(name);
            desc = trimToken(desc);
            activeStr = trimToken(activeStr);

            // Bỏ qua dòng tiêu đề cột
            if (id == "Ma ID" || id.length() != 5 || id.substr(0, 2) != "NP")
            {
                continue;
            }

            ServiceGroup sg(name, desc);
            sg.setId(id);
            if (activeStr == "0")
            {
                sg.deactivate();
            }
            else
            {
                sg.activate();
            }
            result.push_back(sg);
        }
    }
    return result;
}

/**
 * @brief Ghi toàn bộ danh sách Nhóm dịch vụ ra file văn bản kẻ hàng cột ngay ngắn
 * @param filePath Đường dẫn file (data/service_groups.txt)
 * @param data Dữ liệu cần ghi
 */
void FileManager<ServiceGroup>::save(const string& filePath, const vector<ServiceGroup>& data)
{
    // Đảm bảo thư mục cha tồn tại trước khi tạo file
    filesystem::path p(filePath);
    if (p.has_parent_path())
    {
        filesystem::create_directories(p.parent_path());
    }

    ofstream outFile(filePath);
    if (!outFile.is_open())
    {
        return;
    }

    // Kẻ bảng: Dòng tiêu đề cột và dòng gạch ngang phân cách
    outFile << left
            << setw(8)  << "Ma ID"            << " | "
            << setw(25) << "Ten nhom dich vu" << " | "
            << setw(45) << "Mo ta"            << " | "
            << "Trang thai" << "\n";
    outFile << string(95, '-') << "\n";

    for (const auto& sg : data)
    {
        outFile << left
                << setw(8)  << sg.getId()          << " | "
                << setw(25) << sg.getName()        << " | "
                << setw(45) << sg.getDescription() << " | "
                << (sg.isActive() ? 1 : 0)         << "\n";
    }
}

// ============================================================================
// CHUYÊN BIỆT HÓA FILEMANAGER CHO TICKET (LƯU TRỮ TRÊN FILE RIÊNG)
// ============================================================================

/**
 * @brief Nạp danh sách Ticket từ file văn bản có kẻ hàng cột
 * @param filePath Đường dẫn file (data/tickets.txt)
 * @return Danh sách vector các đối tượng Ticket đã đọc được
 */
vector<Ticket> FileManager<Ticket>::load(const string& filePath)
{
    vector<Ticket> result;
    ifstream inFile(filePath);
    if (!inFile.is_open())
    {
        return result; // Nếu file chưa tồn tại thì trả về danh sách rỗng
    }

    string line;
    while (getline(inFile, line))
    {
        if (line.empty()) continue;
        size_t firstChar = line.find_first_not_of(" \t\r\n");
        // Bỏ qua dòng trống hoặc dòng kẻ ngang phân cách
        if (firstChar == string::npos || line[firstChar] == '-') continue;

        stringstream ss(line);
        string id, title, desc, customerId, sgId, statusStr;
        // Đọc các trường dữ liệu ngăn cách bằng ký tự '|'
        if (getline(ss, id, '|') &&
            getline(ss, title, '|') &&
            getline(ss, desc, '|') &&
            getline(ss, customerId, '|') &&
            getline(ss, sgId, '|') &&
            getline(ss, statusStr, '|'))
        {
            id = trimToken(id);
            title = trimToken(title);
            desc = trimToken(desc);
            customerId = trimToken(customerId);
            sgId = trimToken(sgId);
            statusStr = trimToken(statusStr);

            // Bỏ qua dòng tiêu đề cột
            if (id == "Ma ID" || id.length() != 5 || id.substr(0, 2) != "NP")
            {
                continue;
            }

            Ticket t(title, desc, customerId, sgId);
            t.setId(id);
            try
            {
                int st = stoi(statusStr);
                if (st >= 0 && st <= 3)
                {
                    t.changeStatus(static_cast<TicketStatus>(st));
                }
            }
            catch (...)
            {
                t.changeStatus(TicketStatus::OPEN);
            }
            result.push_back(t);
        }
    }
    return result;
}

/**
 * @brief Ghi toàn bộ danh sách Ticket ra file văn bản kẻ hàng cột ngay ngắn
 * @param filePath Đường dẫn file (data/tickets.txt)
 * @param data Dữ liệu cần ghi
 */
void FileManager<Ticket>::save(const string& filePath, const vector<Ticket>& data)
{
    // Đảm bảo thư mục cha tồn tại trước khi tạo file
    filesystem::path p(filePath);
    if (p.has_parent_path())
    {
        filesystem::create_directories(p.parent_path());
    }

    ofstream outFile(filePath);
    if (!outFile.is_open())
    {
        return;
    }

    // Kẻ bảng: Dòng tiêu đề cột và dòng gạch ngang phân cách
    outFile << left
            << setw(8)  << "Ma ID"          << " | "
            << setw(25) << "Tieu de"        << " | "
            << setw(40) << "Mo ta chi tiet" << " | "
            << setw(10) << "Ma KH"          << " | "
            << setw(10) << "Ma Nhom DV"     << " | "
            << "Trang thai" << "\n";
    outFile << string(110, '-') << "\n";

    for (const auto& t : data)
    {
        outFile << left
                << setw(8)  << t.getId()             << " | "
                << setw(25) << t.getTitle()          << " | "
                << setw(40) << t.getDescription()    << " | "
                << setw(10) << t.getCustomerId()     << " | "
                << setw(10) << t.getServiceGroupId() << " | "
                << static_cast<int>(t.getStatus())   << "\n";
    }
}

// ============================================================================
// TRIỂN KHAI LỚP APPLICATION
// ============================================================================

/**
 * @brief Hàm khởi tạo Application
 * Gán file dữ liệu riêng trong thư mục 'data/' cho mỗi Repository và tự động nạp dữ liệu cũ (nếu có).
 */
Application::Application()
    : serviceGroupRepository("data/service_groups.txt"),
      ticketRepository("data/tickets.txt")
{
    serviceGroupRepository.load();
    ticketRepository.load();
}

/**
 * @brief Vòng lặp chính của chương trình
 */
void Application::run()
{
    bool running = true;

    while (running)
    {
        showMainMenu();

        int choice;

        cout << "Nhap lua chon: ";
        if (!(cin >> choice))
        {
            cin.clear();
            cin.ignore(10000, '\n');
            cout << "Lua chon khong hop le!" << endl << endl;
            continue;
        }
        cin.ignore(10000, '\n'); // Xóa ký tự xuống dòng

        switch (choice)
        {
        case 1:
            serviceGroupMenu();
            break;

        case 2:
            ticketMenu();
            break;

        case 0:
            running = false;
            cout << "Dang luu du lieu va thoat chuong trinh..." << endl;
            serviceGroupRepository.save();
            ticketRepository.save();
            break;

        default:
            cout << "Lua chon khong hop le!" << endl;
        }

        cout << endl;
    }
}

/**
 * @brief Hiển thị giao diện Menu chính
 */
void Application::showMainMenu()
{
    cout << "==============================" << endl;
    cout << "   QUAN LY TRUNG TAM CSKH" << endl;
    cout << "==============================" << endl;

    cout << "1. Quan ly nhom dich vu" << endl;
    cout << "2. Quan ly ticket" << endl;
    cout << "0. Thoat" << endl;

    cout << "==============================" << endl;
}

/**
 * @brief Điều hướng sang giao diện quản lý Nhóm dịch vụ (ServiceGroupMenu)
 */
void Application::serviceGroupMenu()
{
    ServiceGroupMenu menu(serviceGroupRepository, ticketRepository);
    menu.run();
}

/**
 * @brief Điều hướng sang giao diện quản lý Ticket (TicketMenu)
 */
void Application::ticketMenu()
{
    TicketMenu menu(ticketRepository, serviceGroupRepository);
    menu.run();
}
