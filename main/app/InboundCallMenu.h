#pragma once

/**
 * @file InboundCallMenu.h
 * @brief Lớp điều khiển giao diện Menu và các thao tác CRUD cho Cuộc gọi tiếp nhận (InboundCall)
 * [PHẦN THÊM MỚI]
 * Quản lý các thuộc tính: Mã ID (CAxxx), Số điện thoại, Mã kênh (CNxxx), Mã DTV (AGxxx), Thời gian, Thời lượng, Trạng thái, Ghi chú.
 * Đảm bảo các ràng buộc:
 *   - Mã kênh tiếp nhận phải tồn tại và đang hoạt động.
 *   - Mã điện thoại viên tiếp nhận (nếu có nhập) phải tồn tại trong danh sách Agent.
 *   - Tự động sinh ID theo tiền tố "CA" (CA001, CA002,...).
 */

#include "../Repository/Repository.h"
#include "../Entity/InboundCall.h"
#include "../Entity/ReceptionChannel.h"
#include "../Entity/Agent.h"

class InboundCallMenu
{
private:
    Repository<InboundCall>& callRepository;
    Repository<ReceptionChannel>& channelRepository; // Khóa ngoại kiểm tra mã kênh
    Repository<Agent>& agentRepository;             // Khóa ngoại kiểm tra mã nhân viên tiếp nhận

    void showMenu();
    void displayAll();
    void findById();
    void add();
    void update();
    void filterByChannel();
    void remove();

public:
    InboundCallMenu(
        Repository<InboundCall>& cRepo,
        Repository<ReceptionChannel>& chRepo,
        Repository<Agent>& agRepo
    );

    void run();
};