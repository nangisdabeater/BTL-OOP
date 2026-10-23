#pragma once 

#include "../Repository/Repository.h"
#include "../Entity/Agent.h"
#include "../Entity/AgentShift.h"

class AgentMenu {
private:
    // Truyền tham chiếu (&) để thao tác trực tiếp trên kho dữ liệu Điện thoại viên gốc
    Repository<Agent>& agentRepository;
    // Truyền thêm tham chiếu kho Ca trực để kiểm tra xem nhân viên có đang được phân công trực không trước khi xóa
    Repository<AgentShift>& agentShiftRepository;
    
    void showMenu();   // Hiển thị danh mục chức năng quản lý Điện thoại viên
    void displayAll(); // Hiển thị toàn bộ danh sách điện thoại viên
    void findById();   // Tìm kiếm điện thoại viên theo mã ID
    void add();        // Thêm mới điện thoại viên (kiểm tra trùng SĐT, Email, Máy lẻ)
    void update();     // Cập nhật thông tin điện thoại viên
    void remove();     // Xóa điện thoại viên (chặn xóa nếu đang có tên trong ca trực)

public:
    AgentMenu(Repository<Agent>& agRepo, Repository<AgentShift>& asRepo);

    void run();        // Chạy vòng lặp điều khiển menu Điện thoại viên
};
