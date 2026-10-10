#pragma once

/**
 * @file TransferTicketMenu.h
 * @brief Menu va CRUD cho Phieu chuyen xu ly.
 * Rang buoc khoa ngoai: Ticket, Agent chuyen, Agent nhan phai ton tai; Agent phai dang hoat dong;
 * Agent chuyen khac Agent nhan; khong chuyen Ticket da dong; phieu Hoan tat bi khoa sua/xoa.
 */

#include "App.h"

using namespace std;

class TransferTicketMenu
{
private:
    Repository<TransferTicket>& transferRepository;
    Repository<Ticket>& ticketRepository;
    Repository<Agent>& agentRepository;

    void showMenu();
    void displayAll();
    void findById();
    void add();
    void update();
    void remove();

    static string statusToString(TransferStatus s);
    string agentNameOf(const string& agentId);   // tra ten agent de hien thi

public:
    TransferTicketMenu(
        Repository<TransferTicket>& tfRepo,
        Repository<Ticket>& tRepo,
        Repository<Agent>& aRepo
    );
    void run();
};
