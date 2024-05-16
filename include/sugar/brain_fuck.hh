#pragma once

#ifndef OHTOAI_SUGAR_BRAIN_FUCK_HH
#define OHTOAI_SUGAR_BRAIN_FUCK_HH

#include <iostream>
#include <type_traits>

namespace ohtoai::sugar {
    namespace funny::bf {
        template<size_t N>
        class Stream {
        public:
            constexpr void push(char c) { data_[idx_++] = c; }
            constexpr operator const char* () const { return data_; }
            constexpr size_t size() { return idx_; }
        private:
            size_t idx_{};
            char data_[N]{};
        };

        template<typename STREAM>
        constexpr auto parse(const char* input, bool skip, char* cells,
            size_t& pc, STREAM&& output) -> size_t {
            const char* c = input;
            while (*c) {
                switch (*c) {
                case '+': if (!skip) ++cells[pc]; break;
                case '-': if (!skip) --cells[pc]; break;
                case '.': if (!skip) output.push(cells[pc]); break;
                case '>': if (!skip) ++pc; break;
                case '<': if (!skip) --pc; break;
                case '[': {
                    while (!skip && cells[pc] != 0)
                        parse(c + 1, false, cells, pc, output);
                    c += parse(c + 1, true, cells, pc, output) + 1;
                } break;
                case ']': return c - input;
                default: break;
                }
                ++c;
            }
            return c - input;
        }

        constexpr size_t CELL_SIZE = 16;
        template<typename STREAM>
        constexpr auto parse(const char* input, STREAM&& output) -> STREAM&& {
            char cells[CELL_SIZE]{};
            size_t pc{};
            parse(input, false, cells, pc, output);
            return output;
        }

        template<size_t OUTPUT_SIZE = 15>
        constexpr auto brain_fuck(const char* input) {
            Stream<OUTPUT_SIZE> output;
            return parse(input, output);
        }

        constexpr auto brain_fuck_output_size(const char* input) -> size_t {
            struct {
                size_t sz{};
                constexpr void push(...) { ++sz; }
            } dummy;
            return parse(input, dummy).sz + 1;
        }
    }
}

#endif // !OHTOAI_SUGAR_BRAIN_FUCK_HH