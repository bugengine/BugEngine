from .cppobject import CppObject
from .scope import Scope, ScopeError
from .type import CastOptions, CastError
from typing import cast


class Parameter(CppObject):
    def __init__(self, lexer, position, name, type, default_value):
        # type: (ClLexer, Position, Optional[str], TypeRef, Optional[Value]) -> None
        CppObject.__init__(self, lexer, position, name)
        self.type = type
        self.default_value = default_value

    def get_token_type(self):
        # type: () -> str
        return 'VARIABLE_ID'

    def get_unresolved_parameters(self):
        # type: () -> List[BaseTemplateParameter]
        return []

    def write_to(self, writer):
        # type: (ClDocumentWriter) -> None
        raise NotImplementedError

    def _create_template_instance(self, template, arguments, position):
        # type: (Template, ArgumentList, Position) -> Parameter
        return Parameter(
            self.lexer, self.position, self.name, self.type.create_template_instance(template, arguments, position),
            self.default_value and self.default_value.create_template_instance(template, arguments, position))


class CodeBlock(Scope):
    def is_definition_scope(self):
        # type: () -> bool
        return False


class OverloadScope(CodeBlock):
    def find(self, name, position, source_context, is_current_scope):
        # type: (str, Position, CppObject, bool) -> Optional[CppObject]
        try:
            result = CodeBlock.find(self, name, position, source_context, is_current_scope)
        except ScopeError:
            result = None
        if not result:
            owner = self.owner
            assert isinstance(owner, Overload)
            for v in owner.parameters:
                if v.name == name:
                    return v
            if not result and is_current_scope:
                raise ScopeError("no member named '%s' in %s" % (name, self.owner.pretty_name()), position)
        return result


class Overload(CppObject):
    def __init__(self, lexer, position, name, template, parameters, return_type, attributes):
        # type: (ClLexer, Position, Optional[str], Optional[Template], List[Parameter], Optional[TypeRef], List[str]) -> None
        CppObject.__init__(self, lexer, position, name)
        self.parameters = parameters[:]
        self.return_type = return_type
        self.template = template
        if return_type:
            assert not return_type.is_void()
        self.attributes = attributes

    def define(self, position):
        # type: (Position) -> None
        self.push_scope_recursive(position, OverloadScope(self, position))

    def match(self, parameters, position, return_type, attributes, binding):
        # type: (List[Parameter], Position, Optional[TypeRef], List[str], Dict[BaseTemplateParameter, Tuple[int, Template]]) -> bool
        #attributes should be reflected in argument signature instread
        if 'const' in self.attributes and not 'const' in attributes:
            return False
        if not 'const' in self.attributes and 'const' in attributes:
            return False
        if len(parameters) != len(self.parameters):
            return False
        for p1, p2 in zip(parameters, self.parameters):
            try:
                d = p1.type.distance(p2.type, CastOptions(CastOptions.CAST_NONE, template_bindings=binding))
            except CastError:
                return False
            else:
                if not d.exact_match():
                    return False
        for p1, p2 in zip(self.parameters, parameters):
            if p1.default_value and p2.default_value:
                self.lexer.error('redefinition of default argument', p2.position)
                self.lexer.note('previous definition is here', p1.position)
        if self.return_type and return_type:
            try:
                return_type.distance(self.return_type,
                                     CastOptions(CastOptions.CAST_NONE, template_bindings=binding))
            except CastError:
                return_type.distance(self.return_type,
                                     CastOptions(CastOptions.CAST_NONE, template_bindings=binding))
                self.lexer.error('functions that differ only by return type cannot be overloaded', return_type.position)
                self.lexer.note('previous declaration is here', self.return_type.position)
        elif return_type:
            if not return_type.is_void():
                self.lexer.error('functions that differ only by return type cannot be overloaded', return_type.position)
                self.lexer.note('previous declaration is here', self.position)
        elif self.return_type:
            self.lexer.error('functions that differ only by return type cannot be overloaded', position)
            self.lexer.note('previous declaration is here', self.return_type.position)
        return True

    def _create_template_instance(self, template, arguments, position):
        # type: (Template, ArgumentList, Position) -> Overload
        return Overload(self.lexer, self.position, self.name, self.template and self.template.create_template_instance(template, arguments, position),
                        [p.create_template_instance(template, arguments, position)
                         for p in self.parameters], self.return_type
                        and self.return_type.create_template_instance(template, arguments, position), self.attributes)

    def get_unresolved_parameters(self):
        # type: () -> List[BaseTemplateParameter]
        return []

    def write_to(self, writer):
        # type: (ClDocumentWriter) -> None
        raise NotImplementedError

    def debug_dump(self, indent=''):
        # type: (str) -> None
        print('%s%s%s(%s);\n' %
              (indent, self.return_type and
               ('%s ' % self.return_type) or '', self.name, ', '.join('%s%s' %
                                                                      (str(p.type), p.name and (' %s' % p.name) or '')
                                                                      for p in self.parameters)))

    def debug_dump_overload(self, indent):
        # type: (str) -> None
        if self.template:
            self.template.debug_dump(indent)
        else:
            self.debug_dump(indent)


class Method(CppObject):
    index = 1

    def __init__(self, lexer, position, name, owner):
        # type: (ClLexer, Position, Optional[str], CppObject) -> None
        CppObject.__init__(self, lexer, position, name)
        self.id = Method.index
        self.owner = owner
        Method.index += 1
        self.overloads = []  # type: List[Overload]

    def find_overload(self, template, parameters, position, return_type, attributes):
        # type: (Optional[Template], List[Parameter], Position, Optional[TypeRef], List[str]) -> Optional[Overload]
        for o in self.overloads:
            if bool(template) == bool(o.template):
                binding = {} # type: Dict[BaseTemplateParameter, Tuple[int, Template]]
                if template:
                    assert o.template
                    if len(template.parameters) != len(o.template.parameters):
                        continue
                    binding = template.fill_temporary_binding(o.template, binding)
                if o.match(parameters, position, return_type, attributes, binding):
                    return o
        return None

    def create_overload(self, template, parameters, position, return_type, attributes):
        # type: (Optional[Template], List[Parameter], Position, Optional[TypeRef], List[str]) -> Overload
        if return_type and return_type.is_void():
            return_type = None
        o = Overload(self.lexer, position, self.name, template, parameters, return_type, attributes)
        self.overloads.append(o)
        if template:
            template.scope.add(o)
        return o

    def get_token_type(self):
        # type: () -> str
        for o in self.overloads:
            if o.template is not None:
                return 'TEMPLATE_METHOD_ID'
        return 'METHOD_ID'

    def get_unresolved_parameters(self):
        # type: () -> List[BaseTemplateParameter]
        return []
        
    def _create_template_instance(self, template, arguments, position):
        # type: (Template, ArgumentList, Position) -> Method
        return Method(self.lexer, self.position, self.name,
                      self.owner.create_template_instance(template, arguments, position))

    def _complete_template_instance(self, result, template, arguments, position):
        # type: (Method, Template, ArgumentList, Position) -> None
        for o in self.overloads:
            result.overloads.append(o.create_template_instance(template, arguments, position))

    def write_to(self, writer):
        # type: (ClDocumentWriter) -> None
        for i, o in enumerate(self.overloads):
            with writer.create_method(o.position, self.id, self.name or 'anonymous', i) as method:
                pass

    def debug_dump(self, indent=''):
        # type: (str) -> None
        for overload in self.overloads:
            overload.debug_dump_overload(indent)


class SpecialMethod(Method):
    def __init__(self, lexer, position, name, owner):
        # type: (ClLexer, Position, Optional[str], CppObject) -> None
        Method.__init__(self, lexer, position, name, owner)

    def get_token_type(self):
        # type: () -> str
        return 'SPECIAL_METHOD_ID'


from typing import TYPE_CHECKING
if TYPE_CHECKING:
    from typing import Dict, List, Optional, Tuple
    from ..cl_lexer import ClLexer
    from ..cl_document_writer import ClDocumentWriter
    from .position import Position
    from .typeref import TypeRef
    from .value import Value
    from .ast_templates import BaseTemplateParameter, Template
    from .argument_list import ArgumentList
    from .specifier import Specifier