#pragma once

/**
 * @file SLALevelMenu.h
 * @brief Menu va CRUD cho Muc SLA.
 * Rang buoc: ten khong rong va khong trung, thoi gian > 0, uu tien 1-5.
 */

#include "App.h"

using namespace std;

class SLALevelMenu
{
private:
    Repository<SLALevel>& slaRepository;

    void showMenu();
    void displayAll();
    void findById();
    void add();
    void update();
    void remove();

    // Nhap so nguyen trong khoang [minValue, maxValue]; Enter (khi allowKeep) = giu nguyen
    static int inputInt(const string& prompt, int minValue, int maxValue);

public:
    explicit SLALevelMenu(Repository<SLALevel>& repo);
    void run();
};
