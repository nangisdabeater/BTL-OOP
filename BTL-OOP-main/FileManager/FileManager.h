#pragma once

#include <string>
#include <vector>
#include "../Entity/Ticket.h"
#include "../Entity/ServiceGroup.h"
#include "../Entity/Agent.h"
#include "../Entity/SLALevel.h"            //Added
#include "../Entity/TransferTicket.h"      //Added

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
template <>
class FileManager<SLALevel>
{
public:
    static std::vector<SLALevel> load(const std::string& filePath);
    static void save(const std::string& filePath, const std::vector<SLALevel>& data);
};

// Added
template <>
class FileManager<TransferTicket>
{
public:
    static std::vector<TransferTicket> load(const std::string& filePath);
    static void save(const std::string& filePath, const std::vector<TransferTicket>& data);
};
  
