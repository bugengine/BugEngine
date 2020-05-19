from .ir_object import IrObject
from be_typing import TYPE_CHECKING


class IrExpression(IrObject):
    def resolve(self, module):
        # type: (IrModule) -> IrExpression
        return self

    def __str__(self):
        # type: () -> str
        raise NotImplementedError


class IrExpressionConstant(IrExpression):
    def __init__(self, value):
        # type: (Union[int, str, bool, None]) -> None
        IrExpression.__init__(self)
        self._value = value

    def __str__(self):
        # type: () -> str
        return str(self._value)


class IrExpressionUndef(IrExpression):
    def __init__(self):
        # type: () -> None
        IrExpression.__init__(self)

    def __str__(self):
        # type: () -> str
        return 'undefined'


class IrExpressionArray(IrExpression):
    def __init__(self, values):
        # type: (List[IrValue]) -> None
        IrExpression.__init__(self)
        self._values = values

    def resolve(self, module):
        # type: (IrModule) -> IrExpression
        self._values = [v.resolve(module) for v in self._values]
        return self

    def __str__(self):
        # type: () -> str
        return '[%s]' % ', '.join(str(v) for v in self._values)


class IrExpressionAggregate(IrExpression):
    def __init__(self, values):
        # type: (List[IrValue]) -> None
        IrExpression.__init__(self)
        self._values = values

    def resolve(self, module):
        # type: (IrModule) -> IrExpression
        self._values = [v.resolve(module) for v in self._values]
        return self

    def __str__(self):
        # type: () -> str
        return '{%s}' % ', '.join(str(v) for v in self._values)


class IrExpressionReference(IrExpression):
    def __init__(self, reference):
        # type: (IrReference) -> None
        IrExpression.__init__(self)
        self._reference = reference
    
    def resolve(self, module):
        # type: (IrModule) -> IrExpression
        # TODO
        return self

    def __str__(self):
        # type: () -> str
        return self._reference


class IrExpressionCast(IrExpression):
    def __init__(self, result_type, value, cast_type):
        # type: (IrType, IrValue, str) -> None
        IrExpression.__init__(self)
        self._result_type = result_type
        self._value = value
        self._cast_type = cast_type

    def __str__(self):
        # type: () -> str
        return '%s_cast<%s>(%s)' % (self._cast_type, self._result_type, self._value)


if TYPE_CHECKING:
    from typing import List, Union
    from .ir_type import IrType
    from .ir_module import IrModule
    from .ir_value import IrValue
    from .ir_reference import IrReference