#pragma once

#ifndef OHTOAI_SUGAR_FNV1A_HH
#define OHTOAI_SUGAR_FNV1A_HH

#include <cstdint>

namespace ai::sugar {
    // FNV-1a (Fowler–Noll–Vo) hash functions — compile-time capable
    namespace hash::fnv1a {
        // 32-bit FNV-1a hash of a null-terminated string
        constexpr std::uint32_t fnv1a_32(const char* s, std::uint32_t h = 0x811c9dc5) {
            return (*s == 0) ? h : fnv1a_32(s + 1, (h ^ static_cast<std::uint32_t>(*s)) * 0x01000193U);
        }
    } // namespace hash::fnv1a
} // namespace ai::sugar

#endif // OHTOAI_SUGAR_FNV1A_HH
