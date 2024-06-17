#include <amd64.cgu.hxx>

enum class RegisterStatus
{
	invalid = 0,
	free = 1,
	used = 2,
	preserve = 3
};

class cpustate_amd64
{
protected:
	RegisterStatus Register_rax           = RegisterStatus::invalid;
    RegisterStatus Register_rcx           = RegisterStatus::invalid;
    RegisterStatus Register_rdx           = RegisterStatus::invalid;
    RegisterStatus Register_rbx           = RegisterStatus::invalid;
    RegisterStatus Register_rsp           = RegisterStatus::invalid;
    RegisterStatus Register_rbp           = RegisterStatus::invalid;
    RegisterStatus Register_rsi           = RegisterStatus::invalid;
    RegisterStatus Register_rdi           = RegisterStatus::invalid;
    RegisterStatus Register_ah            = RegisterStatus::invalid;
    RegisterStatus Register_ch            = RegisterStatus::invalid;
    RegisterStatus Register_dh            = RegisterStatus::invalid;
    RegisterStatus Register_bh            = RegisterStatus::invalid;
    RegisterStatus Register_r8            = RegisterStatus::invalid;
    RegisterStatus Register_r9            = RegisterStatus::invalid;
    RegisterStatus Register_r10           = RegisterStatus::invalid;
    RegisterStatus Register_r11           = RegisterStatus::invalid;
    RegisterStatus Register_r12           = RegisterStatus::invalid;
    RegisterStatus Register_r13           = RegisterStatus::invalid;
    RegisterStatus Register_r14           = RegisterStatus::invalid;
    RegisterStatus Register_r15           = RegisterStatus::invalid;
    RegisterStatus Register_mmx0          = RegisterStatus::invalid;
    RegisterStatus Register_mmx1          = RegisterStatus::invalid;
    RegisterStatus Register_mmx2          = RegisterStatus::invalid;
    RegisterStatus Register_mmx3          = RegisterStatus::invalid;
    RegisterStatus Register_mmx4          = RegisterStatus::invalid;
    RegisterStatus Register_mmx5          = RegisterStatus::invalid;
    RegisterStatus Register_mmx6          = RegisterStatus::invalid;
    RegisterStatus Register_mmx7          = RegisterStatus::invalid;
    RegisterStatus Register_xmm0          = RegisterStatus::invalid;
    RegisterStatus Register_xmm1          = RegisterStatus::invalid;
    RegisterStatus Register_xmm2          = RegisterStatus::invalid;
    RegisterStatus Register_xmm3          = RegisterStatus::invalid;
    RegisterStatus Register_xmm4          = RegisterStatus::invalid;
    RegisterStatus Register_xmm5          = RegisterStatus::invalid;
    RegisterStatus Register_xmm6          = RegisterStatus::invalid;
    RegisterStatus Register_xmm7          = RegisterStatus::invalid;
    RegisterStatus Register_ymm0          = RegisterStatus::invalid;
    RegisterStatus Register_ymm1          = RegisterStatus::invalid;
    RegisterStatus Register_ymm2          = RegisterStatus::invalid;
    RegisterStatus Register_ymm3          = RegisterStatus::invalid;
    RegisterStatus Register_ymm4          = RegisterStatus::invalid;
    RegisterStatus Register_ymm5          = RegisterStatus::invalid;
    RegisterStatus Register_ymm6          = RegisterStatus::invalid;
    RegisterStatus Register_ymm7          = RegisterStatus::invalid;
    RegisterStatus Register_efer          = RegisterStatus::invalid;
    RegisterStatus Register_star          = RegisterStatus::invalid;
    RegisterStatus Register_lstar         = RegisterStatus::invalid;
    RegisterStatus Register_cstar         = RegisterStatus::invalid;
    RegisterStatus Register_sfmask        = RegisterStatus::invalid;
    RegisterStatus Register_fs_base       = RegisterStatus::invalid;
    RegisterStatus Register_gs_base       = RegisterStatus::invalid;
    RegisterStatus Register_kernel_gs_base= RegisterStatus::invalid;
    RegisterStatus Register_tsc_aux       = RegisterStatus::invalid;
    RegisterStatus Register_syscfg        = RegisterStatus::invalid;
    RegisterStatus Register_iorr_base0    = RegisterStatus::invalid;
    RegisterStatus Register_iorrmask0     = RegisterStatus::invalid;
    RegisterStatus Register_iorr_base1    = RegisterStatus::invalid;
    RegisterStatus Register_iorrmask1     = RegisterStatus::invalid;
    RegisterStatus Register_tom2          = RegisterStatus::invalid;
    RegisterStatus Register_ls_cfg        = RegisterStatus::invalid;
    RegisterStatus Register_ic_cfg        = RegisterStatus::invalid;
    RegisterStatus Register_dc_cfg        = RegisterStatus::invalid;
    RegisterStatus Register_bu_cfg        = RegisterStatus::invalid;
    RegisterStatus Register_mc0_ctl       = RegisterStatus::invalid;
    RegisterStatus Register_mc0_status    = RegisterStatus::invalid;
    RegisterStatus Register_mc0_addr      = RegisterStatus::invalid;
    RegisterStatus Register_mc0_misc      = RegisterStatus::invalid;
    RegisterStatus Register_perf_ctl0     = RegisterStatus::invalid;
    RegisterStatus Register_perf_ctr0     = RegisterStatus::invalid;
    RegisterStatus Register_perf_ctl1     = RegisterStatus::invalid;
    RegisterStatus Register_perf_ctr1     = RegisterStatus::invalid;
    RegisterStatus Register_top_mem       = RegisterStatus::invalid;
    RegisterStatus Register_top_mem2      = RegisterStatus::invalid;
    RegisterStatus Register_vm_cr         = RegisterStatus::invalid;
    RegisterStatus Register_vm_hsave_pa   = RegisterStatus::invalid;
public:
	uint64_t cpl = 3;
	void cpustate_amd64::registerStatus(amd64::Register reg, RegisterStatus status);
	RegisterStatus cpustate_amd64::registerStatus(amd64::Register reg) const;
};

class cpustate
{
public:
	cpustate_amd64 amd64;
};