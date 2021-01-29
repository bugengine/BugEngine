from ircc.generators import IrccCDeclaration, IrccCDefinition, IrccCExpression
from ircc.generators.ircc_c_expressions import IrccCExpressionCast
from ircc import IrccType
from be_typing import TYPE_CHECKING


class IrccClExpressionVectorValue(IrccCExpression):
    def __init__(self, values):
        # type: (List[IrccExpression]) -> None
        IrccCExpression.__init__(self, IrccCExpression._PREC_CONSTANT)
        self._values = values

    def __str__(self):
        # type: () -> str
        return '(%s)' % (', '.join([str(v) for v in self._values]))


class IrccClExpressionVectorValueBroadcast(IrccCExpression):
    def __init__(self, value):
        # type: (IrccExpression) -> None
        IrccCExpression.__init__(self, IrccCExpression._PREC_CONSTANT)
        self._value = value

    def __str__(self):
        # type: () -> str
        return '(%s)' % (self._value)


class IrccClExpressionVectorExtract(IrccCExpression):
    def __init__(self, element_type, value, index):
        # type: (IrccType, IrccExpression, IrccExpression) -> None
        IrccCExpression.__init__(self, IrccCExpression._PREC_SUBSCRIPT)
        self._element_type = element_type
        self._value = value
        self._index = index

    def __str__(self):
        # type: () -> str
        return '((%s*)&%s)[%s]' % (self._element_type.format([]), self._value, self._index)


class IrccClExpressionVectorExtractConstant(IrccCExpression):
    def __init__(self, value, index):
        # type: (IrccExpression, int) -> None
        IrccCExpression.__init__(self, IrccCExpression._PREC_CONSTANT)
        self._value = value
        self._index = index

    def __str__(self):
        # type: () -> str
        return '%s.s%x' % (self._value, self._index)


class IrccClExpressionVectorShuffle(IrccCExpression):
    def __init__(self, source_size, vector1, vector2, mask):
        # type: (int, IrccExpression, IrccExpression, List[int]) -> None
        IrccCExpression.__init__(self, IrccCExpression._PREC_CONSTANT)
        self._source_size = source_size
        self._vector1 = vector1
        self._vector2 = vector2
        self._mask = mask

    def __str__(self):
        # type: () -> str
        v1 = str(self._vector1)
        v2 = str(self._vector2)

        def get_member(i):
            # type: (int) -> str
            if i < self._source_size:
                return '%s.s%x' % (v1, i)
            else:
                return '%s.s%x' % (v2, i - self._source_size)

        return '(%s)' % (', '.join([get_member(i) for i in self._mask]))


class ClDeclaration(IrccCDeclaration):
    _VECTOR_TYPES = {
        'i8': 'char%(size)d',
        'i16': 'short%(size)d',
        'i32': 'int%(size)d',
        'i64': 'long%(size)d',
        'u8': 'uchar%(size)d',
        'u16': 'ushort%(size)d',
        'u32': 'uint%(size)d',
        'u64': 'ulong%(size)d',
        'float': 'float%(size)d',
    }

    def __init__(self, file):
        # type: (TextIO) -> None
        IrccCDeclaration.__init__(self, file)
        self._address_spaces = {0: '__private', 1: '__global', 2: '__constant', 3: '__local'}

    def type_metadata(self):
        # type: () -> IrccType
        return IrccType(['', '', 'meta', ''])

    def make_address_space(self, type, address_space):
        # type: (IrccType, int) -> IrccType
        return IrccType(
            ['', self._address_spaces[address_space], ''], ['', self._address_spaces[address_space], ''], type
        )

    def begin_module(self):
        # type: () -> bool
        self._out_file.write(
            '/* generated code; do not edit */\n'
            'typedef bool           i1;\n'
            'typedef signed char    i8;\n'
            'typedef signed short   i16;\n'
            'typedef signed int     i32;\n'
            'typedef signed long    i64;\n'
            'typedef unsigned char  u8;\n'
            'typedef unsigned short u16;\n'
            'typedef unsigned int   u32;\n'
            'typedef unsigned long  u64;\n'
            'typedef void* meta;\n'
            '\n'
        )
        return True

    def end_module(self):
        # type: () -> None
        self._out_file.write('\n')

    def begin_method(self, name, return_type, parameters, calling_convention, has_implementation):
        # type: (str, IrccType, List[Tuple[IrccType, str]], str, bool) -> bool
        if name.startswith('llvm.'):
            # intrinsic
            return False
        if calling_convention == 'spir_kernel_flat':
            self._out_file.write('__kernel\n')
        return IrccCDeclaration.begin_method(
            self, name, return_type, parameters, calling_convention, has_implementation
        )


class ClDefinition(IrccCDefinition):
    _VECTOR_TYPES = {
        'i8': 'char%(size)d',
        'i16': 'short%(size)d',
        'i32': 'int%(size)d',
        'i64': 'long%(size)d',
        'u8': 'uchar%(size)d',
        'u16': 'ushort%(size)d',
        'u32': 'uint%(size)d',
        'u64': 'ulong%(size)d',
        'float': 'float%(size)d',
    }

    def __init__(self, file):
        # type: (TextIO) -> None
        IrccCDefinition.__init__(self, file)
        self._address_spaces = {0: '__private', 1: '__global', 2: '__constant', 3: '__local'}

    def type_metadata(self):
        # type: () -> IrccType
        return IrccType(['', '', 'meta', ''])

    def make_address_space(self, type, address_space):
        # type: (IrccType, int) -> IrccType
        return IrccType(
            ['', self._address_spaces[address_space], ''], ['', self._address_spaces[address_space], ''], type
        )

    def begin_method(self, name, return_type, parameters, calling_convention, has_implementation):
        # type: (str, IrccType, List[Tuple[IrccType, str]], str, bool) -> bool
        if not has_implementation:
            if calling_convention == 'spir_kernel_flat':
                self._out_file.write(
                    '__kernel %s %s(%s)\n{\n}\n\n' % (
                        return_type.format(['', '', '', '']
                                           ), name, ', '.join('%s' % t.format(['', '', n, '']) for t, n in parameters)
                    )
                )
            return False
        if calling_convention == 'spir_kernel_flat':
            self._out_file.write('__kernel\n')
        return IrccCDefinition.begin_method(self, name, return_type, parameters, calling_convention, has_implementation)

    def instruction_vector_insert(self, element_type, vector, index, element):
        # type: (IrccType, IrccExpression, IrccExpression, IrccExpression) -> None
        self._out_file.write(
            '%s((%s*)%s)[%s] = %s;\n' % (self._indent, element_type.format([]), vector, index, element)
        )

    def instruction_vector_insert_constant(self, vector, index, element):
        # type: (IrccExpression, int, IrccExpression) -> None
        self._out_file.write('%s%s.s%x = %s;\n' % (self._indent, vector, index, element))

    def make_expression_vector_value(self, vector_type, values):
        # type: (IrccType, List[IrccExpression]) -> IrccExpression
        return IrccCExpressionCast(IrccClExpressionVectorValue(values), vector_type)

    def make_expression_vector_value_broadcast(self, vector_type, value):
        # type: (IrccType, IrccExpression) -> IrccExpression
        return IrccCExpressionCast(IrccClExpressionVectorValueBroadcast(value), vector_type)

    def make_expression_vector_extract(self, element_type, vector, index):
        # type: (IrccType, IrccExpression, IrccExpression) -> IrccExpression
        return IrccClExpressionVectorExtract(element_type, vector, index)

    def make_expression_vector_extract_constant(self, vector, index):
        # type: (IrccExpression, int) -> IrccExpression
        return IrccClExpressionVectorExtractConstant(vector, index)

    def make_expression_vector_shuffle(self, result, source_size, v1, v2, mask):
        # type: (IrccType, int, IrccExpression, IrccExpression, List[int]) -> IrccExpression
        return IrccCExpressionCast(IrccClExpressionVectorShuffle(source_size, v1, v2, mask), result)


if TYPE_CHECKING:
    from typing import List, TextIO, Tuple
    from ircc import IrccType, IrccExpression