#pragma once

/**
 * @file ReceptionChannelMenu.h
 * @brief Lớp điều khiển giao diện Menu và các thao tác CRUD cho Kênh tiếp nhận (ReceptionChannel)
 * [PHẦN THÊM MỚI]
 * Quản lý các thuộc tính: Mã ID (CNxxx), Tên kênh, Mô tả, Trạng thái hoạt động.
 * Đảm bảo các ràng buộc: Kiểm tra rỗng, kiểm tra trùng tên kênh, kiểm tra toàn vẹn trước khi xóa (ràng buộc với Cuộc gọi).
 */

#include "../Repository/Repository.h"
#include "../Entity/ReceptionChannel.h"
#include "../Entity/InboundCall.h"

class ReceptionChannelMenu
{
private:
    Repository<ReceptionChannel>& channelRepository;
    Repository<InboundCall>& callRepository; // Dùng để kiểm tra ràng buộc toàn vẹn khi xóa

    void showMenu();
    void displayAll();
    void findById();
    void add();
    void update();
    void toggleStatus();
    void remove();

public:
    ReceptionChannelMenu(Repository<ReceptionChannel>& chRepo, Repository<InboundCall>& cRepo);

    void run();
};
