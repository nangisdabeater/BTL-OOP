#pragma once

/**
 * @file Repository.h
 * @brief Lớp khuôn mẫu (Template Class) quản lý kho lưu trữ dữ liệu dạng CRUD
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
class Repository
{
private:
    std::vector<T> data;
    int counter = 1;         // bo dem id: 1 -> 999
    std::string idPrefix;    // Tien to ID, vd: "SG" cho ServiceGroup, "TK" cho Ticket
    std::string filePath;    // duong dan file du lieu (rong = khong luu file)

    // Sinh id tu dong dua tren prefix
    std::string generateId();

    // Ghi file qua FileManager, neu loi thi khoi phuc du lieu truoc thao tac
    void saveOrRollback(const std::vector<T>& backup, int backupCounter);

public:
    // Cập nhật constructor để bắt buộc truyền prefix
    explicit Repository(const std::string& prefix, const std::string& filePath = "") 
        : idPrefix(prefix), filePath(filePath) {}

    // Nap du lieu tu file (qua FileManager), khoi phuc bo dem id
    void load();

    // Ghi toan bo du lieu ra file (qua FileManager)
    void save() const;

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

// ==============================
// SINH ID TU DONG
// ==============================
template <typename T>
std::string Repository<T>::generateId()
{
    if (counter > 999)
    {
        throw std::overflow_error("Da het ID!");
    }
    // Sử dụng Utils để gen ID
    return Utils::formatId(idPrefix, counter++);
}

// ==============================
// NAP / GHI FILE (qua FileManager)
// ==============================
template <typename T>
void Repository<T>::load()
{
    data.clear();
    counter = 1;

    if (filePath.empty())
    {
        return;
    }

    std::vector<T> loaded = FileManager<T>::load(filePath);

    for (const T& entity : loaded)
    {
        // Sử dụng Utils để trích xuất số và validate
        int number = Utils::extractIdNumber(entity.getId(), idPrefix);

        if (number == -1 || findById(entity.getId()) != nullptr)
        {
            std::cout << "Canh bao: bo qua ban ghi co ma khong hop le hoac trung: "
                      << entity.getId() << std::endl;
            continue;
        }

        data.push_back(entity);

        // Khoi phuc bo dem de id moi khong trung id da co
        if (number >= counter)
        {
            counter = number + 1;
        }
    }
}

template <typename T>
void Repository<T>::save() const
{
    if (filePath.empty())
    {
        return;
    }
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
        counter = backupCounter;
        throw;
    }
}

// ==============================
// THEM ENTITY
// ==============================
template <typename T>
void Repository<T>::add(const T& entity)
{
    std::vector<T> backup = data;
    int backupCounter = counter;

    T newEntity = entity;
    newEntity.setId(generateId());   // cap id moi cho entity

    data.push_back(newEntity);

    saveOrRollback(backup, backupCounter);
}

// ==============================
// TIM ENTITY THEO ID
// ==============================
template <typename T>
T* Repository<T>::findById(const std::string& id)
{
    for (T& entity : data)
    {
        if (entity.getId() == id)
        {
            return &entity;
        }
    }
    return nullptr;
}

// ==============================
// XOA ENTITY THEO ID
// ==============================
template <typename T>
bool Repository<T>::remove(const std::string& id)
{
    std::vector<T> backup = data;

    auto it = std::remove_if(
        data.begin(),
        data.end(),
        [id](const T& entity)
        {
            return entity.getId() == id;
        }
    );

    if (it == data.end())
    {
        return false;
    }

    data.erase(it, data.end());

    saveOrRollback(backup, counter);

    return true;
}

// ==============================
// LAY TAT CA ENTITY
// ==============================
template <typename T>
const std::vector<T>& Repository<T>::getAll() const
{
    return data;
}

// ==============================
// CAP NHAT ENTITY
// ==============================
template <typename T>
bool Repository<T>::update(const T& entity)
{
    T* existingEntity = findById(entity.getId());

    if (existingEntity == nullptr)
    {
        return false;
    }

    std::vector<T> backup = data;

    *existingEntity = entity;

    saveOrRollback(backup, counter);

    return true;
}