from ..type import Type, CastError
from ..scope import Scope, ScopeError
from ..position import Position
from ..ast_templates import TemplateTypenameParameter
from ..dependent_name import DependentName
from ..cppobject import CppObject
from be_typing import cast


class StructScope(Scope):
    def __init__(self, owner, position, visibility, parent_visibility, parent):
        # type: (Struct, Position, str, Optional[str], Optional[Type]) -> None
        Scope.__init__(self, owner, position, visibility)
        if parent:
            assert isinstance(parent, Type)
            if not (
                isinstance(parent, Struct) or isinstance(parent, DependentName)
                or isinstance(parent, TemplateTypenameParameter)
            ):
                owner.lexer.logger.C0306(position)
        self.parent_visibility = parent_visibility
        self.parent = parent
        self.constructor = None    # type: Optional[Method]
        self.destructor = None     # type: Optional[Method]
        self.casts = []            # type: List[Tuple[TypeRef, Method]]
        self.operators = []        # type: List[Method]

    def debug_dump(self, indent):
        # type: (str) -> None
        if self.constructor:
            self.constructor.debug_dump(indent)
        if self.destructor:
            self.destructor.debug_dump(indent)
        #for _, cast in self.casts:
        #    cast.debug_dump(indent)
        Scope.debug_dump(self, indent)

    def find(self, name, position, source_context, is_current_scope):
        # type: (str, Position, CppObject, bool) -> Optional[CppObject]
        if is_current_scope and name == self.owner.name and self.constructor:
            return self.constructor
        if name == '~%s' % self.owner.name:
            return self.destructor
        try:
            result = Scope.find(self, name, position, source_context, is_current_scope)
        except ScopeError:
            if self.parent:
                return self.parent.scope.find(name, position, source_context, is_current_scope)
            else:
                raise
        else:
            return result or (self.parent and self.parent.scope.find(name, position, source_context, is_current_scope))


class Struct(Type):
    INITIAL_SCOPE = CppObject.NotDefinedScope

    global_index = 0

    def __init__(self, lexer, position, struct_type, name):
        # type: (ClLexer, Position, str, Optional[str]) -> None
        Type.__init__(self, lexer, position, name)
        self.struct_type = struct_type
        self.default_visibility = (struct_type == 'class') and 'protected' or 'public'
        self.index = Struct.global_index
        Struct.global_index += 1

    def get_token_type(self):
        # type: () -> str
        return 'STRUCT_ID'

    def define(self, parent_visibility, parent, position):
        # type: (Optional[str], Optional[Type], Position) -> None
        if self.struct_type == 'union' and parent:
            self.lexer.logger.C0307(position)
        self.push_scope_recursive(
            position,
            StructScope(self, position, self.default_visibility, parent_visibility or self.default_visibility, parent)
        )

    def _create_template_instance(self, template, arguments, position):
        # type: (Template, ArgumentList, Position) -> Struct
        return Struct(self.lexer, self.position, self.struct_type, self.name)

    def _complete_template_instance(self, result, template, arguments, position):
        # type: (Struct, Template, ArgumentList, Position) -> None
        if isinstance(self.scope, StructScope):
            parent = self.scope.parent and self.scope.parent.create_template_instance(template, arguments, position)
            result.define(self.scope.parent_visibility, parent and parent.get_type(), position)
            try:
                self.scope.create_template_instance(result.scope, template, arguments, position)
            except Exception as e:
                result.pop_scope_recursive()
                raise
            else:
                result.pop_scope_recursive()

    def _distance(self, cast_to, cast_options, typeref_from, typeref_to):
        # type: (Type, CastOptions, TypeRef, TypeRef) -> Type.Distance
        if isinstance(cast_to, Struct):
            if cast_options.allowed_cast <= cast_options.CAST_ATTRIB:
                if cast_to != self:
                    raise CastError(self.lexer.logger.C0300, self.position, from_type=typeref_from, to_type=typeref_to)
                else:
                    d = Type.Distance()
                    return d.match_attributes(cast_options.allowed_cast, typeref_from, typeref_to)
            cast = 0
            variant = 0
            p = self                                                                                               # type: Optional[Type]
            while p:
                if p == cast_to:
                    break
                cast += 1
                p = isinstance(p.scope, StructScope) and p.scope.parent or None
            else:
                if cast_options.allowed_cast >= cast_options.CAST_UPCAST:
                    cast = 0
                    p = cast_to
                    while p:
                        if p == self:
                            break
                        cast -= 1
                    else:
                        if cast_options.allowed_cast < cast_options.CAST_UNRELATED:
                            raise CastError(
                                self.lexer.logger.C0300, self.position, from_type=typeref_from, to_type=typeref_to
                            )
                        variant = -1
                        cast = 0
                else:
                    raise CastError(self.lexer.logger.C0300, self.position, from_type=typeref_from, to_type=typeref_to)
            d = Type.Distance(variant=variant, cast=cast)
            return d.match_attributes(cast_options.allowed_cast, typeref_from, typeref_to)
        elif cast_options.allowed_cast == cast_options.CAST_UNRELATED:
            return Type.Distance(variant=-1)
        else:
            raise CastError(self.lexer.logger.C0300, self.position, from_type=typeref_from, to_type=typeref_to)

    def __str__(self):
        # type: () -> str
        return self.name or '<anonymous>'

    def pretty_name(self):
        # type: () -> str
        return "%s '%s'" % (self.struct_type, str(self))

    def signature(self, template_bindings={}):
        # type: (Dict[BaseTemplateParameter, Tuple[int, BaseTemplateObject]]) -> str
        return '\\{\\}%s_%d' % (self.name or 'anonymous', self.index)

    def write_to(self, namespace, writer):
        # type: (List[str], ClDocumentWriter) -> None
        name = namespace + [self.name or ('anonymous_%d' % self.index)]
        if isinstance(self.scope, StructScope):
            struct = writer.create_struct(
                self.position, name, self.index, self.scope.parent and cast(Struct, self.scope.parent).index or None
            )
            if self.scope:
                self.scope.write_to(name, writer)

    def transform(self, writer):
        # type: (ClTypeWriter) -> ClType
        return writer.struct(self.name, self.index)


from be_typing import TYPE_CHECKING
if TYPE_CHECKING:
    from typing import Dict, List, Optional, Set, Tuple, Union
    from ...cl_lexer import ClLexer
    from ...cl_codegen import ClDocumentWriter, ClTypeWriter, ClType
    from ..ast_templates import BaseTemplateParameter, BaseTemplateObject, Template
    from ..argument_list import ArgumentList
    from ..typeref import TypeRef
    from ..type import CastOptions
    from ..method import Method
