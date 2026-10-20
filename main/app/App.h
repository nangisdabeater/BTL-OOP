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

using namespace std;

class Application{
private:
    Repository<ServiceGroup> serviceGroupRepository; // Kho lưu trữ Dịch vụ / ServiceManager (file: data/service_groups.txt, prefix SM)
    Repository<Ticket> ticketRepository;             // Kho lưu trữ Ticket (file: data/tickets.txt, prefix TK)
    Repository<Agent> agentRepository;               // Kho lưu trữ Agent (file: data/agents.txt, prefix AG)
    Repository<ReceptionChannel> channelRepository;   // Kho lưu trữ Kênh tiếp nhận (file: data/channels.txt, prefix CN)
    Repository<InboundCall> inboundCallRepository;   // Kho lưu trữ Cuộc gọi tiếp nhận (file: data/inbound_calls.txt, prefix CA)
    Repository<SLALevel> slaRepository;              // Kho lưu trữ Mức SLA (file: data/sla_levels.txt, prefix SL)
    Repository<TransferTicket> transferRepository;   // Kho lưu trữ Phiếu chuyển (file: data/transfer_tickets.txt, prefix TF)
    Repository<Customer> customerRepository;         // Kho lưu trữ Khách hàng (file: data/customers.txt, prefix KH)
    Repository<Feedback> feedbackRepository;         // Kho lưu trữ Đánh giá (file: data/feedback.txt, prefix DG)
    Repository<AgentShift> agentShiftRepository;     // Kho lưu trữ Đánh giá (file: data/agent_shifts.txt, prefix AS)

public:
    // Khởi tạo ứng dụng và nạp dữ liệu từ các file tương ứng
    Application();

    // Vòng lặp điều phối chính của chương trình
    void run();

private:
    // Hiển thị menu chung / Menu chính
    void showMainMenu();

    // Menu quản lý Dịch vụ / ServiceManager (điều hướng sang ServiceGroupMenu)
    void serviceGroupMenu();

    // Menu quản lý Ticket (điều hướng sang TicketMenu)
    void ticketMenu();

    // Menu quản lý Agent (điều hướng sang AgentMenu)
    void agentMenu();

    // Menu quản lý Kênh tiếp nhận (điều hướng sang ReceptionChannelMenu)
    void receptionChannelMenu();

    // Menu quản lý Cuộc gọi tiếp nhận (điều hướng sang InboundCallMenu)
    void inboundCallMenu();

    // Menu quản lý Mức SLA (điều hướng sang SLALevelMenu)
    void slaLevelMenu();

    // Menu quản lý Phiếu chuyển xử lý (điều hướng sang TransferTicketMenu)
    void transferTicketMenu();

    // Menu quản lý Khách hàng (điều hướng sang CustomerMenu)
    void customerMenu();

    // Menu quản lý Đánh giá hài lòng (điều hướng sang FeedbackMenu)
    void feedbackMenu();

    // Menu quản lý Ca trực (điều hướng sang AgentShiftMenu)
    void agentShiftMenu();
};