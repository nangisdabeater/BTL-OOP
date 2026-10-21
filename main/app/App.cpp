#include "App.h"
#include "ServiceGroupMenu.h"
#include "TicketMenu.h"
#include "AgentMenu.h"
#include "ReceptionChannelMenu.h"
#include "InboundCallMenu.h"
#include "SLALevelMenu.h"
#include "TransferTicketMenu.h"
#include "CustomerMenu.h"
#include "FeedbackMenu.h"
#include "AgentShiftMenu.h"
#include <iostream>
#include <fstream>
#include <sstream>
#include <iomanip>

using namespace std;

// TRIỂN KHAI LỚP APPLICATION

/**
 *Hàm khởi tạo Application
 * Gán file dữ liệu riêng trong thư mục 'data/' cho mỗi Repository và tự động nạp dữ liệu cũ (nếu có).
 * - ServiceManager / ServiceGroup: Tiền tố SM (SMxxx)
 * - Ticket: Tiền tố TK (TKxxx)
 * - Agent: Tiền tố AG (AGxxx)
 * - ReceptionChannel: Tiền tố CN (CNxxx)
 * - InboundCall: Tiền tố CA (CAxxx)
 * - SLALevel: Tiền tố SL (SLxxx)
 * - TransferTicket: Tiền tố TF (TFxxx)
 * - Customer: Tiền tố KH (KHxxx)
 * - Feedback: Tiền tố DG (DGxxx)
 */
Application::Application()
    : serviceGroupRepository("SM", "data/service_groups.txt"),
      ticketRepository("TK", "data/tickets.txt"),
      agentRepository("AG", "data/agents.txt"),
      channelRepository("CN", "data/channels.txt"),
      inboundCallRepository("CA", "data/inbound_calls.txt"),
      slaRepository("SL", "data/sla_levels.txt"),
      transferRepository("TF", "data/transfer_tickets.txt"),
      customerRepository("KH", "data/customers.txt"),
      feedbackRepository("DG", "data/feedback.txt"),
      agentShiftRepository("AS", "data/agent_shifts.txt"){
    
    serviceGroupRepository.load();
    ticketRepository.load();
    agentRepository.load();
    channelRepository.load();
    inboundCallRepository.load();
    slaRepository.load();
    transferRepository.load();
    customerRepository.load();
    feedbackRepository.load();
    agentShiftRepository.load();
}

//Vòng lặp chính của chương trình

void Application::run(){
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
        case 4:
            receptionChannelMenu();
            break;
        case 5:
            inboundCallMenu();
            break;
        case 6:
            slaLevelMenu();
            break;
        case 7:
            transferTicketMenu();
            break;
        case 8:
            customerMenu();
            break;
        case 9:
            feedbackMenu();
            break;
        case 10:
            agentShiftMenu();
            break;
        case 0:
            running = false;
            cout << "Dang luu du lieu va thoat chuong trinh..." << endl;
            try { serviceGroupRepository.save(); }
            catch (const exception& e) { cout << "Loi khi luu dich vu: " << e.what() << endl; }

            try { ticketRepository.save(); }
            catch (const exception& e) { cout << "Loi khi luu ticket: " << e.what() << endl; }

            try { agentRepository.save(); }
            catch (const exception& e) { cout << "Loi khi luu dien thoai vien: " << e.what() << endl; }

            try { channelRepository.save(); }
            catch (const exception& e) { cout << "Loi khi luu kenh tiep nhan: " << e.what() << endl; }

            try { inboundCallRepository.save(); }
            catch (const exception& e) { cout << "Loi khi luu cuoc goi: " << e.what() << endl; }

            try { slaRepository.save(); }
            catch (const exception& e) { cout << "Loi khi luu muc SLA: " << e.what() << endl; }

            try { transferRepository.save(); }
            catch (const exception& e) { cout << "Loi khi luu phieu chuyen: " << e.what() << endl; }

            try { customerRepository.save(); }
            catch (const exception& e) { cout << "Loi khi luu khach hang: " << e.what() << endl; }

            try { feedbackRepository.save(); }
            catch (const exception& e) { cout << "Loi khi luu danh gia: " << e.what() << endl; }

            try { agentShiftRepository.save(); }
            catch (const exception& e) { cout << "Loi khi luu ca truc: " << e.what() << endl; }
            break;

        default:
            cout << "Lua chon khong hop le!" << endl;
        }

        cout << endl;
    }
}

//Hiển thị giao diện Menu chính

void Application::showMainMenu(){
    cout << "==============================" << endl;
    cout << "   QUAN LY TRUNG TAM CSKH" << endl;
    cout << "==============================" << endl;

    cout << "1. Quan ly dich vu (Service Manager)" << endl;
    cout << "2. Quan ly ticket" << endl;
    cout << "3. Quan ly dien thoai vien" << endl;
    cout << "4. Quan ly kenh tiep nhan" << endl;
    cout << "5. Quan ly cuoc goi tiep nhan" << endl;
    cout << "6. Quan ly muc cam ket SLA" << endl;
    cout << "7. Quan ly phieu chuyen xu ly (Transfer Ticket)" << endl;
    cout << "8. Quan ly khach hang" << endl;
    cout << "9. Quan ly danh gia hai long (Feedback)" << endl;
    cout << "10. Quan ly ca truc" << endl;
    cout << "0. Thoat" << endl;

    cout << "==============================" << endl;
}


//Điều hướng sang giao diện quản lý Dịch vụ (ServiceGroupMenu)

void Application::serviceGroupMenu(){
    ServiceGroupMenu menu(serviceGroupRepository, ticketRepository);
    menu.run();
}


//Điều hướng sang giao diện quản lý Ticket (TicketMenu)

void Application::ticketMenu(){
    TicketMenu menu(ticketRepository, serviceGroupRepository);
    menu.run();
}

//Điều hướng sang giao diện quản lý Điện thoại viên (AgentMenu)

void Application::agentMenu() {
    AgentMenu menu(agentRepository, agentShiftRepository);
    menu.run();
}

//Điều hướng sang giao diện quản lý Kênh tiếp nhận (ReceptionChannelMenu)

void Application::receptionChannelMenu(){
    ReceptionChannelMenu menu(channelRepository, inboundCallRepository);
    menu.run();
}

//Điều hướng sang giao diện quản lý Cuộc gọi tiếp nhận (InboundCallMenu)

void Application::inboundCallMenu(){
    InboundCallMenu menu(inboundCallRepository, channelRepository);
    menu.run();
}

//Điều hướng sang giao diện quản lý Mức SLA (SLALevelMenu)
 
void Application::slaLevelMenu(){
    SLALevelMenu menu(slaRepository, transferRepository);
    menu.run();
}

//Điều hướng sang giao diện quản lý Phiếu chuyển xử lý (TransferTicketMenu)

void Application::transferTicketMenu(){
    TransferTicketMenu menu(transferRepository, slaRepository);
    menu.run();
}

//Điều hướng sang giao diện quản lý Khách hàng (CustomerMenu)

void Application::customerMenu(){
    CustomerMenu menu(customerRepository, feedbackRepository);
    menu.run();
}

//Điều hướng sang giao diện quản lý Đánh giá hài lòng (FeedbackMenu)

void Application::feedbackMenu(){
    FeedbackMenu menu(feedbackRepository, customerRepository);
    menu.run();
}

//Điều hướng sang giao diện quản lý Đánh giá hài lòng (FeedbackMenu)

void Application::agentShiftMenu(){
    AgentShiftMenu menu(agentShiftRepository, agentRepository);
    menu.run();
}

