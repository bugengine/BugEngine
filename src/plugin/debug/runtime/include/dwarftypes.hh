/* BugEngine / 2008-2012  Nicolas MERCIER <mercier.nicolas@gmail.com>
   see LICENSE for detail */

#ifndef BE_DEBUG_RUNTIME_DWARFTYPES_HH_
#define BE_DEBUG_RUNTIME_DWARFTYPES_HH_
/*****************************************************************************/

namespace BugEngine { namespace Runtime { namespace Dwarf
{

enum Tag
{
    Tag_end                 = 0x00,
    Array_type              = 0x01,
    Class_type              = 0x02,
    Entry_point             = 0x03,
    Enumeration_type        = 0x04,
    Formal_parameter        = 0x05,
    Imported_declaration    = 0x08,
    Label                   = 0x0a,
    Lexical_block           = 0x0b,
    Member                  = 0x0d,
    Pointer_type            = 0x0f,
    Reference_type          = 0x10,
    Compile_unit            = 0x11,
    String_type             = 0x12,
    Structure_type          = 0x13,
    Subroutine_type         = 0x15,
    Typedef                 = 0x16,
    Union_type              = 0x17,
    Unspecified_parameters  = 0x18,
    Variant                 = 0x19,
    Common_block            = 0x1a,
    Common_inclusion        = 0x1b,
    Inheritance             = 0x1c,
    Inlined_subroutine      = 0x1d,
    Module                  = 0x1e,
    Ptr_to_member_type      = 0x1f,
    Set_type                = 0x20,
    Subrange_type           = 0x21,
    With_stmt               = 0x22,
    Access_declaration      = 0x23,
    Base_type               = 0x24,
    Catch_block             = 0x25,
    Const_type              = 0x26,
    Constant                = 0x27,
    Enumerator              = 0x28,
    File_type               = 0x29,
    Friend                  = 0x2a,
    Namelist                = 0x2b,
    /* Early releases of this header had the following
       misspelled with a trailing 's' */
    Namelist_item           = 0x2c /* DWARF3/2 spelling */,
    Namelist_items          = 0x2c /* SGI misspelling/typo */,
    Packed_type             = 0x2d,
    Subprogram              = 0x2e,
    /* The DWARF2 document had two spellings of the following
       two TAGs, DWARF3 specifies the longer spelling. */
    Template_type_parameter = 0x2f /* DWARF3/2 spelling*/,
    Template_type_param     = 0x2f /* DWARF2   spelling*/,
    Template_value_parameter= 0x30 /* DWARF3/2 spelling*/,
    Template_value_param    = 0x30 /* DWARF2   spelling*/,
    Thrown_type             = 0x31,
    Try_block               = 0x32,
    Variant_part            = 0x33,
    Variable                = 0x34,
    Volatile_type           = 0x35,
    Dwarf_procedure         = 0x36  /* DWARF3 */,
    Restrict_type           = 0x37  /* DWARF3 */,
    Interface_type          = 0x38  /* DWARF3 */,
    Namespace               = 0x39  /* DWARF3 */,
    Imported_module         = 0x3a  /* DWARF3 */,
    Unspecified_type        = 0x3b  /* DWARF3 */,
    Partial_unit            = 0x3c  /* DWARF3 */,
    Imported_unit           = 0x3d  /* DWARF3 */,
        /* Do not use DW_TAG_mutable_type */
    Mutable_type            = 0x3e /* Withdrawn from DWARF3 by DWARF3f. */,
    Condition               = 0x3f  /* DWARF3f */,
    Shared_type             = 0x40  /* DWARF3f */,
    Lo_user                 = 0x4080,

    MIPS_loop               = 0x4081,

/* HP extensions: ftp://ftp.hp.com/pub/lang/tools/WDB/wdb-4.0.tar.gz  */
    HP_array_descriptor     = 0x4090 /* HP */,

/* GNU extensions.  The first 3 missing the GNU_. */
    Format_label            = 0x4101 /* GNU. Fortran. */,
    Function_template       = 0x4102 /* GNU. For C++ */,
    Class_template          = 0x4103 /* GNU. For C++ */,
    GNU_BINCL               = 0x4104 /* GNU */,
    GNU_EINCL               = 0x4105 /* GNU */,

/* ALTIUM extensions */
    /* DSP-C/Starcore __circ qualifier */
    ALTIUM_circ_type        = 0x5101 /* ALTIUM */,
    /* Starcore __mwa_circ qualifier */ 
    ALTIUM_mwa_circ_type    = 0x5102 /* ALTIUM */,
    /* Starcore __rev_carry qualifier */
    ALTIUM_rev_carry_type   = 0x5103 /* ALTIUM */,
    /* M16 __rom qualifier */
    ALTIUM_rom              = 0x5111 /* ALTIUM */,

/* The following 3 are extensions to support UPC */
    Upc_shared_type         = 0x8765 /* UPC */,
    Upc_strict_type         = 0x8766 /* UPC */,
    Upc_relaxed_type        = 0x8767 /* UPC */,

/* PGI (STMicroelectronics) extensions. */
    PGI_kanji_type          = 0xa000 /* PGI */,
    PGI_interface_block     = 0xa020 /* PGI */,
/* The following are SUN extensions */

    SUN_function_template   = 0x4201 /* SUN */,
    SUN_class_template      = 0x4202 /* SUN */,
    SUN_struct_template     = 0x4203 /* SUN */,
    SUN_union_template      = 0x4204 /* SUN */,
    SUN_indirect_inheritance= 0x4205 /* SUN */,
    SUN_codeflags           = 0x4206 /* SUN */,
    SUN_memop_info          = 0x4207 /* SUN */,
    SUN_omp_child_func      = 0x4208 /* SUN */,
    SUN_rtti_descriptor     = 0x4209 /* SUN */,
    SUN_dtor_info           = 0x420a /* SUN */,
    SUN_dtor                = 0x420b /* SUN */,
    SUN_f90_interface       = 0x420c /* SUN */,
    SUN_fortran_vax_structure = 0x420d /* SUN */,
    SUN_hi                  = 0x42ff /* SUN */,


    Hi_user                 = 0xffff
};

enum Children
{
    Children_no             = 0x00,
    Children_yes            = 0x01
};

enum Attribute
{
    Attribute_end                              = 0x00,
    Attribute_sibling                          = 0x01,
    Attribute_location                         = 0x02,
    Attribute_name                             = 0x03,
    Attribute_ordering                         = 0x09,
    Attribute_subscr_data                      = 0x0a,
    Attribute_byte_size                        = 0x0b,
    Attribute_bit_offset                       = 0x0c,
    Attribute_bit_size                         = 0x0d,
    Attribute_element_list                     = 0x0f,
    Attribute_stmt_list                        = 0x10,
    Attribute_low_pc                           = 0x11,
    Attribute_high_pc                          = 0x12,
    Attribute_language                         = 0x13,
    Attribute_member                           = 0x14,
    Attribute_discr                            = 0x15,
    Attribute_discr_value                      = 0x16,
    Attribute_visibility                       = 0x17,
    Attribute_import                           = 0x18,
    Attribute_string_length                    = 0x19,
    Attribute_common_reference                 = 0x1a,
    Attribute_comp_dir                         = 0x1b,
    Attribute_const_value                      = 0x1c,
    Attribute_containing_type                  = 0x1d,
    Attribute_default_value                    = 0x1e,
    Attribute_inline                           = 0x20,
    Attribute_is_optional                      = 0x21,
    Attribute_lower_bound                      = 0x22,
    Attribute_producer                         = 0x25,
    Attribute_prototyped                       = 0x27,
    Attribute_return_addr                      = 0x2a,
    Attribute_start_scope                      = 0x2c,
    Attribute_bit_stride                       = 0x2e /* DWARF3 name */,
    Attribute_stride_size                      = 0x2e /* DWARF2 name */,
    Attribute_upper_bound                      = 0x2f,
    Attribute_abstract_origin                  = 0x31,
    Attribute_accessibility                    = 0x32,
    Attribute_address_class                    = 0x33,
    Attribute_artificial                       = 0x34,
    Attribute_base_types                       = 0x35,
    Attribute_calling_convention               = 0x36,
    Attribute_count                            = 0x37,
    Attribute_data_member_location             = 0x38,
    Attribute_decl_column                      = 0x39,
    Attribute_decl_file                        = 0x3a,
    Attribute_decl_line                        = 0x3b,
    Attribute_declaration                      = 0x3c,
    Attribute_discr_list                       = 0x3d,
    Attribute_encoding                         = 0x3e,
    Attribute_external                         = 0x3f,
    Attribute_frame_base                       = 0x40,
    Attribute_friend                           = 0x41,
    Attribute_identifier_case                  = 0x42,
    Attribute_macro_info                       = 0x43,
    Attribute_namelist_item                    = 0x44,
    Attribute_priority                         = 0x45,
    Attribute_segment                          = 0x46,
    Attribute_specification                    = 0x47,
    Attribute_static_link                      = 0x48,
    Attribute_type                             = 0x49,
    Attribute_use_location                     = 0x4a,
    Attribute_variable_parameter               = 0x4b,
    Attribute_virtuality                       = 0x4c,
    Attribute_vtable_elem_location             = 0x4d,
    Attribute_allocated                        = 0x4e /* DWARF3 */,
    Attribute_associated                       = 0x4f /* DWARF3 */,
    Attribute_data_location                    = 0x50 /* DWARF3 */,
    Attribute_byte_stride                      = 0x51 /* DWARF3f */,
    Attribute_stride                           = 0x51 /* DWARF3 (do not use) */,
    Attribute_entry_pc                         = 0x52 /* DWARF3 */,
    Attribute_use_UTF8                         = 0x53 /* DWARF3 */,
    Attribute_extension                        = 0x54 /* DWARF3 */,
    Attribute_ranges                           = 0x55 /* DWARF3 */,
    Attribute_trampoline                       = 0x56 /* DWARF3 */,
    Attribute_call_column                      = 0x57 /* DWARF3 */,
    Attribute_call_file                        = 0x58 /* DWARF3 */,
    Attribute_call_line                        = 0x59 /* DWARF3 */,
    Attribute_description                      = 0x5a /* DWARF3 */,
    Attribute_binary_scale                     = 0x5b /* DWARF3f */,
    Attribute_decimal_scale                    = 0x5c /* DWARF3f */,
    Attribute_small                            = 0x5d /* DWARF3f */,
    Attribute_decimal_sign                     = 0x5e /* DWARF3f */,
    Attribute_digit_count                      = 0x5f /* DWARF3f */,
    Attribute_picture_string                   = 0x60 /* DWARF3f */,
    Attribute_mutable                          = 0x61 /* DWARF3f */,
    Attribute_threads_scaled                   = 0x62 /* DWARF3f */,
    Attribute_explicit                         = 0x63 /* DWARF3f */,
    Attribute_object_pointer                   = 0x64 /* DWARF3f */,
    Attribute_endianity                        = 0x65 /* DWARF3f */,
    Attribute_elemental                        = 0x66 /* DWARF3f */,
    Attribute_pure                             = 0x67 /* DWARF3f */,
    Attribute_recursive                        = 0x68 /* DWARF3f */,
    Attribute_max                              =  Attribute_recursive+1
};

enum Type
{
    Type_addr                   = 0x01,
    Type_block2                 = 0x03,
    Type_block4                 = 0x04,
    Type_data2                  = 0x05,
    Type_data4                  = 0x06,
    Type_data8                  = 0x07,
    Type_string                 = 0x08,
    Type_block                  = 0x09,
    Type_block1                 = 0x0a,
    Type_data1                  = 0x0b,
    Type_flag                   = 0x0c,
    Type_sdata                  = 0x0d,
    Type_strp                   = 0x0e,
    Type_udata                  = 0x0f,
    Type_ref_addr               = 0x10,
    Type_ref1                   = 0x11,
    Type_ref2                   = 0x12,
    Type_ref4                   = 0x13,
    Type_ref8                   = 0x14,
    Type_ref_udata              = 0x15,
    Type_indirect               = 0x16
};

}}}

/*****************************************************************************/
#endif

