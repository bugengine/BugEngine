/* BugEngine / 2008-2012  Nicolas MERCIER <mercier.nicolas@gmail.com>
   see LICENSE for detail */

#ifndef BE_CORE_PREPROCESSOR_H_
#define BE_CORE_PREPROCESSOR_H_
/*****************************************************************************/


#define     BE_STRINGIZE_IMPL(n)    #n
#define     BE_STRINGIZE(n)         BE_STRINGIZE_IMPL(n)
#define     BE_CONCAT_IMPL(i,j)     i##j
#define     BE_CONCAT(i,j)          BE_CONCAT_IMPL(i,j)

#define BE_PP_NUM_0                 0
#define BE_PP_NUM_1                 1
#define BE_PP_NUM_2                 2
#define BE_PP_NUM_3                 3
#define BE_PP_NUM_4                 4
#define BE_PP_NUM_5                 5
#define BE_PP_NUM_6                 6
#define BE_PP_NUM_7                 7
#define BE_PP_NUM_8                 8
#define BE_PP_NUM_9                 9
#define BE_PP_NUM_10                10
#define BE_PP_NUM_11                11
#define BE_PP_NUM_12                12
#define BE_PP_NUM_13                13
#define BE_PP_NUM_14                14
#define BE_PP_NUM_15                15
#define BE_PP_NUM_16                16
#define BE_PP_BOOL_0                0
#define BE_PP_BOOL_1                1
#define BE_PP_BOOL_2                1
#define BE_PP_BOOL_3                1
#define BE_PP_BOOL_4                1
#define BE_PP_BOOL_5                1
#define BE_PP_BOOL_6                1
#define BE_PP_BOOL_7                1
#define BE_PP_BOOL_8                1
#define BE_PP_BOOL_9                1
#define BE_PP_BOOL_10               1
#define BE_PP_BOOL_11               1
#define BE_PP_BOOL_12               1
#define BE_PP_BOOL_13               1
#define BE_PP_BOOL_14               1
#define BE_PP_BOOL_15               1
#define BE_PP_BOOL_16               1
#define BE_PP_NOT_1                 0
#define BE_PP_NOT_0                 1
#define BE_PP_AND_00                0
#define BE_PP_AND_01                0
#define BE_PP_AND_10                0
#define BE_PP_AND_11                1
#define BE_PP_OR_00                 0
#define BE_PP_OR_01                 1
#define BE_PP_OR_10                 1
#define BE_PP_OR_11                 1
#define BE_PP_XOR_00                0
#define BE_PP_XOR_01                1
#define BE_PP_XOR_10                1
#define BE_PP_XOR_11                0


#define BE_BOOL(x)                  BE_CONCAT(BE_PP_BOOL_,x)
#define BE_OP_NOT(x)                BE_CONCAT(BE_PP_NOT_,BE_BOOL(x))
#define BE_OP_AND(x,y)              BE_CONCAT(BE_PP_AND_,BE_CONCAT(BE_BOOL(x),BE_BOOL(y)))
#define BE_OP_OR(x,y)               BE_CONCAT(BE_PP_OR_, BE_CONCAT(BE_BOOL(x),BE_BOOL(y)))
#define BE_OP_XOR(x,y)              BE_CONCAT(BE_PP_XOR_,BE_CONCAT(BE_BOOL(x),BE_BOOL(y)))


#define BE_PP_IF_0(x)               
#define BE_PP_IF_1(x)               x
#define BE_PP_IFELSE_0(x,y)         x
#define BE_PP_IFELSE_1(x,y)         y

#define BE_IF(cond,exec)            BE_CONCAT(BE_PP_IF_,BE_BOOL(cond))(exec)
#define BE_IFELSE(cond,exec1,exec2) BE_CONCAT(BE_PP_IFELSE_,BE_BOOL(cond))(exec)



#define BE_COMMA_0                  
#define BE_COMMA_1                  ,
#define BE_COMMA(cond)              BE_CONCAT(BE_COMMA_,BE_BOOL(cond))


#define BE_PP_LIST_0(x)           
#define BE_PP_LIST_1(x)           x##1
#define BE_PP_LIST_2(x)           BE_PP_LIST_1(x) , x##2
#define BE_PP_LIST_3(x)           BE_PP_LIST_2(x) , x##3
#define BE_PP_LIST_4(x)           BE_PP_LIST_3(x) , x##4
#define BE_PP_LIST_5(x)           BE_PP_LIST_4(x) , x##5
#define BE_PP_LIST_6(x)           BE_PP_LIST_5(x) , x##6
#define BE_PP_LIST_7(x)           BE_PP_LIST_6(x) , x##7
#define BE_PP_LIST_8(x)           BE_PP_LIST_7(x) , x##8
#define BE_PP_LIST_9(x)           BE_PP_LIST_8(x) , x##9
#define BE_PP_LIST_10(x)          BE_PP_LIST_9(x) , x##10
#define BE_PP_LIST_11(x)          BE_PP_LIST_10(x) , x##11
#define BE_PP_LIST_12(x)          BE_PP_LIST_11(x) , x##12
#define BE_PP_LIST_13(x)          BE_PP_LIST_12(x) , x##13
#define BE_PP_LIST_14(x)          BE_PP_LIST_13(x) , x##14
#define BE_PP_LIST_15(x)          BE_PP_LIST_14(x) , x##15
#define BE_PP_LIST_16(x)          BE_PP_LIST_15(x) , x##16

#define BE_LIST(count,x)          BE_CONCAT(BE_PP_LIST_, count)(x)

#define BE_PP_LIST2_0(x,y)        
#define BE_PP_LIST2_1(x,y)        x##1 y
#define BE_PP_LIST2_2(x,y)        BE_PP_LIST2_1(x,y) , x##2 y
#define BE_PP_LIST2_3(x,y)        BE_PP_LIST2_2(x,y) , x##3 y
#define BE_PP_LIST2_4(x,y)        BE_PP_LIST2_3(x,y) , x##4 y
#define BE_PP_LIST2_5(x,y)        BE_PP_LIST2_4(x,y) , x##5 y
#define BE_PP_LIST2_6(x,y)        BE_PP_LIST2_5(x,y) , x##6 y
#define BE_PP_LIST2_7(x,y)        BE_PP_LIST2_6(x,y) , x##7 y
#define BE_PP_LIST2_8(x,y)        BE_PP_LIST2_7(x,y) , x##8 y
#define BE_PP_LIST2_9(x,y)        BE_PP_LIST2_8(x,y) , x##9 y
#define BE_PP_LIST2_10(x,y)       BE_PP_LIST2_9(x,y) , x##10 y
#define BE_PP_LIST2_11(x,y)       BE_PP_LIST2_10(x,y) , x##11 y
#define BE_PP_LIST2_12(x,y)       BE_PP_LIST2_11(x,y) , x##12 y
#define BE_PP_LIST2_13(x,y)       BE_PP_LIST2_12(x,y) , x##13 y
#define BE_PP_LIST2_14(x,y)       BE_PP_LIST2_13(x,y) , x##14 y
#define BE_PP_LIST2_15(x,y)       BE_PP_LIST2_14(x,y) , x##15 y
#define BE_PP_LIST2_16(x,y)       BE_PP_LIST2_15(x,y) , x##16 y
 
#define BE_LIST2(count,x,y)       BE_CONCAT(BE_PP_LIST2_, count)(x,y)


#define BE_PP_LIST3_0(x,y,z)   
#define BE_PP_LIST3_1(x,y,z)      x##1 y##1 z
#define BE_PP_LIST3_2(x,y,z)      BE_PP_LIST3_1(x,y,z) , x##2 y##2 z
#define BE_PP_LIST3_3(x,y,z)      BE_PP_LIST3_2(x,y,z) , x##3 y##3 z
#define BE_PP_LIST3_4(x,y,z)      BE_PP_LIST3_3(x,y,z) , x##4 y##4 z
#define BE_PP_LIST3_5(x,y,z)      BE_PP_LIST3_4(x,y,z) , x##5 y##5 z
#define BE_PP_LIST3_6(x,y,z)      BE_PP_LIST3_5(x,y,z) , x##6 y##6 z
#define BE_PP_LIST3_7(x,y,z)      BE_PP_LIST3_6(x,y,z) , x##7 y##7 z
#define BE_PP_LIST3_8(x,y,z)      BE_PP_LIST3_7(x,y,z) , x##8 y##8 z
#define BE_PP_LIST3_9(x,y,z)      BE_PP_LIST3_8(x,y,z) , x##9 y##9 z
#define BE_PP_LIST3_10(x,y,z)     BE_PP_LIST3_9(x,y,z) , x##10 y##10 z
#define BE_PP_LIST3_11(x,y,z)     BE_PP_LIST3_10(x,y,z) , x##11 y##11 z
#define BE_PP_LIST3_12(x,y,z)     BE_PP_LIST3_11(x,y,z) , x##12 y##12 z
#define BE_PP_LIST3_13(x,y,z)     BE_PP_LIST3_12(x,y,z) , x##13 y##13 z
#define BE_PP_LIST3_14(x,y,z)     BE_PP_LIST3_13(x,y,z) , x##14 y##14 z
#define BE_PP_LIST3_15(x,y,z)     BE_PP_LIST3_14(x,y,z) , x##15 y##15 z
#define BE_PP_LIST3_16(x,y,z)     BE_PP_LIST3_15(x,y,z) , x##16 y##16 z
 
#define BE_LIST3(count,x,y,z)     BE_CONCAT(BE_PP_LIST3_, count)(x,y,z)


#define BE_PP_LIST3_AND_0(x,y,z)      1
#define BE_PP_LIST3_AND_1(x,y,z)      (x##1 y##1 z)
#define BE_PP_LIST3_AND_2(x,y,z)      BE_PP_LIST3_1(x,y,z) && (x##2 y##2 z)
#define BE_PP_LIST3_AND_3(x,y,z)      BE_PP_LIST3_2(x,y,z) && (x##3 y##3 z)
#define BE_PP_LIST3_AND_4(x,y,z)      BE_PP_LIST3_3(x,y,z) && (x##4 y##4 z)
#define BE_PP_LIST3_AND_5(x,y,z)      BE_PP_LIST3_4(x,y,z) && (x##5 y##5 z)
#define BE_PP_LIST3_AND_6(x,y,z)      BE_PP_LIST3_5(x,y,z) && (x##6 y##6 z)
#define BE_PP_LIST3_AND_7(x,y,z)      BE_PP_LIST3_6(x,y,z) && (x##7 y##7 z)
#define BE_PP_LIST3_AND_8(x,y,z)      BE_PP_LIST3_7(x,y,z) && (x##8 y##8 z)
#define BE_PP_LIST3_AND_9(x,y,z)      BE_PP_LIST3_8(x,y,z) && (x##9 y##9 z)
#define BE_PP_LIST3_AND_10(x,y,z)     BE_PP_LIST3_9(x,y,z) && (x##10 y##10 z)
#define BE_PP_LIST3_AND_11(x,y,z)     BE_PP_LIST3_10(x,y,z) && (x##11 y##11 z)
#define BE_PP_LIST3_AND_12(x,y,z)     BE_PP_LIST3_11(x,y,z) && (x##12 y##12 z)
#define BE_PP_LIST3_AND_13(x,y,z)     BE_PP_LIST3_12(x,y,z) && (x##13 y##13 z)
#define BE_PP_LIST3_AND_14(x,y,z)     BE_PP_LIST3_13(x,y,z) && (x##14 y##14 z)
#define BE_PP_LIST3_AND_15(x,y,z)     BE_PP_LIST3_14(x,y,z) && (x##15 y##15 z)
#define BE_PP_LIST3_AND_16(x,y,z)     BE_PP_LIST3_15(x,y,z) && (x##16 y##16 z)

#define BE_LIST3_AND(count,x,y,z)     BE_CONCAT(BE_PP_LIST3_AND_, count)(x,y,z)


/*****************************************************************************/
#endif
