#pragma once

#ifndef OHTOAI_SUGAR_STRING_POOL_HH
#define OHTOAI_SUGAR_STRING_POOL_HH

#include <string>
#include <unordered_set>

namespace ai::sugar {
    class string_pool {
    public:
        string_pool(size_t initial_capacity = 1024) {
            pool.reserve(initial_capacity);
        }

        std::string_view intern(const std::string& str) {
            return *pool.emplace(str).first;
        }

        std::string_view intern(std::string_view str) {
            return *pool.emplace(std::string(str)).first;
        }

        std::string_view intern(const char* str) {
            return *pool.emplace(str).first;
        }
    private:
        struct string_hash {
            using is_transparent = void;
            std::size_t operator()(std::string_view sv) const noexcept {
                return std::hash<std::string_view>{}(sv);
            }
        };

        struct string_equal {
            using is_transparent = void;
            bool operator()(std::string_view lhs, std::string_view rhs) const noexcept {
                return lhs == rhs;
            }
        };
    private:
        std::unordered_set<std::string, string_hash, string_equal> pool;
    };

}

#endif // !OHTOAI_SUGAR_STRING_POOL_HH
