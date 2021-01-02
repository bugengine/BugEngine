from .ir_object import IrObject
from .ir_type import IrTypeBuiltin, IrTypeUndef, IrTypeZero
from .ir_declaration import IrDeclaration
from be_typing import TYPE_CHECKING


class IrExpression(IrObject):
    def __init__(self):
        # type: () -> None
        IrObject.__init__(self)

    def resolve(self, module):
        # type: (IrModule) -> IrExpression
        return self

    def get_type(self):
        # type: () -> IrType
        raise NotImplementedError

    def __str__(self):
        # type: () -> str
        raise NotImplementedError

    def is_typed(self):
        # type: () -> bool
        return True


class IrExpressionDeclaration(IrDeclaration):
    def __init__(self, expression):
        # type: (IrExpression) -> None
        IrDeclaration.__init__(self)
        self._expression = expression

    def resolve(self, module):
        # type: (IrModule) -> IrExpressionDeclaration
        self._expression.resolve(module)
        return self


class IrExpressionZero(IrExpression):
    def __init__(self):
        # type: () -> None
        IrExpression.__init__(self)

    def get_type(self):
        # type: () -> IrType
        return IrTypeZero()

    def __str__(self):
        # type: () -> str
        return 'zero'

    def is_typed(self):
        # type: () -> bool
        return False


class IrExpressionConstant(IrExpression):
    def __init__(self, value):
        # type: (Union[int, str, bool, None]) -> None
        IrExpression.__init__(self)
        self._value = value

    def get_type(self):
        # type: () -> IrType
        if isinstance(self._value, int):
            return IrTypeBuiltin('i64')
        if isinstance(self._value, bool):
            return IrTypeBuiltin('i1')
        if isinstance(self._value, str):
            raise NotImplementedError
        raise NotImplementedError

    def __str__(self):
        # type: () -> str
        return str(self._value)

    def is_typed(self):
        # type: () -> bool
        return False


class IrExpressionUndef(IrExpression):
    def __init__(self):
        # type: () -> None
        IrExpression.__init__(self)

    def get_type(self):
        # type: () -> IrType
        return IrTypeUndef()

    def __str__(self):
        # type: () -> str
        return 'undefined'

    def is_typed(self):
        # type: () -> bool
        return False


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
        self._expression = None    # type: Optional[IrExpression]

    def resolve(self, module):
        # type: (IrModule) -> IrExpression
        declaration = module.get(self._reference, IrExpressionDeclaration)
        #declaration = declaration.resolve(module)
        self._expression = declaration._expression
        return self

    def get_type(self):
        # type: () -> IrType
        assert self._expression is not None
        return self._expression.get_type()

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

    def get_type(self):
        # type: () -> IrType
        assert self._result_type.is_defined()
        return self._result_type

    def __str__(self):
        # type: () -> str
        return '%s_cast<%s>(%s)' % (self._cast_type, self._result_type, self._value)


if TYPE_CHECKING:
    from typing import Dict, List, Optional, Union
    from .ir_type import IrType
    from .ir_module import IrModule
    from .ir_value import IrValue
    from .ir_reference import IrReference