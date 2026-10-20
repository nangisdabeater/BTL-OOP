#pragma once

/**
 * Lớp khuôn mẫu (Template Class) quản lý kho lưu trữ dữ liệu dạng CRUD
 */

#include <vector>
#include <algorithm>
#include <string>
#include <stdexcept>
#include <iostream>
#include "Utilities.h"
#include "FileManager.h"

// T phai ke thua Entity, co constructor mac dinh
template <typename T>
class Repository{
private:
    std::vector<T> data;
    int count = 1;         // bo dem id: 1 -> 999
    std::string idPrefix;    // Tien to ID, vd: "SG" cho ServiceGroup, "TK" cho Ticket
    std::string filePath;    // duong dan file du lieu (rong = khong luu file)

    // Sinh id tu dong dua tren prefix
    std::string generateId();

    // Ghi file qua FileManager, neu loi thi khoi phuc du lieu truoc thao tac
    void saveOrRollback(const std::vector<T>& backup, int backupCounter);

public:
    // Cập nhật constructor để bắt buộc truyền prefix và tự động chuẩn hóa đường dẫn về thư mục data gốc
    Repository(const std::string& prefix, const std::string& filePath = "") {
    idPrefix = prefix;
    this->filePath = filePath;
}
    // Nap du lieu tu file (qua FileManager), khoi phuc bo dem id
    void load();

    // Ghi toan bo du lieu ra file (qua FileManager)
    void save();

    // Them entity
    void add(const T& entity);

    // Xoa entity theo ID
    bool remove(const std::string& id);

    // Tim entity theo ID
    // LƯU Ý: Không lưu lại biến con trỏ này sang ngữ cảnh khác vì bộ nhớ vector 
    // có thể di chuyển sau khi gọi add/remove. Chỉ dùng tạm thời trong hàm.
    T* findById(const std::string& id);

    // Lay tat ca entity
    const std::vector<T>& getAll() const;

    // Cap nhat entity
    bool update(const T& entity);
};

// SINH ID TU DONG

template <typename T>
std::string Repository<T>::generateId(){
    if (count > 999)
    {
        throw std::overflow_error("Da het ID!");
    }
    // Sử dụng Utils để gen ID
    return Utils::formatId(idPrefix, count++);
}

// NAP / GHI FILE (qua FileManager)

template <typename T>
void Repository<T>::load(){
    data.clear();
    count = 1;

    std::vector<T> loaded = FileManager<T>::load(filePath);

    for (const T& entity : loaded)
    {
        // Sử dụng Utils để trích xuất số và xác thực ID (để ktra nguồn nhập từ bên ngoài).
        int number = Utils::extractIdNumber(entity.getId(), idPrefix);

        if (number == -1 || findById(entity.getId()) != nullptr)
        {
            std::cout << "Canh bao: bo qua ban ghi co ma khong hop le hoac trung: "
                      << entity.getId() << std::endl;
            continue;
        }

        data.push_back(entity);

        // Khoi phuc bo dem de id moi khong trung id da co
        if (number >= count)
        {
            count = number + 1;
        }
    }
}

template <typename T>
void Repository<T>::save(){

    FileManager<T>::save(filePath, data);
}

template <typename T>
void Repository<T>::saveOrRollback(const std::vector<T>& backup, int backupCounter)
{
    try
    {
        save();
    }
    catch (...)
    {
        data = backup;
        count = backupCounter;
        throw;
    }
}

// ==============================
// THEM ENTITY
// ==============================
template <typename T>
void Repository<T>::add(const T& entity){
    //Gọi danh sách và ID ra để xử lí
    std::vector<T> backup = data;
    int backupCounter = count;

    T newEntity = entity;
    newEntity.setId(generateId());   // cấp id mới cho entity

    data.push_back(newEntity);

    saveOrRollback(backup, backupCounter);
}

// ==============================
// TIM ENTITY THEO ID
// ==============================
template <typename T>
//Dùng T* để tìm ID thông qua 2 trạng thái , có thì trả về địa chỉ entity,không thì nullptr
//Nếu dùng T& bắt buộc phải dùng throw, mà throw cần try catch lằng nhằng hơn

T* Repository<T>::findById(const std::string& id){
    for (T& entity : data){
        if (entity.getId() == id){
            return &entity;
        }
    }
    return nullptr;
}

// XOA ENTITY THEO ID

template <typename T>
bool Repository<T>::remove(const std::string& id){
    std::vector<T> backup = data;

    auto it = std::remove_if(       //auto tự gán kiểu dữ liệu
        data.begin(),
        data.end(),             //quét từng entity
        [id](const T& entity){      //ktra ID cần xóa, dùng lambda để tránh viết thêm hàm ở bên ngoài(chỉ dùng duy nhất cho 1 hàm)
        return entity.getId() == id;
        }
    );

    if (it == data.end()){  //K có ID trùng thì báo k có
        return false;
    }

    data.erase(it, data.end());

    saveOrRollback(backup, count); //Cập nhật lại danh sách

    return true;
}

// LAY TAT CA ENTITY

template <typename T>
const std::vector<T>& Repository<T>::getAll() const{
    return data;
}


// CAP NHAT ENTITY

template <typename T>
bool Repository<T>::update(const T& entity){
    T* existingEntity = findById(entity.getId());

    if (existingEntity == nullptr){
        return false;
    }

    std::vector<T> backup = data;

    *existingEntity = entity;

    saveOrRollback(backup, count);

    return true;
}