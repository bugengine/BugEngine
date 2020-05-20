from .ir_object import IrObject
from .ir_declaration import IrDeclaration
from be_typing import TYPE_CHECKING


class IrMethodParameter:
    def __init__(self, type, name, attributes):
        # type: (IrType, IrReference, List[str]) -> None
        self._type = type
        self._id = name
        self._name = None


class IrMethodMetadataParameter(IrMethodParameter):
    pass


class IrMethod(IrObject):
    def resolve(self, module):
        # type: (IrModule) -> IrMethod
        return self


class IrMethodDeclaration(IrDeclaration):
    def __init__(self, method):
        # type: (IrMethod) -> None
        self._method = method

    def resolve(self, module):
        # type: (IrModule) -> IrDeclaration
        self._method = self._method.resolve(module)
        return self


class IrMethodLink(IrMethod):
    def __init__(self, reference):
        # type: (IrReference) -> None
        self._reference = reference

    def resolve(self, module):
        # type: (IrModule) -> IrMethod
        result = module.get(self._reference, IrMethodDeclaration)
        assert isinstance(result, IrMethod)
        result.resolve(module)
        return result


class IrMethodObject(IrMethod):
    def __init__(self, return_type, parameters, calling_convention):
        # type: (Optional[IrType], List[IrMethodParameter], str) -> None
        self._parameters = parameters
        self._calling_convention = calling_convention
        self._definition = None    # type: Optional[IrMethodBody]

    def define(self, instruction_list):
        # type: (List[IrInstruction]) -> None
        self._definition = IrMethodBody(instruction_list)

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