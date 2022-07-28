/*
 * Created by WiwilZ on 2022/7/27.
 */

#pragma once

#include <cstdint>
#include <cstddef>
#include <x86intrin.h>


#define INLINE [[using gnu : always_inline, artificial]] inline


namespace {
#ifdef __SSE2__

	template <bool aligned>
	INLINE __m128i Load(const __m128i* a) noexcept {
		return aligned ? _mm_load_si128(a) : _mm_loadu_si128(a);
	}

	template <bool aligned>
	INLINE void Store(__m128i* dst, __m128i src) noexcept {
		aligned ? _mm_store_si128(dst, src) : _mm_storeu_si128(dst, src);
	}

#ifdef __AVX__

	template <bool aligned>
	INLINE __m256i Load(const __m256i* a) noexcept {
		return aligned ? _mm256_load_si256(a) : _mm256_loadu_si256(a);
	}

	template <bool aligned>
	INLINE void Store(__m256i* dst, __m256i src) noexcept {
		aligned ? _mm256_store_si256(dst, src) : _mm256_storeu_si256(dst, src);
	}

#ifdef __AVX512F__

	template <bool aligned>
	INLINE __m512i Load(const __m512i* a) noexcept {
		return aligned ? _mm512_load_si512(a) : _mm512_loadu_si512(a);
	}

	template <bool aligned>
	INLINE void Store(__m512i* dst, __m512i src) noexcept {
		aligned ? _mm512_store_si512(dst, src) : _mm512_storeu_si512(dst, src);
	}

#endif
#endif
#endif

	template <size_t n_bytes, bool dst_aligned = false, bool src_aligned = false>
	INLINE void Copy_bytes(void* dst, const void* src) noexcept {
		switch (n_bytes) {
		case 1:
			*static_cast<uint8_t*>(dst) = *static_cast<const uint8_t*>(src);
			return;
		case 2:
			*static_cast<uint16_t*>(dst) = *static_cast<const uint16_t*>(src);
			return;
		case 4:
			*static_cast<uint32_t*>(dst) = *static_cast<const uint32_t*>(src);
			return;
		case 8:
			*static_cast<uint64_t*>(dst) = *static_cast<const uint64_t*>(src);
			return;
#ifdef __SSE2__
		case 16:
			Store<dst_aligned>(static_cast<__m128i*>(dst), Load<src_aligned>(static_cast<const __m128i*>(src)));
			return;
#ifdef __AVX__
		case 32:
			Store<dst_aligned>(static_cast<__m256i*>(dst), Load<src_aligned>(static_cast<const __m256i*>(src)));
			return;
#ifdef __AVX512F__
		case 64:
			Store<dst_aligned>(static_cast<__m512i*>(dst), Load<src_aligned>(static_cast<const __m512i*>(src)));
			return;
#endif
#endif
#endif
		default:
			return;
		}
	}
}


#define COPY_BYTES_FWD(n, dst_aligned, src_aligned) Copy_bytes<n, dst_aligned, src_aligned>(pd, ps); pd += n; ps += n;
#define COPY_BYTES_BWD(n, dst_aligned, src_aligned) pd -= n; ps -= n; Copy_bytes<n, dst_aligned, src_aligned>(pd, ps);

#ifdef __SSE2__
#ifdef __AVX__
#ifdef __AVX512F__
#define COPY_SMALL_FWD(out, dst_aligned, src_aligned)                   \
        case 32:                                                        \
            Copy_bytes<32, dst_aligned, src_aligned>(pd, ps);           \
        case 0:                                                         \
            out;                                                        \
        case 33:                                                        \
            COPY_BYTES_FWD(32, dst_aligned, src_aligned)                \
        case 1:                                                         \
            Copy_bytes<1, dst_aligned, src_aligned>(pd, ps);            \
            out;                                                        \
        case 34:                                                        \
            COPY_BYTES_FWD(32, dst_aligned, src_aligned)                \
        case 2:                                                         \
            Copy_bytes<2, dst_aligned, src_aligned>(pd, ps);            \
            out;                                                        \
        case 35:                                                        \
            COPY_BYTES_FWD(32, dst_aligned, src_aligned)                \
        case 3:                                                         \
            Copy_bytes<2, dst_aligned, src_aligned>(pd, ps);            \
            Copy_bytes<1, dst_aligned, src_aligned>(pd + 2, ps + 2);    \
            out;                                                        \
        case 36:                                                        \
            COPY_BYTES_FWD(32, dst_aligned, src_aligned)                \
        case 4:                                                         \
            Copy_bytes<4, dst_aligned, src_aligned>(pd, ps);            \
            out;                                                        \
        case 37:                                                        \
            COPY_BYTES_FWD(32, dst_aligned, src_aligned)                \
        case 5:                                                         \
            Copy_bytes<4, dst_aligned, src_aligned>(pd, ps);            \
            Copy_bytes<1, dst_aligned, src_aligned>(pd + 4, ps + 4);    \
            out;                                                        \
        case 38:                                                        \
            COPY_BYTES_FWD(32, dst_aligned, src_aligned)                \
        case 6:                                                         \
            Copy_bytes<4, dst_aligned, src_aligned>(pd, ps);            \
            Copy_bytes<2, dst_aligned, src_aligned>(pd + 4, ps + 4);    \
            out;                                                        \
        case 39:                                                        \
            COPY_BYTES_FWD(32, dst_aligned, src_aligned)                \
        case 7:                                                         \
            Copy_bytes<4, dst_aligned, src_aligned>(pd, ps);            \
            Copy_bytes<2, dst_aligned, src_aligned>(pd + 4, ps + 4);    \
            Copy_bytes<1, dst_aligned, src_aligned>(pd + 6, ps + 6);    \
            out;                                                        \
        case 40:                                                        \
            COPY_BYTES_FWD(32, dst_aligned, src_aligned)                \
        case 8:                                                         \
            Copy_bytes<8, dst_aligned, src_aligned>(pd, ps);            \
            out;                                                        \
        case 41:                                                        \
            COPY_BYTES_FWD(32, dst_aligned, src_aligned)                \
        case 9:                                                         \
            Copy_bytes<8, dst_aligned, src_aligned>(pd, ps);            \
            Copy_bytes<1, dst_aligned, src_aligned>(pd + 8, ps + 8);    \
            out;                                                        \
        case 42:                                                        \
            COPY_BYTES_FWD(32, dst_aligned, src_aligned)                \
        case 10:                                                        \
            Copy_bytes<8, dst_aligned, src_aligned>(pd, ps);            \
            Copy_bytes<2, dst_aligned, src_aligned>(pd + 8, ps + 8);    \
            out;                                                        \
        case 43:                                                        \
            COPY_BYTES_FWD(32, dst_aligned, src_aligned)                \
        case 11:                                                        \
            Copy_bytes<8, dst_aligned, src_aligned>(pd, ps);            \
            Copy_bytes<2, dst_aligned, src_aligned>(pd + 8, ps + 8);    \
            Copy_bytes<1, dst_aligned, src_aligned>(pd + 10, ps + 10);  \
            out;                                                        \
        case 44:                                                        \
            COPY_BYTES_FWD(32, dst_aligned, src_aligned)                \
        case 12:                                                        \
            Copy_bytes<8, dst_aligned, src_aligned>(pd, ps);            \
            Copy_bytes<4, dst_aligned, src_aligned>(pd + 8, ps + 8);    \
            out;                                                        \
        case 45:                                                        \
            COPY_BYTES_FWD(32, dst_aligned, src_aligned)                \
        case 13:                                                        \
            Copy_bytes<8, dst_aligned, src_aligned>(pd, ps);            \
            Copy_bytes<4, dst_aligned, src_aligned>(pd + 8, ps + 8);    \
            Copy_bytes<1, dst_aligned, src_aligned>(pd + 12, ps + 12);  \
            out;                                                        \
        case 46:                                                        \
            COPY_BYTES_FWD(32, dst_aligned, src_aligned)                \
        case 14:                                                        \
            Copy_bytes<8, dst_aligned, src_aligned>(pd, ps);            \
            Copy_bytes<4, dst_aligned, src_aligned>(pd + 8, ps + 8);    \
            Copy_bytes<2, dst_aligned, src_aligned>(pd + 12, ps + 12);  \
            out;                                                        \
        case 47:                                                        \
            COPY_BYTES_FWD(32, dst_aligned, src_aligned)                \
        case 15:                                                        \
            Copy_bytes<8, dst_aligned, src_aligned>(pd, ps);            \
            Copy_bytes<4, dst_aligned, src_aligned>(pd + 8, ps + 8);    \
            Copy_bytes<2, dst_aligned, src_aligned>(pd + 12, ps + 12);  \
            Copy_bytes<1, dst_aligned, src_aligned>(pd + 14, ps + 14);  \
            out;                                                        \
        case 48:                                                        \
            COPY_BYTES_FWD(32, dst_aligned, src_aligned)                \
        case 16:                                                        \
            Copy_bytes<16, dst_aligned, src_aligned>(pd, ps);           \
            out;                                                        \
        case 49:                                                        \
            COPY_BYTES_FWD(32, dst_aligned, src_aligned)                \
        case 17:                                                        \
            Copy_bytes<16, dst_aligned, src_aligned>(pd, ps);           \
            Copy_bytes<1, dst_aligned, src_aligned>(pd + 16, ps + 16);  \
            out;                                                        \
        case 50:                                                        \
            COPY_BYTES_FWD(32, dst_aligned, src_aligned)                \
        case 18:                                                        \
            Copy_bytes<16, dst_aligned, src_aligned>(pd, ps);           \
            Copy_bytes<2, dst_aligned, src_aligned>(pd + 16, ps + 16);  \
            out;                                                        \
        case 51:                                                        \
            COPY_BYTES_FWD(32, dst_aligned, src_aligned)                \
        case 19:                                                        \
            Copy_bytes<16, dst_aligned, src_aligned>(pd, ps);           \
            Copy_bytes<2, dst_aligned, src_aligned>(pd + 16, ps + 16);  \
            Copy_bytes<1, dst_aligned, src_aligned>(pd + 18, ps + 18);  \
            out;                                                        \
        case 52:                                                        \
            COPY_BYTES_FWD(32, dst_aligned, src_aligned)                \
        case 20:                                                        \
            Copy_bytes<16, dst_aligned, src_aligned>(pd, ps);           \
            Copy_bytes<4, dst_aligned, src_aligned>(pd + 16, ps + 16);  \
            out;                                                        \
        case 53:                                                        \
            COPY_BYTES_FWD(32, dst_aligned, src_aligned)                \
        case 21:                                                        \
            Copy_bytes<16, dst_aligned, src_aligned>(pd, ps);           \
            Copy_bytes<4, dst_aligned, src_aligned>(pd + 16, ps + 16);  \
            Copy_bytes<1, dst_aligned, src_aligned>(pd + 20, ps + 20);  \
            out;                                                        \
        case 54:                                                        \
            COPY_BYTES_FWD(32, dst_aligned, src_aligned)                \
        case 22:                                                        \
            Copy_bytes<16, dst_aligned, src_aligned>(pd, ps);           \
            Copy_bytes<4, dst_aligned, src_aligned>(pd + 16, ps + 16);  \
            Copy_bytes<2, dst_aligned, src_aligned>(pd + 20, ps + 20);  \
            out;                                                        \
        case 55:                                                        \
            COPY_BYTES_FWD(32, dst_aligned, src_aligned)                \
        case 23:                                                        \
            Copy_bytes<16, dst_aligned, src_aligned>(pd, ps);           \
            Copy_bytes<4, dst_aligned, src_aligned>(pd + 16, ps + 16);  \
            Copy_bytes<2, dst_aligned, src_aligned>(pd + 20, ps + 20);  \
            Copy_bytes<1, dst_aligned, src_aligned>(pd + 22, ps + 22);  \
            out;                                                        \
        case 56:                                                        \
            COPY_BYTES_FWD(32, dst_aligned, src_aligned)                \
        case 24:                                                        \
            Copy_bytes<16, dst_aligned, src_aligned>(pd, ps);           \
            Copy_bytes<8, dst_aligned, src_aligned>(pd + 16, ps + 16);  \
            out;                                                        \
        case 57:                                                        \
            COPY_BYTES_FWD(32, dst_aligned, src_aligned)                \
        case 25:                                                        \
            Copy_bytes<16, dst_aligned, src_aligned>(pd, ps);           \
            Copy_bytes<8, dst_aligned, src_aligned>(pd + 16, ps + 16);  \
            Copy_bytes<1, dst_aligned, src_aligned>(pd + 24, ps + 24);  \
            out;                                                        \
        case 58:                                                        \
            COPY_BYTES_FWD(32, dst_aligned, src_aligned)                \
        case 26:                                                        \
            Copy_bytes<16, dst_aligned, src_aligned>(pd, ps);           \
            Copy_bytes<8, dst_aligned, src_aligned>(pd + 16, ps + 16);  \
            Copy_bytes<2, dst_aligned, src_aligned>(pd + 24, ps + 24);  \
            out;                                                        \
        case 59:                                                        \
            COPY_BYTES_FWD(32, dst_aligned, src_aligned)                \
        case 27:                                                        \
            Copy_bytes<16, dst_aligned, src_aligned>(pd, ps);           \
            Copy_bytes<8, dst_aligned, src_aligned>(pd + 16, ps + 16);  \
            Copy_bytes<2, dst_aligned, src_aligned>(pd + 24, ps + 24);  \
            Copy_bytes<1, dst_aligned, src_aligned>(pd + 26, ps + 26);  \
            out;                                                        \
        case 60:                                                        \
            COPY_BYTES_FWD(32, dst_aligned, src_aligned)                \
        case 28:                                                        \
            Copy_bytes<16, dst_aligned, src_aligned>(pd, ps);           \
            Copy_bytes<8, dst_aligned, src_aligned>(pd + 16, ps + 16);  \
            Copy_bytes<4, dst_aligned, src_aligned>(pd + 24, ps + 24);  \
            out;                                                        \
        case 61:                                                        \
            COPY_BYTES_FWD(32, dst_aligned, src_aligned)                \
        case 29:                                                        \
            Copy_bytes<16, dst_aligned, src_aligned>(pd, ps);           \
            Copy_bytes<8, dst_aligned, src_aligned>(pd + 16, ps + 16);  \
            Copy_bytes<4, dst_aligned, src_aligned>(pd + 24, ps + 24);  \
            Copy_bytes<1, dst_aligned, src_aligned>(pd + 28, ps + 28);  \
            out;                                                        \
        case 62:                                                        \
            COPY_BYTES_FWD(32, dst_aligned, src_aligned)                \
        case 30:                                                        \
            Copy_bytes<16, dst_aligned, src_aligned>(pd, ps);           \
            Copy_bytes<8, dst_aligned, src_aligned>(pd + 16, ps + 16);  \
            Copy_bytes<4, dst_aligned, src_aligned>(pd + 24, ps + 24);  \
            Copy_bytes<2, dst_aligned, src_aligned>(pd + 28, ps + 28);  \
            out;                                                        \
        case 63:                                                        \
            COPY_BYTES_FWD(32, dst_aligned, src_aligned)                \
        case 31:                                                        \
            Copy_bytes<16, dst_aligned, src_aligned>(pd, ps);           \
            Copy_bytes<8, dst_aligned, src_aligned>(pd + 16, ps + 16);  \
            Copy_bytes<4, dst_aligned, src_aligned>(pd + 24, ps + 24);  \
            Copy_bytes<2, dst_aligned, src_aligned>(pd + 28, ps + 28);  \
            Copy_bytes<1, dst_aligned, src_aligned>(pd + 30, ps + 30);  \
            out;

#define COPY_SMALL_BWD(out, dst_aligned, src_aligned)                   \
        case 32:                                                        \
            Copy_bytes<32, dst_aligned, src_aligned>(pd - 32, ps - 32); \
        case 0:                                                         \
            out;                                                        \
        case 33:                                                        \
            COPY_BYTES_BWD(32, dst_aligned, src_aligned)                \
        case 1:                                                         \
            Copy_bytes<1, dst_aligned, src_aligned>(pd - 1, ps - 1);    \
            out;                                                        \
        case 34:                                                        \
            COPY_BYTES_BWD(32, dst_aligned, src_aligned)                \
        case 2:                                                         \
            Copy_bytes<2, dst_aligned, src_aligned>(pd - 2, ps - 2);    \
            out;                                                        \
        case 35:                                                        \
            COPY_BYTES_BWD(32, dst_aligned, src_aligned)                \
        case 3:                                                         \
            Copy_bytes<2, dst_aligned, src_aligned>(pd - 2, ps - 2);    \
            Copy_bytes<1, dst_aligned, src_aligned>(pd - 3, ps - 3);    \
            out;                                                        \
        case 36:                                                        \
            COPY_BYTES_BWD(32, dst_aligned, src_aligned)                \
        case 4:                                                         \
            Copy_bytes<4, dst_aligned, src_aligned>(pd - 4, ps - 4);    \
            out;                                                        \
        case 37:                                                        \
            COPY_BYTES_BWD(32, dst_aligned, src_aligned)                \
        case 5:                                                         \
            Copy_bytes<4, dst_aligned, src_aligned>(pd - 4, ps - 4);    \
            Copy_bytes<1, dst_aligned, src_aligned>(pd - 5, ps - 5);    \
            out;                                                        \
        case 38:                                                        \
            COPY_BYTES_BWD(32, dst_aligned, src_aligned)                \
        case 6:                                                         \
            Copy_bytes<4, dst_aligned, src_aligned>(pd - 4, ps - 4);    \
            Copy_bytes<2, dst_aligned, src_aligned>(pd - 6, ps - 6);    \
            out;                                                        \
        case 39:                                                        \
            COPY_BYTES_BWD(32, dst_aligned, src_aligned)                \
        case 7:                                                         \
            Copy_bytes<4, dst_aligned, src_aligned>(pd - 4, ps - 4);    \
            Copy_bytes<2, dst_aligned, src_aligned>(pd - 6, ps - 6);    \
            Copy_bytes<1, dst_aligned, src_aligned>(pd - 7, ps - 7);    \
            out;                                                        \
        case 40:                                                        \
            COPY_BYTES_BWD(32, dst_aligned, src_aligned)                \
        case 8:                                                         \
            Copy_bytes<8, dst_aligned, src_aligned>(pd - 8, ps - 8);    \
            out;                                                        \
        case 41:                                                        \
            COPY_BYTES_BWD(32, dst_aligned, src_aligned)                \
        case 9:                                                         \
            Copy_bytes<8, dst_aligned, src_aligned>(pd - 8, ps - 8);    \
            Copy_bytes<1, dst_aligned, src_aligned>(pd - 9, ps - 9);    \
            out;                                                        \
        case 42:                                                        \
            COPY_BYTES_BWD(32, dst_aligned, src_aligned)                \
        case 10:                                                        \
            Copy_bytes<8, dst_aligned, src_aligned>(pd - 8, ps - 8);    \
            Copy_bytes<2, dst_aligned, src_aligned>(pd - 10, ps - 10);  \
            out;                                                        \
        case 43:                                                        \
            COPY_BYTES_BWD(32, dst_aligned, src_aligned)                \
        case 11:                                                        \
            Copy_bytes<8, dst_aligned, src_aligned>(pd - 8, ps - 8);    \
            Copy_bytes<2, dst_aligned, src_aligned>(pd - 10, ps - 10);  \
            Copy_bytes<1, dst_aligned, src_aligned>(pd - 11, ps - 11);  \
            out;                                                        \
        case 44:                                                        \
            COPY_BYTES_BWD(32, dst_aligned, src_aligned)                \
        case 12:                                                        \
            Copy_bytes<8, dst_aligned, src_aligned>(pd - 8, ps - 8);    \
            Copy_bytes<4, dst_aligned, src_aligned>(pd - 12, ps - 12);  \
            out;                                                        \
        case 45:                                                        \
            COPY_BYTES_BWD(32, dst_aligned, src_aligned)                \
        case 13:                                                        \
            Copy_bytes<8, dst_aligned, src_aligned>(pd - 8, ps - 8);    \
            Copy_bytes<4, dst_aligned, src_aligned>(pd - 12, ps - 12);  \
            Copy_bytes<1, dst_aligned, src_aligned>(pd - 13, ps - 13);  \
            out;                                                        \
        case 46:                                                        \
            COPY_BYTES_BWD(32, dst_aligned, src_aligned)                \
        case 14:                                                        \
            Copy_bytes<8, dst_aligned, src_aligned>(pd - 8, ps - 8);    \
            Copy_bytes<4, dst_aligned, src_aligned>(pd - 12, ps - 12);  \
            Copy_bytes<2, dst_aligned, src_aligned>(pd - 14, ps - 14);  \
            out;                                                        \
        case 47:                                                        \
            COPY_BYTES_BWD(32, dst_aligned, src_aligned)                \
        case 15:                                                        \
            Copy_bytes<8, dst_aligned, src_aligned>(pd - 8, ps - 8);    \
            Copy_bytes<4, dst_aligned, src_aligned>(pd - 12, ps - 12);  \
            Copy_bytes<2, dst_aligned, src_aligned>(pd - 14, ps - 14);  \
            Copy_bytes<1, dst_aligned, src_aligned>(pd - 15, ps - 15);  \
            out;                                                        \
        case 48:                                                        \
            COPY_BYTES_BWD(32, dst_aligned, src_aligned)                \
        case 16:                                                        \
            Copy_bytes<16, dst_aligned, src_aligned>(pd - 16, ps - 16); \
            out;                                                        \
        case 49:                                                        \
            COPY_BYTES_BWD(32, dst_aligned, src_aligned)                \
        case 17:                                                        \
            Copy_bytes<16, dst_aligned, src_aligned>(pd - 16, ps - 16); \
            Copy_bytes<1, dst_aligned, src_aligned>(pd - 17, ps - 17);  \
            out;                                                        \
        case 50:                                                        \
            COPY_BYTES_BWD(32, dst_aligned, src_aligned)                \
        case 18:                                                        \
            Copy_bytes<16, dst_aligned, src_aligned>(pd - 16, ps - 16); \
            Copy_bytes<2, dst_aligned, src_aligned>(pd - 18, ps - 18);  \
            out;                                                        \
        case 51:                                                        \
            COPY_BYTES_BWD(32, dst_aligned, src_aligned)                \
        case 19:                                                        \
            Copy_bytes<16, dst_aligned, src_aligned>(pd - 16, ps - 16); \
            Copy_bytes<2, dst_aligned, src_aligned>(pd - 18, ps - 18);  \
            Copy_bytes<1, dst_aligned, src_aligned>(pd - 19, ps - 19);  \
            out;                                                        \
        case 52:                                                        \
            COPY_BYTES_BWD(32, dst_aligned, src_aligned)                \
        case 20:                                                        \
            Copy_bytes<16, dst_aligned, src_aligned>(pd - 16, ps - 16); \
            Copy_bytes<4, dst_aligned, src_aligned>(pd - 20, ps - 20);  \
            out;                                                        \
        case 53:                                                        \
            COPY_BYTES_BWD(32, dst_aligned, src_aligned)                \
        case 21:                                                        \
            Copy_bytes<16, dst_aligned, src_aligned>(pd - 16, ps - 16); \
            Copy_bytes<4, dst_aligned, src_aligned>(pd - 20, ps - 20);  \
            Copy_bytes<1, dst_aligned, src_aligned>(pd - 21, ps - 21);  \
            out;                                                        \
        case 54:                                                        \
            COPY_BYTES_BWD(32, dst_aligned, src_aligned)                \
        case 22:                                                        \
            Copy_bytes<16, dst_aligned, src_aligned>(pd - 16, ps - 16); \
            Copy_bytes<4, dst_aligned, src_aligned>(pd - 20, ps - 20);  \
            Copy_bytes<2, dst_aligned, src_aligned>(pd - 22, ps - 22);  \
            out;                                                        \
        case 55:                                                        \
            COPY_BYTES_BWD(32, dst_aligned, src_aligned)                \
        case 23:                                                        \
            Copy_bytes<16, dst_aligned, src_aligned>(pd - 16, ps - 16); \
            Copy_bytes<4, dst_aligned, src_aligned>(pd - 20, ps - 20);  \
            Copy_bytes<2, dst_aligned, src_aligned>(pd - 22, ps - 22);  \
            Copy_bytes<1, dst_aligned, src_aligned>(pd - 23, ps - 23);  \
            out;                                                        \
        case 56:                                                        \
            COPY_BYTES_BWD(32, dst_aligned, src_aligned)                \
        case 24:                                                        \
            Copy_bytes<16, dst_aligned, src_aligned>(pd - 16, ps - 16); \
            Copy_bytes<8, dst_aligned, src_aligned>(pd - 24, ps - 24);  \
            out;                                                        \
        case 57:                                                        \
            COPY_BYTES_BWD(32, dst_aligned, src_aligned)                \
        case 25:                                                        \
            Copy_bytes<16, dst_aligned, src_aligned>(pd - 16, ps - 16); \
            Copy_bytes<8, dst_aligned, src_aligned>(pd - 24, ps - 24);  \
            Copy_bytes<1, dst_aligned, src_aligned>(pd - 25, ps - 25);  \
            out;                                                        \
        case 58:                                                        \
            COPY_BYTES_BWD(32, dst_aligned, src_aligned)                \
        case 26:                                                        \
            Copy_bytes<16, dst_aligned, src_aligned>(pd - 16, ps - 16); \
            Copy_bytes<8, dst_aligned, src_aligned>(pd - 24, ps - 24);  \
            Copy_bytes<2, dst_aligned, src_aligned>(pd - 26, ps - 26);  \
            out;                                                        \
        case 59:                                                        \
            COPY_BYTES_BWD(32, dst_aligned, src_aligned)                \
        case 27:                                                        \
            Copy_bytes<16, dst_aligned, src_aligned>(pd - 16, ps - 16); \
            Copy_bytes<8, dst_aligned, src_aligned>(pd - 24, ps - 24);  \
            Copy_bytes<2, dst_aligned, src_aligned>(pd - 26, ps - 26);  \
            Copy_bytes<1, dst_aligned, src_aligned>(pd - 27, ps - 27);  \
            out;                                                        \
        case 60:                                                        \
            COPY_BYTES_BWD(32, dst_aligned, src_aligned)                \
        case 28:                                                        \
            Copy_bytes<16, dst_aligned, src_aligned>(pd - 16, ps - 16); \
            Copy_bytes<8, dst_aligned, src_aligned>(pd - 24, ps - 24);  \
            Copy_bytes<4, dst_aligned, src_aligned>(pd - 28, ps - 28);  \
            out;                                                        \
        case 61:                                                        \
            COPY_BYTES_BWD(32, dst_aligned, src_aligned)                \
        case 29:                                                        \
            Copy_bytes<16, dst_aligned, src_aligned>(pd - 16, ps - 16); \
            Copy_bytes<8, dst_aligned, src_aligned>(pd - 24, ps - 24);  \
            Copy_bytes<4, dst_aligned, src_aligned>(pd - 28, ps - 28);  \
            Copy_bytes<1, dst_aligned, src_aligned>(pd - 29, ps - 29);  \
            out;                                                        \
        case 62:                                                        \
            COPY_BYTES_BWD(32, dst_aligned, src_aligned)                \
        case 30:                                                        \
            Copy_bytes<16, dst_aligned, src_aligned>(pd - 16, ps - 16); \
            Copy_bytes<8, dst_aligned, src_aligned>(pd - 24, ps - 24);  \
            Copy_bytes<4, dst_aligned, src_aligned>(pd - 28, ps - 28);  \
            Copy_bytes<2, dst_aligned, src_aligned>(pd - 30, ps - 30);  \
            out;                                                        \
        case 63:                                                        \
            COPY_BYTES_BWD(32, dst_aligned, src_aligned)                \
        case 31:                                                        \
            Copy_bytes<16, dst_aligned, src_aligned>(pd - 16, ps - 16); \
            Copy_bytes<8, dst_aligned, src_aligned>(pd - 24, ps - 24);  \
            Copy_bytes<4, dst_aligned, src_aligned>(pd - 28, ps - 28);  \
            Copy_bytes<2, dst_aligned, src_aligned>(pd - 30, ps - 30);  \
            Copy_bytes<1, dst_aligned, src_aligned>(pd - 31, ps - 31);  \
            out;
#else
#define COPY_SMALL_FWD(out, dst_aligned, src_aligned)                   \
		case 16:                                                        \
			Copy_bytes<16, dst_aligned, src_aligned>(pd, ps);           \
		case 0:                                                         \
			out;                                                        \
		case 17:                                                        \
			COPY_BYTES_FWD(16, dst_aligned, src_aligned)                \
		case 1:                                                         \
			Copy_bytes<1, dst_aligned, src_aligned>(pd, ps);            \
			out;                                                        \
		case 18:                                                        \
			COPY_BYTES_FWD(16, dst_aligned, src_aligned)                \
		case 2:                                                         \
			Copy_bytes<2, dst_aligned, src_aligned>(pd, ps);            \
			out;                                                        \
		case 19:                                                        \
			COPY_BYTES_FWD(16, dst_aligned, src_aligned)                \
		case 3:                                                         \
			Copy_bytes<2, dst_aligned, src_aligned>(pd, ps);            \
			Copy_bytes<1, dst_aligned, src_aligned>(pd + 2, ps + 2);    \
			out;                                                        \
		case 20:                                                        \
			COPY_BYTES_FWD(16, dst_aligned, src_aligned)                \
		case 4:                                                         \
			Copy_bytes<4, dst_aligned, src_aligned>(pd, ps);            \
			out;                                                        \
		case 21:                                                        \
			COPY_BYTES_FWD(16, dst_aligned, src_aligned)                \
		case 5:                                                         \
			Copy_bytes<4, dst_aligned, src_aligned>(pd, ps);            \
			Copy_bytes<1, dst_aligned, src_aligned>(pd + 4, ps + 4);    \
			out;                                                        \
		case 22:                                                        \
			COPY_BYTES_FWD(16, dst_aligned, src_aligned)                \
		case 6:                                                         \
			Copy_bytes<4, dst_aligned, src_aligned>(pd, ps);            \
			Copy_bytes<2, dst_aligned, src_aligned>(pd + 4, ps + 4);    \
			out;                                                        \
		case 23:                                                        \
			COPY_BYTES_FWD(16, dst_aligned, src_aligned)                \
		case 7:                                                         \
			Copy_bytes<4, dst_aligned, src_aligned>(pd, ps);            \
			Copy_bytes<2, dst_aligned, src_aligned>(pd + 4, ps + 4);    \
			Copy_bytes<1, dst_aligned, src_aligned>(pd + 6, ps + 6);    \
			out;                                                        \
		case 24:                                                        \
			COPY_BYTES_FWD(16, dst_aligned, src_aligned)                \
		case 8:                                                         \
			Copy_bytes<8, dst_aligned, src_aligned>(pd, ps);            \
			out;                                                        \
		case 25:                                                        \
			COPY_BYTES_FWD(16, dst_aligned, src_aligned)                \
		case 9:                                                         \
			Copy_bytes<8, dst_aligned, src_aligned>(pd, ps);            \
			Copy_bytes<1, dst_aligned, src_aligned>(pd + 8, ps + 8);    \
			out;                                                        \
		case 26:                                                        \
			COPY_BYTES_FWD(16, dst_aligned, src_aligned)                \
		case 10:                                                        \
			Copy_bytes<8, dst_aligned, src_aligned>(pd, ps);            \
			Copy_bytes<2, dst_aligned, src_aligned>(pd + 8, ps + 8);    \
			out;                                                        \
		case 27:                                                        \
			COPY_BYTES_FWD(16, dst_aligned, src_aligned)                \
		case 11:                                                        \
			Copy_bytes<8, dst_aligned, src_aligned>(pd, ps);            \
			Copy_bytes<2, dst_aligned, src_aligned>(pd + 8, ps + 8);    \
			Copy_bytes<1, dst_aligned, src_aligned>(pd + 10, ps + 10);  \
			out;                                                        \
		case 28:                                                        \
			COPY_BYTES_FWD(16, dst_aligned, src_aligned)                \
		case 12:                                                        \
			Copy_bytes<8, dst_aligned, src_aligned>(pd, ps);            \
			Copy_bytes<4, dst_aligned, src_aligned>(pd + 8, ps + 8);    \
			out;                                                        \
		case 29:                                                        \
			COPY_BYTES_FWD(16, dst_aligned, src_aligned)                \
		case 13:                                                        \
			Copy_bytes<8, dst_aligned, src_aligned>(pd, ps);            \
			Copy_bytes<4, dst_aligned, src_aligned>(pd + 8, ps + 8);    \
			Copy_bytes<1, dst_aligned, src_aligned>(pd + 12, ps + 12);  \
			out;                                                        \
		case 30:                                                        \
			COPY_BYTES_FWD(16, dst_aligned, src_aligned)                \
		case 14:                                                        \
			Copy_bytes<8, dst_aligned, src_aligned>(pd, ps);            \
			Copy_bytes<4, dst_aligned, src_aligned>(pd + 8, ps + 8);    \
			Copy_bytes<2, dst_aligned, src_aligned>(pd + 12, ps + 12);  \
			out;                                                        \
		case 31:                                                        \
			COPY_BYTES_FWD(16, dst_aligned, src_aligned)                \
		case 15:                                                        \
			Copy_bytes<8, dst_aligned, src_aligned>(pd, ps);            \
			Copy_bytes<4, dst_aligned, src_aligned>(pd + 8, ps + 8);    \
			Copy_bytes<2, dst_aligned, src_aligned>(pd + 12, ps + 12);  \
			Copy_bytes<1, dst_aligned, src_aligned>(pd + 14, ps + 14);  \
			out;

#define COPY_SMALL_BWD(out, dst_aligned, src_aligned)                   \
		case 16:                                                        \
			Copy_bytes<16, dst_aligned, src_aligned>(pd - 16, ps - 16); \
		case 0:                                                         \
			out;                                                        \
		case 17:                                                        \
			COPY_BYTES_BWD(16, dst_aligned, src_aligned)                \
		case 1:                                                         \
			Copy_bytes<1, dst_aligned, src_aligned>(pd - 1, ps - 1);    \
			out;                                                        \
		case 18:                                                        \
			COPY_BYTES_BWD(16, dst_aligned, src_aligned)                \
		case 2:                                                         \
			Copy_bytes<2, dst_aligned, src_aligned>(pd - 2, ps - 2);    \
			out;                                                        \
		case 19:                                                        \
			COPY_BYTES_BWD(16, dst_aligned, src_aligned)                \
		case 3:                                                         \
			Copy_bytes<2, dst_aligned, src_aligned>(pd - 2, ps - 2);    \
			Copy_bytes<1, dst_aligned, src_aligned>(pd - 3, ps - 3);    \
			out;                                                        \
		case 20:                                                        \
			COPY_BYTES_BWD(16, dst_aligned, src_aligned)                \
		case 4:                                                         \
			Copy_bytes<4, dst_aligned, src_aligned>(pd - 4, ps - 4);    \
			out;                                                        \
		case 21:                                                        \
			COPY_BYTES_BWD(16, dst_aligned, src_aligned)                \
		case 5:                                                         \
			Copy_bytes<4, dst_aligned, src_aligned>(pd - 4, ps - 4);    \
			Copy_bytes<1, dst_aligned, src_aligned>(pd - 5, ps - 5);    \
			out;                                                        \
		case 22:                                                        \
			COPY_BYTES_BWD(16, dst_aligned, src_aligned)                \
		case 6:                                                         \
			Copy_bytes<4, dst_aligned, src_aligned>(pd - 4, ps - 4);    \
			Copy_bytes<2, dst_aligned, src_aligned>(pd - 6, ps - 6);    \
			out;                                                        \
		case 23:                                                        \
			COPY_BYTES_BWD(16, dst_aligned, src_aligned)                \
		case 7:                                                         \
			Copy_bytes<4, dst_aligned, src_aligned>(pd - 4, ps - 4);    \
			Copy_bytes<2, dst_aligned, src_aligned>(pd - 6, ps - 6);    \
			Copy_bytes<1, dst_aligned, src_aligned>(pd - 7, ps - 7);    \
			out;                                                        \
		case 24:                                                        \
			COPY_BYTES_BWD(16, dst_aligned, src_aligned)                \
		case 8:                                                         \
			Copy_bytes<8, dst_aligned, src_aligned>(pd - 8, ps - 8);    \
			out;                                                        \
		case 25:                                                        \
			COPY_BYTES_BWD(16, dst_aligned, src_aligned)                \
		case 9:                                                         \
			Copy_bytes<8, dst_aligned, src_aligned>(pd - 8, ps - 8);    \
			Copy_bytes<1, dst_aligned, src_aligned>(pd - 9, ps - 9);    \
			out;                                                        \
		case 26:                                                        \
			COPY_BYTES_BWD(16, dst_aligned, src_aligned)                \
		case 10:                                                        \
			Copy_bytes<8, dst_aligned, src_aligned>(pd - 8, ps - 8);    \
			Copy_bytes<2, dst_aligned, src_aligned>(pd - 10, ps - 10);  \
			out;                                                        \
		case 27:                                                        \
			COPY_BYTES_BWD(16, dst_aligned, src_aligned)                \
		case 11:                                                        \
			Copy_bytes<8, dst_aligned, src_aligned>(pd - 8, ps - 8);    \
			Copy_bytes<2, dst_aligned, src_aligned>(pd - 10, ps - 10);  \
			Copy_bytes<1, dst_aligned, src_aligned>(pd - 11, ps - 11);  \
			out;                                                        \
		case 28:                                                        \
			COPY_BYTES_BWD(16, dst_aligned, src_aligned)                \
		case 12:                                                        \
			Copy_bytes<8, dst_aligned, src_aligned>(pd - 8, ps - 8);    \
			Copy_bytes<4, dst_aligned, src_aligned>(pd - 12, ps - 12);  \
			out;                                                        \
		case 29:                                                        \
			COPY_BYTES_BWD(16, dst_aligned, src_aligned)                \
		case 13:                                                        \
			Copy_bytes<8, dst_aligned, src_aligned>(pd - 8, ps - 8);    \
			Copy_bytes<4, dst_aligned, src_aligned>(pd - 12, ps - 12);  \
			Copy_bytes<1, dst_aligned, src_aligned>(pd - 13, ps - 13);  \
			out;                                                        \
		case 30:                                                        \
			COPY_BYTES_BWD(16, dst_aligned, src_aligned)                \
		case 14:                                                        \
			Copy_bytes<8, dst_aligned, src_aligned>(pd - 8, ps - 8);    \
			Copy_bytes<4, dst_aligned, src_aligned>(pd - 12, ps - 12);  \
			Copy_bytes<2, dst_aligned, src_aligned>(pd - 14, ps - 14);  \
			out;                                                        \
		case 31:                                                        \
			COPY_BYTES_BWD(16, dst_aligned, src_aligned)                \
		case 15:                                                        \
			Copy_bytes<8, dst_aligned, src_aligned>(pd - 8, ps - 8);    \
			Copy_bytes<4, dst_aligned, src_aligned>(pd - 12, ps - 12);  \
			Copy_bytes<2, dst_aligned, src_aligned>(pd - 14, ps - 14);  \
			Copy_bytes<1, dst_aligned, src_aligned>(pd - 15, ps - 15);  \
			out;
#endif
#else
#define COPY_SMALL_FWD(out, dst_aligned, src_aligned)                   \
		case 8:                                                         \
			Copy_bytes<8, dst_aligned, src_aligned>(pd, ps);            \
		case 0:                                                         \
			out;                                                        \
		case 9:                                                         \
			COPY_BYTES_FWD(8, dst_aligned, src_aligned)                 \
		case 1:                                                         \
			Copy_bytes<1, dst_aligned, src_aligned>(pd, ps);            \
			out;                                                        \
		case 10:                                                        \
			COPY_BYTES_FWD(8, dst_aligned, src_aligned)                 \
		case 2:                                                         \
			Copy_bytes<2, dst_aligned, src_aligned>(pd, ps);            \
			out;                                                        \
		case 11:                                                        \
			COPY_BYTES_FWD(8, dst_aligned, src_aligned)                 \
		case 3:                                                         \
			Copy_bytes<2, dst_aligned, src_aligned>(pd, ps);            \
			Copy_bytes<1, dst_aligned, src_aligned>(pd + 2, ps + 2);    \
			out;                                                        \
		case 12:                                                        \
			COPY_BYTES_FWD(8, dst_aligned, src_aligned)                 \
		case 4:                                                         \
			Copy_bytes<4, dst_aligned, src_aligned>(pd, ps);            \
			out;                                                        \
		case 13:                                                        \
			COPY_BYTES_FWD(8, dst_aligned, src_aligned)                 \
		case 5:                                                         \
			Copy_bytes<4, dst_aligned, src_aligned>(pd, ps);            \
			Copy_bytes<1, dst_aligned, src_aligned>(pd + 4, ps + 4);    \
			out;                                                        \
		case 14:                                                        \
			COPY_BYTES_FWD(8, dst_aligned, src_aligned)                 \
		case 6:                                                         \
			Copy_bytes<4, dst_aligned, src_aligned>(pd, ps);            \
			Copy_bytes<2, dst_aligned, src_aligned>(pd + 4, ps + 4);    \
			out;                                                        \
		case 15:                                                        \
			COPY_BYTES_FWD(8, dst_aligned, src_aligned)                 \
		case 7:                                                         \
			Copy_bytes<4, dst_aligned, src_aligned>(pd, ps);            \
			Copy_bytes<2, dst_aligned, src_aligned>(pd + 4, ps + 4);    \
			Copy_bytes<1, dst_aligned, src_aligned>(pd + 6, ps + 6);    \
			out;

#define COPY_SMALL_BWD(out, dst_aligned, src_aligned)                   \
		case 8:                                                         \
			Copy_bytes<8, dst_aligned, src_aligned>(pd - 8, ps - 8);    \
		case 0:                                                         \
			out;                                                        \
		case 9:                                                         \
			COPY_BYTES_BWD(8, dst_aligned, src_aligned)                 \
		case 1:                                                         \
			Copy_bytes<1, dst_aligned, src_aligned>(pd - 1, ps - 1);    \
			out;                                                        \
		case 10:                                                        \
			COPY_BYTES_BWD(8, dst_aligned, src_aligned)                 \
		case 2:                                                         \
			Copy_bytes<2, dst_aligned, src_aligned>(pd - 2, ps - 2);    \
			out;                                                        \
		case 11:                                                        \
			COPY_BYTES_BWD(8, dst_aligned, src_aligned)                 \
		case 3:                                                         \
			Copy_bytes<2, dst_aligned, src_aligned>(pd - 2, ps - 2);    \
			Copy_bytes<1, dst_aligned, src_aligned>(pd - 3, ps - 3);    \
			out;                                                        \
		case 12:                                                        \
			COPY_BYTES_BWD(8, dst_aligned, src_aligned)                 \
		case 4:                                                         \
			Copy_bytes<4, dst_aligned, src_aligned>(pd - 4, ps - 4);    \
			out;                                                        \
		case 13:                                                        \
			COPY_BYTES_BWD(8, dst_aligned, src_aligned)                 \
		case 5:                                                         \
			Copy_bytes<4, dst_aligned, src_aligned>(pd - 4, ps - 4);    \
			Copy_bytes<1, dst_aligned, src_aligned>(pd - 5, ps - 5);    \
			out;                                                        \
		case 14:                                                        \
			COPY_BYTES_BWD(8, dst_aligned, src_aligned)                 \
		case 6:                                                         \
			Copy_bytes<4, dst_aligned, src_aligned>(pd - 4, ps - 4);    \
			Copy_bytes<2, dst_aligned, src_aligned>(pd - 6, ps - 6);    \
			out;                                                        \
		case 15:                                                        \
			COPY_BYTES_BWD(8, dst_aligned, src_aligned)                 \
		case 7:                                                         \
			Copy_bytes<4, dst_aligned, src_aligned>(pd - 4, ps - 4);    \
			Copy_bytes<2, dst_aligned, src_aligned>(pd - 6, ps - 6);    \
			Copy_bytes<1, dst_aligned, src_aligned>(pd - 7, ps - 7);    \
			out;
#endif
#else
#define COPY_SMALL_FWD(out, dst_aligned, src_aligned)                   \
		case 4:                                                         \
			Copy_bytes<4, dst_aligned, src_aligned>(pd, ps);            \
		case 0:                                                         \
			out;                                                        \
		case 5:                                                         \
			COPY_BYTES_FWD(4, dst_aligned, src_aligned)                 \
		case 1:                                                         \
			Copy_bytes<1, dst_aligned, src_aligned>(pd, ps);            \
			out;                                                        \
		case 6:                                                         \
			COPY_BYTES_FWD(4, dst_aligned, src_aligned)                 \
		case 2:                                                         \
			Copy_bytes<2, dst_aligned, src_aligned>(pd, ps);            \
			out;                                                        \
		case 7:                                                         \
			COPY_BYTES_FWD(4, dst_aligned, src_aligned)                 \
		case 3:                                                         \
			Copy_bytes<2, dst_aligned, src_aligned>(pd, ps);            \
			Copy_bytes<1, dst_aligned, src_aligned>(pd + 2, ps + 2);    \
			out;

#define COPY_SMALL_BWD(out, dst_aligned, src_aligned)                   \
		case 4:                                                         \
			Copy_bytes<4, dst_aligned, src_aligned>(pd - 4, ps - 4);    \
		case 0:                                                         \
			out;                                                        \
		case 5:                                                         \
			COPY_BYTES_BWD(4, dst_aligned, src_aligned)                 \
		case 1:                                                         \
			Copy_bytes<1, dst_aligned, src_aligned>(pd - 1, ps - 1);    \
			out;                                                        \
		case 6:                                                         \
			COPY_BYTES_BWD(4, dst_aligned, src_aligned)                 \
		case 2:                                                         \
			Copy_bytes<2, dst_aligned, src_aligned>(pd - 2, ps - 2);    \
			out;                                                        \
		case 7:                                                         \
			COPY_BYTES_BWD(4, dst_aligned, src_aligned)                 \
		case 3:                                                         \
			Copy_bytes<2, dst_aligned, src_aligned>(pd - 2, ps - 2);    \
			Copy_bytes<1, dst_aligned, src_aligned>(pd - 3, ps - 3);    \
			out;
#endif


namespace simd {
	[[gnu::nonnull(1, 2)]] void copy_forwards(void* __restrict dst, const void* __restrict src, size_t size) noexcept {
		auto pd = static_cast<std::byte*>(dst);
		auto ps = static_cast<const std::byte*>(src);

#ifdef __SSE2__
#ifdef __AVX__
#ifdef __AVX512F__
		constexpr size_t base_size = 64;
#else
		constexpr size_t base_size = 32;
#endif
#else
		constexpr size_t base_size = 16;
#endif
#else
		constexpr size_t base_size = 8;
#endif

		switch (size) {
		COPY_SMALL_FWD(return, false, false)
		case base_size:
			Copy_bytes<base_size, false, false>(dst, src);
			return;
		default:
			break;
		}

		const size_t padding = -reinterpret_cast<uintptr_t>(dst) % base_size;
		switch (padding) {
		COPY_SMALL_FWD(break, false, false)
		default:
			break;
		}
		size -= padding;

		pd = static_cast<std::byte*>(dst) + padding;
		ps = static_cast<const std::byte*>(src) + padding;

		if (reinterpret_cast<uintptr_t>(ps) % base_size == 0) {
			while (true) {
				switch (size) {
				COPY_SMALL_FWD(return, true, true)
				case base_size:
					Copy_bytes<base_size, true, true>(pd, ps);
					return;
				default:
					Copy_bytes<base_size, true, true>(pd, ps);
					pd += base_size;
					ps += base_size;
					size -= base_size;
				}
			}
		} else {
			while (true) {
				switch (size) {
				COPY_SMALL_FWD(return, true, false)
				case base_size:
					Copy_bytes<base_size, true, false>(pd, ps);
					return;
				default:
					Copy_bytes<base_size, true, false>(pd, ps);
					pd += base_size;
					ps += base_size;
					size -= base_size;
				}
			}
		}
	}


	[[gnu::nonnull(1, 2)]] void copy_backwards(void* __restrict dst, const void* __restrict src, size_t size) noexcept {
		auto pd = static_cast<std::byte*>(dst) + size;
		auto ps = static_cast<const std::byte*>(src) + size;

#ifdef __SSE2__
#ifdef __AVX__
#ifdef __AVX512F__
		constexpr size_t base_size = 64;
#else
		constexpr size_t base_size = 32;
#endif
#else
		constexpr size_t base_size = 16;
#endif
#else
		constexpr size_t base_size = 8;
#endif

		switch (size) {
		COPY_SMALL_BWD(return, false, false)
		case base_size:
			Copy_bytes<base_size, false, false>(dst, src);
			return;
		default:
			break;
		}

		const size_t padding = reinterpret_cast<uintptr_t>(pd) % base_size;
		switch (padding) {
		COPY_SMALL_BWD(break, false, false)
		default:
			break;
		}
		size -= padding;

		pd = static_cast<std::byte*>(dst) + size;
		ps = static_cast<const std::byte*>(src) + size;

		if (reinterpret_cast<uintptr_t>(ps) % base_size == 0) {
			while (true) {
				switch (size) {
				COPY_SMALL_BWD(return, true, true)
				case base_size:
					Copy_bytes<base_size, true, true>(pd - base_size, ps - base_size);
					return;
				default:
					pd -= base_size;
					ps -= base_size;
					Copy_bytes<base_size, true, true>(pd, ps);
					size -= base_size;
				}
			}
		} else {
			while (true) {
				switch (size) {
				COPY_SMALL_BWD(return, true, false)
				case base_size:
					Copy_bytes<base_size, true, false>(pd - base_size, ps - base_size);
					return;
				default:
					pd -= base_size;
					ps -= base_size;
					Copy_bytes<base_size, true, false>(pd, ps);
					size -= base_size;
				}
			}
		}
	}


	[[gnu::nonnull(1, 2)]] void memory_copy(void* dst, const void* src, size_t size) noexcept {
		if (reinterpret_cast<uintptr_t>(dst) - reinterpret_cast<uintptr_t>(src) >= size) {
			copy_forwards(dst, src, size);
		} else if (dst != src) {
			copy_backwards(dst, src, size);
		}
	}
}


#undef COPY_BYTES_FWD
#undef COPY_BYTES_BWD
#undef COPY_SMALL_FWD
#undef COPY_SMALL_BWD

