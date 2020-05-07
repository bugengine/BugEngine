from .ir_declaration import IrDeclaration
from .ir_value import IrValue
from .ir_type import IrType
from be_typing import TYPE_CHECKING


class IrTypeMetadata(IrType):
    pass


class IrMetadata(IrValue):
    def __init__(self):
        # type: () -> None
        IrValue.__init__(self, IrTypeMetadata())

    def resolve(self, module):
        # type: (IrModule) -> IrMetadata
        return self


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


class IrMetadataNode(IrMetadataValue):
    def __init__(self, value):
        # type: (List[IrValue]) -> None
        self._value = value


class IrSpecializedMetadata(IrMetadataValue):
    def __init__(self, specialized_class, properties):
        # type: (str, List[Tuple[str, IrMetadata]]) -> None
        self._specialized_class = specialized_class
        self._values = {}
        for property_name, property_value in properties:
            self._values[property_name] = property_value


class IrMetadataNull(IrMetadata):
    pass


if TYPE_CHECKING:
    from typing import List, Tuple
    from .ir_module import IrModule
    from .ir_reference import IrReference