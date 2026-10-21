#pragma once 

/**
 * @file AgentMenu.h
 * @brief Lớp điều khiển giao diện Menu và các thao tác CRUD cho Ca trực (AgentShift).
 * Quản lý các thuộc tính: Mã ID, Tên ca trực, Ngày trực, Bắt đầu, Kết thúc, Trạng thái hoạt động.
 * Đảm bảo các ràng buộc: Kiểm tra rỗng, kiểm tra trùng tên, kiểm tra toàn vẹn dữ liệu khi xóa.
 */

#include "../Repository/Repository.h"
#include "../Entity/AgentShift.h"
#include "../Entity/Agent.h"

class AgentShiftMenu {
  private:
    Repository<AgentShift>& agentShiftRepository;
    Repository<Agent>& agentRepository;
    
    void showMenu();
    void displayAll();
    void findById();
    void add();
    void update();
    void remove();
    void manageAgentShift();

  public:
    AgentShiftMenu(Repository<AgentShift>& asRepo, Repository<Agent>& agRepo);

    void run();

};