//
// Created by WiwilZ on 2022/2/2.
//

#pragma once

#include <bit>
#include <numeric>
#include <compare>
#include <cstdint>
#include <x86intrin.h>


std::strong_ordering sse2_strcmp(const char* s1, const char* s2) noexcept __attribute_pure__ __nonnull ((1, 2));

std::strong_ordering sse4_strcmp(const char* s1, const char* s2) noexcept __attribute_pure__ __nonnull ((1, 2));

std::strong_ordering avx2_strcmp(const char* s1, const char* s2) noexcept __attribute_pure__ __nonnull ((1, 2));

std::strong_ordering avx512_strcmp(const char* s1, const char* s2) noexcept __attribute_pure__ __nonnull ((1, 2));

std::strong_ordering sse2_strcmp(const char* s1, const char* s2) noexcept {
    static const __m128i zero = _mm_setzero_si128();
    for (size_t r = 0;; r += sizeof(__m128i)) {
        const __m128i x = _mm_lddqu_si128(reinterpret_cast<const __m128i*>(s1 + r));
        const __m128i y = _mm_lddqu_si128(reinterpret_cast<const __m128i*>(s2 + r));
        const uint16_t eqMask = _mm_movemask_epi8(_mm_cmpeq_epi8(x, y));
        const uint16_t s1NilMask = _mm_movemask_epi8(_mm_cmpeq_epi8(zero, x));
        // 存在不相等的字符
        if (eqMask ^ std::numeric_limits<uint16_t>::max()) {
            // 判断s1、s2是否含有'\0'
            const uint16_t s2NilMask = _mm_movemask_epi8(_mm_cmpeq_epi8(zero, y));
            const size_t lastNeIdx = std::countr_one(eqMask);
            // s1和s2都含有'\0'
            if (s1NilMask != 0 && s2NilMask != 0) {
                const size_t s1NilIdx = std::countr_zero(s1NilMask);
                const size_t s2NilIdx = std::countr_zero(s2NilMask);
                // '\0'的位置相同且第一个不相等字符的位置在其之后
                if (s1NilIdx == s2NilIdx && lastNeIdx > s1NilIdx) {
                    return std::strong_ordering::equal;
                }
            }
            const size_t idx = r + lastNeIdx;  // 第一个不相等字符的下标
            return s1[idx] <=> s2[idx];
        }
        // 16个字符都相等，且s1中有一个字符为'\0'（故s2此位置也为'\0'）
        if (s1NilMask != 0) {
            return std::strong_ordering::equal;
        }
    }
}

std::strong_ordering sse4_strcmp(const char* s1, const char* s2) noexcept {
    static const __m128i zero = _mm_setzero_si128();
    for (size_t r = 0;; r += sizeof(__m128i)) {
        const __m128i x = _mm_lddqu_si128(reinterpret_cast<const __m128i*>(s1 + r));
        const __m128i y = _mm_lddqu_si128(reinterpret_cast<const __m128i*>(s2 + r));
        const size_t eqCnt = _mm_cmpistri(x, y, _SIDD_CMP_EQUAL_EACH | _SIDD_NEGATIVE_POLARITY);
        // 存在不相等的字符
        if (eqCnt != sizeof(__m128i)) {
            // 判断s1、s2是否含有'\0'
            const size_t s1NonNilCnt = _mm_cmpistri(zero, x, _SIDD_CMP_EQUAL_EACH);
            const size_t s2NonNilCnt = _mm_cmpistri(zero, y, _SIDD_CMP_EQUAL_EACH);
            // s1和s2都含有'\0'
            if (s1NonNilCnt != sizeof(__m128i) && s2NonNilCnt != sizeof(__m128i) &&
                s1NonNilCnt == s2NonNilCnt && eqCnt > s1NonNilCnt) {
                return std::strong_ordering::equal;
            }
            const size_t idx = r + eqCnt;
            return s1[idx] <=> s2[idx];
        }
        // 16个字符都相等，且s1中有一个字符为'\0'（故s2此位置也为'\0'）
        if (_mm_cmpistrs(x, zero, _SIDD_UBYTE_OPS)) {
            return std::strong_ordering::equal;
        }
    }
}


std::strong_ordering avx2_strcmp(const char* s1, const char* s2) noexcept {
    static const __m256i zero = _mm256_setzero_si256();
    for (size_t r = 0;; r += sizeof(__m256i)) {
        const __m256i x = _mm256_lddqu_si256(reinterpret_cast<const __m256i*>(s1 + r));
        const __m256i y = _mm256_lddqu_si256(reinterpret_cast<const __m256i*>(s2 + r));
        const uint32_t eqMask = _mm256_movemask_epi8(_mm256_cmpeq_epi8(x, y));
        const uint32_t s1NilMask = _mm256_movemask_epi8(_mm256_cmpeq_epi8(zero, x));
        // 存在不相等的字符
        if (eqMask ^ std::numeric_limits<uint32_t>::max()) {
            // 判断s1、s2是否含有'\0'
            const uint32_t s2NilMask = _mm256_movemask_epi8(_mm256_cmpeq_epi8(zero, y));
            const size_t lastNeIdx = std::countr_one(eqMask);
            // s1和s2都含有'\0'
            if (s1NilMask != 0 && s2NilMask != 0) {
                const size_t s1NilIdx = std::countr_zero(s1NilMask);
                const size_t s2NilIdx = std::countr_zero(s2NilMask);
                // '\0'的位置相同且第一个不相等字符的位置在其之后
                if (s1NilIdx == s2NilIdx && lastNeIdx > s1NilIdx) {
                    return std::strong_ordering::equal;
                }
            }
            const size_t idx = r + lastNeIdx;  // 第一个不相等字符的下标
            return s1[idx] <=> s2[idx];
        }
        // 32个字符都相等，且s1中有一个字符为'\0'（故s2此位置也为'\0'）
        if (s1NilMask != 0) {
            return std::strong_ordering::equal;
        }
    }
}


std::strong_ordering avx512_strcmp(const char* s1, const char* s2) noexcept {
    static const __m512i zero = _mm512_setzero_si512();
    for (size_t r = 0;; r += sizeof(__m512i)) {
        const __m512i x = _mm512_loadu_si512(s1 + r);
        const __m512i y = _mm512_loadu_si512(s2 + r);
        const uint64_t eqMask = _mm512_cmpeq_epi8_mask(x, y);
        const uint64_t s1NilMask = _mm512_cmpeq_epi8_mask(zero, x);
        // 存在不相等的字符
        if (eqMask ^ std::numeric_limits<uint64_t>::max()) {
            // 判断s1、s2是否含有'\0'
            const uint64_t s2NilMask = _mm512_cmpeq_epi8_mask(zero, y);
            const size_t lastNeIdx = std::countr_one(eqMask);
            // s1和s2都含有'\0'
            if (s1NilMask != 0 && s2NilMask != 0) {
                const size_t s1NilIdx = std::countr_zero(s1NilMask);
                const size_t s2NilIdx = std::countr_zero(s2NilMask);
                // '\0'的位置相同且第一个不相等字符的位置在其之后
                if (s1NilIdx == s2NilIdx && lastNeIdx > s1NilIdx) {
                    return std::strong_ordering::equal;
                }
            }
            const size_t idx = r + lastNeIdx;  // 第一个不相等字符的下标
            return s1[idx] <=> s2[idx];
        }
        // 64个字符都相等，且s1中有一个字符为'\0'（故s2此位置也为'\0'）
        if (s1NilMask != 0) {
            return std::strong_ordering::equal;
        }
    }
}

