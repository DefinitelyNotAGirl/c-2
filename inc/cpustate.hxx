#pragma once
#include <amd64.cgu.hxx>
#include <issues.hxx>

#define cpustate_main

enum class RegisterStatus
{
	invalid = 0,
	free = 1,
	used = 2,
	preserve = 3
};

template<typename T>
class RegisterState {
public:
	RegisterStatus status = RegisterStatus::preserve;
	bool valueKnown = false;
	T value;
};

enum class FlagState
{
	invalid = 0,
	clear = 1,
	set = 2,
	unknown = 3
};

class cpufeatures_amd64
{
public:
};

class cpustate_amd64
{
protected:
	/*
		. integer registers
	*/
	RegisterState< uint64_t> Register_rax;
    RegisterState< uint64_t> Register_rcx;
    RegisterState< uint64_t> Register_rdx;
    RegisterState< uint64_t> Register_rbx;
    RegisterState< uint64_t> Register_rsp;
    RegisterState< uint64_t> Register_rbp;
    RegisterState< uint64_t> Register_rsi;
    RegisterState< uint64_t> Register_rdi;
    RegisterState<  uint8_t> Register_ah;
    RegisterState<  uint8_t> Register_ch;
    RegisterState<  uint8_t> Register_dh;
    RegisterState<  uint8_t> Register_bh;
    RegisterState< uint64_t> Register_r8;
    RegisterState< uint64_t> Register_r9;
    RegisterState< uint64_t> Register_r10;
    RegisterState< uint64_t> Register_r11;
    RegisterState< uint64_t> Register_r12;
    RegisterState< uint64_t> Register_r13;
    RegisterState< uint64_t> Register_r14;
    RegisterState< uint64_t> Register_r15;
	/*
		. floating point registers
	*/
    RegisterState< uint64_t> Register_mmx0;
    RegisterState< uint64_t> Register_mmx1;
    RegisterState< uint64_t> Register_mmx2;
    RegisterState< uint64_t> Register_mmx3;
    RegisterState< uint64_t> Register_mmx4;
    RegisterState< uint64_t> Register_mmx5;
    RegisterState< uint64_t> Register_mmx6;
    RegisterState< uint64_t> Register_mmx7;
    RegisterState<uint128_t> Register_xmm0;
    RegisterState<uint128_t> Register_xmm1;
    RegisterState<uint128_t> Register_xmm2;
    RegisterState<uint128_t> Register_xmm3;
    RegisterState<uint128_t> Register_xmm4;
    RegisterState<uint128_t> Register_xmm5;
    RegisterState<uint128_t> Register_xmm6;
    RegisterState<uint128_t> Register_xmm7;
    RegisterState<uint256_t> Register_ymm0;
    RegisterState<uint256_t> Register_ymm1;
    RegisterState<uint256_t> Register_ymm2;
    RegisterState<uint256_t> Register_ymm3;
    RegisterState<uint256_t> Register_ymm4;
    RegisterState<uint256_t> Register_ymm5;
    RegisterState<uint256_t> Register_ymm6;
    RegisterState<uint256_t> Register_ymm7;
	/*
		. control registers
	*/
    RegisterState< uint64_t> Register_efer;
    RegisterState< uint64_t> Register_star;
    RegisterState< uint64_t> Register_lstar;
    RegisterState< uint64_t> Register_cstar;
    RegisterState< uint64_t> Register_sfmask;
    RegisterState< uint64_t> Register_fs_base;
    RegisterState< uint64_t> Register_gs_base;
    RegisterState< uint64_t> Register_kernel_gs_base;
    RegisterState< uint64_t> Register_tsc_aux;
    RegisterState< uint64_t> Register_syscfg;
    RegisterState< uint64_t> Register_iorr_base0;
    RegisterState< uint64_t> Register_iorrmask0;
    RegisterState< uint64_t> Register_iorr_base1;
    RegisterState< uint64_t> Register_iorrmask1;
    RegisterState< uint64_t> Register_tom2;
    RegisterState< uint64_t> Register_ls_cfg;
    RegisterState< uint64_t> Register_ic_cfg;
    RegisterState< uint64_t> Register_dc_cfg;
    RegisterState< uint64_t> Register_bu_cfg;
    RegisterState< uint64_t> Register_mc0_ctl;
    RegisterState< uint64_t> Register_mc0_status;
    RegisterState< uint64_t> Register_mc0_addr;
    RegisterState< uint64_t> Register_mc0_misc;
    RegisterState< uint64_t> Register_perf_ctl0;
    RegisterState< uint64_t> Register_perf_ctr0;
    RegisterState< uint64_t> Register_perf_ctl1;
    RegisterState< uint64_t> Register_perf_ctr1;
    RegisterState< uint64_t> Register_top_mem;
    RegisterState< uint64_t> Register_top_mem2;
    RegisterState< uint64_t> Register_vm_cr;
    RegisterState< uint64_t> Register_vm_hsave_pa;
	RegisterState< uint64_t> Register_cr0;
	RegisterState< uint64_t> Register_cr2;
	RegisterState< uint64_t> Register_cr3;
	RegisterState< uint64_t> Register_cr4;
	RegisterState< uint64_t> Register_cr8;
	RegisterState< uint80_t> Register_gdtr;
	RegisterState< uint80_t> Register_idtr;
	RegisterState< uint80_t> Register_ldtr;
public:
	void registerStatus(amd64::Register reg, RegisterStatus status);
	RegisterStatus registerStatus(amd64::Register reg) const;
	#include <cpustate.templates.hxx>
	bool registerValueKnown(amd64::Register reg) const;
	amd64::Register getFreeRegister();
	/*
		. Flags
	*/
	FlagState OverflowFlag = FlagState::unknown;
	FlagState DirectionFlag = FlagState::unknown;
	FlagState SignFlag = FlagState::unknown;
	FlagState ZeroFlag = FlagState::unknown;
	FlagState AuxiliaryCarryFlag = FlagState::unknown;
	FlagState ParityFlag = FlagState::unknown;
	FlagState CarryFlag = FlagState::unknown;
	//+ system flags
	FlagState IdFlag = FlagState::unknown;
	FlagState VirtualInterruptPendingFlag = FlagState::unknown;
	FlagState VirtualInterruptFlag = FlagState::unknown;
	FlagState AlignmentCheckFlag = FlagState::unknown;
	FlagState Virtual8086ModeFlag = FlagState::unknown;
	FlagState ResumeFlag = FlagState::unknown;
	FlagState NestedTaskFlag = FlagState::unknown;
	FlagState TrapFlag = FlagState::unknown;
	FlagState InterruptFlag = FlagState::unknown;
	/*
		. Privilege levels
	*/
	uint8_t cpuPrivilegeLevel = 3;
	uint8_t ioPrivilegeLevel = 0;
	/*
		. Features
	*/
	cpufeatures_amd64 Features;
};

class cpustate
{
public:
	cpustate_amd64 amd64;
};