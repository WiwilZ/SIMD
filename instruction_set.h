//
// Created by WiwilZ on 2022/1/31.
//

#pragma once

#ifdef _MSC_VER
#include <intrin.h>
#else

#include <x86intrin.h>
#include <cpuid.h>

#endif

#include <unordered_set>
#include <bitset>


namespace {
	struct Impl {
		Impl(const Impl&) = delete;
		Impl(Impl&&) noexcept = delete;
		Impl& operator=(const Impl&) = delete;
		Impl& operator=(Impl&&) noexcept = delete;

		static const Impl& instance() {
			static Impl inst;
			return inst;
		}

		std::string vendor;
		std::string brand;
		std::unordered_set<std::string> instructions;
		bool isIntel;
		bool isAMD;
		std::bitset<32> f1ECX;
		std::bitset<32> f1EDX;
		std::bitset<32> f7EBX;
		std::bitset<32> f7ECX;
		std::bitset<32> f7EDX;
		std::bitset<32> f81ECX;
		std::bitset<32> f81EDX;

	private:
		static auto cpuid(int level) {
			std::array<int, 4> reg;
#if defined(__clang__) || defined(__GNUC__)
			__cpuid(level, reg[0], reg[1], reg[2], reg[3]);
#else
			__cpuid(reg.data(), level);
#endif
			return reg;
		}

		Impl() {
			auto reg = cpuid(0);
			int n = reg[0];

			int _vendor[] = { reg[1], reg[3], reg[2] };
			vendor = reinterpret_cast<char*>(_vendor);
			if (vendor == "GenuineIntel") {
				isIntel = true;
			} else if (vendor == "AuthenticAMD") {
				isAMD = true;
			}

			if (n >= 1) {
				reg = cpuid(1);
				f1ECX = reg[2];
				f1EDX = reg[3];
			}
			if (n >= 7) {
				__cpuidex(reg.data(), 7, 0);
				f7EBX = reg[1];
				f7ECX = reg[2];
				f7EDX = reg[3];
			}

			reg = cpuid(0x80000000);
			n = reg[0];
			if (n >= 0x80000001) {
				__cpuidex(reg.data(), 0x80000001, 0);
				f81ECX = reg[2];
				f81EDX = reg[3];
			}
			if (n >= 0x80000004) {
				int _brand[12] = { 0 };
				__cpuidex(_brand, 0x80000002, 0);
				__cpuidex(_brand + 4, 0x80000003, 0);
				__cpuidex(_brand + 8, 0x80000004, 0);
				brand = reinterpret_cast<char*>(_brand);
			}

			initInstructions();
		}

		void initInstructions() {
#define CPUID_REG(name, reg, bit) if (reg[bit]) instructions.insert(#name);
#define CPUID_f1ECX(name, bit) CPUID_REG(name, f1ECX, bit)
			CPUID_f1ECX(sse3, 0)
			CPUID_f1ECX(pclmulqdq, 1)
			CPUID_f1ECX(dtes64, 2)
			CPUID_f1ECX(monitor, 3)
			CPUID_f1ECX(dscpl, 4)
			CPUID_f1ECX(vmx, 5)
			CPUID_f1ECX(smx, 6)
			CPUID_f1ECX(eist, 7)
			CPUID_f1ECX(tm2, 8)
			CPUID_f1ECX(ssse3, 9)
			CPUID_f1ECX(cnxtid, 10)
			CPUID_f1ECX(sdbg, 11)
			CPUID_f1ECX(fma, 12)
			CPUID_f1ECX(cx16, 13)
			CPUID_f1ECX(xtpr, 14)
			CPUID_f1ECX(pdcm, 15)
			CPUID_f1ECX(pcid, 17)
			CPUID_f1ECX(dca, 18)
			CPUID_f1ECX(sse4.1, 19)
			CPUID_f1ECX(sse4.2, 20)
			CPUID_f1ECX(x2apic, 21)
			CPUID_f1ECX(movbe, 22)
			CPUID_f1ECX(popcnt, 23)
			CPUID_f1ECX(tscdeadline, 24)
			CPUID_f1ECX(aes, 25)
			CPUID_f1ECX(xsave, 26)
			CPUID_f1ECX(osxsave, 27)
			CPUID_f1ECX(avx, 28)
			CPUID_f1ECX(f16c, 29)
			CPUID_f1ECX(rdrand, 30)
#undef CPUID_f1ECX

#define CPUID_f1EDX(name, bit) CPUID_REG(name, f1EDX, bit)
			CPUID_f1EDX(fpu, 0)
			CPUID_f1EDX(vme, 1)
			CPUID_f1EDX(de, 2)
			CPUID_f1EDX(pse, 3)
			CPUID_f1EDX(tsc, 4)
			CPUID_f1EDX(msr, 5)
			CPUID_f1EDX(pae, 6)
			CPUID_f1EDX(mce, 7)
			CPUID_f1EDX(cx8, 8)
			CPUID_f1EDX(apic, 9)
			CPUID_f1EDX(sep, 11)
			CPUID_f1EDX(mtrr, 12)
			CPUID_f1EDX(pge, 13)
			CPUID_f1EDX(mca, 14)
			CPUID_f1EDX(cmov, 15)
			CPUID_f1EDX(pat, 16)
			CPUID_f1EDX(pse36, 17)
			CPUID_f1EDX(psn, 18)
			CPUID_f1EDX(clfsh, 19)
			CPUID_f1EDX(ds, 21)
			CPUID_f1EDX(acpi, 22)
			CPUID_f1EDX(mmx, 23)
			CPUID_f1EDX(fxsr, 24)
			CPUID_f1EDX(sse, 25)
			CPUID_f1EDX(sse2, 26)
			CPUID_f1EDX(ss, 27)
			CPUID_f1EDX(htt, 28)
			CPUID_f1EDX(tm, 29)
			CPUID_f1EDX(pbe, 31)
#undef CPUID_f1EDX


#define CPUID_f7EBX(name, bit) CPUID_REG(name, f7EBX, bit)
			CPUID_f7EBX(fsgsbase, 0)
			CPUID_f7EBX(ia32_tsc_adjust_msr, 1)
			CPUID_f7EBX(sgx, 2)
			CPUID_f7EBX(bmi1, 3)
			CPUID_f7EBX(hle, 4)
			CPUID_f7EBX(avx2, 5)
			CPUID_f7EBX(fdp_excptn_only, 6)
			CPUID_f7EBX(smep, 7)
			CPUID_f7EBX(bmi2, 8)
			CPUID_f7EBX(erms, 9)
			CPUID_f7EBX(invpcid, 10)
			CPUID_f7EBX(rtm, 11)
			CPUID_f7EBX(rdtm, 12)
			CPUID_f7EBX(rdta, 15)
			CPUID_f7EBX(avx512f, 16)
			CPUID_f7EBX(avx512dq, 17)
			CPUID_f7EBX(rdseed, 18)
			CPUID_f7EBX(adx, 19)
			CPUID_f7EBX(smap, 20)
			CPUID_f7EBX(avx512_ifma, 21)
			CPUID_f7EBX(clflushopt, 23)
			CPUID_f7EBX(clwb, 24)
			CPUID_f7EBX(avx512pf, 26)
			CPUID_f7EBX(avx512er, 27)
			CPUID_f7EBX(avx512cd, 28)
			CPUID_f7EBX(sha, 29)
			CPUID_f7EBX(avx512bw, 30)
			CPUID_f7EBX(avx512vl, 31)
#undef CPUID_f7EBX

#define CPUID_f7ECX(name, bit) CPUID_REG(name, f7ECX, bit)
			CPUID_f7ECX(prefetchwt1, 0)
			CPUID_f7ECX(avx512_vbmi, 1)
			CPUID_f7ECX(umip, 2)
			CPUID_f7ECX(pku, 3)
			CPUID_f7ECX(ospke, 4)
			CPUID_f7ECX(waitpkg, 5)
			CPUID_f7ECX(avx512_vbmi2, 6)
			CPUID_f7ECX(cetss, 7)
			CPUID_f7ECX(gfni, 8)
			CPUID_f7ECX(vaes, 9)
			CPUID_f7ECX(vpclmulqdq, 10)
			CPUID_f7ECX(avx512_vnni, 11)
			CPUID_f7ECX(avx512_bitalg, 12)
			CPUID_f7ECX(tmeen, 13)
			CPUID_f7ECX(avx512_vpopcntdq, 14)
			CPUID_f7ECX(la57, 16)
			CPUID_f7ECX(kl, 23)
			CPUID_f7ECX(cldemote, 25)
			CPUID_f7ECX(movdiri, 27)
			CPUID_f7ECX(movdir64b, 28)
			CPUID_f7ECX(enqcmd, 29)
			CPUID_f7ECX(sgxlc, 30)
			CPUID_f7ECX(pks, 31)
#undef CPUID_f7ECX

#define CPUID_f7EDX(name, bit) CPUID_REG(name, f7EDX, bit)
			CPUID_f7EDX(avx512_4vnniw, 2)
			CPUID_f7EDX(avx512_4fmaps, 3)
			CPUID_f7EDX(fsrm, 4)
			CPUID_f7EDX(uintr, 5)
			CPUID_f7EDX(avx512_vp2intersect, 8)
			CPUID_f7EDX(md_clear, 10)
			CPUID_f7EDX(serialize, 14)
			CPUID_f7EDX(hybrid, 15)
			CPUID_f7EDX(tsxldtrk, 16)
			CPUID_f7EDX(pconfig, 18)
			CPUID_f7EDX(cet_ibt, 20)
			CPUID_f7EDX(amx_bf16, 22)
			CPUID_f7EDX(avx512_fp16, 23)
			CPUID_f7EDX(amx_tile, 24)
			CPUID_f7EDX(amx_int8, 25)
#undef CPUID_f7EDX

			CPUID_REG(lahf, f81ECX, 0)
#undef CPUID_REG
#define CPUID_REG(name, cpu, reg, bit) if (is##cpu && reg[bit]) instructions.insert(#name);
#define CPUID_f81ECX(name, cpu, bit) CPUID_REG(name, cpu, f81ECX, bit)
			CPUID_f81ECX(lzcnt, Intel, 5)
			CPUID_f81ECX(abm, AMD, 5)
			CPUID_f81ECX(sse4a, AMD, 6)
			CPUID_f81ECX(prefetchw, Intel, 8)
			CPUID_f81ECX(xop, AMD, 11)
			CPUID_f81ECX(tbm, AMD, 21)
#undef CPUID_f81ECX

#define CPUID_f81EDX(name, cpu, bit) CPUID_REG(name, cpu, f81EDX, bit)
			CPUID_f81EDX(syscall, Intel, 11)
			CPUID_f81EDX(mmxext, AMD, 22)
			CPUID_f81EDX(rdtscp, Intel, 27)
			CPUID_f81EDX(3dnowext, AMD, 30)
			CPUID_f81EDX(3dnow, AMD, 31)
#undef CPUID_f81EDX

#undef CPUID_REG
		}
	};
}

struct InstructionSet {
	static auto vendor() {
		return Impl::instance().vendor;
	}

	static auto brand() {
		return Impl::instance().brand;
	}

	static auto supported_instructions() {
		return Impl::instance().instructions;
	}

	static bool is_supported(const std::string& instruction) {
		return supported_instructions().contains(instruction);
	}
};