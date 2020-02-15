from .cppobject import CppObject
from .position import Position


class Namespace(CppObject):
    INITIAL_SCOPE = CppObject.NotDefinedScope

    def __init__(self, lexer, position, name):
        # type: (ClLexer, Position, Optional[str]) -> None
        CppObject.__init__(self, lexer, position, name)
        self.register()

    def get_token_type(self):
        # type: () -> str
        return 'NAMESPACE_ID'

    def write_to(self, writer):
        # type: (ClDocumentWriter) -> None
        with writer.create_namespace(self.position, self.name or '') as namespace:
            self.scope.write_to(namespace)

    def pretty_name(self):
        # type: () -> str
        return "namespace '%s'" % self.name

    def _create_template_instance(self, template, arguments, position):
        # type: (Template, ArgumentList, Position) -> Namespace
        raise NotImplementedError

    def get_unresolved_parameters(self):
        # type: () -> List[BaseTemplateParameter]
        return []


class AnonymousNamespace(Namespace):
    def __init__(self, lexer, position):
        # type: (ClLexer, Position) -> None
        Namespace.__init__(self, lexer, position, None)
        self.register()

    def find(self, name):
        # type: (str) -> Optional[CppObject]
        if self.scope:
            return self.scope.find(name, self.position, self, False)
        return None

    def pretty_name(self):
        # type: () -> str
        return "anonymous namespace"


class RootNamespace(CppObject):
    def __init__(self, lexer):
        # type: (ClLexer) -> None
        CppObject.__init__(self, lexer, Position(lexer.filename, 1, 1, 1, lexer.lexdata()), None)

    def write_to(self, writer):
        # type: (ClDocumentWriter) -> None
        with writer.create_document() as document:
            self.scope.write_to(document)

    def pretty_name(sef):
        # type: () -> str
        return "the global namespace"

    def _create_template_instance(self, template, arguments, position):
        # type: (Template, ArgumentList, Position) -> RootNamespace
        raise NotImplementedError

    def get_unresolved_parameters(self):
        # type: () -> List[BaseTemplateParameter]
        return []


from be_typing import TYPE_CHECKING
if TYPE_CHECKING:
    from typing import List, Optional
    from ..cl_lexer import ClLexer
    from ..cl_document_writer import ClDocumentWriter
    from .ast_templates import BaseTemplateParameter, Template
    from .argument_list import ArgumentList
