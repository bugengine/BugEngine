from .ir_object import IrObject
from .ir_declaration import IrDeclaration
from ..ir_codegen import IrccType
from be_typing import TYPE_CHECKING
from abc import abstractmethod


class IrAddressSpace:
    # First unused generic address space
    GENERIC_ADDRESS_SPACE = 10

    def __init__(self, address_space):
        # type: (int) -> None
        if address_space == 4:
            address_space = IrAddressSpace.GENERIC_ADDRESS_SPACE
            IrAddressSpace.GENERIC_ADDRESS_SPACE += 1
        self._address_space = address_space

    def __str__(self):
        # type: () -> str
        addrspaces = ['private', 'constant', 'local', 'global']
        try:
            return addrspaces[self._address_space]
        except IndexError:
            return 'generic'

    def __int__(self):
        # type: () -> int
        assert self._address_space <= 3
        return self._address_space


class IrAddressSpaceInference:
    class Equivalence:
        def __init__(self, address_space, location):
            # type: (int, None) -> None
            self._address_space = address_space
            self._location = location

    def __init__(self):
        # type: () -> None
        self.address_spaces = {}   # type: Dict[int, List[IrAddressSpaceInference.Equivalence]]

    def add(self, address_space, equivalent):
        # type: (IrAddressSpace, IrAddressSpace) -> None
        eq = IrAddressSpaceInference.Equivalence(address_space._address_space, None)
        try:
            self.address_spaces[equivalent._address_space].append(eq)
        except KeyError:
            self.address_spaces[equivalent._address_space] = [eq]


class IrType(IrObject):
    def __init__(self):
        # type: () -> None
        IrObject.__init__(self)

    def resolve(self, module):
        # type: (IrModule) -> IrType
        return self

    def __str__(self):
        # type: () -> str
        raise NotImplementedError

    def signature(self):
        # type: () -> str
        raise NotImplementedError

    def _dependency_list(self):
        # type: () -> List[Tuple[str, IrDeclaration]]
        return []

    def flatten(self, allow_pointer=True):
        # type: (bool) -> List[IrType]
        return [self]

    def is_defined(self):
        # type: () -> bool
        return True

    def _get_target_type(self):
        # type: () -> IrType
        return self

    def extract(self, index):
        # type: (int) -> IrType
        raise NotImplementedError

    def uniquify(self):
        # type: () -> IrType
        return self

    @abstractmethod
    def create_generator_type(self, generator):
        # type: (IrccGenerator) -> IrccType
        raise NotImplementedError

    @abstractmethod
    def add_equivalence(self, equivalence, other_type, address_space):
        # type: (IrAddressSpaceInference, IrType, IrAddressSpace) -> None
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
        # type: (IrccGenerator, str) -> None
        assert self._type is not None
        assert self._type._name is not None
        generator.declare_type(self._type.create_generator_type(generator), self._type._name, ir_name)


class IrTypeReference(IrType):
    def __init__(self, reference):
        # type: (IrReference) -> None
        IrType.__init__(self)
        self._reference = reference
        self._declaration = None   # type: Optional[IrTypeDeclaration]
        self._target = None        # type: Optional[IrType]

    def resolve(self, module):
        # type: (IrModule) -> IrType
        self._declaration = module.get(self._reference, IrTypeDeclaration)
        self._target = self._declaration._type
        return self

    def extract(self, index):
        # type: (int) -> IrType
        assert self._target is not None
        return self._target.extract(index)

    def _get_target_type(self):
        # type: () -> IrType
        assert self._target is not None
        return self._target

    def _dependency_list(self):
        # type: () -> List[Tuple[str, IrDeclaration]]
        assert self._declaration is not None
        return self._declaration.collect(self._reference)

    def create_generator_type(self, generator):
        # type: (IrccGenerator) -> IrccType
        assert self._target is not None
        assert self._target._name is not None
        return generator.type_declared(self._target._name)

    def flatten(self, allow_pointer=True):
        # type: (bool) -> List[IrType]
        assert self._target is not None
        return self._target.flatten(allow_pointer)

    def __str__(self):
        # type: () -> str
        return str(self._reference)

    def signature(self):
        # type: () -> str
        assert self._target is not None
        return self._target.signature()

    def is_defined(self):
        # type: () -> bool
        assert self._target is not None
        return self._target.is_defined()

    def add_equivalence(self, equivalence, other_type, address_space):
        # type: (IrAddressSpaceInference, IrType, IrAddressSpace) -> None
        assert self._target is not None
        return self._target.add_equivalence(equivalence, other_type, address_space)


class IrTypeOpaque(IrType):
    def __str__(self):
        # type: () -> str
        return 'opaque'

    def signature(self):
        # type: () -> str
        return '?'

    def create_generator_type(self, generator):
        # type: (IrccGenerator) -> IrccType
        return generator.type_void()

    def add_equivalence(self, equivalence, other_type, address_space):
        # type: (IrAddressSpaceInference, IrType, IrAddressSpace) -> None
        assert isinstance(other_type._get_target_type(), IrTypeOpaque)


class IrTypeVoid(IrType):
    def __str__(self):
        # type: () -> str
        return 'void'

    def signature(self):
        # type: () -> str
        return '%'

    def create_generator_type(self, generator):
        # type: (IrccGenerator) -> IrccType
        return generator.type_void()

    def add_equivalence(self, equivalence, other_type, address_space):
        # type: (IrAddressSpaceInference, IrType, IrAddressSpace) -> None
        assert isinstance(other_type._get_target_type(), IrTypeVoid)


class IrTypeMetadata(IrType):
    def create_generator_type(self, generator):
        # type: (IrccGenerator) -> IrccType
        return generator.type_metadata()

    def signature(self):
        # type: () -> str
        return '~'

    def add_equivalence(self, equivalence, other_type, address_space):
        # type: (IrAddressSpaceInference, IrType, IrAddressSpace) -> None
        assert isinstance(other_type._get_target_type(), IrTypeMetadata)


class IrTypeBuiltin(IrType):
    def __init__(self, builtin):
        # type: (str) -> None
        IrType.__init__(self)
        self._builtin = builtin

    def __str__(self):
        # type: () -> str
        return self._builtin

    def signature(self):
        # type: () -> str
        return '#%s' % self._builtin

    def create_generator_type(self, generator):
        # type: (IrccGenerator) -> IrccType
        return generator.type_builtin(self._builtin)

    def add_equivalence(self, equivalence, other_type, address_space):
        # type: (IrAddressSpaceInference, IrType, IrAddressSpace) -> None
        other_type = other_type._get_target_type()
        assert isinstance(other_type, IrTypeBuiltin)
        assert self._builtin == other_type._builtin


class IrTypePtr(IrType):
    def __init__(self, pointee, addrspace):
        # type: (IrType, int) -> None
        IrType.__init__(self)
        self._pointee = pointee
        self._address_space = IrAddressSpace(addrspace)

    def resolve(self, module):
        # type: (IrModule) -> IrType
        self._pointee = self._pointee.resolve(module)
        return self

    def __str__(self):
        # type: () -> str
        return '%s %s*' % (self._pointee, self._address_space)

    def signature(self):
        # type: () -> str
        return '*[%s]%s' % (self._address_space, self._pointee.signature())

    def create_generator_type(self, generator):
        # type: (IrccGenerator) -> IrccType
        return generator.make_ptr(
            generator.make_address_space(self._pointee.create_generator_type(generator), int(self._address_space))
        )

    def flatten(self, allow_pointer=True):
        # type: (bool) -> List[IrType]
        #if not allow_pointer:
        #    # todo: source location
        #    raise Exception('invalid kernel parameter')
        return [self]

    def is_defined(self):
        # type: () -> bool
        return self._address_space != 4 and self._pointee.is_defined()

    def add_equivalence(self, equivalence, other_type, address_space):
        # type: (IrAddressSpaceInference, IrType, IrAddressSpace) -> None
        other_type = other_type._get_target_type()
        assert isinstance(other_type, IrTypePtr)
        equivalence.add(self._address_space, other_type._address_space)
        self._pointee.add_equivalence(equivalence, other_type._pointee, self._address_space)


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

    def signature(self):
        # type: () -> str
        return '[%d]%s' % (self._count, self._type.signature())

    def create_generator_type(self, generator):
        # type: (IrccGenerator) -> IrccType
        return generator.make_array(self._type.create_generator_type(generator), self._count)

    def flatten(self, allow_pointer=True):
        # type: (bool) -> List[IrType]
        #if not allow_pointer:
        #    # todo: source location
        #    raise Exception('invalid kernel parameter')
        return [self._type] * self._count

    def is_defined(self):
        # type: () -> bool
        return self._type.is_defined()

    def add_equivalence(self, equivalence, other_type, address_space):
        # type: (IrAddressSpaceInference, IrType, IrAddressSpace) -> None
        other_type = other_type._get_target_type()
        assert isinstance(other_type, IrTypeArray)
        assert self._count == other_type._count
        self._type.add_equivalence(equivalence, other_type._type, address_space)


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

    def signature(self):
        # type: () -> str
        return '<%d>%s' % (self._count, self._type.signature())

    def create_generator_type(self, generator):
        # type: (IrccGenerator) -> IrccType
        raise NotImplementedError

    def is_defined(self):
        # type: () -> bool
        return self._type.is_defined()

    def add_equivalence(self, equivalence, other_type, address_space):
        # type: (IrAddressSpaceInference, IrType, IrAddressSpace) -> None
        other_type = other_type._get_target_type()
        assert isinstance(other_type, IrTypeVector)
        assert self._count == other_type._count
        self._type.add_equivalence(equivalence, other_type._type, address_space)


class IrTypeStruct(IrType):
    def __init__(self, fields, packed):
        # type: (List[IrType], bool) -> None
        IrType.__init__(self)
        self._fields = [(f, 'field_%d' % i) for i, f in enumerate(fields)]
        self._packed = packed

    def _dependency_list(self):
        # type: () -> List[Tuple[str, IrDeclaration]]
        result = []    # type: List[Tuple[str, IrDeclaration]]
        for t, _ in self._fields:
            result += t._dependency_list()
        return result

    def resolve(self, module):
        # type: (IrModule) -> IrType
        self._fields = [(f.resolve(module), n) for f, n in self._fields]
        return self

    def __str__(self):
        # type: () -> str
        return '{%s}' % (', '.join(str(x) for x, _ in self._fields))

    def signature(self):
        # type: () -> str
        return '{%s}' % (';'.join(x.signature() for x, _ in self._fields))

    def create_generator_type(self, generator):
        # type: (IrccGenerator) -> IrccType
        return generator.make_struct([(f.create_generator_type(generator), n) for f, n in self._fields])

    def flatten(self, allow_pointer=True):
        # type: (bool) -> List[IrType]
        result = []    # type: List[IrType]
        for field in self._fields:
            result += field[0].flatten(False)
        return result

    def is_defined(self):
        # type: () -> bool
        result = True
        for t, _ in self._fields:
            result &= t.is_defined()
        return result

    def add_equivalence(self, equivalence, other_type, address_space):
        # type: (IrAddressSpaceInference, IrType, IrAddressSpace) -> None
        other_type = other_type._get_target_type()
        assert isinstance(other_type, IrTypeStruct)
        assert len(self._fields) == len(other_type._fields)
        for (t1, n1), (t2, n2) in zip(self._fields, other_type._fields):
            assert n1 == n2
            t1.add_equivalence(equivalence, t2, address_space)

    def extract(self, index):
        # type: (int) -> IrType
        assert index < len(self._fields)
        return self._fields[index][0]


class IrTypeMethod(IrType):
    def __init__(self, return_type, argument_types):
        # type: (IrType, List[IrType]) -> None
        IrType.__init__(self)
        self._return_type = return_type
        self._argument_types = argument_types

    def _dependency_list(self):
        # type: () -> List[Tuple[str, IrDeclaration]]
        result = self._return_type._dependency_list()
        for t in self._argument_types:
            result += t._dependency_list()
        return result

    def resolve(self, module):
        # type: (IrModule) -> IrType
        self._return_type = self._return_type.resolve(module)
        self._argument_types = [t.resolve(module) for t in self._argument_types]
        return self

    def __str__(self):
        # type: () -> str
        return '%s(*)(%s)' % (self._return_type, ', '.join(str(x) for x in self._argument_types))

    def signature(self):
        # type: () -> str
        return '(%s(%s))' % (self._return_type.signature(), ';'.join(x.signature() for x in self._argument_types))

    def create_generator_type(self, generator):
        # type: (IrccGenerator) -> IrccType
        raise NotImplementedError

    def is_defined(self):
        # type: () -> bool
        result = self._return_type.is_defined()
        for t in self._argument_types:
            result &= t.is_defined()
        return result

    def add_equivalence(self, equivalence, other_type, address_space):
        # type: (IrAddressSpaceInference, IrType, IrAddressSpace) -> None
        other_type = other_type._get_target_type()
        assert isinstance(other_type, IrTypeMethod)
        assert len(self._argument_types) == len(other_type._argument_types)
        self._return_type.add_equivalence(equivalence, other_type._return_type, address_space)
        for t1, t2 in zip(self._argument_types, other_type._argument_types):
            t1.add_equivalence(equivalence, t2, address_space)


if TYPE_CHECKING:
    from typing import Dict, List, Optional, Set, Tuple
    from .ir_module import IrModule
    from .ir_reference import IrReference
    from ..ir_codegen import IrccGenerator, IrccType