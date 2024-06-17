#include <cpustate.hxx>

// Function to get the register status
RegisterStatus cpustate_amd64::registerStatus(amd64::Register reg) const {
	switch (reg) {
		case amd64::Register::rax:            return Register_rax;
		case amd64::Register::rcx:            return Register_rcx;
		case amd64::Register::rdx:            return Register_rdx;
		case amd64::Register::rbx:            return Register_rbx;
		case amd64::Register::rsp:            return Register_rsp;
		case amd64::Register::rbp:            return Register_rbp;
		case amd64::Register::rsi:            return Register_rsi;
		case amd64::Register::rdi:            return Register_rdi;
		case amd64::Register::ah:             return Register_ah;
		case amd64::Register::ch:             return Register_ch;
		case amd64::Register::dh:             return Register_dh;
		case amd64::Register::bh:             return Register_bh;
		case amd64::Register::r8:             return Register_r8;
		case amd64::Register::r9:             return Register_r9;
		case amd64::Register::r10:            return Register_r10;
		case amd64::Register::r11:            return Register_r11;
		case amd64::Register::r12:            return Register_r12;
		case amd64::Register::r13:            return Register_r13;
		case amd64::Register::r14:            return Register_r14;
		case amd64::Register::r15:            return Register_r15;
		case amd64::Register::mmx0:           return Register_mmx0;
		case amd64::Register::mmx1:           return Register_mmx1;
		case amd64::Register::mmx2:           return Register_mmx2;
		case amd64::Register::mmx3:           return Register_mmx3;
		case amd64::Register::mmx4:           return Register_mmx4;
		case amd64::Register::mmx5:           return Register_mmx5;
		case amd64::Register::mmx6:           return Register_mmx6;
		case amd64::Register::mmx7:           return Register_mmx7;
		case amd64::Register::xmm0:           return Register_xmm0;
		case amd64::Register::xmm1:           return Register_xmm1;
		case amd64::Register::xmm2:           return Register_xmm2;
		case amd64::Register::xmm3:           return Register_xmm3;
		case amd64::Register::xmm4:           return Register_xmm4;
		case amd64::Register::xmm5:           return Register_xmm5;
		case amd64::Register::xmm6:           return Register_xmm6;
		case amd64::Register::xmm7:           return Register_xmm7;
		case amd64::Register::ymm0:           return Register_ymm0;
		case amd64::Register::ymm1:           return Register_ymm1;
		case amd64::Register::ymm2:           return Register_ymm2;
		case amd64::Register::ymm3:           return Register_ymm3;
		case amd64::Register::ymm4:           return Register_ymm4;
		case amd64::Register::ymm5:           return Register_ymm5;
		case amd64::Register::ymm6:           return Register_ymm6;
		case amd64::Register::ymm7:           return Register_ymm7;
		case amd64::Register::efer:           return Register_efer;
		case amd64::Register::star:           return Register_star;
		case amd64::Register::lstar:          return Register_lstar;
		case amd64::Register::cstar:          return Register_cstar;
		case amd64::Register::sfmask:         return Register_sfmask;
		case amd64::Register::fs_base:        return Register_fs_base;
		case amd64::Register::gs_base:        return Register_gs_base;
		case amd64::Register::kernel_gs_base: return Register_kernel_gs_base;
		case amd64::Register::tsc_aux:        return Register_tsc_aux;
		case amd64::Register::syscfg:         return Register_syscfg;
		case amd64::Register::iorr_base0:     return Register_iorr_base0;
		case amd64::Register::iorrmask0:      return Register_iorrmask0;
		case amd64::Register::iorr_base1:     return Register_iorr_base1;
		case amd64::Register::iorrmask1:      return Register_iorrmask1;
		case amd64::Register::tom2:           return Register_tom2;
		case amd64::Register::ls_cfg:         return Register_ls_cfg;
		case amd64::Register::ic_cfg:         return Register_ic_cfg;
		case amd64::Register::dc_cfg:         return Register_dc_cfg;
		case amd64::Register::bu_cfg:         return Register_bu_cfg;
		case amd64::Register::mc0_ctl:        return Register_mc0_ctl;
		case amd64::Register::mc0_status:     return Register_mc0_status;
		case amd64::Register::mc0_addr:       return Register_mc0_addr;
		case amd64::Register::mc0_misc:       return Register_mc0_misc;
		case amd64::Register::perf_ctl0:      return Register_perf_ctl0;
		case amd64::Register::perf_ctr0:      return Register_perf_ctr0;
		case amd64::Register::perf_ctl1:      return Register_perf_ctl1;
		case amd64::Register::perf_ctr1:      return Register_perf_ctr1;
		case amd64::Register::top_mem:        return Register_top_mem;
		case amd64::Register::top_mem2:       return Register_top_mem2;
		case amd64::Register::vm_cr:          return Register_vm_cr;
		case amd64::Register::vm_hsave_pa:    return Register_vm_hsave_pa;
		default:
			compilerBug("invalid amd64 register");
			return RegisterStatus::invalid; // Should never be reached
	}
}

// Function to set the register status
void cpustate_amd64::registerStatus(amd64::Register reg, RegisterStatus status) {
	switch (reg) {
		case amd64::Register::rax:            Register_rax = status;Register_ah = status; break;
		case amd64::Register::rcx:            Register_rcx = status;Register_ch = status; break;
		case amd64::Register::rdx:            Register_rdx = status;Register_dh = status; break;
		case amd64::Register::rbx:            Register_rbx = status;Register_bh = status; break;
		case amd64::Register::rsp:            Register_rsp = status; break;
		case amd64::Register::rbp:            Register_rbp = status; break;
		case amd64::Register::rsi:            Register_rsi = status; break;
		case amd64::Register::rdi:            Register_rdi = status; break;
		case amd64::Register::ah:             Register_ah = status;Register_rax = status; break;
		case amd64::Register::ch:             Register_ch = status;Register_rcx = status; break;
		case amd64::Register::dh:             Register_dh = status;Register_rdx = status; break;
		case amd64::Register::bh:             Register_bh = status;Register_rbx = status; break;
		case amd64::Register::r8:             Register_r8 = status; break;
		case amd64::Register::r9:             Register_r9 = status; break;
		case amd64::Register::r10:            Register_r10 = status; break;
		case amd64::Register::r11:            Register_r11 = status; break;
		case amd64::Register::r12:            Register_r12 = status; break;
		case amd64::Register::r13:            Register_r13 = status; break;
		case amd64::Register::r14:            Register_r14 = status; break;
		case amd64::Register::r15:            Register_r15 = status; break;
		case amd64::Register::mmx0:           Register_mmx0 = status; break;
		case amd64::Register::mmx1:           Register_mmx1 = status; break;
		case amd64::Register::mmx2:           Register_mmx2 = status; break;
		case amd64::Register::mmx3:           Register_mmx3 = status; break;
		case amd64::Register::mmx4:           Register_mmx4 = status; break;
		case amd64::Register::mmx5:           Register_mmx5 = status; break;
		case amd64::Register::mmx6:           Register_mmx6 = status; break;
		case amd64::Register::mmx7:           Register_mmx7 = status; break;
		case amd64::Register::xmm0:           Register_xmm0 = status; break;
		case amd64::Register::xmm1:           Register_xmm1 = status; break;
		case amd64::Register::xmm2:           Register_xmm2 = status; break;
		case amd64::Register::xmm3:           Register_xmm3 = status; break;
		case amd64::Register::xmm4:           Register_xmm4 = status; break;
		case amd64::Register::xmm5:           Register_xmm5 = status; break;
		case amd64::Register::xmm6:           Register_xmm6 = status; break;
		case amd64::Register::xmm7:           Register_xmm7 = status; break;
		case amd64::Register::ymm0:           Register_ymm0 = status; break;
		case amd64::Register::ymm1:           Register_ymm1 = status; break;
		case amd64::Register::ymm2:           Register_ymm2 = status; break;
		case amd64::Register::ymm3:           Register_ymm3 = status; break;
		case amd64::Register::ymm4:           Register_ymm4 = status; break;
		case amd64::Register::ymm5:           Register_ymm5 = status; break;
		case amd64::Register::ymm6:           Register_ymm6 = status; break;
		case amd64::Register::ymm7:           Register_ymm7 = status; break;
		case amd64::Register::efer:           Register_efer = status; break;
		case amd64::Register::star:           Register_star = status; break;
		case amd64::Register::lstar:          Register_lstar = status; break;
		case amd64::Register::cstar:          Register_cstar = status; break;
		case amd64::Register::sfmask:         Register_sfmask = status; break;
		case amd64::Register::fs_base:        Register_fs_base = status; break;
		case amd64::Register::gs_base:        Register_gs_base = status; break;
		case amd64::Register::kernel_gs_base: Register_kernel_gs_base = status; break;
		case amd64::Register::tsc_aux:        Register_tsc_aux = status; break;
		case amd64::Register::syscfg:         Register_syscfg = status; break;
		case amd64::Register::iorr_base0:     Register_iorr_base0 = status; break;
		case amd64::Register::iorrmask0:      Register_iorrmask0 = status; break;
		case amd64::Register::iorr_base1:     Register_iorr_base1 = status; break;
		case amd64::Register::iorrmask1:      Register_iorrmask1 = status; break;
		case amd64::Register::tom2:           Register_tom2 = status; break;
		case amd64::Register::ls_cfg:         Register_ls_cfg = status; break;
		case amd64::Register::ic_cfg:         Register_ic_cfg = status; break;
		case amd64::Register::dc_cfg:         Register_dc_cfg = status; break;
		case amd64::Register::bu_cfg:         Register_bu_cfg = status; break;
		case amd64::Register::mc0_ctl:        Register_mc0_ctl = status; break;
		case amd64::Register::mc0_status:     Register_mc0_status = status; break;
		case amd64::Register::mc0_addr:       Register_mc0_addr = status; break;
		case amd64::Register::mc0_misc:       Register_mc0_misc = status; break;
		case amd64::Register::perf_ctl0:      Register_perf_ctl0 = status; break;
		case amd64::Register::perf_ctr0:      Register_perf_ctr0 = status; break;
		case amd64::Register::perf_ctl1:      Register_perf_ctl1 = status; break;
		case amd64::Register::perf_ctr1:      Register_perf_ctr1 = status; break;
		case amd64::Register::top_mem:        Register_top_mem = status; break;
		case amd64::Register::top_mem2:       Register_top_mem2 = status; break;
		case amd64::Register::vm_cr:          Register_vm_cr = status; break;
		case amd64::Register::vm_hsave_pa:    Register_vm_hsave_pa = status; break;
		default:
			compilerBug("invalid amd64 register");
			break; // Should never be reached
	}
}