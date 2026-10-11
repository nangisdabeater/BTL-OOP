#pragma once

#include <vector>
#include <string>
#include "../Entity/ServiceGroup.h"
#include "../Entity/Ticket.h"
#include "../Repository/Repository.h"
#include "../Entity/Agent.h"
// [PHẦN THÊM MỚI] Include thực thể Kênh tiếp nhận và Cuộc gọi tiếp nhận
#include "../Entity/ReceptionChannel.h"
#include "../Entity/InboundCall.h"

using namespace std;

class Application
{
private:
    Repository<ServiceGroup> serviceGroupRepository; // Kho lưu trữ Nhóm dịch vụ (file: data/service_groups.txt)
    Repository<Ticket> ticketRepository;             // Kho lưu trữ Ticket (file: data/tickets.txt)
    Repository<Agent> agentRepository;               // Kho lưu trữ Agent (file: data/agents.txt)

    // [PHẦN THÊM MỚI] Kho lưu trữ Kênh tiếp nhận (prefix: CN) và Cuộc gọi tiếp nhận (prefix: CA)
    Repository<ReceptionChannel> channelRepository;   // Kho lưu trữ Kênh tiếp nhận (file: data/channels.txt)
    Repository<InboundCall> inboundCallRepository;   // Kho lưu trữ Cuộc gọi tiếp nhận (file: data/inbound_calls.txt)

public:
    // Khởi tạo ứng dụng và nạp dữ liệu từ các file tương ứng
    Application();

    // Vòng lặp điều phối chính của chương trình
    void run();

private:
    // Hiển thị menu chung / Menu chính
    void showMainMenu();

    // Menu quản lý Nhóm dịch vụ (điều hướng sang ServiceGroupMenu)
    void serviceGroupMenu();

    // Menu quản lý Ticket (điều hướng sang TicketMenu)
    void ticketMenu();

    // Menu quản lý Agent (điều hướng sang AgentMenu)
    void agentMenu();

    // [PHẦN THÊM MỚI] Menu quản lý Kênh tiếp nhận (điều hướng sang ReceptionChannelMenu)
    void receptionChannelMenu();

    // [PHẦN THÊM MỚI] Menu quản lý Cuộc gọi tiếp nhận (điều hướng sang InboundCallMenu)
    void inboundCallMenu();
};