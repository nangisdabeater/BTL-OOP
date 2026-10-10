#include "App.h"
#include "ServiceGroupMenu.h"
#include "TicketMenu.h"
#include "AgentMenu.h"
#include "SLALevelMenu.h"        //Added
#include "TransferTicketMenu.h"
#include <iostream>
#include <fstream>
#include <sstream>
#include <filesystem>
#include <iomanip>

using namespace std;

// ============================================================================
// TRIỂN KHAI LỚP APPLICATION
// ============================================================================

/**
 * @brief Hàm khởi tạo Application
 * Gán file dữ liệu riêng trong thư mục 'data/' cho mỗi Repository và tự động nạp dữ liệu cũ (nếu có).
 */
Application::Application()
        : serviceGroupRepository("SG", "data/service_groups.txt"),
            ticketRepository("NP", "data/tickets.txt"),
            agentRepository("AG", "data/agents.txt"),

        slaLevelRepository("SL", "data/sla_levels.txt"),                  //Added
        transferTicketRepository("TF", "data/transfer_tickets.txt")
{
    serviceGroupRepository.load();
    ticketRepository.load();
    agentRepository.load();
        
    slaLevelRepository.load();         //Added
    transferTicketRepository.load();
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
        case 3: 
            agentMenu();
            break;

        case 4:                   //Added
            slaLevelMenu();
            break;
        case 5:
            transferTicketMenu();
            break;

                
        case 0:
            running = false;
            cout << "Dang luu du lieu va thoat chuong trinh..." << endl;
            try
            {
                serviceGroupRepository.save();
            }
            catch (const exception& e)
            {
                cout << "Loi khi luu nhom dich vu: " << e.what() << endl;
            }
            try
            {
                ticketRepository.save();
            }
            catch (const exception& e)
            {
                cout << "Loi khi luu ticket: " << e.what() << endl;
            }
            break;
            try
            {
                agentRepository.save();
            }
            catch(const std::exception& e)
            {
                cout << "Loi khi luu dien thoai vien: " << e.what() << endl;
            }
            
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
    cout << "3. Quan ly dien thoai vien" << endl;

    cout << "4. Quan ly muc SLA" << endl;                   //Added
    cout << "5. Quan ly phieu chuyen xu ly" << endl;
        
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

/**
 * @brief Điều hướng sang giao diện quản lý Điện thoại viên (AgentMenu)
 */
void Application::agentMenu() {
    AgentMenu menu(agentRepository);
    menu.run();
}
                                           //Added
void Application::slaLevelMenu() {
    SLALevelMenu menu(slaLevelRepository);
    menu.run();
}

void Application::transferTicketMenu() {
    TransferTicketMenu menu(transferTicketRepository, ticketRepository, agentRepository);
    menu.run();
}
