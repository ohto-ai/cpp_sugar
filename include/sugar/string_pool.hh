#pragma once

#ifndef OHTOAI_SUGAR_STRING_POOL_HH
#define OHTOAI_SUGAR_STRING_POOL_HH

#include <string>
#include <unordered_set>

namespace ai::sugar {
    // Interns strings so that equal strings share a single allocation.
    // Returned string_view objects remain valid for the lifetime of the pool.
    class string_pool {
    public:
        explicit string_pool(size_t initial_capacity = 1024) {
            pool_.reserve(initial_capacity);
        }

        // Insert str into the pool (if not already present) and return a stable view of it
        std::string_view intern(std::string_view str) {
            return *pool_.emplace(str).first;
        }

    private:
        // Transparent hash/equality so the set can be looked up with string_view
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

        std::unordered_set<std::string, string_hash, string_equal> pool_;
    };

} // namespace ai::sugar

#endif // !OHTOAI_SUGAR_STRING_POOL_HH
