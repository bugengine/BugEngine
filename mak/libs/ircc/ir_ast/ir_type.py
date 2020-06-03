from .ir_object import IrObject
from .ir_declaration import IrDeclaration
from be_typing import TYPE_CHECKING
from abc import abstractmethod


class IrType(IrObject):
    def resolve(self, module):
        # type: (IrModule) -> IrType
        return self

    def __str__(self):
        # type: () -> str
        raise NotImplementedError

    def _dependency_list(self):
        # type: () -> List[Tuple[str, IrDeclaration]]
        return []

    @abstractmethod
    def create_generator_type(self, generator):
        # type: (IrCodeGenerator) -> str
        raise NotImplementedError


class IrTypeDeclaration(IrDeclaration):
    TYPE_INDEX = 0
    def __init__(self, type):
        # type: (IrType) -> None
        IrDeclaration.__init__(self)
        self._type = type
        assert self._type._name is None
        self._type._name = 'type_%d' % IrTypeDeclaration.TYPE_INDEX
        IrTypeDeclaration.TYPE_INDEX += 1

    def resolve(self, module):
        # type: (IrModule) -> IrDeclaration
        self._type = self._type.resolve(module)
        return self

    def collect(self, ir_name):
        # type: (str) -> List[Tuple[str, IrDeclaration]]
        result = self._type._dependency_list()
        result.append((ir_name, self))
        return result

    def visit(self, generator, ir_name):
        # type: (IrCodeGenerator, str) -> None
        assert self._type is not None
        assert self._type._name is not None
        generator.declare_type(self._type.create_generator_type(generator), self._type._name, ir_name)


class IrTypeReference(IrType):
    def __init__(self, reference):
        # type: (IrReference) -> None
        IrType.__init__(self)
        self._reference = reference
        self._declaration = None # type: Optional[IrTypeDeclaration]
        self._target = None # type: Optional[IrType]

    def resolve(self, module):
        # type: (IrModule) -> IrType
        self._declaration = module.get(self._reference, IrTypeDeclaration)
        self._target = self._declaration._type
        return self

    def _dependency_list(self):
        # type: () -> List[Tuple[str, IrDeclaration]]
        assert self._declaration is not None
        return self._declaration.collect(self._reference)

    def create_generator_type(self, generator):
        # type: (IrCodeGenerator) -> str
        assert self._target is not None
        assert self._target._name is not None
        return generator.type_declared(self._target._name)


class IrTypeOpaque(IrType):
    def __str__(self):
        # type: () -> str
        return 'opaque'

    def create_generator_type(self, generator):
        # type: (IrCodeGenerator) -> str
        return generator.type_void()


class IrTypeMetadata(IrType):
    def create_generator_type(self, generator):
        # type: (IrCodeGenerator) -> str
        return 'metadata'


class IrTypeBuiltin(IrType):
    def __init__(self, builtin):
        # type: (str) -> None
        IrType.__init__(self)
        self._builtin = builtin

    def __str__(self):
        # type: () -> str
        return self._builtin

    def create_generator_type(self, generator):
        # type: (IrCodeGenerator) -> str
        return generator.type_builtin(self._builtin)


class IrTypePtr(IrType):
    def __init__(self, pointee, addrspace):
        # type: (IrType, int) -> None
        IrType.__init__(self)
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

    def create_generator_type(self, generator):
        # type: (IrCodeGenerator) -> str
        return generator.make_ptr(generator.make_address_space(self._pointee.create_generator_type(generator), self._address_space))


class IrTypeArray(IrType):
    def __init__(self, type, count):
        # type: (IrType, int) -> None
        IrType.__init__(self)
        self._type = type
        self._count = count

    def resolve(self, module):
        # type: (IrModule) -> IrType
        self._type = self._type.resolve(module)
        return self

    def _dependency_list(self):
        # type: () -> List[Tuple[str, IrDeclaration]]
        return self._type._dependency_list()

    def __str__(self):
        # type: () -> str
        return '%s[%d]' % (self._type, self._count)

    def create_generator_type(self, generator):
        # type: (IrCodeGenerator) -> str
        return generator.make_array(self._type.create_generator_type(generator), self._count)


class IrTypeVector(IrType):
    def __init__(self, type, count):
        # type: (IrType, int) -> None
        IrType.__init__(self)
        self._type = type
        self._count = count

    def resolve(self, module):
        # type: (IrModule) -> IrType
        self._type = self._type.resolve(module)
        return self

    def _dependency_list(self):
        # type: () -> List[Tuple[str, IrDeclaration]]
        return self._type._dependency_list()

    def __str__(self):
        # type: () -> str
        return '%s%d' % (self._type, self._count)


class IrTypeStruct(IrType):
    def __init__(self, fields, packed):
        # type: (List[IrType], bool) -> None
        IrType.__init__(self)
        self._fields = [(f, 'field_%d'%i) for i, f in enumerate(fields)]
        self._packed = packed

    def _dependency_list(self):
        # type: () -> List[Tuple[str, IrDeclaration]]
        result = []
        for t, _ in self._fields:
            result += t._dependency_list()
        return result

    def resolve(self, module):
        # type: (IrModule) -> IrType
        self._fields = [(f.resolve(module), n) for f, n in self._fields]
        return self

    def __str__(self):
        # type: () -> str
        return '{%s}' % (', '.join(str(x) for x,_ in self._fields))

    def create_generator_type(self, generator):
        # type: (IrCodeGenerator) -> str
        return generator.make_struct([(f.create_generator_type(generator), n) for f, n in self._fields])


class IrTypeMethod(IrType):
    def __init__(self, return_type, argument_types):
        # type: (Optional[IrType], List[IrType]) -> None
        IrType.__init__(self)
        self._return_type = return_type
        self._argument_types = argument_types

    def _dependency_list(self):
        # type: () -> List[Tuple[str, IrDeclaration]]
        result = self._return_type and self._return_type._dependency_list() or []
        for t in self._argument_types:
            result += t._dependency_list()
        return result

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
    from typing import List, Optional, Tuple
    from .ir_module import IrModule
    from .ir_reference import IrReference
    from ..ir_codegen import IrCodeGenerator