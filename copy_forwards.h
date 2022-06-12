/*
 * Created by WiwilZ on 2022/6/12.
 */

#pragma once

#include <bit>
#include "copy_specific_bytes.h"

namespace my {
    template <size_t I, int Dest, int Src>
    inline constexpr void Copy_forwards_left_single(size_t size, std::byte*& p, const std::byte*& q) noexcept {
        if (size & I) {
            copy_bytes<I, Dest, Src>(p, q);
            p += I;
            q += I;
        }
    }

    template <int Dest, int Src, size_t... Is>
    inline constexpr void Copy_forwards_left(size_t size, std::byte* p, const std::byte* q) {
        (Copy_forwards_left_single<Is, Dest, Src>(size, p, q), ...);
    }

    template <int Dest, int Src>
    void Copy_forwards(std::byte* pd, const std::byte* ps, size_t size) {
        while (size >= 2048) {
            copy_bytes<2048, Dest, Src>(pd, ps);
            pd += 2048;
            ps += 2048;
            size -= 2048;
        }
        Copy_forwards_left<Dest, Src, 1024, 512, 256, 128, 64, 32, 16, 8, 4, 2, 1>(size, pd, ps);
    }
}

void copy_forwards(std::byte* pd, const std::byte* ps, size_t size) {
    auto pda = reinterpret_cast<std::byte*>((reinterpret_cast<uintptr_t>(pd) + 63) & ~63);
    auto psa = reinterpret_cast<const std::byte*>((reinterpret_cast<uintptr_t>(ps) + 63) & ~63);

    if (pd == pda && ps == psa) {
        if (size > CACHE_SIZE) {
            my::Copy_forwards<1, 1>(pd, ps, size);
        } else {
            my::Copy_forwards<0, 0>(pd, ps, size);
        }
    } else if (pd == pda && ps != psa) {
        if (size > CACHE_SIZE) {
            my::Copy_forwards<1, -1>(pd, ps, size);
        } else {
            my::Copy_forwards<0, -1>(pd, ps, size);
        }
    } else if (pd != pda && ps == psa) {
        if (size > CACHE_SIZE) {
            my::Copy_forwards<-1, 1>(pd, ps, size);
        } else {
            my::Copy_forwards<-1, 0>(pd, ps, size);
        }
    } else {
        size_t diff = pda - pd;
        size_t ceil_diff = std::bit_ceil(diff);
        if (size > CACHE_SIZE) {
            my::Copy_diff<64, 32, 16, 8, 4, 2, 1>(ceil_diff, pd, ps);
            pd = pda;
            ps += diff;
            my::Copy_forwards<1, -1>(pd, ps, size - diff);
        } else if (size > ceil_diff) {
            my::Copy_diff<64, 32, 16, 8, 4, 2, 1>(ceil_diff, pd, ps);
            pd = pda;
            ps += diff;
            my::Copy_forwards<0, -1>(pd, ps, size - diff);
        } else if (size == ceil_diff) {
            my::Copy_diff<64, 32, 16, 8, 4, 2, 1>(ceil_diff, pd, ps);
        } else {
            my::Copy_forwards_left<-1, -1, 32, 16, 8, 4, 2, 1>(size, pd, ps);
        }
    }
}
