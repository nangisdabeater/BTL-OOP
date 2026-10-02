#pragma once

#include <vector>
#include <string>
#include "../Entity/ServiceGroup.h"
#include "../Entity/Ticket.h"
#include "../Repository/Repository.h"

using namespace std;

/**
 * @brief Chuyên biệt hóa lớp FileManager cho Thực thể ServiceGroup
 * Quản lý đọc và ghi file dữ liệu riêng trong thư mục data: 'data/service_groups.txt'
 */
template <>
class FileManager<ServiceGroup>
{
public:
    static vector<ServiceGroup> load(const string& filePath);
    static void save(const string& filePath, const vector<ServiceGroup>& data);
};

/**
 * @brief Chuyên biệt hóa lớp FileManager cho Thực thể Ticket
 * Quản lý đọc và ghi file dữ liệu riêng trong thư mục data: 'data/tickets.txt'
 */
template <>
class FileManager<Ticket>
{
public:
    static vector<Ticket> load(const string& filePath);
    static void save(const string& filePath, const vector<Ticket>& data);
};

/**
 * @brief Lớp điều phối trung tâm của toàn bộ ứng dụng (Application)
 * Quản lý vòng lặp chính, menu tổng và kết nối tới các kho lưu trữ Repository.
 */
class Application
{
private:
    Repository<ServiceGroup> serviceGroupRepository; // Kho lưu trữ Nhóm dịch vụ (file: data/service_groups.txt)
    Repository<Ticket> ticketRepository;             // Kho lưu trữ Ticket (file: data/tickets.txt)

public:
    // Khởi tạo ứng dụng và nạp dữ liệu từ các file tương ứng
    Application();

    // Vòng lặp điều phối chính của chương trình
    void run();

private:
    // Hiển thị menu chung / Menu chính
    void showMainMenu();

    // Menu quản lý Nhóm dịch vụ (điều hướng sang ServiceGroupMenu)
    void serviceGroupMenu();

    // Menu quản lý Ticket (điều hướng sang TicketMenu)
    void ticketMenu();
};