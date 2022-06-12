/*
 * Created by WiwilZ on 2022/6/11.
 */

#pragma once

#include <cstdint>
#include <x86intrin.h>


namespace my {
#define CACHE_SIZE (size_t{ 36608 } << 10)

    template <size_t N>
    struct UInt;

    template <>
    struct UInt<1> {
        using type = uint8_t;
    };

    template <>
    struct UInt<2> {
        using type = uint16_t;
    };

    template <>
    struct UInt<4> {
        using type = uint32_t;
    };

    template <>
    struct UInt<8> {
        using type = uint64_t;
    };

    template <size_t I, int Dest, int Src>
    inline constexpr void copy_bytes(void* dest, const void* src) noexcept {
        using type = typename UInt<I>::type;
        *static_cast<type*>(dest) = *static_cast<const type*>(src);
    }

/*--------------------------------stream stream-------------------------------*/
    template <>
    inline void copy_bytes<16, 1, 1>(void* dest, const void* src) noexcept {
        _mm_stream_si128(
                static_cast<__m128i*>(dest), _mm_stream_load_si128(static_cast<__m128i*>(const_cast<void*>(src))));
    }

    template <>
    inline void copy_bytes<32, 1, 1>(void* dest, const void* src) noexcept {
        _mm256_stream_si256(static_cast<__m256i*>(dest), _mm256_stream_load_si256(static_cast<const __m256i*>(src)));
    }

    template <>
    inline void copy_bytes<64, 1, 1>(void* dest, const void* src) noexcept {
        _mm512_stream_si512(static_cast<__m512i*>(dest), _mm512_stream_load_si512(const_cast<void*>(src)));
    }

#define LOAD(i) const __m512i zmm##i = _mm512_stream_load_si512(static_cast<__m512i*>(const_cast<void*>(src)) + i);
#define STORE(i) _mm512_stream_si512(static_cast<__m512i*>(dest) + i, zmm##i);

    template <>
    inline void copy_bytes<128, 1, 1>(void* dest, const void* src) noexcept {
        LOAD(0)
        LOAD(1)
        STORE(0)
        STORE(1)
    }

    template <>
    inline void copy_bytes<256, 1, 1>(void* dest, const void* src) noexcept {
        LOAD(0)
        LOAD(1)
        LOAD(2)
        LOAD(3)
        STORE(0)
        STORE(1)
        STORE(2)
        STORE(3)
    }

    template <>
    inline void copy_bytes<512, 1, 1>(void* dest, const void* src) noexcept {
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

    template <>
    inline void copy_bytes<1024, 1, 1>(void* dest, const void* src) noexcept {
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

    template <>
    inline void copy_bytes<2048, 1, 1>(void* dest, const void* src) noexcept {
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
/*--------------------------------stream stream-------------------------------*/

/*--------------------------------stream aligned-------------------------------*/
    template <>
    inline void copy_bytes<16, 1, 0>(void* dest, const void* src) noexcept {
        _mm_stream_si128(static_cast<__m128i*>(dest), _mm_load_si128(static_cast<const __m128i*>(src)));
    }

    template <>
    inline void copy_bytes<32, 1, 0>(void* dest, const void* src) noexcept {
        _mm256_stream_si256(static_cast<__m256i*>(dest), _mm256_load_si256(static_cast<const __m256i*>(src)));
    }

    template <>
    inline void copy_bytes<64, 1, 0>(void* dest, const void* src) noexcept {
        _mm512_stream_si512(static_cast<__m512i*>(dest), _mm512_load_si512(src));
    }

#define LOAD(i) const __m512i zmm##i = _mm512_load_si512(static_cast<const __m512i*>(src) + i);
#define STORE(i) _mm512_stream_si512(static_cast<__m512i*>(dest) + i, zmm##i);

    template <>
    inline void copy_bytes<128, 1, 0>(void* dest, const void* src) noexcept {
        LOAD(0)
        LOAD(1)
        STORE(0)
        STORE(1)
    }

    template <>
    inline void copy_bytes<256, 1, 0>(void* dest, const void* src) noexcept {
        LOAD(0)
        LOAD(1)
        LOAD(2)
        LOAD(3)
        STORE(0)
        STORE(1)
        STORE(2)
        STORE(3)
    }

    template <>
    inline void copy_bytes<512, 1, 0>(void* dest, const void* src) noexcept {
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

    template <>
    inline void copy_bytes<1024, 1, 0>(void* dest, const void* src) noexcept {
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

    template <>
    inline void copy_bytes<2048, 1, 0>(void* dest, const void* src) noexcept {
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
/*--------------------------------stream aligned-------------------------------*/

/*--------------------------------stream unaligned-------------------------------*/
    template <>
    inline void copy_bytes<16, 1, -1>(void* dest, const void* src) noexcept {
        _mm_stream_si128(static_cast<__m128i*>(dest), _mm_lddqu_si128(static_cast<const __m128i*>(src)));
    }

    template <>
    inline void copy_bytes<32, 1, -1>(void* dest, const void* src) noexcept {
        _mm256_stream_si256(static_cast<__m256i*>(dest), _mm256_lddqu_si256(static_cast<const __m256i*>(src)));
    }

    template <>
    inline void copy_bytes<64, 1, -1>(void* dest, const void* src) noexcept {
        _mm512_stream_si512(static_cast<__m512i*>(dest), _mm512_loadu_si512(src));
    }

#define LOAD(i) const __m512i zmm##i = _mm512_loadu_si512(static_cast<const __m512i*>(src) + i);
#define STORE(i) _mm512_stream_si512(static_cast<__m512i*>(dest) + i, zmm##i);

    template <>
    inline void copy_bytes<128, 1, -1>(void* dest, const void* src) noexcept {
        LOAD(0)
        LOAD(1)
        STORE(0)
        STORE(1)
    }

    template <>
    inline void copy_bytes<256, 1, -1>(void* dest, const void* src) noexcept {
        LOAD(0)
        LOAD(1)
        LOAD(2)
        LOAD(3)
        STORE(0)
        STORE(1)
        STORE(2)
        STORE(3)
    }

    template <>
    inline void copy_bytes<512, 1, -1>(void* dest, const void* src) noexcept {
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

    template <>
    inline void copy_bytes<1024, 1, -1>(void* dest, const void* src) noexcept {
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

    template <>
    inline void copy_bytes<2048, 1, -1>(void* dest, const void* src) noexcept {
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
/*--------------------------------stream unaligned-------------------------------*/

/*--------------------------------aligned stream-------------------------------*/
    template <>
    inline void copy_bytes<16, 0, 1>(void* dest, const void* src) noexcept {
        _mm_store_si128(
                static_cast<__m128i*>(dest), _mm_stream_load_si128(static_cast<__m128i*>(const_cast<void*>(src))));
    }

    template <>
    inline void copy_bytes<32, 0, 1>(void* dest, const void* src) noexcept {
        _mm256_store_si256(static_cast<__m256i*>(dest), _mm256_stream_load_si256(static_cast<const __m256i*>(src)));
    }

    template <>
    inline void copy_bytes<64, 0, 1>(void* dest, const void* src) noexcept {
        _mm512_store_si512(dest, _mm512_stream_load_si512(const_cast<void*>(src)));
    }

#define LOAD(i) const __m512i zmm##i = _mm512_stream_load_si512(static_cast<__m512i*>(const_cast<void*>(src)) + i);
#define STORE(i) _mm512_store_si512(static_cast<__m512i*>(dest) + i, zmm##i);

    template <>
    inline void copy_bytes<128, 0, 1>(void* dest, const void* src) noexcept {
        LOAD(0)
        LOAD(1)
        STORE(0)
        STORE(1)
    }

    template <>
    inline void copy_bytes<256, 0, 1>(void* dest, const void* src) noexcept {
        LOAD(0)
        LOAD(1)
        LOAD(2)
        LOAD(3)
        STORE(0)
        STORE(1)
        STORE(2)
        STORE(3)
    }

    template <>
    inline void copy_bytes<512, 0, 1>(void* dest, const void* src) noexcept {
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

    template <>
    inline void copy_bytes<1024, 0, 1>(void* dest, const void* src) noexcept {
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

    template <>
    inline void copy_bytes<2048, 0, 1>(void* dest, const void* src) noexcept {
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
/*--------------------------------aligned stream-------------------------------*/

/*--------------------------------aligned aligned-------------------------------*/
    template <>
    inline void copy_bytes<16, 0, 0>(void* dest, const void* src) noexcept {
        _mm_store_si128(static_cast<__m128i*>(dest), _mm_load_si128(static_cast<const __m128i*>(src)));
    }

    template <>
    inline void copy_bytes<32, 0, 0>(void* dest, const void* src) noexcept {
        _mm256_store_si256(static_cast<__m256i*>(dest), _mm256_load_si256(static_cast<const __m256i*>(src)));
    }

    template <>
    inline void copy_bytes<64, 0, 0>(void* dest, const void* src) noexcept {
        _mm512_store_si512(dest, _mm512_load_si512(src));
    }

#define LOAD(i) const __m512i zmm##i = _mm512_load_si512(static_cast<const __m512i*>(src) + i);
#define STORE(i) _mm512_store_si512(static_cast<__m512i*>(dest) + i, zmm##i);

    template <>
    inline void copy_bytes<128, 0, 0>(void* dest, const void* src) noexcept {
        LOAD(0)
        LOAD(1)
        STORE(0)
        STORE(1)
    }

    template <>
    inline void copy_bytes<256, 0, 0>(void* dest, const void* src) noexcept {
        LOAD(0)
        LOAD(1)
        LOAD(2)
        LOAD(3)
        STORE(0)
        STORE(1)
        STORE(2)
        STORE(3)
    }

    template <>
    inline void copy_bytes<512, 0, 0>(void* dest, const void* src) noexcept {
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

    template <>
    inline void copy_bytes<1024, 0, 0>(void* dest, const void* src) noexcept {
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

    template <>
    inline void copy_bytes<2048, 0, 0>(void* dest, const void* src) noexcept {
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
/*--------------------------------aligned aligned-------------------------------*/

/*--------------------------------aligned unaligned-------------------------------*/
    template <>
    inline void copy_bytes<16, 0, -1>(void* dest, const void* src) noexcept {
        _mm_store_si128(static_cast<__m128i*>(dest), _mm_lddqu_si128(static_cast<const __m128i*>(src)));
    }

    template <>
    inline void copy_bytes<32, 0, -1>(void* dest, const void* src) noexcept {
        _mm256_store_si256(static_cast<__m256i*>(dest), _mm256_lddqu_si256(static_cast<const __m256i*>(src)));
    }

    template <>
    inline void copy_bytes<64, 0, -1>(void* dest, const void* src) noexcept {
        _mm512_store_si512(dest, _mm512_loadu_si512(src));
    }

#define LOAD(i) const __m512i zmm##i = _mm512_loadu_si512(static_cast<const __m512i*>(src) + i);
#define STORE(i) _mm512_store_si512(static_cast<__m512i*>(dest) + i, zmm##i);

    template <>
    inline void copy_bytes<128, 0, -1>(void* dest, const void* src) noexcept {
        LOAD(0)
        LOAD(1)
        STORE(0)
        STORE(1)
    }

    template <>
    inline void copy_bytes<256, 0, -1>(void* dest, const void* src) noexcept {
        LOAD(0)
        LOAD(1)
        LOAD(2)
        LOAD(3)
        STORE(0)
        STORE(1)
        STORE(2)
        STORE(3)
    }

    template <>
    inline void copy_bytes<512, 0, -1>(void* dest, const void* src) noexcept {
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

    template <>
    inline void copy_bytes<1024, 0, -1>(void* dest, const void* src) noexcept {
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

    template <>
    inline void copy_bytes<2048, 0, -1>(void* dest, const void* src) noexcept {
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
/*--------------------------------aligned unaligned-------------------------------*/

/*--------------------------------unaligned stream-------------------------------*/
    template <>
    inline void copy_bytes<16, -1, 1>(void* dest, const void* src) noexcept {
        _mm_storeu_si128(
                static_cast<__m128i*>(dest), _mm_stream_load_si128(static_cast<__m128i*>(const_cast<void*>(src))));
    }

    template <>
    inline void copy_bytes<32, -1, 1>(void* dest, const void* src) noexcept {
        _mm256_storeu_si256(static_cast<__m256i*>(dest), _mm256_stream_load_si256(static_cast<const __m256i*>(src)));
    }

    template <>
    inline void copy_bytes<64, -1, 1>(void* dest, const void* src) noexcept {
        _mm512_storeu_si512(dest, _mm512_stream_load_si512(const_cast<void*>(src)));
    }

#define LOAD(i) const __m512i zmm##i = _mm512_stream_load_si512(static_cast<__m512i*>(const_cast<void*>(src)) + i);
#define STORE(i) _mm512_storeu_si512(static_cast<__m512i*>(dest) + i, zmm##i);

    template <>
    inline void copy_bytes<128, -1, 1>(void* dest, const void* src) noexcept {
        LOAD(0)
        LOAD(1)
        STORE(0)
        STORE(1)
    }

    template <>
    inline void copy_bytes<256, -1, 1>(void* dest, const void* src) noexcept {
        LOAD(0)
        LOAD(1)
        LOAD(2)
        LOAD(3)
        STORE(0)
        STORE(1)
        STORE(2)
        STORE(3)
    }

    template <>
    inline void copy_bytes<512, -1, 1>(void* dest, const void* src) noexcept {
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

    template <>
    inline void copy_bytes<1024, -1, 1>(void* dest, const void* src) noexcept {
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

    template <>
    inline void copy_bytes<2048, -1, 1>(void* dest, const void* src) noexcept {
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
/*--------------------------------unaligned stream-------------------------------*/

/*--------------------------------unaligned aligned-------------------------------*/
    template <>
    inline void copy_bytes<16, -1, 0>(void* dest, const void* src) noexcept {
        _mm_storeu_si128(static_cast<__m128i*>(dest), _mm_load_si128(static_cast<const __m128i*>(src)));
    }

    template <>
    inline void copy_bytes<32, -1, 0>(void* dest, const void* src) noexcept {
        _mm256_storeu_si256(static_cast<__m256i*>(dest), _mm256_load_si256(static_cast<const __m256i*>(src)));
    }

    template <>
    inline void copy_bytes<64, -1, 0>(void* dest, const void* src) noexcept {
        _mm512_storeu_si512(dest, _mm512_load_si512(src));
    }

#define LOAD(i) const __m512i zmm##i = _mm512_load_si512(static_cast<const __m512i*>(src) + i);
#define STORE(i) _mm512_storeu_si512(static_cast<__m512i*>(dest) + i, zmm##i);

    template <>
    inline void copy_bytes<128, -1, 0>(void* dest, const void* src) noexcept {
        LOAD(0)
        LOAD(1)
        STORE(0)
        STORE(1)
    }

    template <>
    inline void copy_bytes<256, -1, 0>(void* dest, const void* src) noexcept {
        LOAD(0)
        LOAD(1)
        LOAD(2)
        LOAD(3)
        STORE(0)
        STORE(1)
        STORE(2)
        STORE(3)
    }

    template <>
    inline void copy_bytes<512, -1, 0>(void* dest, const void* src) noexcept {
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

    template <>
    inline void copy_bytes<1024, -1, 0>(void* dest, const void* src) noexcept {
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

    template <>
    inline void copy_bytes<2048, -1, 0>(void* dest, const void* src) noexcept {
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
/*--------------------------------unaligned aligned-------------------------------*/

/*--------------------------------unaligned unaligned-------------------------------*/
    template <>
    inline void copy_bytes<16, -1, -1>(void* dest, const void* src) noexcept {
        _mm_storeu_si128(static_cast<__m128i*>(dest), _mm_lddqu_si128(static_cast<const __m128i*>(src)));
    }

    template <>
    inline void copy_bytes<32, -1, -1>(void* dest, const void* src) noexcept {
        _mm256_storeu_si256(static_cast<__m256i*>(dest), _mm256_lddqu_si256(static_cast<const __m256i*>(src)));
    }

    template <>
    inline void copy_bytes<64, -1, -1>(void* dest, const void* src) noexcept {
        _mm512_storeu_si512(dest, _mm512_loadu_si512(src));
    }

#define LOAD(i) const __m512i zmm##i = _mm512_loadu_si512(static_cast<const __m512i*>(src) + i);
#define STORE(i) _mm512_storeu_si512(static_cast<__m512i*>(dest) + i, zmm##i);

    template <>
    inline void copy_bytes<128, -1, -1>(void* dest, const void* src) noexcept {
        LOAD(0)
        LOAD(1)
        STORE(0)
        STORE(1)
    }

    template <>
    inline void copy_bytes<256, -1, -1>(void* dest, const void* src) noexcept {
        LOAD(0)
        LOAD(1)
        LOAD(2)
        LOAD(3)
        STORE(0)
        STORE(1)
        STORE(2)
        STORE(3)
    }

    template <>
    inline void copy_bytes<512, -1, -1>(void* dest, const void* src) noexcept {
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

    template <>
    inline void copy_bytes<1024, -1, -1>(void* dest, const void* src) noexcept {
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

    template <>
    inline void copy_bytes<2048, -1, -1>(void* dest, const void* src) noexcept {
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

/*--------------------------------unaligned unaligned-------------------------------*/


    template <size_t I>
    inline constexpr bool Copy_diff_single(size_t size, std::byte* p, const std::byte* q) noexcept {
        if (size == I) {
            copy_bytes<I, -1, -1>(p, q);
            return true;
        }
        return false;
    }

    template <size_t... Is>
    inline constexpr void Copy_diff(size_t size, std::byte* p, const std::byte* q) {
        (Copy_diff_single<Is>(size, p, q) || ...);
    }
}
