#pragma once

#include <vector>
#include <string>
#include "../Entity/ServiceGroup.h"
#include "../Entity/Ticket.h"
#include "../Repository/Repository.h"
#include "../Entity/Agent.h"
#include "../Entity/ReceptionChannel.h"
#include "../Entity/InboundCall.h"
#include "../Entity/SLALevel.h"
#include "../Entity/TransferTicket.h"
#include "../Entity/Customer.h"
#include "../Entity/Feedback.h"
#include "../Entity/AgentShift.h"

class Application {
private:
    Repository<ServiceGroup> serviceGroupRepository; // Kho lưu trữ Nhóm dịch vụ (file: data/service_groups.txt, tiền tố SM)
    Repository<Ticket> ticketRepository;             // Kho lưu trữ Ticket (file: data/tickets.txt, tiền tố TK)
    Repository<Agent> agentRepository;               // Kho lưu trữ Điện thoại viên (file: data/agents.txt, tiền tố AG)
    Repository<ReceptionChannel> channelRepository;  // Kho lưu trữ Kênh tiếp nhận (file: data/channels.txt, tiền tố CN)
    Repository<InboundCall> inboundCallRepository;   // Kho lưu trữ Cuộc gọi tiếp nhận (file: data/inbound_calls.txt, tiền tố CA)
    Repository<SLALevel> slaRepository;              // Kho lưu trữ Mức cam kết SLA (file: data/sla_levels.txt, tiền tố SL)
    Repository<TransferTicket> transferRepository;   // Kho lưu trữ Phiếu chuyển xử lý (file: data/transfer_tickets.txt, tiền tố TF)
    Repository<Customer> customerRepository;         // Kho lưu trữ Khách hàng (file: data/customers.txt, tiền tố KH)
    Repository<Feedback> feedbackRepository;         // Kho lưu trữ Đánh giá hài lòng (file: data/feedback.txt, tiền tố DG)
    Repository<AgentShift> agentShiftRepository;     // Kho lưu trữ Ca trực (file: data/agent_shifts.txt, tiền tố AS)

public:
    // Hàm khởi tạo: gán đường dẫn file và tự động nạp dữ liệu từ ổ đĩa vào 10 kho lưu trữ
    Application();

    // Chạy vòng lặp menu chính của chương trình
    void run();

private:
    // Các hàm điều hướng nội bộ được đặt ở private vì chỉ gọi từ bên trong hàm run()
    void showMainMenu();         // In bảng menu chính ra màn hình
    void serviceGroupMenu();     // Mở menu quản lý Nhóm dịch vụ
    void ticketMenu();           // Mở menu quản lý Ticket
    void agentMenu();            // Mở menu quản lý Điện thoại viên
    void receptionChannelMenu(); // Mở menu quản lý Kênh tiếp nhận
    void inboundCallMenu();      // Mở menu quản lý Cuộc gọi tiếp nhận
    void slaLevelMenu();         // Mở menu quản lý Mức cam kết SLA
    void transferTicketMenu();   // Mở menu quản lý Phiếu chuyển xử lý
    void customerMenu();         // Mở menu quản lý Khách hàng
    void feedbackMenu();         // Mở menu quản lý Phiếu đánh giá hài lòng
    void agentShiftMenu();       // Mở menu quản lý Ca trực
};
