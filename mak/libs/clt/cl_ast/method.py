from .cppobject import CppObject
from .scope import Scope, ScopeError
from .type import CastOptions, CastError
from be_typing import cast


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

    def write_to(self, namespace, writer):
        # type: (List[str], ClDocumentWriter) -> None
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
                raise ScopeError(self.owner.lexer.logger.C0103, position, lookup=name, owner=self.owner.pretty_name())
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
        # type: (List[Parameter], Position, Optional[TypeRef], List[str], Dict[BaseTemplateParameter, Tuple[int, BaseTemplateObject]]) -> bool
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
                self.lexer.logger.C0201(p2.position)
                self.lexer.logger.I0001(p1.position)
        if self.return_type is not None and return_type is not None:
            try:
                return_type.distance(self.return_type,
                                     CastOptions(CastOptions.CAST_NONE, template_bindings=binding))
            except CastError:
                return_type.distance(self.return_type,
                                     CastOptions(CastOptions.CAST_NONE, template_bindings=binding))
                self.lexer.logger.C0200(return_type.position)
                self.lexer.logger.I0000(self.return_type.position)
        elif return_type is not None:
            if not return_type.is_void():
                self.lexer.logger.C0200(return_type.position)
                self.lexer.logger.I0000(self.position)
        elif self.return_type is not None:
            self.lexer.logger.C0200(position)
            self.lexer.logger.I0000(self.return_type.position)
        return True

    def _create_template_instance(self, template, arguments, position):
        # type: (Template, ArgumentList, Position) -> Overload
        overload_template = None # type: Optional[Template]
        if self.template is not None and self.template != template:
            overload_template = self.template.create_template_instance(template, arguments, position)
        return Overload(self.lexer, self.position, self.name, overload_template,
                        [p.create_template_instance(template, arguments, position)
                         for p in self.parameters], self.return_type
                        and self.return_type.create_template_instance(template, arguments, position), self.attributes)

    def get_unresolved_parameters(self):
        # type: () -> List[BaseTemplateParameter]
        return []

    def write_to(self, namespace, writer):
        # type: (List[str], ClDocumentWriter) -> None
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
                binding = {} # type: Dict[BaseTemplateParameter, Tuple[int, BaseTemplateObject]]
                if template:
                    assert o.template is not None
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

    def write_to(self, namespace, writer):
        # type: (List[str], ClDocumentWriter) -> None
        assert self.name is not None
        name = namespace + [self.name]
        for i, o in enumerate(self.overloads):
            if o.template is not None:
                o.template.write_to(name, writer)
            else:
                method = writer.create_method(o.position, name, self.id, i,
                                            o.return_type and o.return_type.transform(writer.typewriter) or None)
                for p in o.parameters:
                    method.add_parameter(p.type.transform(writer.typewriter), p.name)

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


from be_typing import TYPE_CHECKING
if TYPE_CHECKING:
    from typing import Dict, List, Optional, Tuple
    from ..cl_lexer import ClLexer
    from ..cl_codegen import ClDocumentWriter
    from .position import Position
    from .typeref import TypeRef
    from .value import Value
    from .ast_templates import BaseTemplateParameter, BaseTemplateObject, Template
    from .argument_list import ArgumentList
    from .specifier import Specifier