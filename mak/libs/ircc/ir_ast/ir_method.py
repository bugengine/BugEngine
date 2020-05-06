from .ir_declaration import IrDeclaration
from be_typing import TYPE_CHECKING


class IrParameter:
    def __init__(self, type, name):
        # type: (IrType, IrReference) -> None
        self._type = type
        self._id = name
        self._name = None


class IrMethodDeclaration(IrDeclaration):
    pass


class IrMethodLink(IrMethodDeclaration):
    def __init__(self, reference):
        # type: (IrReference) -> None
        self._reference = reference

    def resolve(self, module):
        # type: (IrModule) -> IrMethod
        result = module.get(self._reference)
        assert isinstance(result, IrMethod)
        result.resolve(module)
        return result


class IrMethod(IrMethodDeclaration):
    def __init__(self, parameters, calling_convention, definition):
        # type: (List[IrParameter], str, Optional[IrMethodBody]) -> None
        self._parameters = parameters
        self._calling_convention = calling_convention
        self._definition = definition

    def resolve(self, module):
        # type: (IrModule) -> IrMethod
        return self


class IrMethodBody:
    def __init__(self, instruction_list):
        # type: (List[IrInstruction]) -> None
        self._instruction_list = instruction_list


if TYPE_CHECKING:
    from typing import List, Optional
    from .ir_module import IrModule
    from .ir_reference import IrReference
    from .ir_instruction import IrInstruction
    from .ir_type import IrType