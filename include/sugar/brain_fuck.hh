#pragma once

#ifndef OHTOAI_SUGAR_BRAIN_FUCK_HH
#define OHTOAI_SUGAR_BRAIN_FUCK_HH

#include <cstddef>

namespace ai::sugar {
    // Compile-time BrainFuck interpreter (constexpr)
    namespace funny::bf {
        // Fixed-size output buffer used to collect BrainFuck output characters
        template<size_t N>
        class Stream {
        public:
            constexpr void push(char c) { data_[idx_++] = c; }
            constexpr operator const char*() const { return data_; }
            constexpr size_t size() const { return idx_; }
        private:
            size_t idx_{};
            char data_[N]{};
        };

        // Number of BrainFuck memory cells
        constexpr size_t CELL_SIZE = 16;

        // Internal recursive parser; skip=true fast-forwards past a loop body
        template<typename Stream>
        constexpr auto parse(const char* input, bool skip, char* cells,
            size_t& pc, Stream&& output) -> size_t {
            const char* c = input;
            while (*c) {
                switch (*c) {
                case '+': if (!skip) ++cells[pc]; break;
                case '-': if (!skip) --cells[pc]; break;
                case '.': if (!skip) output.push(cells[pc]); break;
                case '>': if (!skip) ++pc; break;
                case '<': if (!skip) --pc; break;
                case '[': {
                    // Execute loop body repeatedly until current cell is zero
                    while (!skip && cells[pc] != 0)
                        parse(c + 1, false, cells, pc, output);
                    // Skip over the matching ']' after the loop
                    c += parse(c + 1, true, cells, pc, output) + 1;
                } break;
                case ']': return c - input; // Return offset of ']' to caller
                default: break;
                }
                ++c;
            }
            return c - input;
        }

        // Top-level parse entry point: initialises cells and runs the program
        template<typename Stream>
        constexpr auto parse(const char* input, Stream&& output) -> Stream&& {
            char cells[CELL_SIZE]{};
            size_t pc{};
            parse(input, false, cells, pc, output);
            return output;
        }

        // Run a BrainFuck program and return the output as a fixed-size Stream.
        // OUTPUT_SIZE must be large enough to hold all output characters (including '\0').
        // Use brain_fuck_output_size() to compute the required size at compile time.
        template<size_t OUTPUT_SIZE = 15>
        constexpr auto brain_fuck(const char* input) {
            Stream<OUTPUT_SIZE> output;
            return parse(input, output);
        }

        // Compute the number of output characters produced by a BrainFuck program (+1 for '\0')
        constexpr auto brain_fuck_output_size(const char* input) -> size_t {
            struct {
                size_t sz{};
                constexpr void push(...) { ++sz; }
            } dummy;
            return parse(input, dummy).sz + 1;
        }
    } // namespace funny::bf
} // namespace ai::sugar

#endif // !OHTOAI_SUGAR_BRAIN_FUCK_HH