from .ir_declaration import IrDeclaration
from .ir_value import IrValue
from be_typing import TYPE_CHECKING


class IrVariable(IrValue):
    def resolve(self, module):
        # type: (IrModule) -> IrVariable
        return self


class IrVariableDeclaration(IrDeclaration):
    def __init__(self, variable):
        # type: (IrVariable) -> None
        self._variable = variable

    def resolve(self, module):
        # type: (IrModule) -> IrDeclaration
        self._variable = self._variable.resolve(module)
        return self

    def write_declaration(self, declared_name):
        # type: (IrReference) -> None
        #print('variable - ', declared_name)
        pass


class IrVariableObject(IrVariable):
    def __init__(self, type, initital_value):
        # type: (IrType, Optional[IrValue]) -> None
        self._type = type
        self._initial_value = initital_value

    def resolve(self, module):
        # type: (IrModule) -> IrVariableObject
        self._type = self._type.resolve(module)
        self._initial_value = self._initial_value and self._initial_value.resolve(module) or None
        return self


if TYPE_CHECKING:
    from typing import List, Optional
    from .ir_type import IrType
    from .ir_value import IrValue
    from .ir_module import IrModule
    from .ir_reference import IrReference