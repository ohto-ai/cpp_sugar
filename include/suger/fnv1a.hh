#pragma once

#ifndef OHTOAI_SUGER_FNV1A_HH
#define OHTOAI_SUGER_FNV1A_HH

namespace ohtoai::sugar {
    namespace hash::fnv1a {
        constexpr std::uint32_t fnv1a_32(const char* s, std::uint32_t h = 0x811c9dc5) {
            return (*s == 0) ? h : fnv1a_32(s + 1, (h ^ *s) * 0x01000193);
        }
    }
}

#endif // OHTOAI_SUGER_FNV1A_HH
