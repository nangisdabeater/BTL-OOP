#pragma once 

#include "../Repository/Repository.h"
#include "../Entity/AgentShift.h"
#include "../Entity/Agent.h"

class AgentShiftMenu {
private:
    // Tham chiếu trực tiếp tới kho lưu trữ Ca trực gốc trong lớp Application
    Repository<AgentShift>& agentShiftRepository;
    // Tham chiếu tới kho Điện thoại viên để kiểm tra mã nhân viên có tồn tại và đang hoạt động không khi phân công vào ca
    Repository<Agent>& agentRepository;
    
    void showMenu();         // Hiển thị danh mục chức năng quản lý Ca trực
    void displayAll();       // Hiển thị danh sách tất cả các ca trực
    void findById();         // Tìm kiếm thông tin ca trực theo mã ID
    void add();              // Tạo mới ca trực (kiểm tra trùng khung giờ trong cùng ngày)
    void update();           // Cập nhật thông tin ca trực
    void remove();           // Xóa ca trực (chặn xóa nếu ca trực đã có nhân viên đăng ký)
    void manageAgentShift(); // Thêm hoặc gỡ nhân viên khỏi một ca trực

public:
    AgentShiftMenu(Repository<AgentShift>& asRepo, Repository<Agent>& agRepo);

    void run();              // Chạy vòng lặp điều khiển menu Ca trực
};
