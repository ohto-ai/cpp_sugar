#pragma once

#ifndef OHTOAI_SUGER_PIPE_HH
#define OHTOAI_SUGER_PIPE_HH

#include <ostream>
#include <algorithm>
#include <functional>

namespace ohtoai::sugar {
namespace pipe {
    // operator|
    template <typename Arg, typename Fun>
    constexpr auto operator|(Arg&& arg, Fun&& fun)
        -> decltype(std::invoke(std::forward<Fun>(fun), std::forward<Arg>(arg))) {
        return std::invoke(std::forward<Fun>(fun), std::forward<Arg>(arg));
    }

    // operator| for std::basic_ostream
    template <typename CharT, typename Traits>
    constexpr auto operator|(std::basic_ostream<CharT, Traits>& os, std::basic_ostream<CharT, Traits>& (*manip)(std::basic_ostream<CharT, Traits>&))
        -> decltype(std::invoke(manip, os)) {
        return std::invoke(manip, os);
    }

    // filter
    template <typename F>
    auto filter(F&& f) {
        return [f = std::forward<decltype(f)>(f)](auto&& v) {
            auto vc = v;
            //auto it = std::remove_if(std::begin(vc), std::end(vc), std::not_fn(f));
            auto it = std::remove_if(std::begin(vc), std::end(vc), std::not_fn(f));
            vc.erase(it, std::end(vc));
            return vc;
            };
    }

    // for_each
    template <typename F>
    auto for_each(F&& f) {
        return [f = std::forward<decltype(f)>(f)](auto&& v) {
            auto vc = v;
            std::for_each(std::begin(vc), std::end(vc), f);
            return vc;
            };
    }

    // transform
    template <typename Iter, typename F>
    auto transform(Iter iter, F&& f) {
        return [iter, f = std::forward<decltype(f)>(f)](auto&& v) {
            std::transform(std::begin(v), std::end(v), iter, f);
            return v;
            };
    }

    // all_of
    template <typename F>
    auto all_of(F&& f) {
        return [f = std::forward<decltype(f)>(f)](auto&& v) {
            return std::all_of(std::begin(v), std::end(v), f);
            };
    }

    // any_of
    template <typename F>
    auto any_of(F&& f) {
        return [f = std::forward<decltype(f)>(f)](auto&& v) {
            return std::any_of(std::begin(v), std::end(v), f);
            };
    }

    // none_of
    template <typename F>
    auto none_of(F&& f) {
        return [f = std::forward<decltype(f)>(f)](auto&& v) {
            return std::none_of(std::begin(v), std::end(v), f);
            };
    }

    // reverse
    auto reverse = [](auto&& v) {
        auto vc = v;
        std::reverse(std::begin(vc), std::end(vc));
        return vc;
        };

    // sort
    auto sort = [](auto&& v) {
        auto vc = v;
        std::sort(std::begin(vc), std::end(vc));
        return vc;
        };

    // unique
    auto unique = [](auto&& v) {
        auto vc = v;
        auto it = std::unique(std::begin(vc), std::end(vc));
        vc.erase(it, std::end(vc));
        return vc;
        };

    // copy
    template <typename Out>
    auto copy(Out out) {
        return [out = std::forward<decltype(out)>(out)](auto&& v) {
            std::copy(std::begin(v), std::end(v), out);
            return v;
            };
    }

    // nop
    auto nop = [](auto&& f) {
        return [f = std::forward<decltype(f)>(f)](auto&& v) {
            // 如果f含有参数，则调用f(v)，否则调用f()
            if constexpr (std::is_invocable_v<decltype(f), decltype(v)>) {
                std::invoke(f, v);
            }
            else {
                std::invoke(f);
            }
            return v;
            };
        };

    // make_packet
    template<typename... Ops>
    auto make_packet(Ops&&... ops) {
        return [=](auto&& arg) {
            return (arg | ... | ops);
            };
    }

    // tuple_as_args
    auto tuple_as_args = [](auto&& f) {
        return [f = std::forward<decltype(f)>(f)](auto&& t) {
            return std::apply(f, t);
            };
        };

    // read_vector
    template<typename T>
    auto read_vector = [](const std::string& s) {
        std::istringstream iss{ s };
        std::vector<T> dst;
        std::copy(std::istream_iterator<T>{iss},
            std::istream_iterator<T>{},
            std::back_insert_iterator<std::vector<T>>(dst));
        return dst;
        };
} // namespace pipe

} // namespace ohtoai::sugar

#endif // !OHTOAI_SUGER_PIPE_HH