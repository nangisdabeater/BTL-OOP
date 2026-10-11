#pragma once

#include <string>
#include <vector>
#include "../Entity/Ticket.h"
#include "../Entity/ServiceGroup.h"
#include "../Entity/Agent.h"
// [PHẦN THÊM MỚI] Include các thực thể mới
#include "../Entity/ReceptionChannel.h"
#include "../Entity/InboundCall.h"

template <typename T>
class FileManager;

/**
 * @brief Chuyên biệt hóa lớp FileManager cho Thực thể ServiceGroup
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
