from .ir_declaration import IrDeclaration
from .ir_value import IrValue
from be_typing import TYPE_CHECKING, cast


class IrVariable(IrValue):
    def __init__(self, type, initital_value):
        # type: (IrType, Optional[IrValue]) -> None
        IrValue.__init__(self, type)
        self._initial_value = initital_value

    def resolve(self, module):
        # type: (IrModule) -> IrVariable
        if self._initial_value is not None:
            self._initial_value = self._initial_value.resolve(module)
        return cast('IrVariable', IrValue.resolve(self, module))


class IrVariableDeclaration(IrDeclaration):
    def __init__(self, variable):
        # type: (IrVariable) -> None
        self._variable = variable

    def resolve(self, module):
        # type: (IrModule) -> IrDeclaration
        self._variable = self._variable.resolve(module)
        return self


if TYPE_CHECKING:
    from typing import List, Optional
    from .ir_type import IrType
    from .ir_value import IrValue
    from .ir_module import IrModule
    from .ir_reference import IrReference