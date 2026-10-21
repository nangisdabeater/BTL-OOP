#pragma once 

/**
 * @file AgentMenu.h
 * @brief Lớp điều khiển giao diện Menu và các thao tác CRUD cho Điện thoại viên (Agent).
 * Quản lý các thuộc tính: Mã ID (AGxxx), Họ và tên, Số điện thoại, Email, Số máy lẻ, Trạng thái.
 * Đảm bảo các ràng buộc: Kiểm tra rỗng, kiểm tra trùng số điện thoại/email, kiểm tra toàn vẹn dữ liệu khi xóa.
 */

#include "../Repository/Repository.h"
#include "../Entity/Agent.h"
#include "../Entity/AgentShift.h"

class AgentMenu {
  private:
    Repository<Agent>& agentRepository;
    Repository<AgentShift>& agentShiftRepository; // Kiểm tra ràng buộc toàn vẹn với Ca trực (cùng phân hệ của Thành)
    
    void showMenu();
    void displayAll();
    void findById();
    void add();
    void update();
    void remove();

  public:
    AgentMenu(Repository<Agent>& agRepo, Repository<AgentShift>& asRepo);

    void run();

};