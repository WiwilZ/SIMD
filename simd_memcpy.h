/*
 * Created by WiwilZ on 2022/2/14.
 */

#pragma once


#include <bit>
#include <cstdint>
#include <x86intrin.h>


constexpr void* avx512_memcpy(void* __restrict dst, const void* __restrict src, size_t size) noexcept __attribute__((__nonnull__(1, 2)));


static __inline void __attribute__((__always_inline__)) copy1B(std::byte* dst, const std::byte* src) noexcept {
    *dst = *src;
}

static __inline void __attribute__((__always_inline__)) copy2B(std::byte* dst, const std::byte* src) noexcept {
    *reinterpret_cast<uint16_t*>(dst) = *reinterpret_cast<const uint16_t*>(src);
}

static __inline void __attribute__((__always_inline__)) copy4B(std::byte* dst, const std::byte* src) noexcept {
    *reinterpret_cast<uint32_t*>(dst) = *reinterpret_cast<const uint32_t*>(src);
}

static __inline void __attribute__((__always_inline__)) copy8B(std::byte* dst, const std::byte* src) noexcept {
    *reinterpret_cast<uint64_t*>(dst) = *reinterpret_cast<const uint64_t*>(src);
}

static __inline void __attribute__((__always_inline__)) copy16B(std::byte* dst, const std::byte* src) noexcept {
    *(__uint128_t*) dst = *(__uint128_t*) src;
    _mm_storeu_si128(reinterpret_cast<__m128i*>(dst), _mm_lddqu_si128(reinterpret_cast<const __m128i*>(src)));
}

static __inline void __attribute__((__always_inline__)) copy32B(std::byte* dst, const std::byte* src) noexcept {
    _mm256_storeu_si256(reinterpret_cast<__m256i*>(dst), _mm256_lddqu_si256(reinterpret_cast<const __m256i*>(src)));
}

static __inline void __attribute__((__always_inline__)) copy64B(std::byte* dst, const std::byte* src) noexcept {
    _mm512_storeu_si512(dst, _mm512_loadu_si512(src));
}

#define LOAD(i) __m512i zmm##i = _mm512_loadu_si512(reinterpret_cast<const __m512i*>(src) + i);
#define STORE(i) _mm512_storeu_si512(reinterpret_cast<__m512i*>(dst) + i, zmm##i);

static __inline void __attribute__((__always_inline__)) copy128B(std::byte* dst, const std::byte* src) noexcept {
    LOAD(0)
    LOAD(1)
    STORE(0)
    STORE(1)
}

static __inline void __attribute__((__always_inline__)) copy256B(std::byte* dst, const std::byte* src) noexcept {
    LOAD(0)
    LOAD(1)
    LOAD(2)
    LOAD(3)
    STORE(0)
    STORE(1)
    STORE(2)
    STORE(3)
}

static __inline void __attribute__((__always_inline__)) copy512B(std::byte* dst, const std::byte* src) noexcept {
    LOAD(0)
    LOAD(1)
    LOAD(2)
    LOAD(3)
    LOAD(4)
    LOAD(5)
    LOAD(6)
    LOAD(7)
    STORE(0)
    STORE(1)
    STORE(2)
    STORE(3)
    STORE(4)
    STORE(5)
    STORE(6)
    STORE(7)
}

static __inline void __attribute__((__always_inline__)) copy1024B(std::byte* dst, const std::byte* src) noexcept {
    LOAD(0)
    LOAD(1)
    LOAD(2)
    LOAD(3)
    LOAD(4)
    LOAD(5)
    LOAD(6)
    LOAD(7)
    LOAD(8)
    LOAD(9)
    LOAD(10)
    LOAD(11)
    LOAD(12)
    LOAD(13)
    LOAD(14)
    LOAD(15)
    STORE(0)
    STORE(1)
    STORE(2)
    STORE(3)
    STORE(4)
    STORE(5)
    STORE(6)
    STORE(7)
    STORE(8)
    STORE(9)
    STORE(10)
    STORE(11)
    STORE(12)
    STORE(13)
    STORE(14)
    STORE(15)
}
// AVX-512有32个寄存器
static __inline void __attribute__((__always_inline__)) copy2048B(std::byte* dst, const std::byte* src) noexcept {
    LOAD(0)
    LOAD(1)
    LOAD(2)
    LOAD(3)
    LOAD(4)
    LOAD(5)
    LOAD(6)
    LOAD(7)
    LOAD(8)
    LOAD(9)
    LOAD(10)
    LOAD(11)
    LOAD(12)
    LOAD(13)
    LOAD(14)
    LOAD(15)
    LOAD(16)
    LOAD(17)
    LOAD(18)
    LOAD(19)
    LOAD(20)
    LOAD(21)
    LOAD(22)
    LOAD(23)
    LOAD(24)
    LOAD(25)
    LOAD(26)
    LOAD(27)
    LOAD(28)
    LOAD(29)
    LOAD(30)
    LOAD(31)
    STORE(0)
    STORE(1)
    STORE(2)
    STORE(3)
    STORE(4)
    STORE(5)
    STORE(6)
    STORE(7)
    STORE(8)
    STORE(9)
    STORE(10)
    STORE(11)
    STORE(12)
    STORE(13)
    STORE(14)
    STORE(15)
    STORE(16)
    STORE(17)
    STORE(18)
    STORE(19)
    STORE(20)
    STORE(21)
    STORE(22)
    STORE(23)
    STORE(24)
    STORE(25)
    STORE(26)
    STORE(27)
    STORE(28)
    STORE(29)
    STORE(30)
    STORE(31)
}

#undef STORE
#undef LOAD


constexpr void* avx512_memcpy(void* __restrict dst, const void* __restrict src, size_t size) noexcept {
    if (dst == src) {
        return dst;
    }

    auto p = static_cast<std::byte*>(dst);
    auto q = static_cast<const std::byte*>(src);

    while (size >= 2048) {
        copy2048B(p, q);
        p += 2048;
        q += 2048;
        size -= 2048;
    }
    if (size & 1024) {
        copy1024B(p, q);
        p += 1024;
        q += 1024;
    }
    if (size & 512) {
        copy512B(p, q);
        p += 512;
        q += 512;
    }
    if (size & 256) {
        copy256B(p, q);
        p += 256;
        q += 256;
    }
    if (size & 128) {
        copy128B(p, q);
        p += 128;
        q += 128;
    }
    if (size & 64) {
        copy64B(p, q);
        p += 64;
        q += 64;
    }
    if (size & 32) {
        copy32B(p, q);
        p += 32;
        q += 32;
    }
    if (size & 16) {
        copy16B(p, q);
        p += 16;
        q += 16;
    }
    if (size & 8) {
        copy8B(p, q);
        p += 8;
        q += 8;
    }
    if (size & 4) {
        copy4B(p, q);
        p += 4;
        q += 4;
    }
    if (size & 2) {
        copy2B(p, q);
        p += 2;
        q += 2;
    }
    if (size & 1) {
        copy1B(p, q);
    }

    return dst;
}