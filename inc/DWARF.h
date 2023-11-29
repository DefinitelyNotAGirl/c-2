/**
 * Created Date: Monday October 9th 2023
 * Author: Lilith
 * -----
 * Last Modified: Monday October 9th 2023 10:37:49 pm
 * Modified By: Lilith (definitelynotagirl115169@gmail.com)
 * -----
 * Copyright (c) 2023-2023 DefinitelyNotAGirl@github
 * 
 * Permission is hereby granted, free of charge, to any person
 * obtaining a copy of this software and associated documentation
 * files (the "Software"), to deal in the Software without
 * restriction, including without limitation the rights to use, copy,
 * modify, merge, publish, distribute, sublicense, and/or sell copies
 * of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 * 
 * The above copyright notice and this permission notice shall be
 * included in all copies or substantial portions of the Software.
 * 
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND,
 * EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF
 * MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND
 * NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT
 * HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY,
 * WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER
 * DEALINGS IN THE SOFTWARE.
 */
#pragma once

namespace DWARF5
{
    enum TAG : uint16_t 
    {
        TAG_array_type                  = 0x01  ,
        TAG_class_type                  = 0x02  ,
        TAG_entry_point                 = 0x03  ,
        TAG_enumeration_type            = 0x04  ,
        TAG_formal_parameter            = 0x05  ,
        TAG_imported_declaration        = 0x08  ,
        TAG_label                       = 0x0a  ,
        TAG_lexical_block               = 0x0b  ,
        TAG_member                      = 0x0d  ,
        TAG_pointer_type                = 0x0f  ,
        TAG_reference_type              = 0x10  ,
        TAG_compile_unit                = 0x11  ,
        TAG_string_type                 = 0x12  ,
        TAG_structure_type              = 0x13  ,
        TAG_subroutine_type             = 0x15  ,
        TAG_typedef                     = 0x16  ,
        TAG_union_type                  = 0x17  ,
        TAG_unspecified_parameters      = 0x18  ,
        TAG_variant                     = 0x19  ,
        TAG_common_block                = 0x1a  ,
        TAG_common_inclusion            = 0x1b  ,
        TAG_inheritance                 = 0x1c  ,
        TAG_inlined_subroutine          = 0x1d  ,
        TAG_module                      = 0x1e  ,
        TAG_ptr_to_member_type          = 0x1f  ,
        TAG_set_type                    = 0x20  ,
        TAG_subrange_type               = 0x21  ,
        TAG_with_stmt                   = 0x22  ,
        TAG_access_declaration          = 0x23  ,
        TAG_base_type                   = 0x24  ,
        TAG_catch_block                 = 0x25  ,
        TAG_const_type                  = 0x26  ,
        TAG_constant                    = 0x27  ,
        TAG_enumerator                  = 0x28  ,
        TAG_file_type                   = 0x29  ,
        TAG_friend                      = 0x2a  ,
        TAG_namelist                    = 0x2b  ,
        TAG_namelist_item               = 0x2c  ,
        TAG_packed_type                 = 0x2d  ,
        TAG_subprogram                  = 0x2e  ,
        TAG_template_type_parameter     = 0x2f  ,
        TAG_template_value_parameter    = 0x30  ,
        TAG_thrown_type                 = 0x31  ,
        TAG_try_block                   = 0x32  ,
        TAG_variant_part                = 0x33  ,
        TAG_variable                    = 0x34  ,
        TAG_volatile_type               = 0x35  ,
        TAG_dwarf_procedure             = 0x36  ,
        TAG_restrict_type               = 0x37  ,
        TAG_interface_type              = 0x38  ,
        TAG_namespace                   = 0x39  ,
        TAG_imported_module             = 0x3a  ,
        TAG_unspecified_type            = 0x3b  ,
        TAG_partial_unit                = 0x3c  ,
        TAG_imported_unit               = 0x3d  ,
        TAG_condition                   = 0x3f  ,
        TAG_shared_type                 = 0x40  ,
        TAG_type_unit                   = 0x41  ,
        TAG_rvalue_reference_type       = 0x42  ,
        TAG_template_alias              = 0x43  ,
        TAG_coarray_type                = 0x44  ,
        TAG_generic_subrange            = 0x45  ,
        TAG_dynamic_type                = 0x46  ,
        TAG_MIPS_loop                   = 0x4081,
        TAG_format_label                = 0x4101,
        TAG_function_template           = 0x4102,
        TAG_class_template              = 0x4103,
        TAG_GNU_template_template_param = 0x4106,
        TAG_GNU_template_parameter_pack = 0x4107,
        TAG_GNU_formal_parameter_pack   = 0x4108,
        TAG_lo_user                     = 0x4080,
        TAG_APPLE_property              = 0x4200,
        TAG_hi_user                     = 0xffff
    };
    enum UT : uint8_t
    {
        UT_compile       = 0x01,
        UT_type          = 0x02,
        UT_partial       = 0x03,
        UT_skeleton      = 0x04,
        UT_split_compile = 0x05,
        UT_split_type    = 0x06,
        UT_lo_user       = 0x80,
        UT_hi_user       = 0xff
    };
    enum AT : uint16_t 
    {
        AT_sibling                         = 0x01  ,
        AT_location                        = 0x02  ,
        AT_name                            = 0x03  ,
        AT_ordering                        = 0x09  ,
        AT_byte_size                       = 0x0b  ,
        AT_bit_offset                      = 0x0c  ,
        AT_bit_size                        = 0x0d  ,
        AT_stmt_list                       = 0x10  ,
        AT_low_pc                          = 0x11  ,
        AT_high_pc                         = 0x12  ,
        AT_language                        = 0x13  ,
        AT_discr                           = 0x15  ,
        AT_discr_value                     = 0x16  ,
        AT_visibility                      = 0x17  ,
        AT_import                          = 0x18  ,
        AT_string_length                   = 0x19  ,
        AT_common_reference                = 0x1a  ,
        AT_comp_dir                        = 0x1b  ,
        AT_const_value                     = 0x1c  ,
        AT_containing_type                 = 0x1d  ,
        AT_default_value                   = 0x1e  ,
        AT_inline                          = 0x20  ,
        AT_is_optional                     = 0x21  ,
        AT_lower_bound                     = 0x22  ,
        AT_producer                        = 0x25  ,
        AT_prototyped                      = 0x27  ,
        AT_return_addr                     = 0x2a  ,
        AT_start_scope                     = 0x2c  ,
        AT_bit_stride                      = 0x2e  ,
        AT_upper_bound                     = 0x2f  ,
        AT_abstract_origin                 = 0x31  ,
        AT_accessibility                   = 0x32  ,
        AT_address_class                   = 0x33  ,
        AT_artificial                      = 0x34  ,
        AT_base_types                      = 0x35  ,
        AT_calling_convention              = 0x36  ,
        AT_count                           = 0x37  ,
        AT_data_member_location            = 0x38  ,
        AT_decl_column                     = 0x39  ,
        AT_decl_file                       = 0x3a  ,
        AT_decl_line                       = 0x3b  ,
        AT_declaration                     = 0x3c  ,
        AT_discr_list                      = 0x3d  ,
        AT_encoding                        = 0x3e  ,
        AT_external                        = 0x3f  ,
        AT_frame_base                      = 0x40  ,
        AT_friend                          = 0x41  ,
        AT_identifier_case                 = 0x42  ,
        AT_macro_info                      = 0x43  ,
        AT_namelist_item                   = 0x44  ,
        AT_priority                        = 0x45  ,
        AT_segment                         = 0x46  ,
        AT_specification                   = 0x47  ,
        AT_static_link                     = 0x48  ,
        AT_type                            = 0x49  ,
        AT_use_location                    = 0x4a  ,
        AT_variable_parameter              = 0x4b  ,
        AT_virtuality                      = 0x4c  ,
        AT_vtable_elem_location            = 0x4d  ,
        AT_allocated                       = 0x4e  ,
        AT_associated                      = 0x4f  ,
        AT_data_location                   = 0x50  ,
        AT_byte_stride                     = 0x51  ,
        AT_entry_pc                        = 0x52  ,
        AT_use_UTF8                        = 0x53  ,
        AT_extension                       = 0x54  ,
        AT_ranges                          = 0x55  ,
        AT_trampoline                      = 0x56  ,
        AT_call_column                     = 0x57  ,
        AT_call_file                       = 0x58  ,
        AT_call_line                       = 0x59  ,
        AT_description                     = 0x5a  ,
        AT_binary_scale                    = 0x5b  ,
        AT_decimal_scale                   = 0x5c  ,
        AT_small                           = 0x5d  ,
        AT_decimal_sign                    = 0x5e  ,
        AT_digit_count                     = 0x5f  ,
        AT_picture_string                  = 0x60  ,
        AT_mutable                         = 0x61  ,
        AT_threads_scaled                  = 0x62  ,
        AT_explicit                        = 0x63  ,
        AT_object_pointer                  = 0x64  ,
        AT_endianity                       = 0x65  ,
        AT_elemental                       = 0x66  ,
        AT_pure                            = 0x67  ,
        AT_recursive                       = 0x68  ,
        AT_signature                       = 0x69  ,
        AT_main_subprogram                 = 0x6a  ,
        AT_data_bit_offset                 = 0x6b  ,
        AT_const_expr                      = 0x6c  ,
        AT_enum_class                      = 0x6d  ,
        AT_linkage_name                    = 0x6e  ,
        AT_string_length_bit_size          = 0x6f  ,
        AT_string_length_byte_size         = 0x70  ,
        AT_rank                            = 0x71  ,
        AT_str_offsets_base                = 0x72  ,
        AT_addr_base                       = 0x73  ,
        AT_ranges_base                     = 0x74  ,
        AT_dwo_id                          = 0x75  ,
        AT_dwo_name                        = 0x76  ,
        AT_reference                       = 0x77  ,
        AT_rvalue_reference                = 0x78  ,
        AT_macros                          = 0x79  ,
        AT_lo_user                         = 0x2000,
        AT_hi_user                         = 0x3fff,
        AT_MIPS_loop_begin                 = 0x2002,
        AT_MIPS_tail_loop_begin            = 0x2003,
        AT_MIPS_epilog_begin               = 0x2004,
        AT_MIPS_loop_unroll_factor         = 0x2005,
        AT_MIPS_software_pipeline_depth    = 0x2006,
        AT_MIPS_linkage_name               = 0x2007,
        AT_MIPS_stride                     = 0x2008,
        AT_MIPS_abstract_name              = 0x2009,
        AT_MIPS_clone_origin               = 0x200a,
        AT_MIPS_has_inlines                = 0x200b,
        AT_MIPS_stride_byte                = 0x200c,
        AT_MIPS_stride_elem                = 0x200d,
        AT_MIPS_ptr_dopetype               = 0x200e,
        AT_MIPS_allocatable_dopetype       = 0x200f,
        AT_MIPS_assumed_shape_dopetype     = 0x2010,
        AT_MIPS_assumed_size               = 0x2011,
        AT_sf_names                        = 0x2101,
        AT_src_info                        = 0x2102,
        AT_mac_info                        = 0x2103,
        AT_src_coords                      = 0x2104,
        AT_body_begin                      = 0x2105,
        AT_body_end                        = 0x2106,
        AT_GNU_vector                      = 0x2107,
        AT_GNU_template_name               = 0x2110,
        AT_GNU_odr_signature               = 0x210f,
        AT_GNU_macros                      = 0x2119,
        AT_GNU_dwo_name                    = 0x2130,
        AT_GNU_dwo_id                      = 0x2131,
        AT_GNU_ranges_base                 = 0x2132,
        AT_GNU_addr_base                   = 0x2133,
        AT_GNU_pubnames                    = 0x2134,
        AT_GNU_pubtypes                    = 0x2135,
        AT_GNU_discriminator               = 0x2136,
        AT_BORLAND_property_read           = 0x3b11,
        AT_BORLAND_property_write          = 0x3b12,
        AT_BORLAND_property_implements     = 0x3b13,
        AT_BORLAND_property_index          = 0x3b14,
        AT_BORLAND_property_default        = 0x3b15,
        AT_BORLAND_Delphi_unit             = 0x3b20,
        AT_BORLAND_Delphi_class            = 0x3b21,
        AT_BORLAND_Delphi_record           = 0x3b22,
        AT_BORLAND_Delphi_metaclass        = 0x3b23,
        AT_BORLAND_Delphi_constructor      = 0x3b24,
        AT_BORLAND_Delphi_destructor       = 0x3b25,
        AT_BORLAND_Delphi_anonymous_method = 0x3b26,
        AT_BORLAND_Delphi_interface        = 0x3b27,
        AT_BORLAND_Delphi_ABI              = 0x3b28,
        AT_BORLAND_Delphi_return           = 0x3b29,
        AT_BORLAND_Delphi_frameptr         = 0x3b30,
        AT_BORLAND_closure                 = 0x3b31,
        AT_LLVM_include_path               = 0x3e00,
        AT_LLVM_config_macros              = 0x3e01,
        AT_LLVM_isysroot                   = 0x3e02,
        AT_APPLE_optimized                 = 0x3fe1,
        AT_APPLE_flags                     = 0x3fe2,
        AT_APPLE_isa                       = 0x3fe3,
        AT_APPLE_block                     = 0x3fe4,
        AT_APPLE_major_runtime_vers        = 0x3fe5,
        AT_APPLE_runtime_class             = 0x3fe6,
        AT_APPLE_omit_frame_ptr            = 0x3fe7,
        AT_APPLE_property_name             = 0x3fe8,
        AT_APPLE_property_getter           = 0x3fe9,
        AT_APPLE_property_setter           = 0x3fea,
        AT_APPLE_property_attribute        = 0x3feb,
        AT_APPLE_objc_complete_type        = 0x3fec,
        AT_APPLE_property                  = 0x3fed
    };
    enum FORM : uint16_t 
    {
        FORM_addr           = 0x01  ,
        FORM_block2         = 0x03  ,
        FORM_block4         = 0x04  ,
        FORM_data2          = 0x05  ,
        FORM_data4          = 0x06  ,
        FORM_data8          = 0x07  ,
        FORM_string         = 0x08  ,
        FORM_block          = 0x09  ,
        FORM_block1         = 0x0a  ,
        FORM_data1          = 0x0b  ,
        FORM_flag           = 0x0c  ,
        FORM_sdata          = 0x0d  ,
        FORM_strp           = 0x0e  ,
        FORM_udata          = 0x0f  ,
        FORM_ref_addr       = 0x10  ,
        FORM_ref1           = 0x11  ,
        FORM_ref2           = 0x12  ,
        FORM_ref4           = 0x13  ,
        FORM_ref8           = 0x14  ,
        FORM_ref_udata      = 0x15  ,
        FORM_indirect       = 0x16  ,
        FORM_sec_offset     = 0x17  ,
        FORM_exprloc        = 0x18  ,
        FORM_flag_present   = 0x19  ,
        FORM_ref_sig8       = 0x20  ,
        FORM_GNU_addr_index = 0x1f01,
        FORM_GNU_str_index  = 0x1f02,
        FORM_GNU_ref_alt    = 0x1f20,
        FORM_GNU_strp_alt   = 0x1f21
    };
}
