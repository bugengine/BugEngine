from .ir_declaration import IrDeclaration
from .ir_value import IrValue
from .ir_type import IrTypeMetadata
from ..ir_position import IrPosition
from be_typing import TYPE_CHECKING
import os


class IrMetadata(IrValue):
    def __init__(self):
        # type: () -> None
        IrValue.__init__(self, IrTypeMetadata())

    def resolve(self, module, position):
        # type: (IrModule, IrPosition) -> None
        IrValue.resolve(self, module, position)

    def __str__(self):
        # type: () -> str
        raise NotImplementedError

    def get_position(self):
        # type: () -> IrPosition
        return IrPosition('', 0, 0, 0, '')

    def to_string(self):
        # type: () -> str
        raise NotImplementedError

    def to_int(self):
        # type: () -> int
        raise NotImplementedError


class IrMetadataDeclaration(IrDeclaration):
    def __init__(self, metadata_value):
        # type: (IrMetadata) -> None
        self._metadata = metadata_value

    def resolve(self, module):
        # type: (IrModule) -> None
        self._metadata.resolve(module, IrPosition('', 0, 0, 0, ''))


class IrMetadataLink(IrMetadata):
    def __init__(self, reference):
        # type: (IrReference) -> None
        IrMetadata.__init__(self)
        self._reference = reference
        self._metadata = None  # type: Optional[IrMetadata]

    def resolve(self, module, position):
        # type: (IrModule, IrPosition) -> None
        if self._metadata is None:
            self._metadata = module.get(self._reference, IrMetadataDeclaration)._metadata
            self._metadata.resolve(module, position)
        IrMetadata.resolve(self, module, position)

    def get_position(self):
        # type: () -> IrPosition
        assert self._metadata is not None
        return self._metadata.get_position()

    def to_string(self):
        # type: () -> str
        assert self._metadata is not None
        return self._metadata.to_string()

    def to_int(self):
        # type: () -> int
        assert self._metadata is not None
        return self._metadata.to_int()


class IrMetadataValue(IrMetadata):
    pass


class IrMetadataString(IrMetadataValue):
    def __init__(self, value):
        # type: (str) -> None
        IrMetadata.__init__(self)
        self._value = value

    def __str__(self):
        # type: () -> str
        return '!"%s"' % (self._value)

    def to_string(self):
        # type: () -> str
        return self._value


class IrMetadataInteger(IrMetadataValue):
    def __init__(self, value):
        # type: (int) -> None
        IrMetadata.__init__(self)
        self._value = value

    def __str__(self):
        # type: () -> str
        return '!%d' % (self._value)

    def to_int(self):
        # type: () -> int
        return self._value


class IrMetadataNode(IrMetadataValue):
    def __init__(self, values):
        # type: (List[IrValue]) -> None
        IrMetadata.__init__(self)
        self._values = values

    def resolve(self, module, position):
        # type: (IrModule, IrPosition) -> None
        for v in self._values:
            v.resolve(module, position)
        IrMetadataValue.resolve(self, module, position)

    def __str__(self):
        # type: () -> str
        return '!{%s}' % (', '.join(str(v) for v in self._values))


class IrSpecializedMetadata(IrMetadataValue):
    def __init__(self, specialized_class, properties):
        # type: (str, List[Tuple[str, IrMetadata]]) -> None
        IrMetadata.__init__(self)
        self._specialized_class = specialized_class
        self._values = {}
        self._lines = []   # type: List[int]
        self._data = ''
        for property_name, property_value in properties:
            self._values[property_name] = property_value

    def resolve(self, module, position):
        # type: (IrModule, IrPosition) -> None
        for property_name, property_value in self._values.items():
            property_value.resolve(module, position)
        if self._specialized_class == 'DIFile':
            filename = self._values['filename'].to_string()
            directory = self._values['directory'].to_string()
            with open(os.path.join(directory, filename), 'r') as data:
                self._data = data.read()
                pos = 0
                for line in self._data.split('\n'):
                    self._lines.append(pos)
                    pos += len(line) + 1

    def __str__(self):
        # type: () -> str
        return '!%s(%s)' % (self._specialized_class, ', '.join('%s: %s' % (k, str(v)) for k, v in self._values.items()))

    def get_position(self):
        # type: () -> IrPosition
        try:
            result = self._values['scope'].get_position()
        except KeyError:
            result = IrPosition('', 0, 0, 0, '')
        if self._lines:
            result.lines = self._lines
        if self._data:
            result.lexdata = self._data
        try:
            result.filename = self._values['file'].get_position().filename
        except KeyError:
            pass
        try:
            result.filename = self._values['filename'].to_string()
        except KeyError:
            pass
        try:
            result.line_number = self._values['line'].to_int()
            if result.lines:
                result.start_position = result.lines[result.line_number - 1]
            result.start_position += self._values['column'].to_int() - 1
        except KeyError:
            pass
        return result


class IrMetadataNull(IrMetadata):
    def __str__(self):
        # type: () -> str
        return '!null'

    def to_string(self):
        # type: () -> str
        return ''

    def to_int(self):
        # type: () -> int
        return 0


class IrMetadataFlagList(IrMetadata):
    def __init__(self, flag):
        # type: (str) -> None
        IrMetadata.__init__(self)
        self._flags = [flag]

    def add_flag(self, flag):
        # type: (str) -> None
        self._flags.append(flag)

    def __str__(self):
        # type: () -> str
        return ' | '.join(self._flags)


if TYPE_CHECKING:
    from typing import List, Optional, Tuple
    from .ir_module import IrModule
    from .ir_reference import IrReference