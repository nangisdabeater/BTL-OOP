#pragma once

/**
 * @file SLALevelMenu.h
 * @brief Menu va CRUD cho Muc SLA.
 * Rang buoc: ten khong rong va khong trung, thoi gian > 0, uu tien 1-5.
 */

#include "../Repository/Repository.h"
#include "../Entity/SLALevel.h"
#include "../Entity/TransferTicket.h"

using namespace std;

class SLALevelMenu
{
private:
    Repository<SLALevel>& slaRepository;
    Repository<TransferTicket>& transferRepository; // Kiểm tra ràng buộc toàn vẹn với Phiếu chuyển (cùng phân hệ của Cường)

    void showMenu();
    void displayAll();
    void findById();
    void add();
    void update();
    void remove();

    // Nhap so nguyen trong khoang [minValue, maxValue]; Enter (khi allowKeep) = giu nguyen
    static int inputInt(const string& prompt, int minValue, int maxValue);

public:
    SLALevelMenu(Repository<SLALevel>& repo, Repository<TransferTicket>& tfRepo);
    void run();
};
