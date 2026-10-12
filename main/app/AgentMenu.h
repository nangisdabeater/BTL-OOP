#pragma once 

/**
 * @file AgentMenu.h
 * @brief Lớp điều khiển giao diện Menu và các thao tác CRUD cho Điện thoại viên (Agent).
 * Quản lý các thuộc tính: Mã ID (AGxxx), Họ và tên, Số điện thoại, Email, Số máy lẻ, Trạng thái.
 * Đảm bảo các ràng buộc: Kiểm tra rỗng, kiểm tra trùng số điện thoại/email, kiểm tra toàn vẹn dữ liệu khi xóa.
 */

#include "../Repository/Repository.h"
#include "../Entity/Agent.h"
#include "../Entity/InboundCall.h"

class AgentMenu {
  private:
    Repository<Agent>& agentRepository;
    Repository<InboundCall>* callRepository;
    
    void showMenu();
    void displayAll();
    void findById();
    void add();
    void update();
    void remove();

  public:
    AgentMenu(Repository<Agent>& agRepo, Repository<InboundCall>* cRepo = nullptr);

    void run();

};