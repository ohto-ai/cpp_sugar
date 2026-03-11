#pragma once

#ifndef OHTOAI_SUGAR_PIPE_HH
#define OHTOAI_SUGAR_PIPE_HH

#include <ostream>
#include <algorithm>
#include <functional>
#include <iterator>
#include <sstream>

namespace ai::sugar {
    // Pipe-style functional composition utilities.
    // Usage: value | op1 | op2 | ...
    namespace pipe {
        // Generic pipe: passes arg as the sole argument to fun
        template <typename Arg, typename Fun>
        constexpr auto operator|(Arg&& arg, Fun&& fun)
            -> decltype(std::invoke(std::forward<Fun>(fun), std::forward<Arg>(arg))) {
            return std::invoke(std::forward<Fun>(fun), std::forward<Arg>(arg));
        }

        // Pipe overload for std::ostream manipulators (e.g. os | std::endl)
        template <typename CharT, typename Traits>
        constexpr auto operator|(std::basic_ostream<CharT, Traits>& os,
            std::basic_ostream<CharT, Traits>& (*manip)(std::basic_ostream<CharT, Traits>&))
            -> decltype(std::invoke(manip, os)) {
            return std::invoke(manip, os);
        }

        // Keep only elements satisfying predicate f
        template <typename F>
        auto filter(F&& f) {
            return [f = std::forward<F>(f)](auto v) {
                auto it = std::remove_if(std::begin(v), std::end(v), std::not_fn(f));
                v.erase(it, std::end(v));
                return v;
            };
        }

        // Apply f to every element (returns a copy with f applied)
        template <typename F>
        auto for_each(F&& f) {
            return [f = std::forward<F>(f)](auto v) {
                std::for_each(std::begin(v), std::end(v), f);
                return v;
            };
        }

        // Transform elements using f, writing results through iter.
        // Note: iter may alias the input range (e.g. v.begin()), in which case
        // the range is transformed in-place and the (modified) range is returned.
        template <typename Iter, typename F>
        auto transform(Iter iter, F&& f) {
            return [iter, f = std::forward<F>(f)](auto&& v) {
                std::transform(std::begin(v), std::end(v), iter, f);
                return v;
            };
        }

        // Returns true if f holds for all elements
        template <typename F>
        auto all_of(F&& f) {
            return [f = std::forward<F>(f)](auto&& v) {
                return std::all_of(std::begin(v), std::end(v), f);
            };
        }

        // Returns true if f holds for at least one element
        template <typename F>
        auto any_of(F&& f) {
            return [f = std::forward<F>(f)](auto&& v) {
                return std::any_of(std::begin(v), std::end(v), f);
            };
        }

        // Returns true if f holds for no element
        template <typename F>
        auto none_of(F&& f) {
            return [f = std::forward<F>(f)](auto&& v) {
                return std::none_of(std::begin(v), std::end(v), f);
            };
        }

        // Return a reversed copy of the range
        inline auto reverse = [](auto v) {
            std::reverse(std::begin(v), std::end(v));
            return v;
        };

        // Return a sorted copy of the range
        inline auto sort = [](auto v) {
            std::sort(std::begin(v), std::end(v));
            return v;
        };

        // Return a copy with consecutive duplicates removed (requires sorted input for full dedup)
        inline auto unique = [](auto v) {
            auto it = std::unique(std::begin(v), std::end(v));
            v.erase(it, std::end(v));
            return v;
        };

        // Copy elements to output iterator out, then pass the range through unchanged
        template <typename Out>
        auto copy(Out out) {
            return [out](auto&& v) {
                std::copy(std::begin(v), std::end(v), out);
                return v;
            };
        }

        // Side-effect wrapper: call f (with or without the range value) then return the range unchanged
        inline auto nop = [](auto&& f) {
            return [f = std::forward<decltype(f)>(f)](auto&& v) {
                if constexpr (std::is_invocable_v<decltype(f), decltype(v)>)
                    std::invoke(f, v);
                else
                    std::invoke(f);
                return v;
            };
        };

        // Spread a tuple/array as individual arguments to a callable
        inline auto tuple_as_args = [](auto&& f) {
            return [f = std::forward<decltype(f)>(f)](auto&& t) {
                return std::apply(f, t);
            };
        };

        // Parse whitespace-separated tokens of type T from a string into a vector
        template<typename T>
        inline auto read_vector = [](const std::string& s) {
            std::istringstream iss{s};
            return std::vector<T>(std::istream_iterator<T>{iss}, std::istream_iterator<T>{});
        };

        // Join string elements of a range with sep as separator
        inline auto join = [](const std::string& sep = "") {
            return [sep](const auto& v) {
                std::ostringstream oss;
                if (!v.empty()) {
                    std::copy(std::begin(v), std::end(v) - 1,
                        std::ostream_iterator<std::string>(oss, sep.c_str()));
                    oss << v.back();
                }
                return oss.str();
            };
        };

        // Compose multiple pipe operations into a single reusable packet
        template<typename... Ops>
        auto make_packet(Ops&&... ops) {
            return [=](auto&& arg) {
                return (arg | ... | ops);
            };
        }
    } // namespace pipe
} // namespace ai::sugar

#endif // !OHTOAI_SUGAR_PIPE_HH