#include <algorithm>
#include <cstddef>
#include <memory>
#include <tuple>

template<typename T, typename Deleter = std::default_delete<T>>
class UniquePtr {
public:
    UniquePtr() noexcept: ptr_(nullptr, Deleter()) {}

    UniquePtr(T *elem) noexcept: ptr_(elem, Deleter()) {}

    UniquePtr(T *elem, Deleter d) noexcept: ptr_(elem, d) {}

    UniquePtr(UniquePtr &&u_ptr) noexcept {
        ptr_ = std::move(u_ptr.ptr_);
        u_ptr.ptr_ = {nullptr, Deleter()};
    }

    Deleter &get_deleter() noexcept {
        return std::get<1>(ptr_);
    }

    const Deleter &get_deleter() const noexcept {
        return std::get<1>(ptr_);
    }

    UniquePtr &operator=(std::nullptr_t) noexcept {
        std::get<1>(ptr_)(std::get<0>(ptr_));
        std::get<0>(ptr_) = nullptr;
        return *this;
    }

    UniquePtr &operator=(UniquePtr &&u_ptr) noexcept {
        std::swap(ptr_, u_ptr.ptr_);
        return *this;
    }

    ~UniquePtr() {
        std::get<1>(ptr_)(std::get<0>(ptr_));
    }

    const T &operator*() const {
        return *std::get<0>(ptr_);
    }

    T *operator->() const noexcept {
        return std::get<0>(ptr_);
    }

    T *release() noexcept {
        T *temp_ptr = std::get<0>(ptr_);
        std::get<0>(ptr_) = nullptr;
        return temp_ptr;
    }

    void reset(T *that) noexcept {
        std::get<1>(ptr_)(std::get<0>(ptr_));
        std::get<0>(ptr_) = that;
    }

    void swap(UniquePtr &other) noexcept {
        std::swap(ptr_, other.ptr_);
    }

    T *get() const noexcept {
        return std::get<0>(ptr_);
    }

    explicit operator bool() const noexcept {
        return !(std::get<0>(ptr_) == nullptr);
    }

    UniquePtr(UniquePtr &) = delete;

    UniquePtr &operator=(UniquePtr &) = delete;

private:
    std::tuple<T *, Deleter> ptr_;
};
