/*
 * Created by WiwilZ on 2022/6/12.
 */

#pragma once

#include <bit>
#include <array>
#include "copy_specific_bytes.h"

namespace my {
    template <size_t I, int Dest, int Src>
    constexpr void Copy_forwards_left_single(std::byte*& pd, const std::byte*& ps, size_t size) noexcept {
        if (size & I) {
            copy_bytes<I, Dest, Src>(pd, ps);
            pd += I;
            ps += I;
        }
    }

    template <int Dest, int Src, size_t... Is>
    constexpr void Copy_forwards_left(std::byte* pd, const std::byte* ps, size_t size) noexcept {
        (Copy_forwards_left_single<Is, Dest, Src>(pd, ps, size), ...);
    }

    template <int Dest, int Src>
    constexpr void Copy_forwards(std::byte* pd, const std::byte* ps, size_t size) noexcept {
        while (size >= 2048) {
            copy_bytes<2048, Dest, Src>(pd, ps);
            pd += 2048;
            ps += 2048;
            size -= 2048;
        }
        Copy_forwards_left<Dest, Src, 1024, 512, 256, 128, 64, 32, 16, 8, 4, 2, 1>(pd, ps, size);
    }

    static auto _cf_funcs = std::to_array(
            {
                    _placeholder,
                    _placeholder,
                    Copy_forwards<-1, 0>,
                    Copy_forwards<-1, 1>,
                    Copy_forwards<0, -1>,
                    Copy_forwards<1, -1>,
                    Copy_forwards<0, 0>,
                    Copy_forwards<1, 1>
            }
    );
}

void copy_forwards(std::byte* pd, const std::byte* ps, size_t size) noexcept {
    auto pda = reinterpret_cast<std::byte*>((reinterpret_cast<uintptr_t>(pd) + 63) & ~63);
    auto psa = reinterpret_cast<const std::byte*>((reinterpret_cast<uintptr_t>(ps) + 63) & ~63);

    if (pd != pda && ps != psa) {
        size_t diff = pda - pd;
        my::Copy_forwards_left<-1, -1, 64, 32, 16, 8, 4, 2, 1>(pd, ps, diff);
        pd = pda;
        ps += diff;
    }

    size_t idx = (size_t{ pd == pda } << 2) | (size_t{ ps == psa } << 1) | (size > CACHE_SIZE);
    my::_cf_funcs[idx](pd, ps, size);
}
