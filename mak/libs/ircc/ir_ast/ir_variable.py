from .ir_declaration import IrDeclaration
from .ir_expr import IrExpression
from .ir_type import IrAddressSpace, IrTypePtr
from be_typing import TYPE_CHECKING, cast


class IrVariable(IrExpression):
    def __init__(self, type, initital_value, address_space):
        # type: (IrType, Optional[IrValue], int) -> None
        IrExpression.__init__(self)
        self._type = type
        self._initial_value = initital_value
        self._address_space = IrAddressSpace(address_space)

    def resolve(self, module):
        # type: (IrModule) -> IrVariable
        self._type.resolve(module)
        if self._initial_value is not None:
            self._initial_value = self._initial_value.resolve(module)
        return cast('IrVariable', IrExpression.resolve(self, module))

    def get_type(self):
        # type: () -> Tuple[IrType, IrAddressSpace]
        return IrTypePtr(self._type, int(self._address_space)), IrAddressSpace(0)


if TYPE_CHECKING:
    from typing import List, Optional, Tuple
    from .ir_type import IrType
    from .ir_value import IrValue
    from .ir_module import IrModule
    from .ir_reference import IrReference