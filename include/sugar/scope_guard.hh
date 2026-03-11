#pragma once

#ifndef OHTOAI_SUGAR_SCOPE_GUARD_HH
#define OHTOAI_SUGAR_SCOPE_GUARD_HH

#include <functional>
#include <tuple>
#include <type_traits>
#include <utility>

namespace ai::sugar {
    namespace detail {
#if __cplusplus >= 202002L
        // C++20 version: type-erased via std::function, supports arbitrary copy-constructible callables
        struct scope_guard {
            std::function<void()> f;

            template<typename Func, typename... Args>
                requires std::invocable<Func, std::unwrap_reference_t<Args>...>
            scope_guard(Func&& func, Args&&... args)
                : f{ [func = std::forward<Func>(func), ...args = std::forward<Args>(args)]() mutable {
                    std::invoke(std::forward<std::decay_t<Func>>(func),
                        std::unwrap_reference_t<Args>(std::forward<Args>(args))...);
                } } {}

            ~scope_guard() { f(); }
            scope_guard(const scope_guard&) = delete;
            scope_guard& operator=(const scope_guard&) = delete;
        };
#elif __cplusplus >= 201703L
        // C++17 version: stores callable and arguments directly in a tuple
        template<typename F, typename... Args>
        struct scope_guard {
            F f;
            std::tuple<Args...> values;

            template<typename Fn, typename... Ts>
            scope_guard(Fn&& func, Ts&&... args)
                : f{ std::forward<Fn>(func) }, values{ std::forward<Ts>(args)... } {}

            ~scope_guard() { std::apply(f, values); }
            scope_guard(const scope_guard&) = delete;
            scope_guard& operator=(const scope_guard&) = delete;
        };

        template<typename F, typename... Args>
        scope_guard(F&&, Args&&...) -> scope_guard<std::decay_t<F>, std::decay_t<Args>...>;
#else
        using scope_guard = void;
#endif
    } // namespace detail

    using detail::scope_guard;
} // namespace ai::sugar

#endif // !OHTOAI_SUGAR_SCOPE_GUARD_HH
