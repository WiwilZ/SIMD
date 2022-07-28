/*
 * Created by WiwilZ on 2022/7/13.
 */

#pragma once


#include <bit>
#include <cstdint>
#include <x86intrin.h>


#define INLINE [[using gnu : always_inline, artificial]] inline


namespace {
	template <size_t n_bytes>
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


	enum class Case : uint8_t {
		lower,
		upper,
		flip
	};


	template <size_t n_bytes, bool all_alpha, Case Case>
	requires (all_alpha && (n_bytes == 1 || n_bytes == 2 || n_bytes == 4 || n_bytes == 8))
	INLINE constexpr void Change_case(char* dst, const char* src) noexcept {
		using type = typename UInt<n_bytes>::type;
		const type c0 = 0x2020202020202020;
		const auto e = *reinterpret_cast<const type*>(src);
		*reinterpret_cast<type*>(dst) = Case == Case::lower ? e | c0 : Case == Case::upper ? e & ~c0 : e ^ c0;
	}

#ifdef __SSE2__

	template <size_t n_bytes, bool all_alpha, Case Case>
	requires (all_alpha && n_bytes == 16)
	INLINE void Change_case(char* dst, const char* src) noexcept {
		const auto t0 = _mm_loadu_si128(reinterpret_cast<const __m128i*>(src));
		const auto t1 = Case == Case::lower ? _mm_or_si128(t0, _mm_set1_epi8(0x20)) :
		                Case == Case::upper ? _mm_and_si128(t0, _mm_set1_epi8(~0x20)) :
		                _mm_xor_si128(t0, _mm_set1_epi8(0x20));
		_mm_storeu_si128(reinterpret_cast<__m128i*>(dst), t1);
	}

#if defined(__AVX__) && defined(__AVX2__)

	template <size_t n_bytes, bool all_alpha, Case Case>
	requires (all_alpha && n_bytes == 32)
	INLINE void Change_case(char* dst, const char* src) noexcept {
		const auto t0 = _mm256_loadu_si256(reinterpret_cast<const __m256i*>(src));
		const auto t1 = Case == Case::lower ? _mm256_or_si256(t0, _mm256_set1_epi8(0x20)) :
		                Case == Case::upper ? _mm256_and_si256(t0, _mm256_set1_epi8(~0x20)) :
		                _mm256_xor_si256(t0, _mm256_set1_epi8(0x20));
		_mm256_storeu_si256(reinterpret_cast<__m256i*>(dst), t1);
	}

#ifdef __AVX512F__

	template <size_t n_bytes, bool all_alpha, Case Case>
	requires (all_alpha && n_bytes == 64)
	INLINE void Change_case(char* dst, const char* src) noexcept {
		const auto t0 = _mm512_loadu_si512(src);
		const auto t1 = Case == Case::lower ? _mm512_or_si512(t0, _mm512_set1_epi8(0x20)) :
		                Case == Case::upper ? _mm512_and_si512(t0, _mm512_set1_epi8(~0x20)) :
		                _mm512_xor_si512(t0, _mm512_set1_epi8(0x20));
		_mm512_storeu_si512(dst, t1);
	}

#endif
#endif
#endif


	template <size_t n_bytes, bool all_alpha, Case Case>
	requires (!all_alpha && n_bytes == 1)
	INLINE constexpr void Change_case(char* dst, const char* src) noexcept {
		*dst = Case == Case::lower && 'A' <= *src && *src <= 'Z' ||
		       Case == Case::upper && 'a' <= *src && *src <= 'z' ||
		       Case == Case::flip && ('A' <= *src && *src <= 'Z' || 'a' <= *src && *src <= 'z') ?
		       *src ^ static_cast<char>(0x20) : *src;
	}

#ifdef __MMX__

	template <size_t n_bytes, bool all_alpha, Case Case>
	requires (!all_alpha && n_bytes == 8)
	INLINE void Change_case(char* dst, const char* src) noexcept {
		const auto c0 = _mm_set1_pi8((Case == Case::lower ? 'A' : 'a') + static_cast<char>(128));
		const auto c1 = _mm_set1_pi8(static_cast<char>(-128) + 'z' - 'a');
		const auto c2 = _mm_set1_pi8(0x20);

		const auto t0 = _m_from_int64(*reinterpret_cast<const long long*>(src));
		const auto t1 = _mm_sub_pi8(Case == Case::flip ? _mm_or_si64(t0, c2) : t0, c0);
		const auto t2 = _mm_cmpgt_pi8(t1, c1);
		const auto t3 = _mm_andnot_si64(t2, c2);
		const auto t4 = _mm_xor_si64(t0, t3);
		*reinterpret_cast<long long*>(dst) = _m_to_int64(t4);
	}

#ifdef __SSE2__

	template <size_t n_bytes, bool all_alpha, Case Case>
	requires (!all_alpha && n_bytes == 16)
	INLINE void Change_case(char* dst, const char* src) noexcept {
		const auto c0 = _mm_set1_epi8((Case == Case::lower ? 'A' : 'a') + static_cast<char>(128));
		const auto c1 = _mm_set1_epi8(static_cast<char>(-128) + 'z' - 'a');
		const auto c2 = _mm_set1_epi8(0x20);

		const auto t0 = _mm_loadu_si128(reinterpret_cast<const __m128i*>(src));
		const auto t1 = _mm_sub_epi8(Case == Case::flip ? _mm_or_si128(t0, c2) : t0, c0);
		const auto t2 = _mm_cmpgt_epi8(t1, c1);
		const auto t3 = _mm_andnot_si128(t2, c2);
		const auto t4 = _mm_xor_si128(t0, t3);
		_mm_storeu_si128(reinterpret_cast<__m128i*>(dst), t4);
	}

#if defined(__AVX__) && defined(__AVX2__)

	template <size_t n_bytes, bool all_alpha, Case Case>
	requires (!all_alpha && n_bytes == 32)
	INLINE void Change_case(char* dst, const char* src) noexcept {
		const auto c0 = _mm256_set1_epi8((Case == Case::lower ? 'A' : 'a') + static_cast<char>(128));
		const auto c1 = _mm256_set1_epi8(static_cast<char>(-128) + 'z' - 'a');
		const auto c2 = _mm256_set1_epi8(0x20);

		const auto t0 = _mm256_loadu_si256(reinterpret_cast<const __m256i*>(src));
		const auto t1 = _mm256_sub_epi8(Case == Case::flip ? _mm256_or_si256(t0, c2) : t0, c0);
		const auto t2 = _mm256_cmpgt_epi8(t1, c1);
		const auto t3 = _mm256_andnot_si256(t2, c2);
		const auto t4 = _mm256_xor_si256(t0, t3);
		_mm256_storeu_si256(reinterpret_cast<__m256i*>(dst), t4);
	}

#if defined(__AVX512F__) && defined(__AVX512BW__)

	template <size_t n_bytes, bool all_alpha, Case Case>
	requires (!all_alpha && n_bytes == 64)
	INLINE void Change_case(char* dst, const char* src) noexcept {
		const auto c0 = _mm512_set1_epi8((Case == Case::lower ? 'A' : 'a') + static_cast<char>(128));
		const auto c1 = _mm512_set1_epi8(static_cast<char>(-128) + 'z' - 'a');
		const auto c2 = _mm512_set1_epi8(0x20);
		const auto c3 = _mm512_setzero_si512();

		const auto t0 = _mm512_loadu_si512(src);
		const auto t1 = _mm512_sub_epi8(Case == Case::flip ? _mm512_or_si512(t0, c2) : t0, c0);
		const auto t2 = _mm512_cmple_epi8_mask(t1, c1);
		const auto t3 = _mm512_mask_blend_epi8(t2, c3, c2);
		const auto t4 = _mm512_xor_si512(t0, t3);
		_mm512_storeu_si512(dst, t4);
	}

#endif
#endif
#endif
#endif


	template <bool all_alpha, Case Case>
	requires(all_alpha)
	[[gnu::nonnull(1, 2)]] constexpr void Change_case(char* dst, const char* src, size_t size) noexcept {
		switch (size) {
#ifdef __SSE2__
#if defined(__AVX__) && defined(__AVX2__)
#ifdef __AVX512F__
		case 64:
			Change_case<64, true, Case>(dst, src);
			return;
		case 63:
			Change_case<32, true, Case>(dst, src);
			Change_case<32, true, Case>(dst + 31, src + 31);
			return;
		case 62:
			Change_case<32, true, Case>(dst, src);
			Change_case<32, true, Case>(dst + 30, src + 30);
			return;
		case 61:
			Change_case<32, true, Case>(dst, src);
			Change_case<32, true, Case>(dst + 29, src + 29);
			return;
		case 60:
			Change_case<32, true, Case>(dst, src);
			Change_case<32, true, Case>(dst + 28, src + 28);
			return;
		case 59:
			Change_case<32, true, Case>(dst, src);
			Change_case<32, true, Case>(dst + 27, src + 27);
			return;
		case 58:
			Change_case<32, true, Case>(dst, src);
			Change_case<32, true, Case>(dst + 26, src + 26);
			return;
		case 57:
			Change_case<32, true, Case>(dst, src);
			Change_case<32, true, Case>(dst + 25, src + 25);
			return;
		case 56:
			Change_case<32, true, Case>(dst, src);
			Change_case<32, true, Case>(dst + 24, src + 24);
			return;
		case 55:
			Change_case<32, true, Case>(dst, src);
			Change_case<32, true, Case>(dst + 23, src + 23);
			return;
		case 54:
			Change_case<32, true, Case>(dst, src);
			Change_case<32, true, Case>(dst + 22, src + 22);
			return;
		case 53:
			Change_case<32, true, Case>(dst, src);
			Change_case<32, true, Case>(dst + 21, src + 21);
			return;
		case 52:
			Change_case<32, true, Case>(dst, src);
			Change_case<32, true, Case>(dst + 20, src + 20);
			return;
		case 51:
			Change_case<32, true, Case>(dst, src);
			Change_case<32, true, Case>(dst + 19, src + 19);
			return;
		case 50:
			Change_case<32, true, Case>(dst, src);
			Change_case<32, true, Case>(dst + 18, src + 18);
			return;
		case 49:
			Change_case<32, true, Case>(dst, src);
			Change_case<32, true, Case>(dst + 17, src + 17);
			return;
		case 48:
			Change_case<32, true, Case>(dst, src);
			Change_case<16, true, Case>(dst + 32, src + 32);
			return;
		case 47:
			Change_case<32, true, Case>(dst, src);
			Change_case<16, true, Case>(dst + 31, src + 31);
			return;
		case 46:
			Change_case<32, true, Case>(dst, src);
			Change_case<16, true, Case>(dst + 30, src + 30);
			return;
		case 45:
			Change_case<32, true, Case>(dst, src);
			Change_case<16, true, Case>(dst + 29, src + 29);
			return;
		case 44:
			Change_case<32, true, Case>(dst, src);
			Change_case<16, true, Case>(dst + 28, src + 28);
			return;
		case 43:
			Change_case<32, true, Case>(dst, src);
			Change_case<16, true, Case>(dst + 27, src + 27);
			return;
		case 42:
			Change_case<32, true, Case>(dst, src);
			Change_case<16, true, Case>(dst + 26, src + 26);
			return;
		case 41:
			Change_case<32, true, Case>(dst, src);
			Change_case<16, true, Case>(dst + 25, src + 25);
			return;
		case 40:
			Change_case<32, true, Case>(dst, src);
			Change_case<8, true, Case>(dst + 32, src + 32);
			return;
		case 39:
			Change_case<32, true, Case>(dst, src);
			Change_case<8, true, Case>(dst + 31, src + 31);
			return;
		case 38:
			Change_case<32, true, Case>(dst, src);
			Change_case<8, true, Case>(dst + 30, src + 30);
			return;
		case 37:
			Change_case<32, true, Case>(dst, src);
			Change_case<8, true, Case>(dst + 29, src + 29);
			return;
		case 36:
			Change_case<32, true, Case>(dst, src);
			Change_case<4, true, Case>(dst + 32, src + 32);
			return;
		case 35:
			Change_case<32, true, Case>(dst, src);
			Change_case<4, true, Case>(dst + 31, src + 31);
			return;
		case 34:
			Change_case<32, true, Case>(dst, src);
			Change_case<2, true, Case>(dst + 32, src + 32);
			return;
		case 33:
			Change_case<32, true, Case>(dst, src);
			Change_case<1, true, Case>(dst + 32, src + 32);
			return;
#endif
		case 32:
			Change_case<32, true, Case>(dst, src);
			return;
		case 31:
			Change_case<16, true, Case>(dst, src);
			Change_case<16, true, Case>(dst + 15, src + 15);
			return;
		case 30:
			Change_case<16, true, Case>(dst, src);
			Change_case<16, true, Case>(dst + 14, src + 14);
			return;
		case 29:
			Change_case<16, true, Case>(dst, src);
			Change_case<16, true, Case>(dst + 13, src + 13);
			return;
		case 28:
			Change_case<16, true, Case>(dst, src);
			Change_case<16, true, Case>(dst + 12, src + 12);
			return;
		case 27:
			Change_case<16, true, Case>(dst, src);
			Change_case<16, true, Case>(dst + 11, src + 11);
			return;
		case 26:
			Change_case<16, true, Case>(dst, src);
			Change_case<16, true, Case>(dst + 10, src + 10);
			return;
		case 25:
			Change_case<16, true, Case>(dst, src);
			Change_case<16, true, Case>(dst + 9, src + 9);
			return;
		case 24:
			Change_case<16, true, Case>(dst, src);
			Change_case<8, true, Case>(dst + 16, src + 16);
			return;
		case 23:
			Change_case<16, true, Case>(dst, src);
			Change_case<8, true, Case>(dst + 15, src + 15);
			return;
		case 22:
			Change_case<16, true, Case>(dst, src);
			Change_case<8, true, Case>(dst + 14, src + 14);
			return;
		case 21:
			Change_case<16, true, Case>(dst, src);
			Change_case<8, true, Case>(dst + 13, src + 13);
			return;
		case 20:
			Change_case<16, true, Case>(dst, src);
			Change_case<4, true, Case>(dst + 16, src + 16);
			return;
		case 19:
			Change_case<16, true, Case>(dst, src);
			Change_case<4, true, Case>(dst + 15, src + 15);
			return;
		case 18:
			Change_case<16, true, Case>(dst, src);
			Change_case<2, true, Case>(dst + 16, src + 16);
			return;
		case 17:
			Change_case<16, true, Case>(dst, src);
			Change_case<1, true, Case>(dst + 16, src + 16);
			return;
#endif
		case 16:
			Change_case<16, true, Case>(dst, src);
			return;
		case 15:
			Change_case<8, true, Case>(dst, src);
			Change_case<8, true, Case>(dst + 7, src + 7);
			return;
		case 14:
			Change_case<8, true, Case>(dst, src);
			Change_case<8, true, Case>(dst + 6, src + 6);
			return;
		case 13:
			Change_case<8, true, Case>(dst, src);
			Change_case<8, true, Case>(dst + 5, src + 5);
			return;
		case 12:
			Change_case<8, true, Case>(dst, src);
			Change_case<4, true, Case>(dst + 8, src + 8);
			return;
		case 11:
			Change_case<8, true, Case>(dst, src);
			Change_case<4, true, Case>(dst + 7, src + 7);
			return;
		case 10:
			Change_case<8, true, Case>(dst, src);
			Change_case<2, true, Case>(dst + 8, src + 8);
			return;
		case 9:
			Change_case<8, true, Case>(dst, src);
			Change_case<1, true, Case>(dst + 8, src + 8);
			return;
#endif
		case 8:
			Change_case<8, true, Case>(dst, src);
			return;
		case 7:
			Change_case<4, true, Case>(dst, src);
			Change_case<4, true, Case>(dst + 3, src + 3);
			return;
		case 6:
			Change_case<4, true, Case>(dst, src);
			Change_case<2, true, Case>(dst + 4, src + 4);
			return;
		case 5:
			Change_case<4, true, Case>(dst, src);
			Change_case<1, true, Case>(dst + 4, src + 4);
			return;
		case 4:
			Change_case<4, true, Case>(dst, src);
			return;
		case 3:
			Change_case<2, true, Case>(dst, src);
			Change_case<1, true, Case>(dst + 2, src + 2);
			return;
		case 2:
			Change_case<2, true, Case>(dst, src);
			return;
		case 1:
			Change_case<1, true, Case>(dst, src);
			return;
		case 0:
			return;
		default:
			break;
		}

		auto pd = dst + size;
		auto ps = src + size;

#ifdef __SSE2__
#if defined(__AVX__) && defined(__AVX2__)
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

		while (true) {
			switch (size) {
			case 0:
				return;
			case 1:
				Change_case<1, true, Case>(dst, src);
				return;
			case 2:
				Change_case<2, true, Case>(dst, src);
				return;
			case 3:
				Change_case<4, true, Case>(pd - 4, ps - 4);
				return;
			case 4:
				Change_case<4, true, Case>(dst, src);
				return;
			case 5:
			case 6:
			case 7:
				Change_case<8, true, Case>(pd - 8, ps - 8);
				return;
			case 8:
				Change_case<8, true, Case>(dst, src);
				return;
#ifdef __SSE2__
			case 9:
			case 10:
			case 11:
			case 12:
			case 13:
			case 14:
			case 15:
				Change_case<16, true, Case>(pd - 16, ps - 16);
				return;
			case 16:
				Change_case<16, true, Case>(dst, src);
				return;
#if defined(__AVX__) && defined(__AVX2__)
			case 17:
			case 18:
			case 19:
			case 20:
			case 21:
			case 22:
			case 23:
			case 24:
			case 25:
			case 26:
			case 27:
			case 28:
			case 29:
			case 30:
			case 31:
				Change_case<32, true, Case>(pd - 32, ps - 32);
				return;
			case 32:
				Change_case<32, true, Case>(dst, src);
				return;
#ifdef __AVX512F__
			case 33:
			case 34:
			case 35:
			case 36:
			case 37:
			case 38:
			case 39:
			case 40:
			case 41:
			case 42:
			case 43:
			case 44:
			case 45:
			case 46:
			case 47:
			case 48:
			case 49:
			case 50:
			case 51:
			case 52:
			case 53:
			case 54:
			case 55:
			case 56:
			case 57:
			case 58:
			case 59:
			case 60:
			case 61:
			case 62:
			case 63:
				Change_case<64, true, Case>(pd - 64, ps - 64);
				return;
			case 64:
				Change_case<64, true, Case>(dst, src);
				return;
#endif
#endif
#endif
			default:
				Change_case<base_size, true, Case>(dst, src);
				size -= base_size;
				src += base_size;
				dst += base_size;
			}
		}
	}


	template <bool all_alpha, Case Case>
	requires (!all_alpha)
	[[gnu::nonnull(1, 2)]] constexpr void Change_case(char* dst, const char* src, size_t size) noexcept {
#ifdef __MMX__
#ifdef __SSE2__
#if defined(__AVX__) && defined(__AVX2__)
#if defined(__AVX512F__) && defined(__AVX512BW__)
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
#ifdef __SSE2__
#if defined(__AVX__) && defined(__AVX2__)
#if defined(__AVX512F__) && defined(__AVX512BW__)
		case 64:
			Change_case<64, false, Case>(dst, src);
			return;
		case 63:
			Change_case<32, false, Case>(dst, src);
			Change_case<32, false, Case>(dst + 31, src + 31);
			return;
		case 62:
			Change_case<32, false, Case>(dst, src);
			Change_case<32, false, Case>(dst + 30, src + 30);
			return;
		case 61:
			Change_case<32, false, Case>(dst, src);
			Change_case<32, false, Case>(dst + 29, src + 29);
			return;
		case 60:
			Change_case<32, false, Case>(dst, src);
			Change_case<32, false, Case>(dst + 28, src + 28);
			return;
		case 59:
			Change_case<32, false, Case>(dst, src);
			Change_case<32, false, Case>(dst + 27, src + 27);
			return;
		case 58:
			Change_case<32, false, Case>(dst, src);
			Change_case<32, false, Case>(dst + 26, src + 26);
			return;
		case 57:
			Change_case<32, false, Case>(dst, src);
			Change_case<32, false, Case>(dst + 25, src + 25);
			return;
		case 56:
			Change_case<32, false, Case>(dst, src);
			Change_case<32, false, Case>(dst + 24, src + 24);
			return;
		case 55:
			Change_case<32, false, Case>(dst, src);
			Change_case<32, false, Case>(dst + 23, src + 23);
			return;
		case 54:
			Change_case<32, false, Case>(dst, src);
			Change_case<32, false, Case>(dst + 22, src + 22);
			return;
		case 53:
			Change_case<32, false, Case>(dst, src);
			Change_case<32, false, Case>(dst + 21, src + 21);
			return;
		case 52:
			Change_case<32, false, Case>(dst, src);
			Change_case<32, false, Case>(dst + 20, src + 20);
			return;
		case 51:
			Change_case<32, false, Case>(dst, src);
			Change_case<32, false, Case>(dst + 19, src + 19);
			return;
		case 50:
			Change_case<32, false, Case>(dst, src);
			Change_case<32, false, Case>(dst + 18, src + 18);
			return;
		case 49:
			Change_case<32, false, Case>(dst, src);
			Change_case<32, false, Case>(dst + 17, src + 17);
			return;
		case 48:
			Change_case<32, false, Case>(dst, src);
			Change_case<16, false, Case>(dst + 32, src + 32);
			return;
		case 47:
			Change_case<32, false, Case>(dst, src);
			Change_case<16, false, Case>(dst + 31, src + 31);
			return;
		case 46:
			Change_case<32, false, Case>(dst, src);
			Change_case<16, false, Case>(dst + 30, src + 30);
			return;
		case 45:
			Change_case<32, false, Case>(dst, src);
			Change_case<16, false, Case>(dst + 29, src + 29);
			return;
		case 44:
			Change_case<32, false, Case>(dst, src);
			Change_case<16, false, Case>(dst + 28, src + 28);
			return;
		case 43:
			Change_case<32, false, Case>(dst, src);
			Change_case<16, false, Case>(dst + 27, src + 27);
			return;
		case 42:
			Change_case<32, false, Case>(dst, src);
			Change_case<16, false, Case>(dst + 26, src + 26);
			return;
		case 41:
			Change_case<32, false, Case>(dst, src);
			Change_case<16, false, Case>(dst + 25, src + 25);
			return;
		case 40:
			Change_case<32, false, Case>(dst, src);
			Change_case<8, false, Case>(dst + 32, src + 32);
			return;
		case 39:
			Change_case<32, false, Case>(dst, src);
			Change_case<8, false, Case>(dst + 31, src + 31);
			return;
		case 38:
			Change_case<32, false, Case>(dst, src);
			Change_case<8, false, Case>(dst + 30, src + 30);
			return;
		case 37:
			Change_case<32, false, Case>(dst, src);
			Change_case<8, false, Case>(dst + 29, src + 29);
			return;
		case 36:
			Change_case<1, false, Case>(dst++, src++);
		case 35:
			Change_case<1, false, Case>(dst++, src++);
		case 34:
			Change_case<1, false, Case>(dst++, src++);
		case 33:
			Change_case<1, false, Case>(dst++, src++);
#endif
		case 32:
			Change_case<32, false, Case>(dst, src);
			return;
		case 31:
			Change_case<16, false, Case>(dst, src);
			Change_case<16, false, Case>(dst + 15, src + 15);
			return;
		case 30:
			Change_case<16, false, Case>(dst, src);
			Change_case<16, false, Case>(dst + 14, src + 14);
			return;
		case 29:
			Change_case<16, false, Case>(dst, src);
			Change_case<16, false, Case>(dst + 13, src + 13);
			return;
		case 28:
			Change_case<16, false, Case>(dst, src);
			Change_case<16, false, Case>(dst + 12, src + 12);
			return;
		case 27:
			Change_case<16, false, Case>(dst, src);
			Change_case<16, false, Case>(dst + 11, src + 11);
			return;
		case 26:
			Change_case<16, false, Case>(dst, src);
			Change_case<16, false, Case>(dst + 10, src + 10);
			return;
		case 25:
			Change_case<16, false, Case>(dst, src);
			Change_case<16, false, Case>(dst + 9, src + 9);
			return;
		case 24:
			Change_case<16, false, Case>(dst, src);
			Change_case<8, false, Case>(dst + 16, src + 16);
			return;
		case 23:
			Change_case<16, false, Case>(dst, src);
			Change_case<8, false, Case>(dst + 15, src + 15);
			return;
		case 22:
			Change_case<16, false, Case>(dst, src);
			Change_case<8, false, Case>(dst + 14, src + 14);
			return;
		case 21:
			Change_case<16, false, Case>(dst, src);
			Change_case<8, false, Case>(dst + 13, src + 13);
			return;
		case 20:
			Change_case<1, false, Case>(dst++, src++);
		case 19:
			Change_case<1, false, Case>(dst++, src++);
		case 18:
			Change_case<1, false, Case>(dst++, src++);
		case 17:
			Change_case<1, false, Case>(dst++, src++);
#endif
		case 16:
			Change_case<16, false, Case>(dst, src);
			return;
		case 15:
			Change_case<8, false, Case>(dst, src);
			Change_case<8, false, Case>(dst + 7, src + 7);
			return;
		case 14:
			Change_case<8, false, Case>(dst, src);
			Change_case<8, false, Case>(dst + 6, src + 6);
			return;
		case 13:
			Change_case<8, false, Case>(dst, src);
			Change_case<8, false, Case>(dst + 5, src + 5);
			return;
		case 12:
			Change_case<1, false, Case>(dst++, src++);
		case 11:
			Change_case<1, false, Case>(dst++, src++);
		case 10:
			Change_case<1, false, Case>(dst++, src++);
		case 9:
			Change_case<1, false, Case>(dst++, src++);
#endif
		case 8:
			Change_case<8, false, Case>(dst, src);
			return;
		case 7:
			Change_case<1, false, Case>(dst++, src++);
		case 6:
			Change_case<1, false, Case>(dst++, src++);
		case 5:
			Change_case<1, false, Case>(dst++, src++);
		case 4:
			Change_case<1, false, Case>(dst++, src++);
		case 3:
			Change_case<1, false, Case>(dst++, src++);
		case 2:
			Change_case<1, false, Case>(dst++, src++);
		case 1:
			Change_case<1, false, Case>(dst, src);
		case 0:
			return;
		default:
			break;
		}

		auto pd = dst + size;
		auto ps = src + size;

		while (true) {
			switch (size) {
			case 4:
				Change_case<1, false, Case>(dst++, src++);
			case 3:
				Change_case<1, false, Case>(dst++, src++);
			case 2:
				Change_case<1, false, Case>(dst++, src++);
			case 1:
				Change_case<1, false, Case>(dst, src);
			case 0:
				return;
			case 5:
			case 6:
			case 7:
				Change_case<8, false, Case>(pd - 8, ps - 8);
				return;
			case 8:
				Change_case<8, false, Case>(dst, src);
				return;
#ifdef __SSE2__
			case 9:
			case 10:
			case 11:
			case 12:
			case 13:
			case 14:
			case 15:
				Change_case<16, false, Case>(pd - 16, ps - 16);
				return;
			case 16:
				Change_case<16, false, Case>(dst, src);
				return;
#if defined(__AVX__) && defined(__AVX2__)
			case 17:
			case 18:
			case 19:
			case 20:
			case 21:
			case 22:
			case 23:
			case 24:
			case 25:
			case 26:
			case 27:
			case 28:
			case 29:
			case 30:
			case 31:
				Change_case<32, false, Case>(pd - 32, ps - 32);
				return;
			case 32:
				Change_case<32, false, Case>(dst, src);
				return;
#if defined(__AVX512F__) && defined(__AVX512BW__)
			case 33:
			case 34:
			case 35:
			case 36:
			case 37:
			case 38:
			case 39:
			case 40:
			case 41:
			case 42:
			case 43:
			case 44:
			case 45:
			case 46:
			case 47:
			case 48:
			case 49:
			case 50:
			case 51:
			case 52:
			case 53:
			case 54:
			case 55:
			case 56:
			case 57:
			case 58:
			case 59:
			case 60:
			case 61:
			case 62:
			case 63:
				Change_case<64, false, Case>(pd - 64, ps - 64);
				return;
			case 64:
				Change_case<64, false, Case>(dst, src);
				return;
#endif
#endif
#endif
			default:
				Change_case<base_size, false, Case>(dst, src);
				size -= base_size;
				src += base_size;
				dst += base_size;
			}
		}
#else
		for (size_t i = 0; i < size; i++) {
				Change_case<1, false, Case>(dst, src);
			}
#endif
	}
}


namespace simd {
	template <bool all_alpha = false>
	[[gnu::nonnull(1, 2)]] constexpr void
	to_lower(char* __restrict dst, const char* __restrict src, size_t size) noexcept {
		Change_case<all_alpha, Case::lower>(dst, src, size);
	}

	template <bool all_alpha = false>
	[[gnu::nonnull(1, 2)]] constexpr void
	to_upper(char* __restrict dst, const char* __restrict src, size_t size) noexcept {
		Change_case<all_alpha, Case::upper>(dst, src, size);
	}

	template <bool all_alpha = false>
	[[gnu::nonnull(1, 2)]] constexpr void
	flip_case(char* __restrict dst, const char* __restrict src, size_t size) noexcept {
		Change_case<all_alpha, Case::flip>(dst, src, size);
	}


	template <bool all_alpha = false>
	[[gnu::nonnull(1)]] constexpr void to_lower(char* s, size_t size) noexcept {
		Change_case<all_alpha, Case::lower>(s, s, size);
	}

	template <bool all_alpha = false>
	[[gnu::nonnull(1)]] constexpr void to_upper(char* s, size_t size) noexcept {
		Change_case<all_alpha, Case::upper>(s, s, size);
	}

	template <bool all_alpha = false>
	[[gnu::nonnull(1)]] constexpr void flip_case(char* s, size_t size) noexcept {
		Change_case<all_alpha, Case::flip>(s, s, size);
	}
}