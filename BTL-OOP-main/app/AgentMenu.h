#pragma once 

/**
 * @file AgentMenu.h
 * @brief Lớp điều khiển giao diện Menu và các thao tác CRUD cho Điện thoại viên (Agent).
 * Quản lý các thuộc tính: Mã ID, Tên nhóm dịch vụ, Mô tả, Trạng thái hoạt động.
 * Đảm bảo các ràng buộc: Kiểm tra rỗng, kiểm tra trùng tên, kiểm tra toàn vẹn dữ liệu khi xóa.
 */

 #include "App.h"

class AgentMenu {
  private:
    Repository<Agent>& agentRepository;
    
    void showMenu();
    void displayAll();
    void findById();
    void add();
    void update();
    void remove();

  public:
    AgentMenu(Repository<Agent>& agRepo);

    void run();

};