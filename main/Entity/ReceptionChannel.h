#pragma once
#include <string>
#include "BaseEntity.h"


class ReceptionChannel : public BaseEntity
{
private:
    std::string name;    
    std::string description;  // Mô tả kênh tiếp nhận
    bool active;         // Trạng thái (true: Đang hoạt động, false: Tạm ngưng)

public:
    // Constructor (ID sẽ được Repository cấp tự động dạng CNxxx khi gọi add)
    ReceptionChannel(
        const std::string& name = "",
        const std::string& description = ""
    );

    // Getter
    const std::string& getName() const;
    const std::string& getDescription() const;
    bool isActive() const;

    // Setter 
    void setName(const std::string& name);
    void setDescription(const std::string& description);

    // Triển khai phương thức thuần ảo từ BaseEntity để in thông tin ra màn hình
    void printInfo() const override;
    // quản lý trạng thái
    void activate();
    void deactivate();
};
