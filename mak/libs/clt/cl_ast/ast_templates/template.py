from .base_template_object import BaseTemplateObject, BaseTemplateParameter
from ..scope import Scope as BaseScope
from ..error import CppError
from ..type import Type, CastOptions, CastError
from ..typeref import TypeRef
from ..value import Value
from ..argument_list import ArgumentList
from .parameter_template import TemplateTemplateParameter
from .parameter_type import TemplateTypenameParameter
from be_typing import cast, TYPE_CHECKING


class Template(BaseTemplateObject):
    class Scope(BaseScope):
        def __init__(self, owner, position):
            # type: (Template, Position) -> None
            BaseScope.__init__(self, owner, position, scope_owner=owner.parent.scope.scope_owner)
            self.parameters = []   # type: List[BaseTemplateParameter]

        def add(self, element):
            # type: (CppObject) -> None
            owner = cast(Template, self.owner)
            assert owner.back_link == owner
            if isinstance(element, BaseTemplateParameter):
                self.parameters.append(element)
                owner.add_parameter(element)
            else:
                BaseScope.add(self, element)

        def find(self, name, position, source_context, is_current_scope):
            # type: (str, Position, CppObject, bool) -> Optional[CppObject]
            for element in self.parameters:
                result = element.find(name)
                if result:
                    return result
            #if is_current_scope:
            #    return BaseScope.find(self.owner.back_link.scope, name, position, source_context, is_current_scope)
            #else:
            #    return BaseScope.find(self, name, position, source_context, is_current_scope)
            return None

    class InstantiationError(CppError):
        pass

    def __init__(self, lexer, position):
        # type: (ClLexer, Position) -> None
        BaseTemplateObject.__init__(self, lexer, position)
        self.back_link = self
        self.siblings = []         # type: List[Template]
        self.parameters = []       # type: List[Union[Value, TypeRef, BaseTemplateObject]]
        self.specializations = []  # type: List[Tuple[List[CppObject], CppObject]]
        self.push_scope(position, Template.Scope(self, position))

    def __eq__(self, other):
        # type: (object) -> bool
        if isinstance(other, Template):
            return id(self.back_link) == id(other.back_link)
        else:
            return False

    def __neq__(self, other):
        # type: (object) -> bool
        return not (self == other)

    def signature(self, template_bindings={}):
        # type: (Dict[BaseTemplateParameter, Tuple[int, BaseTemplateObject]]) -> str
        return '$%d' % id(self.back_link)

    def expand_template_arg_list(self, argument_list):
        # type: (List[Union[Value, BaseTemplateObject, TypeRef]]) -> List[Union[Value, BaseTemplateObject, TypeRef]]
        if id(self) != id(self.back_link):
            return self.back_link.expand_template_arg_list(argument_list)
        else:
            result = argument_list[:]
            for i in range(len(argument_list), len(self.parameters)):
                scope = cast(Template.Scope, self.scope)
                value = scope.parameters[i].default_value
                assert value is not None
                assert isinstance(value, Value) or isinstance(value, TypeRef)
                result.append(value)
            return result

    def add_parameter(self, parameter):
        # type: (BaseTemplateParameter) -> None
        if isinstance(parameter, TemplateTypenameParameter):
            self.parameters.append(TypeRef(self.lexer, parameter.position, parameter))
        else:
            assert isinstance(parameter, (BaseTemplateObject, Value))
            self.parameters.append(parameter)

    def get_token_type_raw(self):
        # type: () -> str
        if self.back_link == self:
            assert self.scope
            assert self.scope[0]
            return self.scope[0][1].get_token_type_raw()
        else:
            return self.back_link.get_token_type_raw()

    def get_token_type(self):
        # type: () -> str
        if self.back_link == self:
            assert self.scope
            assert self.scope[0]
            return 'TEMPLATE_' + self.scope[0][1].get_token_type_raw()
        else:
            return self.back_link.get_token_type_raw()

    def bind(self, template):
        # type: (Template) -> None
        assert id(self.back_link) == id(self) or id(self.back_link) == id(template)
        if self.back_link == self:
            self.back_link = template
            template.siblings.append(self)
            if self.back_link != self:
                self.siblings.append(template)
            scope = cast(Template.Scope, self.scope)
            for i, p in enumerate(scope.parameters):
                p.bind(i, template)

    def find(self, name):
        # type: (str) -> Optional[CppObject]
        if self.scope and self.scope.items:
            return self.scope[0][1].find(name) and self or None
        #elif self.back_link != self:
        #    return self.back_link.find(name)
        else:
            return None

    def distance(self, cast_to, cast_options):
        # type: (CppObject, CastOptions) -> Type.Distance
        scope = cast(Template.Scope, self.scope)
        cast_target = cast_to.template_origin if isinstance(cast_to, TypeRef) else cast_to
        if isinstance(cast_target, Template):
            if id(self) == id(cast_target):
                return Type.Distance()
            else:
                raise CastError(self.lexer.logger.C0300, self.position, from_type=self, to_type=cast_to)
        elif isinstance(cast_target, TemplateTemplateParameter):
            params = cast_target.template_params
        else:
            raise CastError(self.lexer.logger.C0300, self.position, from_type=self, to_type=cast_to)
        if len(params) != len(scope.parameters):
            raise CastError(self.lexer.logger.C0300, self.position, from_type=self, to_type=cast_to)
        d = Type.Distance()
        #for p1, p2 in zip(self.parameters, params):
        #    d += p1.distance(p2, cast)
        return d

    def instantiate(self, arguments, position):
        # type: (List[Union[Value, BaseTemplateObject, TypeRef]], Position) -> Optional[CppObject]
        #self.lexer.note('creating instance of template %s'%self.scope[0][1].name, position)
        assert id(self) == id(self.back_link)
        assert isinstance(self.scope, Template.Scope)
        arguments = [p.simplify(a) for p, a in zip(self.scope.parameters, arguments)]
        matches, specialization, scores = self.find_specialization(position, arguments)
        unresolved_params = []                                                                            # type: List[BaseTemplateParameter]
        for a in arguments:
            unresolved_params += a.get_unresolved_parameters()
        if unresolved_params:
            if not scores:
                return None
            for score in scores:
                if not score.exact_match():
                    return None
            else:
                return specialization
        assert matches is not None
        assert specialization is not None
        assert scores is not None
        specialization.push_scope_recursive(position)
        self.parent.push_scope(position)
        try:
            result = specialization.create_template_instance(self, matches, position)
        except Template.InstantiationError as e:
            self.parent.pop_scope()
            specialization.pop_scope_recursive()
            scope = cast(Template.Scope, self.scope)
            match_str = ', '.join(
                '%s = %s' % (scope.parameters[i].name or '<anonymous>', v) for i, v in enumerate(matches)
            )
            raise Template.InstantiationError(
                self.lexer.logger.I0003,
                position,
                e,
                template_name=self.scope[0][1].pretty_name(),
                template_parameters=', '.join(str(x) for x in scope.parameters),
                arguments=match_str
            )
        except:
            self.parent.pop_scope()
            specialization.pop_scope_recursive()
            raise
        else:
            self.parent.pop_scope()
            specialization.pop_scope_recursive()
            return result

    def fill_temporary_binding(self, template, binding):
        # type: (Template, Dict[BaseTemplateParameter, Tuple[int, BaseTemplateObject]]) -> Dict[BaseTemplateParameter, Tuple[int, BaseTemplateObject]]
        scope = cast(Template.Scope, self.scope)
        for i, p in enumerate(scope.parameters):
            binding[p] = (i, template)
        return binding

    def find_instance(self, parameter_binds, arguments, position):
        # type: (Dict[BaseTemplateParameter, Tuple[int, BaseTemplateObject]], List[Union[Value, TypeRef, BaseTemplateObject]], Position) -> Optional[CppObject]
        arguments = self.match(arguments, position)
        assert len(arguments) == len(self.parameters)
        if parameter_binds is not None:
            for i, p in enumerate(arguments):
                if isinstance(p, TypeRef):
                    if p.qualifiers:
                        break
                    assert p.type
                    v = p.type     # type: CppObject
                else:
                    v = p
                if not isinstance(v, BaseTemplateParameter):
                    break
                if v.parameter_bind:
                    break
                if v not in parameter_binds:
                                   # definitely a dependent name
                    return None
                if parameter_binds[v][0] != i:
                    break
                if parameter_binds[v][1] != self:
                    break
            else:
                return self.scope[0][1]
            return self.find_exact_specialization(position, parameter_binds, arguments)
        else:
            return self.instantiate(arguments, position)

    def _create_template_instance(self, template, arguments, position):
        # type: (Template, ArgumentList, Position) -> Template
        assert self == self.back_link
        return Template(self.lexer, position)

    def _complete_template_instance(self, result, template, arguments, position):
        # type: (Template, Template, ArgumentList, Position) -> None
        try:
            scope = cast(Template.Scope, self.scope)
            for p in scope.parameters:
                p = p.create_template_instance(template, arguments, position)
                result.scope.add(p)
            for s in self.siblings:
                sibling_scope = cast(Template.Scope, s.scope)
                for i, p in enumerate(sibling_scope.parameters):
                    p.create_template_instance(template, arguments, position).bind(i, result)
            if self.scope:
                if self.scope.empty():
                    # should not happen?
                    pass
                else:
                    result.scope.add(self.scope[0][1].create_template_instance(template, arguments, position))
            for specialization_parameters, specialization in self.specializations:
                params = [p.create_template_instance(template, arguments, position) for p in specialization_parameters]
                result.specializations.append(
                    (params, specialization.create_template_instance(template, arguments, position))
                )
        except Exception as e:
            print(e)
        finally:
            self.lexer.pop_scope(result.scope)

    def create_specialization(self, arguments, specialization):
        # type: (List[CppObject], CppObject) -> None
        assert id(self) == id(self.back_link)
        self.specializations.append((arguments, specialization))

    def find_exact_specialization(self, position, parameter_binds, arguments):
        # type: (Position, Dict[BaseTemplateParameter, Tuple[int, BaseTemplateObject]], List[Union[Value, BaseTemplateObject, TypeRef]]) -> Optional[CppObject]
        for specialization_arguments, specialization in self.specializations:
            try:
                matches, scores = self.argument_match(specialization_arguments, arguments, parameter_binds)
            except CastError:
                return None
            else:
                for score in scores:
                    if not score.exact_match():
                        return None
                return specialization
        return None

    def find_specialization(self, position, arguments):
        # type: (Position, List[Union[Value, BaseTemplateObject, TypeRef]]) -> Tuple[Optional[ArgumentList], Optional[CppObject], Optional[List[Type.Distance]]]
        specializations = []
        for specialization_arguments, specialization in self.specializations:
            try:
                matches, score = self.argument_match(specialization_arguments, arguments, {})
            except CastError:
                pass
            else:
                specializations.append((score, matches, specialization))
        try:
            matches, score = self.argument_match(self.parameters, arguments, {})
        except CastError as e:
            assert isinstance(self.scope, Template.Scope)
            params = ', '.join(str(a) for a in self.scope.parameters)
            raise Template.InstantiationError(
                self.lexer.logger.I0005,
                position,
                e,
                template_name=self.scope[0][1].name,
                template_parameters=params,
                arguments=', '.join(str(a) for a in arguments)
            )
        else:
            try:
                specializations.append((score, matches, self.scope[0][1]))
            except IndexError:
                pass
        if specializations:
            specializations = sorted(specializations, key=lambda x: x[0])
            score, matches, result = specializations[0]
            if len(specializations) > 1 and specializations[0][0] == specializations[1][0]:
                args = ', '.join(str(a) for a in arguments)
                assert self.scope[0][1].name is not None
                self.lexer.logger.C0400(position, self.scope[0][1].name, args)
                for s, m, r in specializations:
                    if s == score:
                        scope = cast(Template.Scope, r.parent.scope)
                        match_str = ', '.join(
                            '%s = %s' % (scope.parameters[i].name or '<anonymous>', v) for i, v in sorted(m.items())
                        )
                        self.lexer.logger.I0003(r.position, match_str)
                    else:
                        break
            return ArgumentList(matches), result, score
        return None, None, None

    def get_unresolved_parameters(self):
        # type: () -> List[BaseTemplateParameter]
        return []

    def argument_match(self, parameters, arguments, template_bindings):
        # type: (Sequence[CppObject], List[Union[Value, BaseTemplateObject, TypeRef]], Dict[BaseTemplateParameter, Tuple[int, BaseTemplateObject]]) -> Tuple[Dict[int, CppObject], List[Type.Distance]]
        if len(arguments) < len(parameters):
            raise CastError(self.lexer.logger.C0301, self.position, pretty_name=self.scope[0][1].pretty_name())
        if len(arguments) > len(parameters):
            raise CastError(self.lexer.logger.C0302, self.position, pretty_name=self.scope[0][1].pretty_name())
        matches = {}   # type: Dict[int, CppObject]
        result = []
        for a, p in zip(arguments, parameters):
            try:
                score = a.distance(p, CastOptions(CastOptions.CAST_NONE, matches, template_bindings, self))
            except CastError:
                raise CastError(self.lexer.logger.C0303, p.position, parameter_name=str(p))
            for k, v in score.matches.items():
                if k not in matches:
                    matches[k] = v
                else:
                    assert matches[k] == v
            result.append(score)
        return matches, result

    def debug_dump(self, indent=''):
        # type: (str) -> None
        if self.scope and not self.scope.empty():
            scope = cast(Template.Scope, self.scope)
            params = ', '.join(str(p) for p in scope.parameters)
            print('%s%s<%s> [%s]' % (indent, self.__class__.__name__, params, self.position))
            self.scope[0][-1].debug_dump(indent)
            for args, pos, _, instance in self.scope[0][-1].instances:
                print('%s<%s> [%s]' % (indent, args, pos))
                instance.debug_dump(indent + '* ')
        for specialization_arguments, specialization in self.specializations:
            params = ', '.join(str(p) for p in specialization_arguments)
            print('%sspecialization: %s<%s> [%s]' % (indent, self.__class__.__name__, params, self.position))
            specialization.debug_dump(indent + ' | ')
            for args, pos, _, instance in specialization.instances:
                print('%s<%s> [%s]' % (indent, args, pos))
                instance.debug_dump(indent + ' | * ')
            print(indent + ' `-- end specialization')

    def match(self, arguments, position):
        # type: (List[Union[Value, BaseTemplateObject, TypeRef]], Position) -> List[Union[Value, BaseTemplateObject, TypeRef]]
        scope = cast(Template.Scope, self.scope)
        if len(arguments) > len(scope.parameters):
            raise Template.InstantiationError(
                self.lexer.logger.C0302,
                arguments[len(scope.parameters)].position,
                pretty_name=self.scope[0][1].pretty_name()
            )
        for missing_parameter in scope.parameters[len(arguments):]:
            default_value = missing_parameter.default_value
            if not default_value:
                raise Template.InstantiationError(
                    self.lexer.logger.C0301,
                    arguments[len(scope.parameters)].position,
                    pretty_name=self.scope[0][1].pretty_name()
                )
            assert isinstance(default_value, Value) or isinstance(default_value, TypeRef)
            arguments.append(default_value)
        for p, a in zip(scope.parameters, arguments):
            if not p.is_compatible(a):
                raise self.InstantiationError(
                    self.lexer.logger.C0304,
                    a.position,
                    parameter_name=str(p),
                    expected_type=p.get_template_parameter_type(),
                    got_type=a.__class__.__name__
                )
        return arguments

    def write_to(self, namespace, writer):
        # type: (List[str], ClDocumentWriter) -> None
        if self.scope and not self.scope.empty():
            for _, _, _, instance in self.scope[0][-1].instances:
                instance.write_to(namespace, writer)
            for _, specialization in self.specializations:
                for _, _, _, instance in specialization.instances:
                    instance.write_to(namespace, writer)

    def transform(self, writer):
        # type: (ClTypeWriter) -> ClType
        raise NotImplementedError


if TYPE_CHECKING:
    from typing import Dict, List, Optional, Tuple, Union, Sequence
    from ...cl_lexer import ClLexer
    from ...cl_codegen import ClDocumentWriter, ClTypeWriter, ClType
    from ..cppobject import CppObject
    from ..position import Position
