from .cppobject import CppObject
from .value import Value
from be_typing import cast


class Variable(Value):
    def __init__(self, lexer, position, name, type, value, attributes):
        # type: (ClLexer, Position, Optional[str], TypeRef, Optional[Value], List[Specifier]) -> None
        Value.__init__(self, lexer, position, name)
        self.type = type
        self.value = value
        self.attributes = attributes

    def get_token_type(self):
        # type: () -> str
        return 'VARIABLE_ID'

    def debug_dump(self, indent=''):
        # type: (str) -> None
        attrs = self.attributes and (' '.join(str(a) for a in self.attributes) + ' ') or ''
        value = self.value and ' = %s' % str(self.value) or ''
        print(
            '%s%s%s%s%s [%s]' %
            (indent, attrs, str(self.type), self.name and (' %s' % self.name) or '', value, self.position)
        )

    def return_type(self):
        # type: () -> TypeRef
        return self.type

    def simplify(self):
        # type: () -> Value
        for s in self.attributes:
            if s.specifier == 'const' and self.value is not None:
                return self.value
        return self

    def _create_template_instance(self, template, arguments, position):
        # type: (Template, ArgumentList, Position) -> Variable
        return Variable(
            self.lexer, self.position, self.name, self.type.create_template_instance(template, arguments, position),
            self.value and self.value.create_template_instance(template, arguments, position), self.attributes
        )

    def get_unresolved_parameters(self):
        # type: () -> List[BaseTemplateParameter]
        return []

    def write_to(self, writer):
        # type: (ClDocumentWriter) -> None
        pass


from be_typing import TYPE_CHECKING
if TYPE_CHECKING:
    from typing import List, Optional
    from ..cl_lexer import ClLexer
    from ..cl_document_writer import ClDocumentWriter
    from .position import Position
    from .typeref import TypeRef
    from .specifier import Specifier
    from .ast_templates import Template, BaseTemplateParameter
    from .argument_list import ArgumentList