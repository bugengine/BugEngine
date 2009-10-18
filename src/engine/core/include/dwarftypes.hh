/* BugEngine / Copyright (C) 2005-2009  screetch <screetch@gmail.com>
   see LICENSE for detail */

#ifndef BE_CORE_DEBUG_DWARFTYPES_HH_
#define BE_CORE_DEBUG_DWARFTYPES_HH_
/*****************************************************************************/

namespace BugEngine { namespace Debug { namespace Dwarf
{

struct Tag
{
    enum
    {
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
    

        Hi_user                 = 0xffff,
    };
};

}}}

/*****************************************************************************/
#endif

