from .ir_object import IrObject
from .ir_declaration import IrDeclaration
from be_typing import TYPE_CHECKING


class IrType(IrObject):
    def resolve(self, module):
        # type: (IrModule) -> IrType
        return self

    def __str__(self):
        # type: () -> str
        raise NotImplementedError


class IrTypeDeclaration(IrDeclaration):
    def __init__(self, type):
        # type: (IrType) -> None
        self._type = type

    def resolve(self, module):
        # type: (IrModule) -> IrDeclaration
        self._type = self._type.resolve(module)
        return self

    def write_declaration(self, declared_name):
        # type: (IrReference) -> None
        pass


class IrTypeReference(IrType):
    def __init__(self, reference):
        # type: (IrReference) -> None
        self._reference = reference

    def resolve(self, module):
        # type: (IrModule) -> IrType
        return module.get(self._reference, IrTypeDeclaration)._type


class IrTypeOpaque(IrType):
    def __str__(self):
        # type: () -> str
        return 'opaque'


class IrTypeMetadata(IrType):
    pass


class IrTypeBuiltin(IrType):
    def __init__(self, builtin):
        # type: (str) -> None
        self._builtin = builtin

    def __str__(self):
        # type: () -> str
        return self._builtin


class IrTypePtr(IrType):
    def __init__(self, pointee, addrspace):
        # type: (IrType, int) -> None
        self._pointee = pointee
        self._address_space = addrspace

    def resolve(self, module):
        # type: (IrModule) -> IrType
        self._pointee = self._pointee.resolve(module)
        return self

    def __str__(self):
        # type: () -> str
        addrspaces = ['private', 'constant', 'local', 'global', 'generic']
        return '%s %s*' % (self._pointee, addrspaces[self._address_space])


class IrTypeArray(IrType):
    def __init__(self, type, count):
        # type: (IrType, int) -> None
        self._type = type
        self._count = count

    def resolve(self, module):
        # type: (IrModule) -> IrType
        self._type = self._type.resolve(module)
        return self

    def __str__(self):
        # type: () -> str
        return '%s[%d]' % (self._type, self._count)


class IrTypeVector(IrType):
    def __init__(self, type, count):
        # type: (IrType, int) -> None
        self._type = type
        self._count = count

    def resolve(self, module):
        # type: (IrModule) -> IrType
        self._type = self._type.resolve(module)
        return self

    def __str__(self):
        # type: () -> str
        return '%s%d' % (self._type, self._count)


class IrTypeStruct(IrType):
    def __init__(self, fields, packed):
        # type: (List[IrType], bool) -> None
        self._fields = fields
        self._packed = packed

    def resolve(self, module):
        # type: (IrModule) -> IrType
        self._fields = [f.resolve(module) for f in self._fields]
        return self

    def __str__(self):
        # type: () -> str
        return '{%s}' % (', '.join(str(x) for x in self._fields))


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

    def __str__(self):
        # type: () -> str
        return '%s(*)(%s)' % (self._return_type, ', '.join(str(x) for x in self._argument_types))


if TYPE_CHECKING:
    from typing import List, Optional
    from .ir_module import IrModule
    from .ir_reference import IrReference