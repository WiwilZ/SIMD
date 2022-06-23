/*
 * Created by WiwilZ on 2022/6/12.
 */

#pragma once

#include "copy_forwards.h"
#include "copy_backwards.h"


constexpr void* simd_memory_copy(void* dest, const void* src, size_t size) noexcept {
    if (dest != src) {
        auto pd = static_cast<std::byte*>(dest);
        auto ps = static_cast<const std::byte*>(src);
        if (ps < pd && pd < ps + size) {
            copy_backwards(pd, ps, size);
        } else {
            copy_forwards(pd, ps, size);
        }
    }
    return dest;
}
