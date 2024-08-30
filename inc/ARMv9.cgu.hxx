#pragma once
#include "class_variable.h"
class function;
#include "extint.hxx"

namespace ARMv9 {
	inline constexpr uint64_t register_encode_index(uint8_t index){return index;}
	inline constexpr uint64_t register_encode_gp(){return (1<<5);}
	inline constexpr uint64_t register_encode_fp(){return (1<<6);}
	inline constexpr uint64_t register_encode_sve_scalable(){return (1<<7);}
	inline constexpr uint64_t register_encode_sve_predicate(){return (1<<8);}
	/**
	* @brief
	*.	bits 0-4: index
	*.	bit 5: GP
	*.	bit 6: FP
	*.	bit 7: SVE scalable
	*.	bit 8: SVE predicate
	*/
	enum class Register : uint64_t {
		/* integer */
		r0 = register_encode_index(0) 	| register_encode_gp(),
		r1 = register_encode_index(1) 	| register_encode_gp(),
		r2 = register_encode_index(2) 	| register_encode_gp(),
		r3 = register_encode_index(3) 	| register_encode_gp(),
		r4 = register_encode_index(4) 	| register_encode_gp(),
		r5 = register_encode_index(5) 	| register_encode_gp(),
		r6 = register_encode_index(6) 	| register_encode_gp(),
		r7 = register_encode_index(7) 	| register_encode_gp(),
		r8 = register_encode_index(8) 	| register_encode_gp(),
		r9 = register_encode_index(9) 	| register_encode_gp(),
		r10 = register_encode_index(10) | register_encode_gp(),
		r11 = register_encode_index(11) | register_encode_gp(),
		r12 = register_encode_index(12) | register_encode_gp(),
		r13 = register_encode_index(13) | register_encode_gp(),
		r14 = register_encode_index(14) | register_encode_gp(),
		r15 = register_encode_index(15) | register_encode_gp(),
		r16 = register_encode_index(16) | register_encode_gp(),
		r17 = register_encode_index(17) | register_encode_gp(),
		r18 = register_encode_index(18) | register_encode_gp(),
		r19 = register_encode_index(19) | register_encode_gp(),
		r20 = register_encode_index(20) | register_encode_gp(),
		r21 = register_encode_index(21) | register_encode_gp(),
		r22 = register_encode_index(22) | register_encode_gp(),
		r23 = register_encode_index(23) | register_encode_gp(),
		r24 = register_encode_index(24) | register_encode_gp(),
		r25 = register_encode_index(25) | register_encode_gp(),
		r26 = register_encode_index(26) | register_encode_gp(),
		r27 = register_encode_index(27) | register_encode_gp(),
		r28 = register_encode_index(28) | register_encode_gp(),
		r29 = register_encode_index(29) | register_encode_gp(),
		r30 = register_encode_index(30) | register_encode_gp(),
		/* floating point */
		v0 = register_encode_index(0) 	| register_encode_fp(),
		v1 = register_encode_index(1) 	| register_encode_fp(),
		v2 = register_encode_index(2) 	| register_encode_fp(),
		v3 = register_encode_index(3) 	| register_encode_fp(),
		v4 = register_encode_index(4) 	| register_encode_fp(),
		v5 = register_encode_index(5) 	| register_encode_fp(),
		v6 = register_encode_index(6) 	| register_encode_fp(),
		v7 = register_encode_index(7) 	| register_encode_fp(),
		v8 = register_encode_index(8) 	| register_encode_fp(),
		v9 = register_encode_index(9) 	| register_encode_fp(),
		v10 = register_encode_index(10) | register_encode_fp(),
		v11 = register_encode_index(11) | register_encode_fp(),
		v12 = register_encode_index(12) | register_encode_fp(),
		v13 = register_encode_index(13) | register_encode_fp(),
		v14 = register_encode_index(14) | register_encode_fp(),
		v15 = register_encode_index(15) | register_encode_fp(),
		v16 = register_encode_index(16) | register_encode_fp(),
		v17 = register_encode_index(17) | register_encode_fp(),
		v18 = register_encode_index(18) | register_encode_fp(),
		v19 = register_encode_index(19) | register_encode_fp(),
		v20 = register_encode_index(20) | register_encode_fp(),
		v21 = register_encode_index(21) | register_encode_fp(),
		v22 = register_encode_index(22) | register_encode_fp(),
		v23 = register_encode_index(23) | register_encode_fp(),
		v24 = register_encode_index(24) | register_encode_fp(),
		v25 = register_encode_index(25) | register_encode_fp(),
		v26 = register_encode_index(26) | register_encode_fp(),
		v27 = register_encode_index(27) | register_encode_fp(),
		v28 = register_encode_index(28) | register_encode_fp(),
		v29 = register_encode_index(29) | register_encode_fp(),
		v30 = register_encode_index(30) | register_encode_fp(),
		v31 = register_encode_index(31) | register_encode_fp(),
		/* sve scalable */
		z0 = register_encode_index(0) 	| register_encode_sve_scalable(),
		z1 = register_encode_index(1) 	| register_encode_sve_scalable(),
		z2 = register_encode_index(2) 	| register_encode_sve_scalable(),
		z3 = register_encode_index(3) 	| register_encode_sve_scalable(),
		z4 = register_encode_index(4) 	| register_encode_sve_scalable(),
		z5 = register_encode_index(5) 	| register_encode_sve_scalable(),
		z6 = register_encode_index(6) 	| register_encode_sve_scalable(),
		z7 = register_encode_index(7) 	| register_encode_sve_scalable(),
		z8 = register_encode_index(8) 	| register_encode_sve_scalable(),
		z9 = register_encode_index(9) 	| register_encode_sve_scalable(),
		z10 = register_encode_index(10) | register_encode_sve_scalable(),
		z11 = register_encode_index(11) | register_encode_sve_scalable(),
		z12 = register_encode_index(12) | register_encode_sve_scalable(),
		z13 = register_encode_index(13) | register_encode_sve_scalable(),
		z14 = register_encode_index(14) | register_encode_sve_scalable(),
		z15 = register_encode_index(15) | register_encode_sve_scalable(),
		z16 = register_encode_index(16) | register_encode_sve_scalable(),
		z17 = register_encode_index(17) | register_encode_sve_scalable(),
		z18 = register_encode_index(18) | register_encode_sve_scalable(),
		z19 = register_encode_index(19) | register_encode_sve_scalable(),
		z20 = register_encode_index(20) | register_encode_sve_scalable(),
		z21 = register_encode_index(21) | register_encode_sve_scalable(),
		z22 = register_encode_index(22) | register_encode_sve_scalable(),
		z23 = register_encode_index(23) | register_encode_sve_scalable(),
		z24 = register_encode_index(24) | register_encode_sve_scalable(),
		z25 = register_encode_index(25) | register_encode_sve_scalable(),
		z26 = register_encode_index(26) | register_encode_sve_scalable(),
		z27 = register_encode_index(27) | register_encode_sve_scalable(),
		z28 = register_encode_index(28) | register_encode_sve_scalable(),
		z29 = register_encode_index(29) | register_encode_sve_scalable(),
		z30 = register_encode_index(30) | register_encode_sve_scalable(),
		z31 = register_encode_index(31) | register_encode_sve_scalable(),
		/* sve predicate */
		p0 = register_encode_index(0) 	| register_encode_sve_predicate(),
		p1 = register_encode_index(1) 	| register_encode_sve_predicate(),
		p2 = register_encode_index(2) 	| register_encode_sve_predicate(),
		p3 = register_encode_index(3) 	| register_encode_sve_predicate(),
		p4 = register_encode_index(4) 	| register_encode_sve_predicate(),
		p5 = register_encode_index(5) 	| register_encode_sve_predicate(),
		p6 = register_encode_index(6) 	| register_encode_sve_predicate(),
		p7 = register_encode_index(7) 	| register_encode_sve_predicate(),
		p8 = register_encode_index(8) 	| register_encode_sve_predicate(),
		p9 = register_encode_index(9) 	| register_encode_sve_predicate(),
		p10 = register_encode_index(10)	| register_encode_sve_predicate(),
		p11 = register_encode_index(11)	| register_encode_sve_predicate(),
		p12 = register_encode_index(12)	| register_encode_sve_predicate(),
		p13 = register_encode_index(13)	| register_encode_sve_predicate(),
		p14 = register_encode_index(14)	| register_encode_sve_predicate(),
		p15 = register_encode_index(15)	| register_encode_sve_predicate()
	};
	Register SourceImmediateRegister = Register::r0;
	Register SourceBaseRegister = Register::r0;
	Register SourceDisplacementRegister = Register::r1;
	Register DestinationImmediateRegister = Register::r2;
	Register DestinationBaseRegister = Register::r2;
	Register DestinationDisplacementRegister = Register::r3;
	enum class StorageMode {
		IndirectImmediate,
		IndirectRegister,
		DirectImmediate,
		DirectRegister,
	};
	/**
	 * @brief contains storage information for variables
	 * 
	 */
	class VariableStorage {
	public:
		StorageMode mode;
		ImmediateValue immediate;
		ImmediateValue displacement;
		Register reg;
	};

	typedef uint32_t instruction;
	namespace instructions {
		instruction ret() {
		}
	}
}
