#pragma once

/**
 * @file TransferTicketMenu.h
 * @brief Giao dien Menu va thao tac CRUD cho Phieu chuyen xu ly (TransferTicket).
 * Rang buoc: Ma SLA (SLxxx) phai ton tai trong SLALevelRepository;
 * Bo phan chuyen khac Bo phan nhan; phieu Hoan tat bi khoa sua/xoa.
 */

#include "../Repository/Repository.h"
#include "../Entity/TransferTicket.h"
#include "../Entity/SLALevel.h"

using namespace std;

class TransferTicketMenu
{
private:
    Repository<TransferTicket>& transferRepository;
    Repository<SLALevel>& slaRepository;

    void showMenu();
    void displayAll();
    void findById();
    void add();
    void update();
    void remove();

    static string statusToString(TransferStatus s);
    string slaNameOf(const string& slaId);

public:
    TransferTicketMenu(
        Repository<TransferTicket>& tfRepo,
        Repository<SLALevel>& sRepo
    );
    void run();
};
