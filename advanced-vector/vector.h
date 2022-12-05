#pragma once
#include <algorithm>
#include <cassert>
#include <cstdlib>
#include <memory>
#include <new>
#include <utility>

template <typename T>
class RawMemory {
public:
    RawMemory() = default;

    explicit RawMemory(size_t capacity)
        : buffer_(Allocate(capacity))
        , capacity_(capacity) {
    }

    RawMemory(const RawMemory&) = delete;
    RawMemory& operator=(const RawMemory& rhs) = delete;

    RawMemory(RawMemory&& other) noexcept :buffer_(std::move(other.buffer_)), capacity_(std::move(other.capacity_)) {
        other.buffer_ = nullptr;
        other.capacity_ = 0;
    }

    RawMemory& operator=(RawMemory&& rhs) noexcept {
        if (rhs.capacity_ > capacity_) {
            Deallocate(buffer_);

            buffer_ = std::move(rhs.buffer_);
            capacity_ = std::move(rhs.capacity_);

            rhs.buffer_ = nullptr;
            rhs.capacity_ = 0;
        }
        return *this;
    }

    ~RawMemory() {
        if (capacity_ > 0) {
            Deallocate(buffer_);
        }
        buffer_ = nullptr;
        capacity_ = 0;
    }

    T* operator+(size_t offset) noexcept {
    	// Разрешается получать адрес ячейки памяти, следующей за последним элементом массива
        assert(offset <= capacity_);
        return buffer_ + offset;
    }

    const T* operator+(size_t offset) const noexcept {
        return const_cast<RawMemory&>(*this) + offset;
    }

    const T& operator[](size_t index) const noexcept {
        return const_cast<RawMemory&>(*this)[index];
    }

    T& operator[](size_t index) noexcept {
        assert(index < capacity_);
        return buffer_[index];
    }

    void Swap(RawMemory& other) noexcept {
        std::swap(buffer_, other.buffer_);
        std::swap(capacity_, other.capacity_);
    }

    const T* GetAddress() const noexcept {
        return buffer_;
    }

    T* GetAddress() noexcept {
        return buffer_;
    }

    size_t Capacity() const {
        return capacity_;
    }

private:
    // Выделяет сырую память под n элементов и возвращает указатель на неё
    static T* Allocate(size_t n) {
        return n != 0 ? static_cast<T*>(operator new(n * sizeof(T))) : nullptr;
    }

    // Освобождает сырую память, выделенную ранее по адресу buf при помощи Allocate
    static void Deallocate(T* buf) noexcept {
        operator delete(buf);
    }

    T* buffer_ = nullptr;
    size_t capacity_ = 0;
};

template <typename T>
class Vector {
public:
    using iterator = T*;
    using const_iterator = const T*;

    Vector() = default;

    explicit Vector(size_t size)
        : data_(size)
        , size_(size)
    {
        std::uninitialized_value_construct_n(data_.GetAddress(), size);
    }

    Vector(const Vector& other)
        : data_(other.size_)
        , size_(other.size_)
    {
        std::uninitialized_copy_n(other.data_.GetAddress(), other.size_, data_.GetAddress());
    }

    Vector(Vector&& other) noexcept : data_(std::move(other.data_))
        , size_(std::move(other.size_)) {
        other.size_ = 0;
    }

    size_t Size() const noexcept {
        return size_;
    }

    size_t Capacity() const noexcept {
        return data_.Capacity();
    }

    const T& operator[](size_t index) const noexcept {
        return const_cast<Vector&>(*this)[index];
    }

    T& operator[](size_t index) noexcept {
        assert(index < size_);
        return data_[index];
    }

    Vector& operator=(const Vector& rhs) {
        if (this != &rhs) {
            if (rhs.size_ > data_.Capacity()) {
                Vector rhs_copy(rhs);
                Swap(rhs_copy);
            }
            else {
                if (rhs.size_ < size_) {
                    std::copy_n(rhs.data_.GetAddress(), rhs.size_, data_.GetAddress());
                    std::destroy_n(data_.GetAddress() + rhs.size_, size_ - rhs.size_);
                }
                else {
                    std::copy_n(rhs.data_.GetAddress(), size_, data_.GetAddress());
                    std::uninitialized_copy_n(rhs.data_.GetAddress() + size_, rhs.size_ - size_, data_.GetAddress() + size_);
                }
            }
            size_ = rhs.size_;
        }
        return *this;
    }

    Vector& operator=(Vector&& rhs) noexcept {
        if (this != &rhs) {
            data_.Swap(rhs.data_);
            std::swap(size_, rhs.size_);
        }
        return *this;
    }

    void Swap(Vector& other) noexcept {
        std::swap(data_, other.data_);
        std::swap(size_, other.size_);
    } 
    
    void Resize(size_t new_size) {
        if (new_size == size_) {
            return;
        }
        if (new_size > size_) {
            Reserve(new_size);
            std::uninitialized_value_construct_n(data_.GetAddress() + size_, new_size - size_);
            size_ = new_size;
        }
        else {
            std::destroy_n(data_.GetAddress() + new_size, size_ - new_size);
            size_ = new_size;
        }
    }

    void Reserve(size_t new_capacity) {
        if (new_capacity <= data_.Capacity()) {
            return;
        }
        RawMemory<T> new_data(new_capacity);
        RefillData(new_data);
    }

    void PushBack(const T& value) {
        if (size_ == Capacity()) {
            RawMemory<T> new_data(NewRawDataSize());
            new (new_data + size_) T(value);
            RefillData(new_data);
        }
        else {
            new (data_ + size_) T(value);
        }
        ++size_;
    }

    void PushBack(T&& value) {
        if (size_ == Capacity()) {
            RawMemory<T> new_data(NewRawDataSize());
            new (new_data + size_) T(std::move(value));
            RefillData(new_data);
        }
        else {
            new (data_ + size_) T(std::move(value));
        }
        ++size_;
    }

    void PopBack(){
        std::destroy_n(data_.GetAddress() + size_ - 1, 1);
        --size_;
    }

    template <typename... Args>
    T& EmplaceBack(Args&&... args) {
        if (size_ == Capacity()) {
            RawMemory<T> new_data (NewRawDataSize());
            new (new_data + size_) T(std::forward<Args>(args)...);
            RefillData(new_data);
        }
        else {
            new (data_ + size_) T(std::forward<Args>(args)...);
        }
        ++size_;
        return data_[size_ - 1];
    }

    iterator begin() noexcept {
        return data_.GetAddress();
    }
    iterator end() noexcept {
        return data_.GetAddress() + size_;
    }
    const_iterator begin() const noexcept {
        return const_cast<Vector&>(*this).begin();
    }
    const_iterator end() const noexcept {
        return const_cast<Vector&>(*this).end();
    }
    const_iterator cbegin() const noexcept {
        return const_cast<Vector&>(*this).begin();
    }
    const_iterator cend() const noexcept {
        return const_cast<Vector&>(*this).end();
    }

    template <typename... Args>
    iterator Emplace(const_iterator pos, Args&&... args) {
        size_t position = pos - cbegin();
       
        if (size_ == Capacity()) {
            RawMemory<T> new_data(NewRawDataSize());
            new (new_data + position) T(std::forward<Args>(args)...);
            size_t quantity = position;
            CopyOrMoveData(new_data, 0, quantity, 0);
            quantity = size_ - position;
            CopyOrMoveData(new_data, position, quantity, position + 1);
            ReplacingOldMemory(new_data);
        }
        else {
            if (pos == cend()) {
                new(end()) T(std::forward<Args>(args)...);
            }
            else{
                if (end() != begin()) {
                    T temp(std::forward<Args>(args)...);
                    new(end()) T(std::forward<T>(*(end() - 1)));
                    std::move_backward(begin() + position, end() - 1, end());
                    data_[position] = std::forward<T>(temp);
                }
                else {
                    new (data_.GetAddress()) T(std::forward<Args>(args)...);
                }
            }
        }
        ++size_;
        return begin() + position;
    }
    iterator Erase(const_iterator pos){
        size_t position = pos - begin();
        std::move(begin() + position + 1, end(), begin() + position);
        std::destroy_n(end()-1, 1);
        --size_;
        return begin() + position;
    }

    iterator Insert(const_iterator pos, const T& value) {
        return Emplace(pos, std::forward<const T>(value));
    }

    iterator Insert(const_iterator pos, T&& value) {       
        return Emplace(pos, std::forward<T>(value));
    }

    ~Vector() {
        std::destroy_n(data_.GetAddress(), size_);
    }

private:
    RawMemory<T> data_;
    size_t size_ = 0;

    size_t NewRawDataSize() {
        size_t new_capacity = size_;
        size_ == 0 ? ++new_capacity : new_capacity = size_ * 2;
        return new_capacity;
    }

    void RefillData(RawMemory<T>& new_data) {
        try {
            CopyOrMoveData(new_data, 0, size_, 0);
            ReplacingOldMemory(new_data);
        }
        catch(...){
            throw;
        }
    }

    void CopyOrMoveData(RawMemory<T>& new_data, size_t from, size_t quantity, size_t dest_from) {
        if constexpr (std::is_nothrow_move_constructible_v<T> || !std::is_copy_constructible_v<T>) {
            std::uninitialized_move_n(data_.GetAddress() + from, quantity, new_data.GetAddress() + dest_from);
        }
        else {
            std::uninitialized_copy_n(data_.GetAddress() + from, quantity, new_data.GetAddress() + dest_from);
        }
    }

    void ReplacingOldMemory(RawMemory<T>& new_data) {
    	// Разрушаем элементы в data_
        std::destroy_n(data_.GetAddress(), size_);
        // Избавляемся от старой сырой памяти, обменивая её на новую
        data_.Swap(new_data);
        // При выходе из метода старая память будет возвращена в кучу
    }

};
