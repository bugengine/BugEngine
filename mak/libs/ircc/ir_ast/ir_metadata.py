from .ir_declaration import IrDeclaration
from .ir_value import IrValue
from .ir_type import IrTypeMetadata
from be_typing import TYPE_CHECKING


class IrMetadata(IrValue):
    def __init__(self):
        # type: () -> None
        IrValue.__init__(self, IrTypeMetadata())

    def resolve(self, module):
        # type: (IrModule) -> IrMetadata
        return self

    def __str__(self):
        # type: () -> str
        raise NotImplementedError


class IrMetadataDeclaration(IrDeclaration):
    def __init__(self, metadata_value):
        # type: (IrMetadata) -> None
        self._metadata = metadata_value

    def resolve(self, module):
        # type: (IrModule) -> IrMetadataDeclaration
        self._metadata = self._metadata.resolve(module)
        return self


class IrMetadataLink(IrMetadata):
    def __init__(self, reference):
        # type: (IrReference) -> None
        self._reference = reference

    def resolve(self, module):
        # type: (IrModule) -> IrMetadata
        decl = module.get(self._reference, IrMetadataDeclaration)
        return decl._metadata


class IrMetadataValue(IrMetadata):
    pass


class IrMetadataString(IrMetadataValue):
    def __init__(self, value):
        # type: (str) -> None
        self._value = value

    def __str__(self):
        # type: () -> str
        return '!"%s"' % (self._value)


class IrMetadataInteger(IrMetadataValue):
    def __init__(self, value):
        # type: (int) -> None
        self._value = value

    def __str__(self):
        # type: () -> str
        return '!%d' % (self._value)


class IrMetadataNode(IrMetadataValue):
    def __init__(self, values):
        # type: (List[IrValue]) -> None
        self._values = values

    def resolve(self, module):
        # type: (IrModule) -> IrMetadata
        self._values = [v.resolve(module) for v in self._values]
        return self

    def __str__(self):
        # type: () -> str
        return '!{%s}' % (', '.join(str(v) for v in self._values))


class IrSpecializedMetadata(IrMetadataValue):
    def __init__(self, specialized_class, properties):
        # type: (str, List[Tuple[str, IrMetadata]]) -> None
        self._specialized_class = specialized_class
        self._values = {}
        for property_name, property_value in properties:
            self._values[property_name] = property_value

    def resolve(self, module):
        # type: (IrModule) -> IrMetadata
        v = {}
        for property_name, property_value in self._values.items():
            v[property_name] = property_value.resolve(module)
        self._values = v
        return self

    def __str__(self):
        # type: () -> str
        return '!%s(%s)' % (self._specialized_class, ', '.join('%s: %s' % (k, str(v)) for k, v in self._values.items()))


class IrMetadataNull(IrMetadata):
    def __str__(self):
        # type: () -> str
        return '!null'


class IrMetadataFlagList(IrMetadata):
    def __init__(self, flag):
        # type: (str) -> None
        self._flags = [flag]

    def add_flag(self, flag):
        # type: (str) -> None
        self._flags.append(flag)

    def __str__(self):
        # type: () -> str
        return ' | '.join(self._flags)


if TYPE_CHECKING:
    from typing import List, Tuple
    from .ir_module import IrModule
    from .ir_reference import IrReference