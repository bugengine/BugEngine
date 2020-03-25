from .error import CppError


class ScopeError(CppError):
    pass


class Scope:
    def __init__(self, owner, position, visibility='public', scope_owner=None):
        # type: (CppObject, Position, str, Optional[CppObject]) -> None
        self.owner = owner
        self.scope_owner = scope_owner or owner
        self.position = position
        self.visibility = visibility
        self.items = []    # type: List[Tuple[str, CppObject]]

    def is_definition_scope(self):
        # type: () -> bool
        return True

    def add(self, element):
        # type: (CppObject) -> None
        self.items.append((self.visibility, element))

    def remove(self, element):
        # type: (CppObject) -> None
        self.items = [i for i in self.items if i[1] != element]

    def empty(self):
        # type: () -> bool
        return len(self.items) == 0

    def __getitem__(self, index):
        # type: (int) -> Tuple[str, CppObject]
        return self.items[index]

    def find(self, name, position, source_context, is_current_scope):
        # type: (str, Position, CppObject, bool) -> Optional[CppObject]
        for _, element in self.items:
            result = element.find(name)
            if result:
                return result
        if is_current_scope:
            raise ScopeError(self.owner.lexer.logger.C0103, position, lookup=name, owner=self.owner.pretty_name())
        else:
            return None

    def seal(self):
        # type: () -> None
        self.owner.seal()

    def debug_dump(self, indent):
        # type: (str) -> None
        for visibility, element in self.items:
            print('%s%s:' % (indent, visibility))
            element.debug_dump(indent + '  ')

    def create_template_instance(self, target_scope, template, arguments, position):
        # type: (Scope, Template, ArgumentList, Position) -> None
        for visibility, element in self.items:
            target_scope.items.append((visibility, element.create_template_instance(template, arguments, position)))

    def write_to(self, namespace, writer):
        # type: (List[str], ClDocumentWriter) -> None
        for _, object in self.items:
            object.write_to(namespace, writer)


from be_typing import TYPE_CHECKING
if TYPE_CHECKING:
    from typing import Optional, List, Tuple
    from .cppobject import CppObject
    from .position import Position
    from .ast_templates import Template
    from .argument_list import ArgumentList
    from ..cl_codegen import ClDocumentWriter
