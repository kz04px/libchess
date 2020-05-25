#ifndef TT_HPP
#define TT_HPP

#include <cstdint>
#include <cstring>

template <class T>
class TT {
   public:
    TT(unsigned int mb) : filled_{0} {
        if (mb < 0) {
            mb = 1;
        }
        max_entries_ = (mb * 1024 * 1024) / sizeof(T);
        entries_ = new T[max_entries_];
    }

    ~TT() {
        delete entries_;
    }

    [[nodiscard]] T poll(const std::uint64_t hash) const noexcept {
        const auto idx = index(hash);
        return entries_[idx];
    }

    void add(const std::uint64_t hash, const T &t) noexcept {
        const auto idx = index(hash);
        filled_ += (entries_[idx].hash == 0 ? 1 : 0);
        entries_[idx] = t;
    }

    [[nodiscard]] std::size_t size() const noexcept {
        return max_entries_;
    }

    void clear() noexcept {
        filled_ = 0;
        std::memset(entries_, 0, max_entries_ * sizeof(T));
    }

    [[nodiscard]] int hashfull() const noexcept {
        return 1000 * (static_cast<double>(filled_) / max_entries_);
    }

    void prefetch(const std::uint64_t hash) const noexcept {
        const auto idx = index(hash);
        __builtin_prefetch(&entries_[idx]);
    }

   private:
    [[nodiscard]] std::size_t index(const std::uint64_t hash) const noexcept {
        return hash % max_entries_;
    }

    std::size_t max_entries_;
    std::size_t filled_;
    T *entries_;
};

#endif
