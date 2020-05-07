from .ir_declaration import IrDeclaration
from be_typing import TYPE_CHECKING


class IrType:
    def resolve(self, module):
        # type: (IrModule) -> IrType
        return self


class IrTypeDeclaration(IrDeclaration):
    def __init__(self, type):
        # type: (IrType) -> None
        self._type = type

    def resolve(self, module):
        # type: (IrModule) -> IrDeclaration
        self._type = self._type.resolve(module)
        return self


class IrTypeReference(IrType):
    def __init__(self, reference):
        # type: (IrReference) -> None
        self._reference = reference

    def resolve(self, module):
        # type: (IrModule) -> IrType
        return module.get(self._reference, IrTypeDeclaration)._type


class IrTypeOpaque(IrType):
    pass


class IrTypeBuiltin(IrType):
    def __init__(self, builtin):
        # type: (str) -> None
        self._builtin = builtin


class IrTypePtr(IrType):
    def __init__(self, pointee, addrspace):
        # type: (IrType, int) -> None
        self._pointee = pointee
        self._address_space = addrspace

    def resolve(self, module):
        # type: (IrModule) -> IrType
        self._pointee = self._pointee.resolve(module)
        return self


class IrTypeArray(IrType):
    def __init__(self, type, count):
        # type: (IrType, int) -> None
        self._type = type
        self._count = count

    def resolve(self, module):
        # type: (IrModule) -> IrType
        self._type = self._type.resolve(module)
        return self


class IrTypeVector(IrType):
    def __init__(self, type, count):
        # type: (IrType, int) -> None
        self._type = type
        self._count = count

    def resolve(self, module):
        # type: (IrModule) -> IrType
        self._type = self._type.resolve(module)
        return self


class IrTypeStruct(IrType):
    def __init__(self, fields, packed):
        # type: (List[IrType], bool) -> None
        self._fields = fields
        self._packed = packed

    def resolve(self, module):
        # type: (IrModule) -> IrType
        self._fields = [f.resolve(module) for f in self._fields]
        return self


class IrTypeMethod(IrType):
    def __init__(self, return_type, argument_types):
        # type: (Optional[IrType], List[IrType]) -> None
        self._return_type = return_type
        self._argument_types = argument_types

    def resolve(self, module):
        # type: (IrModule) -> IrType
        if self._return_type is not None:
            self._return_type = self._return_type.resolve(module)
        self._argument_types = [t.resolve(module) for t in self._argument_types]
        return self


if TYPE_CHECKING:
    from typing import List, Optional
    from .ir_module import IrModule
    from .ir_reference import IrReference