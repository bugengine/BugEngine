from clt.cl_codegen import ClTypeWriter, ClType
from be_typing import TYPE_CHECKING


class Type(ClType):
    def __init__(self, name):
        # type: (str) -> None
        self._type = name

    def add_modifier(self, modifier):
        # type: (str) -> None
        self._type += ' %s' % modifier

    def __str__(self):
        # type: () -> str
        return self._type


class TypeWriter(ClTypeWriter):
    def builtin(self, typename):
        # type: (str) -> Type
        return Type(typename)

    def struct(self, struct_name, struct_id):
        # type: (Optional[str], int) -> Type
        return Type('struct_%d' % struct_id)

    def enum(self, enum_name, enum_id):
        # type: (Optional[str], int) -> Type
        return Type('enum_%d' % enum_id)

    def pointer(self, pointee):
        # type: (ClType) -> Type
        return Type('%s*' % pointee)

    def reference(self, pointee):
        # type: (ClType) -> Type
        return Type('%s*' % pointee)

    def array(self, pointee, count):
        # type: (ClType, Optional[int]) -> Type
        if count is not None:
            return Type('%s[%d]' % (pointee, count))
        else:
            return Type('%s[]' % pointee)


if TYPE_CHECKING:
    from typing import List, Optional
    from clt.cl_codegen import ClDocumentWriter
