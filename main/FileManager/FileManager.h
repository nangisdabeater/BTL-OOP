#pragma once

#include <string>
#include <vector>
#include "../Entity/Ticket.h"
#include "../Entity/ServiceGroup.h"
#include "../Entity/Agent.h"
#include "../Entity/ReceptionChannel.h"
#include "../Entity/InboundCall.h"
#include "../Entity/SLALevel.h"
#include "../Entity/TransferTicket.h"
#include "../Entity/Customer.h"
#include "../Entity/Feedback.h"
#include "../Entity/AgentShift.h"

// Tại sao dùng khuôn mẫu lớp (template <typename T> class FileManager) kết hợp chuyên biệt hóa (template <>)?
// - Mỗi thực thể có số lượng cột và kiểu dữ liệu trong file .txt hoàn toàn khác nhau nên không thể viết chung 1 hàm đọc/ghi.
// - Nhờ kỹ thuật Chuyên biệt hóa khuôn mẫu (Template Specialization), mỗi thực thể được định nghĩa cách đọc/ghi riêng,
//   nhưng lớp Repository<T> vẫn có thể gọi chung một cú pháp thống nhất là FileManager<T>::load() và FileManager<T>::save()
//   mà không cần dùng if-else để phân loại từng thực thể.
template <typename T>
class FileManager;

// Chuyên biệt hóa đọc/ghi file cho thực thể ServiceGroup (file: data/service_groups.txt)
template <>
class FileManager<ServiceGroup> {
public:
    // Tại sao dùng từ khóa "static" cho hàm load và save?
    // Vì lớp FileManager chỉ thực hiện chức năng đọc/ghi file chứ không lưu trữ dữ liệu bên trong (không có thuộc tính).
    // Dùng static giúp gọi trực tiếp FileManager<ServiceGroup>::load(...) mà không tốn bộ nhớ khởi tạo đối tượng.
    static std::vector<ServiceGroup> load(const std::string& filePath);
    static void save(const std::string& filePath, const std::vector<ServiceGroup>& data);
};

// Chuyên biệt hóa đọc/ghi file cho thực thể Ticket (file: data/tickets.txt)
template <>
class FileManager<Ticket> {
public:
    static std::vector<Ticket> load(const std::string& filePath);
    static void save(const std::string& filePath, const std::vector<Ticket>& data);
};

// Chuyên biệt hóa đọc/ghi file cho thực thể Agent (file: data/agents.txt)
template <>
class FileManager<Agent> {
public:
    static std::vector<Agent> load(const std::string& filePath);
    static void save(const std::string& filePath, const std::vector<Agent>& data);
};

// Chuyên biệt hóa đọc/ghi file cho thực thể AgentShift (file: data/agent_shifts.txt)
template <>
class FileManager<AgentShift> {
public:
    static std::vector<AgentShift> load(const std::string& filePath);
    static void save(const std::string& filePath, const std::vector<AgentShift>& data);
};

// Chuyên biệt hóa đọc/ghi file cho thực thể ReceptionChannel (file: data/channels.txt)
template <>
class FileManager<ReceptionChannel> {
public:
    static std::vector<ReceptionChannel> load(const std::string& filePath);
    static void save(const std::string& filePath, const std::vector<ReceptionChannel>& data);
};

// Chuyên biệt hóa đọc/ghi file cho thực thể InboundCall (file: data/inbound_calls.txt)
template <>
class FileManager<InboundCall> {
public:
    static std::vector<InboundCall> load(const std::string& filePath);
    static void save(const std::string& filePath, const std::vector<InboundCall>& data);
};

// Chuyên biệt hóa đọc/ghi file cho thực thể SLALevel (file: data/sla_levels.txt)
template <>
class FileManager<SLALevel> {
public:
    static std::vector<SLALevel> load(const std::string& filePath);
    static void save(const std::string& filePath, const std::vector<SLALevel>& data);
};

// Chuyên biệt hóa đọc/ghi file cho thực thể TransferTicket (file: data/transfer_tickets.txt)
template <>
class FileManager<TransferTicket> {
public:
    static std::vector<TransferTicket> load(const std::string& filePath);
    static void save(const std::string& filePath, const std::vector<TransferTicket>& data);
};

// Chuyên biệt hóa đọc/ghi file cho thực thể Customer (file: data/customers.txt)
template <>
class FileManager<Customer> {
public:
    static std::vector<Customer> load(const std::string& filePath);
    static void save(const std::string& filePath, const std::vector<Customer>& data);
};

// Chuyên biệt hóa đọc/ghi file cho thực thể Feedback (file: data/feedback.txt)
template <>
class FileManager<Feedback> {
public:
    static std::vector<Feedback> load(const std::string& filePath);
    static void save(const std::string& filePath, const std::vector<Feedback>& data);
};
