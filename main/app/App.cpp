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

// Dùng Danh sách khởi tạo (Member Initializer List) để truyền tiền tố ID và đường dẫn file cho từng Repository
// vì lớp Repository không có hàm khởi tạo mặc định không tham số.
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
      agentShiftRepository("AS", "data/agent_shifts.txt") {
    
    // Nạp toàn bộ dữ liệu từ các file trong thư mục data/ lên bộ nhớ RAM ngay khi mở chương trình
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

void Application::run() {
    bool running = true;

    while (running) {
        showMainMenu();

        int choice;
        cout << "Nhap lua chon: ";
        // Nếu người dùng nhập chữ thay vì số nguyên thì xóa cờ lỗi và dọn sạch bộ đệm bàn phím
        if (!(cin >> choice)) {
            cin.clear();
            cin.ignore(10000, '\n');
            cout << "Lua chon khong hop le!" << endl << endl;
            continue;
        }
        cin.ignore(10000, '\n'); // Xóa ký tự xuống dòng (\n) còn sót lại trong bộ đệm

        switch (choice) {
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
            // Tại sao bọc từng lệnh save() trong try-catch riêng thay vì gộp chung vào 1 khối try-catch lớn?
            // Vì nếu gộp chung, giả sử file thứ nhất bị lỗi thì 9 file phía sau sẽ bị bỏ qua không được lưu.
            // Tách riêng từng khối giúp đảm bảo dù 1 file lỗi thì các file còn lại vẫn được lưu an toàn.
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

void Application::showMainMenu() {
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

// Mỗi cặp chức năng của từng thành viên chỉ truyền đúng 2 Repository trong phân hệ đó,
// đảm bảo các usercase của 5 người hoạt động độc lập, không bị phụ thuộc chéo vào nhau.
void Application::serviceGroupMenu() {
    ServiceGroupMenu menu(serviceGroupRepository, ticketRepository);
    menu.run();
}

void Application::ticketMenu() {
    TicketMenu menu(ticketRepository, serviceGroupRepository);
    menu.run();
}

void Application::agentMenu() {
    AgentMenu menu(agentRepository, agentShiftRepository);
    menu.run();
}

void Application::receptionChannelMenu() {
    ReceptionChannelMenu menu(channelRepository, inboundCallRepository);
    menu.run();
}

void Application::inboundCallMenu() {
    InboundCallMenu menu(inboundCallRepository, channelRepository);
    menu.run();
}

void Application::slaLevelMenu() {
    SLALevelMenu menu(slaRepository, transferRepository);
    menu.run();
}

void Application::transferTicketMenu() {
    TransferTicketMenu menu(transferRepository, slaRepository);
    menu.run();
}

void Application::customerMenu() {
    CustomerMenu menu(customerRepository, feedbackRepository);
    menu.run();
}

void Application::feedbackMenu() {
    FeedbackMenu menu(feedbackRepository, customerRepository);
    menu.run();
}

void Application::agentShiftMenu() {
    AgentShiftMenu menu(agentShiftRepository, agentRepository);
    menu.run();
}