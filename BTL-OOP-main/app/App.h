#pragma once

#include <vector>
#include <string>
#include "../Entity/ServiceGroup.h"
#include "../Entity/Ticket.h"
#include "../Repository/Repository.h"
#include "../Entity/Agent.h"
#include "../Entity/SLALevel.h"           //Added
#include "../Entity/TransferTicket.h"         //Added

using namespace std;

class Application
{
private:
    Repository<ServiceGroup> serviceGroupRepository; // Kho lưu trữ Nhóm dịch vụ (file: data/service_groups.txt)
    Repository<Ticket> ticketRepository;             // Kho lưu trữ Ticket (file: data/tickets.txt)
    Repository<Agent> agentRepository;               // Kho lưu trữ Agent (file: data/agents.txt)
    
    Repository<SLALevel> slaLevelRepository;          //Added
    Repository<TransferTicket> transferTicketRepository;   //Added

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

    void slaLevelMenu();          //Added
    void transferTicketMenu();
};
