//+ intellisense shenanigans
#ifndef cpustate_main
#include <cpustate.hxx>
#endif

template<typename T>
void registerValue(amd64::Register reg, T value)
{
	switch (reg) {
		case amd64::Register::rax:            	Register_rax.value = value;Register_ah.value = value; break;
		case amd64::Register::rcx:            	Register_rcx.value = value;Register_ch.value = value; break;
		case amd64::Register::rdx:            	Register_rdx.value = value;Register_dh.value = value; break;
		case amd64::Register::rbx:            	Register_rbx.value = value;Register_bh.value = value; break;
		case amd64::Register::rsp:            	Register_rsp.value = value; break;
		case amd64::Register::rbp:            	Register_rbp.value = value; break;
		case amd64::Register::rsi:            	Register_rsi.value = value; break;
		case amd64::Register::rdi:            	Register_rdi.value = value; break;
		case amd64::Register::ah:             	Register_ah.value = value;Register_rax.value = value; break;
		case amd64::Register::ch:             	Register_ch.value = value;Register_rcx.value = value; break;
		case amd64::Register::dh:             	Register_dh.value = value;Register_rdx.value = value; break;
		case amd64::Register::bh:             	Register_bh.value = value;Register_rbx.value = value; break;
		case amd64::Register::r8:             	Register_r8.value = value; break;
		case amd64::Register::r9:             	Register_r9.value = value; break;
		case amd64::Register::r10:            	Register_r10.value = value; break;
		case amd64::Register::r11:            	Register_r11.value = value; break;
		case amd64::Register::r12:            	Register_r12.value = value; break;
		case amd64::Register::r13:            	Register_r13.value = value; break;
		case amd64::Register::r14:            	Register_r14.value = value; break;
		case amd64::Register::r15:            	Register_r15.value = value; break;
		case amd64::Register::mmx0:           	Register_mmx0.value = value;Register_xmm0.value = value;Register_ymm0.value = value; break;
		case amd64::Register::mmx1:           	Register_mmx1.value = value;Register_xmm1.value = value;Register_ymm1.value = value; break;
		case amd64::Register::mmx2:           	Register_mmx2.value = value;Register_xmm2.value = value;Register_ymm2.value = value; break;
		case amd64::Register::mmx3:           	Register_mmx3.value = value;Register_xmm3.value = value;Register_ymm3.value = value; break;
		case amd64::Register::mmx4:           	Register_mmx4.value = value;Register_xmm4.value = value;Register_ymm4.value = value; break;
		case amd64::Register::mmx5:           	Register_mmx5.value = value;Register_xmm5.value = value;Register_ymm5.value = value; break;
		case amd64::Register::mmx6:           	Register_mmx6.value = value;Register_xmm6.value = value;Register_ymm6.value = value; break;
		case amd64::Register::mmx7:           	Register_mmx7.value = value;Register_xmm7.value = value;Register_ymm7.value = value; break;
		case amd64::Register::xmm0:           	Register_mmx0.value = value;Register_xmm0.value = value;Register_ymm0.value = value; break;
		case amd64::Register::xmm1:           	Register_mmx1.value = value;Register_xmm1.value = value;Register_ymm1.value = value; break;
		case amd64::Register::xmm2:           	Register_mmx2.value = value;Register_xmm2.value = value;Register_ymm2.value = value; break;
		case amd64::Register::xmm3:           	Register_mmx3.value = value;Register_xmm3.value = value;Register_ymm3.value = value; break;
		case amd64::Register::xmm4:           	Register_mmx4.value = value;Register_xmm4.value = value;Register_ymm4.value = value; break;
		case amd64::Register::xmm5:           	Register_mmx5.value = value;Register_xmm5.value = value;Register_ymm5.value = value; break;
		case amd64::Register::xmm6:           	Register_mmx6.value = value;Register_xmm6.value = value;Register_ymm6.value = value; break;
		case amd64::Register::xmm7:           	Register_mmx7.value = value;Register_xmm7.value = value;Register_ymm7.value = value; break;
		case amd64::Register::ymm0:           	Register_mmx0.value = value;Register_xmm0.value = value;Register_ymm0.value = value; break;
		case amd64::Register::ymm1:           	Register_mmx1.value = value;Register_xmm1.value = value;Register_ymm1.value = value; break;
		case amd64::Register::ymm2:           	Register_mmx2.value = value;Register_xmm2.value = value;Register_ymm2.value = value; break;
		case amd64::Register::ymm3:           	Register_mmx3.value = value;Register_xmm3.value = value;Register_ymm3.value = value; break;
		case amd64::Register::ymm4:           	Register_mmx4.value = value;Register_xmm4.value = value;Register_ymm4.value = value; break;
		case amd64::Register::ymm5:           	Register_mmx5.value = value;Register_xmm5.value = value;Register_ymm5.value = value; break;
		case amd64::Register::ymm6:           	Register_mmx6.value = value;Register_xmm6.value = value;Register_ymm6.value = value; break;
		case amd64::Register::ymm7:           	Register_mmx7.value = value;Register_xmm7.value = value;Register_ymm7.value = value; break;
		case amd64::Register::efer:           	Register_efer.value = value; break;
		case amd64::Register::star:           	Register_star.value = value; break;
		case amd64::Register::lstar:          	Register_lstar.value = value; break;
		case amd64::Register::cstar:          	Register_cstar.value = value; break;
		case amd64::Register::sfmask:         	Register_sfmask.value = value; break;
		case amd64::Register::fs_base:        	Register_fs_base.value = value; break;
		case amd64::Register::gs_base:        	Register_gs_base.value = value; break;
		case amd64::Register::kernel_gs_base: 	Register_kernel_gs_base.value = value; break;
		case amd64::Register::tsc_aux:        	Register_tsc_aux.value = value; break;
		case amd64::Register::syscfg:         	Register_syscfg.value = value; break;
		case amd64::Register::iorr_base0:     	Register_iorr_base0.value = value; break;
		case amd64::Register::iorrmask0:      	Register_iorrmask0.value = value; break;
		case amd64::Register::iorr_base1:     	Register_iorr_base1.value = value; break;
		case amd64::Register::iorrmask1:      	Register_iorrmask1.value = value; break;
		case amd64::Register::ls_cfg:         	Register_ls_cfg.value = value; break;
		case amd64::Register::ic_cfg:         	Register_ic_cfg.value = value; break;
		case amd64::Register::dc_cfg:         	Register_dc_cfg.value = value; break;
		case amd64::Register::bu_cfg:         	Register_bu_cfg.value = value; break;
		case amd64::Register::mc0_ctl:        	Register_mc0_ctl.value = value; break;
		case amd64::Register::mc0_status:     	Register_mc0_status.value = value; break;
		case amd64::Register::mc0_addr:       	Register_mc0_addr.value = value; break;
		case amd64::Register::mc0_misc:       	Register_mc0_misc.value = value; break;
		case amd64::Register::perf_ctl0:      	Register_perf_ctl0.value = value; break;
		case amd64::Register::perf_ctr0:      	Register_perf_ctr0.value = value; break;
		case amd64::Register::perf_ctl1:      	Register_perf_ctl1.value = value; break;
		case amd64::Register::perf_ctr1:      	Register_perf_ctr1.value = value; break;
		case amd64::Register::top_mem:        	Register_top_mem.value = value; break;
		case amd64::Register::top_mem2:       	Register_top_mem2.value = value; break;
		case amd64::Register::vm_cr:          	Register_vm_cr.value = value; break;
		case amd64::Register::vm_hsave_pa:    	Register_vm_hsave_pa.value = value; break;
		case amd64::Register::cr0:				Register_cr0.value = value; break;
		case amd64::Register::cr2:				Register_cr2.value = value; break;
		case amd64::Register::cr3:				Register_cr3.value = value; break;
		case amd64::Register::cr4:				Register_cr4.value = value; break;
		case amd64::Register::cr8:				Register_cr8.value = value; break;
		case amd64::Register::gdtr:				Register_gdtr.value = value; break;
		case amd64::Register::idtr:				Register_idtr.value = value; break;
		case amd64::Register::ldtr:				Register_ldtr.value = value; break;
		default:
			issues::compilerBug("invalid amd64 register");
			break; // Should never be reached
	}
}
#define INVALID_REGISTER_VALUE_FETCH_MSG "invalid register value fetch."
template<typename T>
T registerValue(amd64::Register reg) {
	switch (reg) {
		case amd64::Register::rax:            	if(sizeof( uint64_t) != sizeof(T))issues::compilerBug(INVALID_REGISTER_VALUE_FETCH_MSG);return extintCast((T*)0,Register_rax.value);Register_rax.valueKnown = true;
		case amd64::Register::rcx:            	if(sizeof( uint64_t) != sizeof(T))issues::compilerBug(INVALID_REGISTER_VALUE_FETCH_MSG);return extintCast((T*)0,Register_rcx.value);Register_rcx.valueKnown = true;
		case amd64::Register::rdx:            	if(sizeof( uint64_t) != sizeof(T))issues::compilerBug(INVALID_REGISTER_VALUE_FETCH_MSG);return extintCast((T*)0,Register_rdx.value);Register_rdx.valueKnown = true;
		case amd64::Register::rbx:            	if(sizeof( uint64_t) != sizeof(T))issues::compilerBug(INVALID_REGISTER_VALUE_FETCH_MSG);return extintCast((T*)0,Register_rbx.value);Register_rbx.valueKnown = true;
		case amd64::Register::rsp:            	if(sizeof( uint64_t) != sizeof(T))issues::compilerBug(INVALID_REGISTER_VALUE_FETCH_MSG);return extintCast((T*)0,Register_rsp.value);Register_rsp.valueKnown = true;
		case amd64::Register::rbp:            	if(sizeof( uint64_t) != sizeof(T))issues::compilerBug(INVALID_REGISTER_VALUE_FETCH_MSG);return extintCast((T*)0,Register_rbp.value);Register_rbp.valueKnown = true;
		case amd64::Register::rsi:            	if(sizeof( uint64_t) != sizeof(T))issues::compilerBug(INVALID_REGISTER_VALUE_FETCH_MSG);return extintCast((T*)0,Register_rsi.value);Register_rsi.valueKnown = true;
		case amd64::Register::rdi:            	if(sizeof( uint64_t) != sizeof(T))issues::compilerBug(INVALID_REGISTER_VALUE_FETCH_MSG);return extintCast((T*)0,Register_rdi.value);Register_rdi.valueKnown = true;
		case amd64::Register::ah:             	if(sizeof(  uint8_t) != sizeof(T))issues::compilerBug(INVALID_REGISTER_VALUE_FETCH_MSG);return extintCast((T*)0,Register_ah.value);Register_ah.valueKnown = true;
		case amd64::Register::ch:             	if(sizeof(  uint8_t) != sizeof(T))issues::compilerBug(INVALID_REGISTER_VALUE_FETCH_MSG);return extintCast((T*)0,Register_ch.value);Register_ch.valueKnown = true;
		case amd64::Register::dh:             	if(sizeof(  uint8_t) != sizeof(T))issues::compilerBug(INVALID_REGISTER_VALUE_FETCH_MSG);return extintCast((T*)0,Register_dh.value);Register_dh.valueKnown = true;
		case amd64::Register::bh:             	if(sizeof(  uint8_t) != sizeof(T))issues::compilerBug(INVALID_REGISTER_VALUE_FETCH_MSG);return extintCast((T*)0,Register_bh.value);Register_bh.valueKnown = true;
		case amd64::Register::r8:             	if(sizeof( uint64_t) != sizeof(T))issues::compilerBug(INVALID_REGISTER_VALUE_FETCH_MSG);return extintCast((T*)0,Register_r8.value);Register_r8.valueKnown = true;
		case amd64::Register::r9:             	if(sizeof( uint64_t) != sizeof(T))issues::compilerBug(INVALID_REGISTER_VALUE_FETCH_MSG);return extintCast((T*)0,Register_r9.value);Register_r9.valueKnown = true;
		case amd64::Register::r10:            	if(sizeof( uint64_t) != sizeof(T))issues::compilerBug(INVALID_REGISTER_VALUE_FETCH_MSG);return extintCast((T*)0,Register_r10.value);Register_r10.valueKnown = true;
		case amd64::Register::r11:            	if(sizeof( uint64_t) != sizeof(T))issues::compilerBug(INVALID_REGISTER_VALUE_FETCH_MSG);return extintCast((T*)0,Register_r11.value);Register_r11.valueKnown = true;
		case amd64::Register::r12:            	if(sizeof( uint64_t) != sizeof(T))issues::compilerBug(INVALID_REGISTER_VALUE_FETCH_MSG);return extintCast((T*)0,Register_r12.value);Register_r12.valueKnown = true;
		case amd64::Register::r13:            	if(sizeof( uint64_t) != sizeof(T))issues::compilerBug(INVALID_REGISTER_VALUE_FETCH_MSG);return extintCast((T*)0,Register_r13.value);Register_r13.valueKnown = true;
		case amd64::Register::r14:            	if(sizeof( uint64_t) != sizeof(T))issues::compilerBug(INVALID_REGISTER_VALUE_FETCH_MSG);return extintCast((T*)0,Register_r14.value);Register_r14.valueKnown = true;
		case amd64::Register::r15:            	if(sizeof( uint64_t) != sizeof(T))issues::compilerBug(INVALID_REGISTER_VALUE_FETCH_MSG);return extintCast((T*)0,Register_r15.value);Register_r15.valueKnown = true;
		case amd64::Register::mmx0:           	if(sizeof( uint64_t) != sizeof(T))issues::compilerBug(INVALID_REGISTER_VALUE_FETCH_MSG);return extintCast((T*)0,Register_mmx0.value);Register_mmx0.valueKnown = true;
		case amd64::Register::mmx1:           	if(sizeof( uint64_t) != sizeof(T))issues::compilerBug(INVALID_REGISTER_VALUE_FETCH_MSG);return extintCast((T*)0,Register_mmx1.value);Register_mmx1.valueKnown = true;
		case amd64::Register::mmx2:           	if(sizeof( uint64_t) != sizeof(T))issues::compilerBug(INVALID_REGISTER_VALUE_FETCH_MSG);return extintCast((T*)0,Register_mmx2.value);Register_mmx2.valueKnown = true;
		case amd64::Register::mmx3:           	if(sizeof( uint64_t) != sizeof(T))issues::compilerBug(INVALID_REGISTER_VALUE_FETCH_MSG);return extintCast((T*)0,Register_mmx3.value);Register_mmx3.valueKnown = true;
		case amd64::Register::mmx4:           	if(sizeof( uint64_t) != sizeof(T))issues::compilerBug(INVALID_REGISTER_VALUE_FETCH_MSG);return extintCast((T*)0,Register_mmx4.value);Register_mmx4.valueKnown = true;
		case amd64::Register::mmx5:           	if(sizeof( uint64_t) != sizeof(T))issues::compilerBug(INVALID_REGISTER_VALUE_FETCH_MSG);return extintCast((T*)0,Register_mmx5.value);Register_mmx5.valueKnown = true;
		case amd64::Register::mmx6:           	if(sizeof( uint64_t) != sizeof(T))issues::compilerBug(INVALID_REGISTER_VALUE_FETCH_MSG);return extintCast((T*)0,Register_mmx6.value);Register_mmx6.valueKnown = true;
		case amd64::Register::mmx7:           	if(sizeof( uint64_t) != sizeof(T))issues::compilerBug(INVALID_REGISTER_VALUE_FETCH_MSG);return extintCast((T*)0,Register_mmx7.value);Register_mmx7.valueKnown = true;
		case amd64::Register::xmm0:           	if(sizeof(uint128_t) != sizeof(T))issues::compilerBug(INVALID_REGISTER_VALUE_FETCH_MSG);return extintCast((T*)0,Register_xmm0.value);Register_xmm0.valueKnown = true;
		case amd64::Register::xmm1:           	if(sizeof(uint128_t) != sizeof(T))issues::compilerBug(INVALID_REGISTER_VALUE_FETCH_MSG);return extintCast((T*)0,Register_xmm1.value);Register_xmm1.valueKnown = true;
		case amd64::Register::xmm2:           	if(sizeof(uint128_t) != sizeof(T))issues::compilerBug(INVALID_REGISTER_VALUE_FETCH_MSG);return extintCast((T*)0,Register_xmm2.value);Register_xmm2.valueKnown = true;
		case amd64::Register::xmm3:           	if(sizeof(uint128_t) != sizeof(T))issues::compilerBug(INVALID_REGISTER_VALUE_FETCH_MSG);return extintCast((T*)0,Register_xmm3.value);Register_xmm3.valueKnown = true;
		case amd64::Register::xmm4:           	if(sizeof(uint128_t) != sizeof(T))issues::compilerBug(INVALID_REGISTER_VALUE_FETCH_MSG);return extintCast((T*)0,Register_xmm4.value);Register_xmm4.valueKnown = true;
		case amd64::Register::xmm5:           	if(sizeof(uint128_t) != sizeof(T))issues::compilerBug(INVALID_REGISTER_VALUE_FETCH_MSG);return extintCast((T*)0,Register_xmm5.value);Register_xmm5.valueKnown = true;
		case amd64::Register::xmm6:           	if(sizeof(uint128_t) != sizeof(T))issues::compilerBug(INVALID_REGISTER_VALUE_FETCH_MSG);return extintCast((T*)0,Register_xmm6.value);Register_xmm6.valueKnown = true;
		case amd64::Register::xmm7:           	if(sizeof(uint128_t) != sizeof(T))issues::compilerBug(INVALID_REGISTER_VALUE_FETCH_MSG);return extintCast((T*)0,Register_xmm7.value);Register_xmm7.valueKnown = true;
		case amd64::Register::ymm0:           	if(sizeof(uint256_t) != sizeof(T))issues::compilerBug(INVALID_REGISTER_VALUE_FETCH_MSG);return extintCast((T*)0,Register_ymm0.value);Register_ymm0.valueKnown = true;
		case amd64::Register::ymm1:           	if(sizeof(uint256_t) != sizeof(T))issues::compilerBug(INVALID_REGISTER_VALUE_FETCH_MSG);return extintCast((T*)0,Register_ymm1.value);Register_ymm1.valueKnown = true;
		case amd64::Register::ymm2:           	if(sizeof(uint256_t) != sizeof(T))issues::compilerBug(INVALID_REGISTER_VALUE_FETCH_MSG);return extintCast((T*)0,Register_ymm2.value);Register_ymm2.valueKnown = true;
		case amd64::Register::ymm3:           	if(sizeof(uint256_t) != sizeof(T))issues::compilerBug(INVALID_REGISTER_VALUE_FETCH_MSG);return extintCast((T*)0,Register_ymm3.value);Register_ymm3.valueKnown = true;
		case amd64::Register::ymm4:           	if(sizeof(uint256_t) != sizeof(T))issues::compilerBug(INVALID_REGISTER_VALUE_FETCH_MSG);return extintCast((T*)0,Register_ymm4.value);Register_ymm4.valueKnown = true;
		case amd64::Register::ymm5:           	if(sizeof(uint256_t) != sizeof(T))issues::compilerBug(INVALID_REGISTER_VALUE_FETCH_MSG);return extintCast((T*)0,Register_ymm5.value);Register_ymm5.valueKnown = true;
		case amd64::Register::ymm6:           	if(sizeof(uint256_t) != sizeof(T))issues::compilerBug(INVALID_REGISTER_VALUE_FETCH_MSG);return extintCast((T*)0,Register_ymm6.value);Register_ymm6.valueKnown = true;
		case amd64::Register::ymm7:           	if(sizeof(uint256_t) != sizeof(T))issues::compilerBug(INVALID_REGISTER_VALUE_FETCH_MSG);return extintCast((T*)0,Register_ymm7.value);Register_ymm7.valueKnown = true;
		case amd64::Register::efer:           	if(sizeof( uint64_t) != sizeof(T))issues::compilerBug(INVALID_REGISTER_VALUE_FETCH_MSG);return extintCast((T*)0,Register_efer.value);Register_efer.valueKnown = true;
		case amd64::Register::star:           	if(sizeof( uint64_t) != sizeof(T))issues::compilerBug(INVALID_REGISTER_VALUE_FETCH_MSG);return extintCast((T*)0,Register_star.value);Register_star.valueKnown = true;
		case amd64::Register::lstar:          	if(sizeof( uint64_t) != sizeof(T))issues::compilerBug(INVALID_REGISTER_VALUE_FETCH_MSG);return extintCast((T*)0,Register_lstar.value);Register_lstar.valueKnown = true;
		case amd64::Register::cstar:          	if(sizeof( uint64_t) != sizeof(T))issues::compilerBug(INVALID_REGISTER_VALUE_FETCH_MSG);return extintCast((T*)0,Register_cstar.value);Register_cstar.valueKnown = true;
		case amd64::Register::sfmask:         	if(sizeof( uint64_t) != sizeof(T))issues::compilerBug(INVALID_REGISTER_VALUE_FETCH_MSG);return extintCast((T*)0,Register_sfmask.value);Register_sfmask.valueKnown = true;
		case amd64::Register::fs_base:        	if(sizeof( uint64_t) != sizeof(T))issues::compilerBug(INVALID_REGISTER_VALUE_FETCH_MSG);return extintCast((T*)0,Register_fs_base.value);Register_fs_base.valueKnown = true;
		case amd64::Register::gs_base:        	if(sizeof( uint64_t) != sizeof(T))issues::compilerBug(INVALID_REGISTER_VALUE_FETCH_MSG);return extintCast((T*)0,Register_gs_base.value);Register_gs_base.valueKnown = true;
		case amd64::Register::kernel_gs_base: 	if(sizeof( uint64_t) != sizeof(T))issues::compilerBug(INVALID_REGISTER_VALUE_FETCH_MSG);return extintCast((T*)0,Register_kernel_gs_base.value);Register_kernel_gs_base.valueKnown = true;
		case amd64::Register::tsc_aux:        	if(sizeof( uint64_t) != sizeof(T))issues::compilerBug(INVALID_REGISTER_VALUE_FETCH_MSG);return extintCast((T*)0,Register_tsc_aux.value);Register_tsc_aux.valueKnown = true;
		case amd64::Register::syscfg:         	if(sizeof( uint64_t) != sizeof(T))issues::compilerBug(INVALID_REGISTER_VALUE_FETCH_MSG);return extintCast((T*)0,Register_syscfg.value);Register_syscfg.valueKnown = true;
		case amd64::Register::iorr_base0:     	if(sizeof( uint64_t) != sizeof(T))issues::compilerBug(INVALID_REGISTER_VALUE_FETCH_MSG);return extintCast((T*)0,Register_iorr_base0.value);Register_iorr_base0.valueKnown = true;
		case amd64::Register::iorrmask0:      	if(sizeof( uint64_t) != sizeof(T))issues::compilerBug(INVALID_REGISTER_VALUE_FETCH_MSG);return extintCast((T*)0,Register_iorrmask0.value);Register_iorrmask0.valueKnown = true;
		case amd64::Register::iorr_base1:     	if(sizeof( uint64_t) != sizeof(T))issues::compilerBug(INVALID_REGISTER_VALUE_FETCH_MSG);return extintCast((T*)0,Register_iorr_base1.value);Register_iorr_base1.valueKnown = true;
		case amd64::Register::iorrmask1:      	if(sizeof( uint64_t) != sizeof(T))issues::compilerBug(INVALID_REGISTER_VALUE_FETCH_MSG);return extintCast((T*)0,Register_iorrmask1.value);Register_iorrmask1.valueKnown = true;
		case amd64::Register::ls_cfg:         	if(sizeof( uint64_t) != sizeof(T))issues::compilerBug(INVALID_REGISTER_VALUE_FETCH_MSG);return extintCast((T*)0,Register_ls_cfg.value);Register_ls_cfg.valueKnown = true;
		case amd64::Register::ic_cfg:         	if(sizeof( uint64_t) != sizeof(T))issues::compilerBug(INVALID_REGISTER_VALUE_FETCH_MSG);return extintCast((T*)0,Register_ic_cfg.value);Register_ic_cfg.valueKnown = true;
		case amd64::Register::dc_cfg:         	if(sizeof( uint64_t) != sizeof(T))issues::compilerBug(INVALID_REGISTER_VALUE_FETCH_MSG);return extintCast((T*)0,Register_dc_cfg.value);Register_dc_cfg.valueKnown = true;
		case amd64::Register::bu_cfg:         	if(sizeof( uint64_t) != sizeof(T))issues::compilerBug(INVALID_REGISTER_VALUE_FETCH_MSG);return extintCast((T*)0,Register_bu_cfg.value);Register_bu_cfg.valueKnown = true;
		case amd64::Register::mc0_ctl:        	if(sizeof( uint64_t) != sizeof(T))issues::compilerBug(INVALID_REGISTER_VALUE_FETCH_MSG);return extintCast((T*)0,Register_mc0_ctl.value);Register_mc0_ctl.valueKnown = true;
		case amd64::Register::mc0_status:     	if(sizeof( uint64_t) != sizeof(T))issues::compilerBug(INVALID_REGISTER_VALUE_FETCH_MSG);return extintCast((T*)0,Register_mc0_status.value);Register_mc0_status.valueKnown = true;
		case amd64::Register::mc0_addr:       	if(sizeof( uint64_t) != sizeof(T))issues::compilerBug(INVALID_REGISTER_VALUE_FETCH_MSG);return extintCast((T*)0,Register_mc0_addr.value);Register_mc0_addr.valueKnown = true;
		case amd64::Register::mc0_misc:       	if(sizeof( uint64_t) != sizeof(T))issues::compilerBug(INVALID_REGISTER_VALUE_FETCH_MSG);return extintCast((T*)0,Register_mc0_misc.value);Register_mc0_misc.valueKnown = true;
		case amd64::Register::perf_ctl0:      	if(sizeof( uint64_t) != sizeof(T))issues::compilerBug(INVALID_REGISTER_VALUE_FETCH_MSG);return extintCast((T*)0,Register_perf_ctl0.value);Register_perf_ctl0.valueKnown = true;
		case amd64::Register::perf_ctr0:      	if(sizeof( uint64_t) != sizeof(T))issues::compilerBug(INVALID_REGISTER_VALUE_FETCH_MSG);return extintCast((T*)0,Register_perf_ctr0.value);Register_perf_ctr0.valueKnown = true;
		case amd64::Register::perf_ctl1:      	if(sizeof( uint64_t) != sizeof(T))issues::compilerBug(INVALID_REGISTER_VALUE_FETCH_MSG);return extintCast((T*)0,Register_perf_ctl1.value);Register_perf_ctl1.valueKnown = true;
		case amd64::Register::perf_ctr1:      	if(sizeof( uint64_t) != sizeof(T))issues::compilerBug(INVALID_REGISTER_VALUE_FETCH_MSG);return extintCast((T*)0,Register_perf_ctr1.value);Register_perf_ctr1.valueKnown = true;
		case amd64::Register::top_mem:        	if(sizeof( uint64_t) != sizeof(T))issues::compilerBug(INVALID_REGISTER_VALUE_FETCH_MSG);return extintCast((T*)0,Register_top_mem.value);Register_top_mem.valueKnown = true;
		case amd64::Register::top_mem2:       	if(sizeof( uint64_t) != sizeof(T))issues::compilerBug(INVALID_REGISTER_VALUE_FETCH_MSG);return extintCast((T*)0,Register_top_mem2.value);Register_top_mem2.valueKnown = true;
		case amd64::Register::vm_cr:          	if(sizeof( uint64_t) != sizeof(T))issues::compilerBug(INVALID_REGISTER_VALUE_FETCH_MSG);return extintCast((T*)0,Register_vm_cr.value);Register_vm_cr.valueKnown = true;
		case amd64::Register::vm_hsave_pa:    	if(sizeof( uint64_t) != sizeof(T))issues::compilerBug(INVALID_REGISTER_VALUE_FETCH_MSG);return extintCast((T*)0,Register_vm_hsave_pa.value);Register_vm_hsave_pa.valueKnown = true;
		case amd64::Register::cr0:				if(sizeof( uint64_t) != sizeof(T))issues::compilerBug(INVALID_REGISTER_VALUE_FETCH_MSG);return extintCast((T*)0,Register_cr0.value);Register_cr0.valueKnown = true;
		case amd64::Register::cr2:				if(sizeof( uint64_t) != sizeof(T))issues::compilerBug(INVALID_REGISTER_VALUE_FETCH_MSG);return extintCast((T*)0,Register_cr2.value);Register_cr2.valueKnown = true;
		case amd64::Register::cr3:				if(sizeof( uint64_t) != sizeof(T))issues::compilerBug(INVALID_REGISTER_VALUE_FETCH_MSG);return extintCast((T*)0,Register_cr3.value);Register_cr3.valueKnown = true;
		case amd64::Register::cr4:				if(sizeof( uint64_t) != sizeof(T))issues::compilerBug(INVALID_REGISTER_VALUE_FETCH_MSG);return extintCast((T*)0,Register_cr4.value);Register_cr4.valueKnown = true;
		case amd64::Register::cr8:				if(sizeof( uint64_t) != sizeof(T))issues::compilerBug(INVALID_REGISTER_VALUE_FETCH_MSG);return extintCast((T*)0,Register_cr8.value);Register_cr8.valueKnown = true;
		case amd64::Register::gdtr:				if(sizeof( uint80_t) != sizeof(T))issues::compilerBug(INVALID_REGISTER_VALUE_FETCH_MSG);return extintCast((T*)0,Register_gdtr.value);Register_gdtr.valueKnown = true;
		case amd64::Register::idtr:				if(sizeof( uint80_t) != sizeof(T))issues::compilerBug(INVALID_REGISTER_VALUE_FETCH_MSG);return extintCast((T*)0,Register_idtr.value);Register_idtr.valueKnown = true;
		case amd64::Register::ldtr:				if(sizeof( uint80_t) != sizeof(T))issues::compilerBug(INVALID_REGISTER_VALUE_FETCH_MSG);return extintCast((T*)0,Register_ldtr.value);Register_ldtr.valueKnown = true;
		default:
			issues::compilerBug("invalid amd64 register");
			return RegisterStatus::invalid; // Should never be reached
	}
}
#undef INVALID_REGISTER_VALUE_FETCH_MSG
