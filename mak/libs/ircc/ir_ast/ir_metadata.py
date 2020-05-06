from be_typing import TYPE_CHECKING


class IrMetadata:
    pass


class IrMetadataLink(IrMetadata):
    pass


class IrMetadataValue(IrMetadata):
    pass


class IrMetadataLiteralValue(IrMetadataValue):
    pass


class IrMetadataNode(IrMetadataValue):
    pass


class IrSpecializedMetadata(IrMetadataValue):
    def __init__(self, specialized_class, properties):
        # type: (str, List[Tuple[str, IrMetadata]]) -> None
        self._specialized_class = specialized_class
        self._values = {}
        for property_name, property_value in properties:
            self._values[property_name] = property_value


if TYPE_CHECKING:
    from typing import List, Tuple