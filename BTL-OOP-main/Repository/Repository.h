#pragma once

/**
 * @file Repository.h
 * @brief Lớp khuôn mẫu (Template Class) quản lý kho lưu trữ dữ liệu dạng CRUD
 * Cung cấp các thao tác: Thêm, Xóa, Cập nhật, Tìm kiếm theo ID, Lấy tất cả,
 * tự động sinh mã định danh (NP000 - NP999) và đồng bộ lưu/đọc file qua FileManager.
 */

#include <vector>
#include <algorithm>
#include <string>
#include <stdexcept>
#include <iostream>
#include <cctype>


using namespace std;

template <typename T>
class FileManager;

// T phai ke thua Entity, co constructor mac dinh
template <typename T>
class Repository
{
private:
    vector<T> data;
    int counter = 1;        // bo dem id: 1 -> 999 (NP001 ... NP999)
    string filePath;        // duong dan file du lieu (rong = khong luu file)

    // Sinh id dang NPxxx (NP001 ... NP999)
    string generateId();

    // Tach so tu id dang NPxxx, tra ve false neu id sai dinh dang
    static bool parseIdNumber(const string& id, int& number);

    // Ghi file qua FileManager, neu loi thi khoi phuc du lieu truoc thao tac
    void saveOrRollback(const vector<T>& backup, int backupCounter);

public:
    explicit Repository(const string& filePath = "") : filePath(filePath) {}

    // Nap du lieu tu file (qua FileManager), khoi phuc bo dem id
    void load();

    // Ghi toan bo du lieu ra file (qua FileManager)
    void save() const;

    // Them entity
    void add(const T& entity);

    // Xoa entity theo ID
    bool remove(const string& id);

    // Tim entity theo ID
    T* findById(const string& id);

    // Lay tat ca entity
    const vector<T>& getAll() const;

    // Cap nhat entity
    bool update(const T& entity);
};


// ==============================
// SINH ID TU DONG
// ==============================

template <typename T>
string Repository<T>::generateId()
{
    if (counter > 999)
    {
        throw overflow_error("Da het ID (NP001 - NP999)");
    }

    string number = to_string(counter++);

    // Dem them so 0 cho du 3 chu so
    while (number.length() < 3)
    {
        number = "0" + number;
    }

    return "NP" + number;
}

template <typename T>
bool Repository<T>::parseIdNumber(const string& id, int& number)
{
    if (id.length() != 5 || id.substr(0, 2) != "NP")
    {
        return false;
    }

    for (size_t i = 2; i < id.length(); i++)
    {
        if (!isdigit(static_cast<unsigned char>(id[i])))
        {
            return false;
        }
    }

    number = stoi(id.substr(2));
    if (number < 1 || number > 999)
    {
        return false;
    }

    return true;
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

    vector<T> loaded = FileManager<T>::load(filePath);

    for (const T& entity : loaded)
    {
        int number = 0;

        // Repository chiu trach nhiem ve id: dung dinh dang NPxxx va khong trung
        if (!parseIdNumber(entity.getId(), number)
            || findById(entity.getId()) != nullptr)
        {
            cout << "Canh bao: bo qua ban ghi co ma khong hop le hoac trung: "
                 << entity.getId() << endl;
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
void Repository<T>::saveOrRollback(const vector<T>& backup, int backupCounter)
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
    vector<T> backup = data;
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
T* Repository<T>::findById(const string& id)
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
bool Repository<T>::remove(const string& id)
{
    vector<T> backup = data;

    auto it = remove_if(
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
const vector<T>& Repository<T>::getAll() const
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

    vector<T> backup = data;

    *existingEntity = entity;

    saveOrRollback(backup, counter);

    return true;
}