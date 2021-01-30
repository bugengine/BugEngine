from .ircc_c_types import IrccCTypes
from ..ir_codegen import IrccExpression
from be_typing import TYPE_CHECKING


class IrccCExpression(IrccExpression):
    _PREC_CONSTANT = 0
    _PREC_VARIABLE = 0
    _PREC_CAST = 1
    _PREC_ACCESS = 1
    _PREC_SUBSCRIPT = 1
    _PREC_CALL = 1
    _PREC_ADDRESSOF = 2
    _PREC_DEREFERENCE = 2
    _PREC_MUL = 3
    _PREC_DIV = 3
    _PREC_REM = 3
    _PREC_ADD = 4
    _PREC_SUB = 4
    _PREC_SHL = 5
    _PREC_SHR = 5
    _PREC_GT = 6
    _PREC_GE = 6
    _PREC_LT = 6
    _PREC_LE = 6
    _PREC_EQ = 7
    _PREC_NEQ = 7
    _PREC_AND = 8
    _PREC_XOR = 9
    _PREC_OR = 10

    def __init__(self, precedence):
        # type: (int) -> None
        self._precedence = precedence

    def make_string_left(self, associativity, precedence):
        # type: (Literal['left', 'right'], int) -> str
        if self._precedence > precedence:
            return '(%s)' % str(self)
        elif self._precedence == precedence and associativity == 'right':
            return '(%s)' % str(self)
        else:
            return str(self)

    def make_string_right(self, associativity, precedence):
        # type: (Literal['left', 'right'], int) -> str
        if self._precedence > precedence:
            return '(%s)' % str(self)
        elif self._precedence == precedence and associativity == 'left':
            return '(%s)' % str(self)
        else:
            return str(self)


class IrccCExpressionVoid(IrccCExpression):
    def __init__(self):
        # type: () -> None
        IrccCExpression.__init__(self, IrccCExpression._PREC_CONSTANT)

    def __str__(self):
        # type: () -> str
        return ''


class IrccCExpressionIntegerConstant(IrccCExpression):
    def __init__(self, value):
        # type: (str) -> None
        IrccCExpression.__init__(self, IrccCExpression._PREC_CONSTANT)
        self._value = value

    def __str__(self):
        # type: () -> str
        return self._value


class IrccCExpressionVariable(IrccCExpression):
    def __init__(self, value):
        # type: (str) -> None
        IrccCExpression.__init__(self, IrccCExpression._PREC_VARIABLE)
        self._value = value

    def __str__(self):
        # type: () -> str
        return self._value


class IrccCExpressionAggregateValue(IrccCExpression):
    def __init__(self, values):
        # type: (List[IrccExpression]) -> None
        IrccCExpression.__init__(self, IrccCExpression._PREC_CONSTANT)
        self._values = values

    def __str__(self):
        return '{ %s }' % ', '.join([str(x) for x in self._values])


IrccCExpressionArrayValue = IrccCExpressionAggregateValue


class IrccCExpressionCast(IrccCExpression):
    def __init__(self, value, target_type, unsigned=False):
        # type: (IrccExpression, IrccType, bool) -> None
        IrccCExpression.__init__(self, IrccCExpression._PREC_CAST)
        self._value = value
        self._target_type = target_type
        self._unsigned = unsigned

    def __str__(self):
        # type: () -> str
        target_type = self._target_type.format_unsigned([]) if self._unsigned else self._target_type.format([])
        value = self._value
        while isinstance(value, IrccCExpressionCast):
            value = value._value
        return '(%s)%s' % (target_type, value.make_string_right('right', self._precedence))


class IrccCExpressionAddressOf(IrccCExpression):
    def __init__(self, value):
        # type: (IrccExpression) -> None
        IrccCExpression.__init__(self, IrccCExpression._PREC_ADDRESSOF)
        self._value = value

    def __str__(self):
        # type: () -> str
        return '&%s' % (self._value.make_string_right('right', self._precedence))


class IrccCExpressionDereference(IrccCExpression):
    def __init__(self, value):
        # type: (IrccExpression) -> None
        IrccCExpression.__init__(self, IrccCExpression._PREC_DEREFERENCE)
        self._value = value

    def __str__(self):
        # type: () -> str
        return '*%s' % (self._value.make_string_right('right', self._precedence))


class IrccCExpressionAccess(IrccCExpression):
    def __init__(self, value, field):
        # type: (IrccExpression, str) -> None
        IrccCExpression.__init__(self, IrccCExpression._PREC_ACCESS)
        self._value = value
        self._field = field

    def __str__(self):
        # type: () -> str
        return '%s.%s' % (self._value.make_string_left('left', self._precedence), self._field)


class IrccCExpressionSubscript(IrccCExpression):
    def __init__(self, value, index):
        # type: (IrccExpression, IrccExpression) -> None
        IrccCExpression.__init__(self, IrccCExpression._PREC_SUBSCRIPT)
        self._value = value
        self._index = index

    def __str__(self):
        # type: () -> str
        return '%s[%s]' % (self._value.make_string_left('left', self._precedence), self._index)


class IrccCExpressionCall(IrccCExpression):
    def __init__(self, method, arguments):
        # type: (str, List[IrccExpression]) -> None
        IrccCExpression.__init__(self, IrccCExpression._PREC_CALL)
        self._method = method
        self._arguments = arguments

    def __str__(self):
        # type: () -> str
        return '%s(%s)' % (self._method, ', '.join([str(a) for a in self._arguments]))


class IrccCExpressionIntegerBinaryOperation(IrccCExpression):
    _OPERATORS = {
        'add': ('%(left)s + %(right)s', IrccCExpression._PREC_ADD, False),
        'sub': ('%(left)s - %(right)s', IrccCExpression._PREC_SUB, False),
        'mul': ('%(left)s * %(right)s', IrccCExpression._PREC_MUL, False),
        'udiv': ('%(left)s / %(right)s', IrccCExpression._PREC_DIV, True),
        'sdiv': ('%(left)s / %(right)s', IrccCExpression._PREC_DIV, False),
        'urem': ('%(left)s %% %(right)s', IrccCExpression._PREC_REM, True),
        'srem': ('%(left)s %% %(right)s', IrccCExpression._PREC_REM, False),
        'shl': ('%(left)s << %(right)s', IrccCExpression._PREC_SHL, False),
        'lshr': ('%(left)s >> %(right)s', IrccCExpression._PREC_SHR, True),
        'ashr': ('%(left)s >> %(right)s', IrccCExpression._PREC_SHR, False),
        'and': ('%(left)s & %(right)s', IrccCExpression._PREC_AND, False),
        'or': ('%(left)s | %(right)s', IrccCExpression._PREC_OR, False),
        'xor': ('%(left)s ^ %(right)s', IrccCExpression._PREC_XOR, False),
        'eq': ('%(left)s == %(right)s', IrccCExpression._PREC_EQ, False),
        'ne': ('%(left)s != %(right)s', IrccCExpression._PREC_NEQ, False),
        'ugt': ('%(left)s > %(right)s', IrccCExpression._PREC_GT, True),
        'uge': ('%(left)s >= %(right)s', IrccCExpression._PREC_GE, True),
        'ult': ('%(left)s < %(right)s', IrccCExpression._PREC_LT, True),
        'ule': ('%(left)s <= %(right)s', IrccCExpression._PREC_LE, True),
        'sgt': ('%(left)s > %(right)s', IrccCExpression._PREC_GT, False),
        'sge': ('%(left)s >= %(right)s', IrccCExpression._PREC_GE, False),
        'slt': ('%(left)s < %(right)s', IrccCExpression._PREC_LT, False),
        'sle': ('%(left)s <= %(right)s', IrccCExpression._PREC_LE, False),
    }

    def __init__(self, operation, type, left_operand, right_operand):
        # type: (str, IrccType, IrccExpression, IrccExpression) -> None
        operator, precedence, unsigned = IrccCExpressionIntegerBinaryOperation._OPERATORS[operation]
        IrccCExpression.__init__(self, precedence)
        if unsigned:
            self._left_operand = IrccCExpressionCast(left_operand, type, unsigned=True)   # type: IrccExpression
            self._right_operand = IrccCExpressionCast(right_operand, type, unsigned=True) # type: IrccExpression
        else:
            self._left_operand = left_operand
            self._right_operand = right_operand
        self._operator = operator

    def __str__(self):
        # type: () -> str
        return self._operator % {
            'left': self._left_operand.make_string_left('left', self._precedence),
            'right': self._right_operand.make_string_right('left', self._precedence)
        }


class IrccCExpressionFloatBinaryOperation(IrccCExpression):
    _OPERATORS = {
        'fadd': ('%(left)s + %(right)s', IrccCExpression._PREC_ADD),
        'fsub': ('%(left)s - %(right)s', IrccCExpression._PREC_SUB),
        'fmul': ('%(left)s * %(right)s', IrccCExpression._PREC_MUL),
        'fdiv': ('%(left)s / %(right)s', IrccCExpression._PREC_DIV),
        'frem': ('fmod(%(left)s, %(right)s)', IrccCExpression._PREC_REM),
        'oeq': ('%(left)s == %(right)s', IrccCExpression._PREC_EQ),
        'ueq': ('%(left)s == %(right)s', IrccCExpression._PREC_EQ),
        'one': ('%(left)s != %(right)s', IrccCExpression._PREC_NEQ),
        'une': ('%(left)s != %(right)s', IrccCExpression._PREC_NEQ),
        'ogt': ('%(left)s > %(right)s', IrccCExpression._PREC_GT),
        'ugt': ('%(left)s > %(right)s', IrccCExpression._PREC_GT),
        'oge': ('%(left)s >= %(right)s', IrccCExpression._PREC_GE),
        'uge': ('%(left)s >= %(right)s', IrccCExpression._PREC_GE),
        'olt': ('%(left)s < %(right)s', IrccCExpression._PREC_LT),
        'ult': ('%(left)s < %(right)s', IrccCExpression._PREC_LT),
        'ole': ('%(left)s <= %(right)s', IrccCExpression._PREC_LE),
        'ule': ('%(left)s <= %(right)s', IrccCExpression._PREC_LE),
    }

    def __init__(self, operation, type, left_operand, right_operand):
        # type: (str, IrccType, IrccExpression, IrccExpression) -> None
        operator, precedence = IrccCExpressionFloatBinaryOperation._OPERATORS[operation]
        IrccCExpression.__init__(self, precedence)
        self._left_operand = left_operand
        self._right_operand = right_operand
        self._operator = operator

    def __str__(self):
        # type: () -> str
        return self._operator % {
            'left': self._left_operand.make_string_left('left', self._precedence),
            'right': self._right_operand.make_string_right('left', self._precedence)
        }


class IrccCExpressions(IrccCTypes):
    def __init__(self, file):
        # type: (TextIO) -> None
        IrccCTypes.__init__(self, file)

    def make_expression_constant_void(self):
        # type: () -> IrccExpression
        return IrccCExpressionVoid()

    def make_expression_constant_zero(self):
        # type: () -> IrccExpression
        return IrccCExpressionIntegerConstant('0')

    def make_expression_constant_nullptr(self, type):
        # type: (IrccType) -> IrccExpression
        return IrccCExpressionIntegerConstant('0')

    def make_expression_constant_undef(self, width):
        # type: (int) -> IrccExpression
        return IrccCExpressionIntegerConstant('0x%s' % ('cd' * width))

    def make_expression_constant_bool(self, type, value):
        # type: (IrccType, bool) -> IrccExpression
        if value:
            return IrccCExpressionIntegerConstant('1')
        else:
            return IrccCExpressionIntegerConstant('0')

    def make_expression_constant_int(self, type, value):
        # type: (IrccType, int) -> IrccExpression
        return IrccCExpressionCast(IrccCExpressionIntegerConstant(str(value)), type)

    def make_expression_aggregate_value(self, values):
        # type: (List[IrccExpression]) -> IrccExpression
        return IrccCExpressionAggregateValue(values)

    def make_expression_array_value(self, values):
        # type: (List[IrccExpression]) -> IrccExpression
        return IrccCExpressionArrayValue(values)

    def make_expression_cast(self, result_type, cast, value):
        # type: (IrccType, str, IrccExpression) -> IrccExpression
        if cast != 'addrspacecast':
            return IrccCExpressionCast(value, result_type)
        else:
            return value

    def make_expression_address(self, value):
        # type: (IrccExpression) -> IrccExpression
        if isinstance(value, IrccCExpressionDereference):
            return value._value
        else:
            return IrccCExpressionAddressOf(value)

    def make_expression_access(self, value, field_name):
        # type: (IrccExpression, str) -> IrccExpression
        return IrccCExpressionAccess(value, field_name)

    def make_expression_subscript(self, value, index):
        # type: (IrccExpression, IrccExpression) -> IrccExpression
        return IrccCExpressionSubscript(value, index)

    def make_expression_variable_reference(self, name):
        # type: (str) -> IrccExpression
        return IrccCExpressionVariable(name)

    def make_expression_dereference(self, expression):
        # type: (IrccExpression) -> IrccExpression
        if isinstance(expression, IrccCExpressionAddressOf):
            return expression._value
        else:
            return IrccCExpressionDereference(expression)

    def make_expression_integer_binary_op(self, operation, type, left_operand, right_operand):
        # type: (str, IrccType, IrccExpression, IrccExpression) -> IrccExpression
        return IrccCExpressionIntegerBinaryOperation(operation, type, left_operand, right_operand)

    def make_expression_float_binary_op(self, operation, type, left_operand, right_operand):
        # type: (str, IrccType, IrccExpression, IrccExpression) -> IrccExpression
        return IrccCExpressionFloatBinaryOperation(operation, type, left_operand, right_operand)

    def make_value_load(self, address):
        # type: (IrccExpression) -> IrccExpression
        if isinstance(address, IrccCExpressionAddressOf):
            return address._value
        else:
            return IrccCExpressionDereference(address)

    def make_value_call(self, method_name, arguments):
        # type: (str, List[IrccExpression]) -> IrccExpression
        return IrccCExpressionCall(method_name, arguments)


if TYPE_CHECKING:
    from typing import List, Literal, TextIO
    from ..ir_codegen import IrccType