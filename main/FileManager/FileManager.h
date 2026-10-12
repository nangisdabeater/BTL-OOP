#pragma once

#include <string>
#include <vector>
#include "../Entity/Ticket.h"
#include "../Entity/ServiceGroup.h"
#include "../Entity/Agent.h"
// [PHẦN THÊM MỚI] Include các thực thể mới
#include "../Entity/ReceptionChannel.h"
#include "../Entity/InboundCall.h"
#include "../Entity/SLALevel.h"
#include "../Entity/TransferTicket.h"
#include "../Entity/Customer.h"
#include "../Entity/Feedback.h"

template <typename T>
class FileManager;

/**
 * @brief Chuyên biệt hóa lớp FileManager cho Thực thể ServiceGroup (ServiceManager)
 * Quản lý đọc và ghi file dữ liệu riêng trong thư mục data: 'data/service_groups.txt'
 */
template <>
class FileManager<ServiceGroup>
{
public:
    static std::vector<ServiceGroup> load(const std::string& filePath);
    static void save(const std::string& filePath, const std::vector<ServiceGroup>& data);
};

/**
 * @brief Chuyên biệt hóa lớp FileManager cho Thực thể Ticket
 * Quản lý đọc và ghi file dữ liệu riêng trong thư mục data: 'data/tickets.txt'
 */
template <>
class FileManager<Ticket>
{
public:
    static std::vector<Ticket> load(const std::string& filePath);
    static void save(const std::string& filePath, const std::vector<Ticket>& data);
};

/**
 * @brief Chuyên biệt hóa lớp FileManager cho Thực thể Agent
 * Quản lý đọc và ghi file dữ liệu riêng trong thư mục data: 'data/agents.txt'
 */
template <>
class FileManager<Agent> 
{
public:
    static std::vector<Agent> load(const std::string& filePath);
    static void save(const std::string& filePath, const std::vector<Agent>& data);
};

/**
 * @brief [PHẦN THÊM MỚI] Chuyên biệt hóa lớp FileManager cho Thực thể ReceptionChannel (Kênh tiếp nhận)
 * Quản lý đọc và ghi file dữ liệu riêng trong thư mục data: 'data/channels.txt'
 */
template <>
class FileManager<ReceptionChannel>
{
public:
    static std::vector<ReceptionChannel> load(const std::string& filePath);
    static void save(const std::string& filePath, const std::vector<ReceptionChannel>& data);
};

/**
 * @brief [PHẦN THÊM MỚI] Chuyên biệt hóa lớp FileManager cho Thực thể InboundCall (Cuộc gọi tiếp nhận)
 * Quản lý đọc và ghi file dữ liệu riêng trong thư mục data: 'data/inbound_calls.txt'
 */
template <>
class FileManager<InboundCall>
{
public:
    static std::vector<InboundCall> load(const std::string& filePath);
    static void save(const std::string& filePath, const std::vector<InboundCall>& data);
};

/**
 * @brief Chuyên biệt hóa lớp FileManager cho Thực thể SLALevel (Mức SLA)
 * Quản lý đọc và ghi file dữ liệu riêng trong thư mục data: 'data/sla_levels.txt'
 */
template <>
class FileManager<SLALevel>
{
public:
    static std::vector<SLALevel> load(const std::string& filePath);
    static void save(const std::string& filePath, const std::vector<SLALevel>& data);
};

/**
 * @brief Chuyên biệt hóa lớp FileManager cho Thực thể TransferTicket (Phiếu chuyển xử lý)
 * Quản lý đọc và ghi file dữ liệu riêng trong thư mục data: 'data/transfer_tickets.txt'
 */
template <>
class FileManager<TransferTicket>
{
public:
    static std::vector<TransferTicket> load(const std::string& filePath);
    static void save(const std::string& filePath, const std::vector<TransferTicket>& data);
};

/**
 * @brief Chuyên biệt hóa lớp FileManager cho Thực thể Customer (Khách hàng)
 * Quản lý đọc và ghi file dữ liệu riêng trong thư mục data: 'data/customers.txt'
 */
template <>
class FileManager<Customer>
{
public:
    static std::vector<Customer> load(const std::string& filePath);
    static void save(const std::string& filePath, const std::vector<Customer>& data);
};

/**
 * @brief Chuyên biệt hóa lớp FileManager cho Thực thể Feedback (Đánh giá hài lòng)
 * Quản lý đọc và ghi file dữ liệu riêng trong thư mục data: 'data/feedback.txt'
 */
template <>
class FileManager<Feedback>
{
public:
    static std::vector<Feedback> load(const std::string& filePath);
    static void save(const std::string& filePath, const std::vector<Feedback>& data);
};
