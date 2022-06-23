/*
 * Created by WiwilZ on 2022/6/12.
 */

#pragma once

#include <bit>
#include "copy_specific_bytes.h"

namespace my {
    template <size_t I, int Dest, int Src>
    constexpr void Copy_backwards_left_single(std::byte*& pd, const std::byte*& ps, size_t size) noexcept {
        if (size & I) {
            pd -= I;
            ps -= I;
            copy_bytes<I, Dest, Src>(pd, ps);
        }
    }

    template <int Dest, int Src, size_t... Is>
    constexpr void Copy_backwards_left(std::byte* pd, const std::byte* ps, size_t size) noexcept {
        (Copy_backwards_left_single<Is, Dest, Src>(pd, ps, size), ...);
    }

    template <int Dest, int Src>
    constexpr void Copy_backwards(std::byte* pd, const std::byte* ps, size_t size) noexcept {
        while (size >= 2048) {
            pd -= 2048;
            ps -= 2048;
            size -= 2048;
            copy_bytes<2048, Dest, Src>(pd, ps);
        }
        Copy_backwards_left<Dest, Src, 1024, 512, 256, 128, 64, 32, 16, 8, 4, 2, 1>(pd, ps, size);
    }

    static auto _bf_funcs = std::to_array(
            {
                    _placeholder,
                    _placeholder,
                    Copy_backwards<-1, 0>,
                    Copy_backwards<-1, 1>,
                    Copy_backwards<0, -1>,
                    Copy_backwards<1, -1>,
                    Copy_backwards<0, 0>,
                    Copy_backwards<1, 1>
            }
    );
}

void copy_backwards(std::byte* pd, const std::byte* ps, size_t size) noexcept {
    pd += size;
    ps += size;

    auto pda = reinterpret_cast<std::byte*>((reinterpret_cast<uintptr_t>(pd) - 63) & ~63);
    auto psa = reinterpret_cast<const std::byte*>((reinterpret_cast<uintptr_t>(ps) - 63) & ~63);

    if (pd != pda && ps != psa) {
        size_t diff = pd - pda;
        my::Copy_backwards_left<-1, -1, 64, 32, 16, 8, 4, 2, 1>(pd, ps, diff);
        pd = pda;
        ps -= diff;
    }

    size_t idx = (size_t{ pd == pda } << 2) | (size_t{ ps == psa } << 1) | (size > CACHE_SIZE);
    my::_bf_funcs[idx](pd, ps, size);
}