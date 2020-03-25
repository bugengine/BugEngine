import sys
import argparse
from be_typing import TypeVar, cast

T = TypeVar('T', bound='Callable[..., Dict[str, Any]]')


def diagnostic(func):
    # type: (T) -> T
    def call(self, position, *args, **kw_args):
        # type: (Logger, ClPosition, Union[str, int], Union[str, int]) -> None
        if call in self._expected_diagnostics:
            self._expected_diagnostics.remove(cast(T, call))
        format_values = func(self, position, *args, **kw_args)
        self._msg('note', position, getattr(self.LANG, func.__name__, func.__doc__).format(**format_values))

    return cast(T, call)


def warning(flag_name, enabled=False, enabled_in=[]):
    # type: (str, bool, List[str]) -> Callable[[T], T]
    def inner(func):
        # type: (T) -> T
        def call(self, position, *args, **kw_args):
            # type: (Logger, ClPosition, *Union[str, int], **Union[str, int]) -> None
            if not getattr(self._arguments, flag_name):
                return
            format_values = func(self, position, *args, **kw_args)
            self._warning_count += 1
            if self._warning_count == 1 and self._arguments.warn_error:
                self.C0002(position)
            self._msg(
                'warning', position,
                getattr(self.LANG, func.__name__, func.__doc__).format(**format_values) + ' [-W{}]'.format(flag_name)
            )

        setattr(call, 'flag_name', flag_name)
        setattr(call, 'enabled_in', enabled_in)

        return cast(T, call)

    return inner


def error(func):
    # type: (T) -> T
    def call(self, position, *args, **kw_args):
        # type: (Logger, ClPosition, *Union[str, int], **Union[str, int]) -> None
        self._error_count += 1
        format_values = func(self, position, *args, **kw_args)
        self._msg('error', position, getattr(self.LANG, func.__name__, func.__doc__).format(**format_values))

    call.__name__ = func.__name__
    return cast(T, call)


class Logger:
    LANG = None

    COLOR_LIST = {
        'BOLD': '\x1b[01;1m',
        'BLACK': '\x1b[30m',
        'RED': '\x1b[31m',
        'GREEN': '\x1b[32m',
        'YELLOW': '\x1b[33m',
        'BLUE': '\x1b[34m',
        'PINK': '\x1b[35m',
        'CYAN': '\x1b[36m',
        'WHITE': '\x1b[37m',
        'BBLACK': '\x1b[01;30m',
        'BRED': '\x1b[01;31m',
        'BGREEN': '\x1b[01;32m',
        'BYELLOW': '\x1b[01;33m',
        'BBLUE': '\x1b[01;34m',
        'BPINK': '\x1b[01;35m',
        'BCYAN': '\x1b[01;36m',
        'BWHITE': '\x1b[01;37m',
        'NORMAL': '\x1b[0m',
    }

    DEFAULT_COLOR_PATTERN = (
        COLOR_LIST['BWHITE'], COLOR_LIST['BWHITE'], COLOR_LIST['NORMAL'], COLOR_LIST['BGREEN'], COLOR_LIST['NORMAL']
    )

    COLOR_PATTERN = {
        'note':
            (
                COLOR_LIST['BBLACK'], COLOR_LIST['BWHITE'], COLOR_LIST['NORMAL'], COLOR_LIST['BGREEN'],
                COLOR_LIST['NORMAL']
            ),
        'info':
            (
                COLOR_LIST['BWHITE'], COLOR_LIST['BWHITE'], COLOR_LIST['NORMAL'], COLOR_LIST['BGREEN'],
                COLOR_LIST['NORMAL']
            ),
        'warning':
            (
                COLOR_LIST['BYELLOW'], COLOR_LIST['BWHITE'], COLOR_LIST['NORMAL'], COLOR_LIST['BGREEN'],
                COLOR_LIST['NORMAL']
            ),
        'error':
            (
                COLOR_LIST['BRED'], COLOR_LIST['BWHITE'], COLOR_LIST['BWHITE'], COLOR_LIST['BGREEN'],
                COLOR_LIST['NORMAL']
            ),
    }

    IDE_FORMAT = {
        'msvc':
            '{color_filename}{filename}{color_off}({line:d},{column:d}) : {color_error_type}{error_type}{color_off}: {color_message}{message}{color_off}',
        'unix':
            '{color_filename}{filename}{color_off}:{line:d}:{column:d}: {color_error_type}{error_type}{color_off}: {color_message}{message}{color_off}',
        'vi':
            '{color_filename}{filename}{color_off} +{line:d}:{column:d}: {color_error_type}{error_type}{color_off}: {color_message}{message}{color_off}',
    }

    @classmethod
    def init_diagnostic_flags(self, group):
        # type: (_ArgumentGroup) -> None
        group.add_argument(
            "-e",
            "--diagnostics-format",
            dest="diagnostics_format",
            choices=('msvc', 'unix', 'vi'),
            help="Select diagnostics format to match IDE.",
            default='unix'
        )
        group.add_argument(
            "-Werror", dest="warn_error", help="Treat warning as errors", default=False, action="store_true"
        )
        for m in self.__dict__.values():
            flag = getattr(m, 'flag_name', None)
            if flag is not None:
                group.add_argument('-W{}'.format(flag), dest=flag, help=argparse.SUPPRESS, action='store_true')
                group.add_argument('-Wno-{}'.format(flag), dest=flag, help=argparse.SUPPRESS, action='store_false')

    def __init__(self, arguments):
        # type: (Namespace) -> None
        self._arguments = arguments
        self._error_color = sys.stderr.isatty()
        self._diagnostics_format = Logger.IDE_FORMAT[getattr(arguments, 'diagnostics_format')]
        self._warning_count = 0
        self._error_count = 0
        self._expected_diagnostics = [] # type: List[Callable[..., Dict[str, Any]]]

    def _msg(self, error_type, position, message):
        # type: (str, ClPosition, str) -> None
        if self._error_color:
            (color_error_type, color_filename, color_message, color_caret,
             color_off) = self.COLOR_PATTERN.get(error_type, self.DEFAULT_COLOR_PATTERN)
        else:
            color_error_type = ''
            color_filename = ''
            color_message = ''
            color_caret = ''
            color_off = ''

        filename = position.filename
        line = position.line_number
        offset = position.start_position
        end = position.end_position

        while offset > 0 and position.lexdata[offset - 1] != '\n':
            offset -= 1
        while end < len(position.lexdata) and position.lexdata[end] != '\n':
            end += 1

        column = position.start_position - offset + 1
        sys.stderr.write(self._diagnostics_format.format(**locals()))
        sys.stderr.write(position.lexdata[offset:end + 1])
        sys.stderr.write(
            '%s%s%s%s\n' % (
                ' ' * (position.start_position - offset), color_caret, '^' *
                (position.end_position - position.start_position), color_off
            )
        )

    def push_expected_diagnostics(self, diagnostics):
        # type: (List[Callable[..., Dict[str, Any]]]) -> None
        self._expected_diagnostics += diagnostics

    def pop_expected_diagnostics(self):
        # type: () -> bool
        result = bool(self._expected_diagnostics)
        self._expected_diagnostics = []
        return result

    #
    # Indication of location
    #
    @diagnostic
    def I0000(self, position):
        # type: (ClPosition) -> Dict[str, Any]
        """first declared here"""
        return locals()

    @diagnostic
    def I0001(self, position):
        # type: (ClPosition) -> Dict[str, Any]
        """first defined here"""
        return locals()

    @diagnostic
    def I0002(self, position, object_name):
        # type: (ClPosition, str) -> Dict[str, Any]
        """forward declaration of {object_name}"""
        return locals()

    @diagnostic
    def I0003(self, position, match):
        # type: (ClPosition, str) -> Dict[str, Any]
        """partial specialization matches [{match}]"""
        return locals()

    @diagnostic
    def I0004(self, position):
        # type: (ClPosition) -> Dict[str, Any]
        """template is declared here"""
        return locals()

    @diagnostic
    def I0005(self, position, template_name, template_parameters, arguments):
        # type: (ClPosition, str, str, str) -> Dict[str, Any]
        """when instantiating template {template_name}<{template_parameters}> [with {arguments}]"""
        return locals()

    @diagnostic
    def I0006(self, position, typedef_name, typedef_target):
        # type: (ClPosition, str, str) -> Dict[str, Any]
        """with {typedef_name} defined as {typedef_target}"""
        return locals()

    #
    # High-level parse errors
    #
    @error
    def C0000(self, position, char):
        # type: (Logger, ClPosition, str) -> Dict[str, Any]
        """invalid character 'char' in input stream"""
        return locals()

    @error
    def C0001(self, position, option_context):
        # type: (ClPosition, ArgumentParser) -> Dict[str, Any]
        """invalid command line\n{usage}"""
        usage = option_context.usage
        return locals()

    @error
    def C0002(self, position):
        # type: (Logger, ClPosition) -> Dict[str, Any]
        """warning treated as error"""
        return locals()

    @error
    def C0003(self, position, ident):
        # type: (Logger, ClPosition, str) -> Dict[str, Any]
        """invalid #ident directive {ident}"""
        return locals()

    @error
    def C0004(self, position, pragma):
        # type: (Logger, ClPosition, str) -> Dict[str, Any]
        """invalid #pragma directive {pragma}"""
        return locals()

    @error
    def C0005(self, position):
        # type: (Logger, ClPosition) -> Dict[str, Any]
        """filename before line number in #line"""
        return locals()

    @error
    def C0006(self, position):
        # type: (Logger, ClPosition) -> Dict[str, Any]
        """line number missing in #line"""
        return locals()

    @error
    def C0007(self, position):
        # type: (Logger, ClPosition) -> Dict[str, Any]
        """invalid #line directive"""
        return locals()

    @error
    def C0008(self, position, type):
        # type: (Logger, ClPosition, str) -> Dict[str, Any]
        """Invalid {type} constant"""
        return locals()

    @error
    def C0009(self, position):
        # type: (Logger, ClPosition) -> Dict[str, Any]
        """Unmatched '"""
        return locals()

    @error
    def C0010(self, position):
        # type: (Logger, ClPosition) -> Dict[str, Any]
        """string contains invalid escape code"""
        return locals()

    @error
    def C0011(self, position, token, token_type):
        # type: (Logger, ClPosition, str, str) -> Dict[str, Any]
        """syntax error near token {token} ({token_type})"""
        return locals()

    @error
    def C0012(self, position, diagnostic):
        # type: (Logger, ClPosition, str) -> Dict[str, Any]
        """expected diagnostic {diagnostic} was not encountered"""
        return locals()

    @error
    def C0013(self, position, specifier):
        # type: (Logger, ClPosition, str) -> Dict[str, Any]
        """cannot combine with previous '{specifier}' declaration specifier"""
        return locals()

    @error
    def C0014(self, position):
        # type: (Logger, ClPosition) -> Dict[str, Any]
        """internal error"""
        return locals()

    @error
    def C0015(self, position):
        # type: (Logger, ClPosition) -> Dict[str, Any]
        """expected unqualified id"""
        return locals()

    @error
    def C0016(self, position):
        # type: (Logger, ClPosition) -> Dict[str, Any]
        """unexpected template specifier"""
        return locals()

    #
    # Name clashing or name resolution issue
    #
    @error
    def C0100(self, position, object_name):
        # type: (Logger, ClPosition, str) -> Dict[str, Any]
        """redefinition of object {object_name}"""
        return locals()

    @error
    def C0101(self, position, object_name):
        # type: (Logger, ClPosition, str) -> Dict[str, Any]
        """{object_name} is not a class, namespace or enumeration"""
        return locals()

    @error
    def C0102(self, position, object_name):
        # type: (Logger, ClPosition, str) -> Dict[str, Any]
        """incomplete type '{object_name}' named in nested name specifier"""
        return locals()

    @error
    def C0103(self, position, lookup, owner):
        # type: (Logger, ClPosition, str, str) -> Dict[str, Any]
        """no member named '{lookup}' in {owner}"""
        return locals()

    @error
    def C0104(self, position, name):
        # type: (Logger, ClPosition, str) -> Dict[str, Any]
        """'{name}' is not a template"""
        return locals()

    @error
    def C0105(self, position, name):
        # type: (Logger, ClPosition, str) -> Dict[str, Any]
        """template class '{name}' used without template arguments"""
        return locals()

    @error
    def C0106(self, position, name):
        # type: (Logger, ClPosition, str) -> Dict[str, Any]
        """need 'typename' before dependent scope name {name}"""
        return locals()

    @error
    def C0107(self, position, name, argument_list):
        # type: (Logger, ClPosition, str, str) -> Dict[str, Any]
        """type/value mismatch in template parameter list for template<{argument_list}> class '{name}'"""
        return locals()

    @error
    def C0108(self, position, name):
        # type: (Logger, ClPosition, str) -> Dict[str, Any]
        """use of '{name}' with tag type that does not match previous declaration"""
        return locals()

    @error
    def C0109(self, position, name, symbol_type, qualified_name):
        # type: (Logger, ClPosition, str, str, str) -> Dict[str, Any]
        """no {symbol_type} named {name} in {qualified_name}"""
        return locals()

    @error
    def C0110(self, position):
        # type: (Logger, ClPosition) -> Dict[str, Any]
        """expected class name"""
        return locals()

    @error
    def C0111(self, position, name):
        # type: (Logger, ClPosition, str) -> Dict[str, Any]
        """name '{name}' does not refer to a type"""
        return locals()

    @error
    def C0112(self, position, name):
        # type: (Logger, ClPosition, str) -> Dict[str, Any]
        """qualified name '{name}' does not refer to a method"""
        return locals()

    @error
    def C0113(self, position, name):
        # type: (Logger, ClPosition, str) -> Dict[str, Any]
        """name '{name}' does not refer to a method"""
        return locals()

    @error
    def C0114(self, position, name):
        # type: (Logger, ClPosition, str) -> Dict[str, Any]
        """{name} redeclared as different kind of symbol"""
        return locals()

    @error
    def C0115(self, position):
        # type: (Logger, ClPosition) -> Dict[str, Any]
        """expected qualified name"""
        return locals()

    #
    # Method and variable declaration/definition issue
    #
    @error
    def C0200(self, position):
        # type: (Logger, ClPosition) -> Dict[str, Any]
        """functions that differ only by return type cannot be overloaded"""
        return locals()

    @error
    def C0201(self, position):
        # type: (Logger, ClPosition) -> Dict[str, Any]
        """redefinition of default argument"""
        return locals()

    @error
    def C0202(self, position, argument_name):
        # type: (Logger, ClPosition, str) -> Dict[str, Any]
        """argument '{argument_name}' may not have void type"""
        return locals()

    @error
    def C0203(self, position):
        # type: (Logger, ClPosition) -> Dict[str, Any]
        """expected the class name after '~' to name the enclosing class"""
        return locals()

    @error
    def C0204(self, position):
        # type: (Logger, ClPosition) -> Dict[str, Any]
        """expected enclosing class name in constructor declaration"""
        return locals()

    @error
    def C0205(self, position, method_name, namespace_name):
        # type: (Logger, ClPosition, str, str) -> Dict[str, Any]
        """out-of-line definition of '{method_name}' does not match any declaration in {namespace_name}"""
        return locals()

    @error
    def C0206(self, position, method_name):
        # type: (Logger, ClPosition, str) -> Dict[str, Any]
        """'{method_name}' declared as an array of functions"""
        return locals()

    @error
    def C0207(self, position, variable_name, type_new, type_original):
        # type: (Logger, ClPosition, str, str, str) -> Dict[str, Any]
        """redefinition of '{variable_name}}' with a different type: '{type_new}' vs '{type_original}'"""
        return locals()

    @error
    def C0208(self, position):
        # type: (Logger, ClPosition) -> Dict[str, Any]
        """inline can only be used on functions"""
        return locals()

    @error
    def C0209(self, position, variable_name):
        # type: (Logger, ClPosition, str) -> Dict[str, Any]
        """'"""
        return locals()

    @error
    def C0210(self, position, variable_name):
        # type: (Logger, ClPosition, str) -> Dict[str, Any]
        """'"""
        return locals()

    #
    # Type errors
    #
    @error
    def C0300(self, position, from_type, to_type):
        # type: (Logger, ClPosition, str, str) -> Dict[str, Any]
        """type {from_type} is not compatible with {to_type}"""
        return locals()

    @error
    def C0301(self, position, pretty_name):
        # type: (Logger, ClPosition, str) -> Dict[str, Any]
        """too few template arguments for template {pretty_name}"""
        return locals()

    @error
    def C0302(self, position, pretty_name):
        # type: (Logger, ClPosition, str) -> Dict[str, Any]
        """too many template arguments for template {pretty_name}"""
        return locals()

    @error
    def C0303(self, position, parameter_name):
        # type: (Logger, ClPosition, str) -> Dict[str, Any]
        """'template argument for template parameter {parameter_name} is incompatible'"""
        return locals()

    @error
    def C0304(self, position, parameter_name, expected_type, got_type):
        # type: (Logger, ClPosition, str, str, str) -> Dict[str, Any]
        """invalid template argument for template parameter {parameter_name}; expected {expected_type}, got {got_type}"""
        return locals()

    @error
    def C0305(self, position, from_type, to_type, qualifier):
        # type: (Logger, ClPosition, str, str, str) -> Dict[str, Any]
        """"invalid cast from {from_type} to {to_type}: cannot discard '{qualifier}' qualifier"""
        return locals()

    @error
    def C0306(self, position):
        # type: (Logger, ClPosition) -> Dict[str, Any]
        """expected struct name"""
        return locals()

    @error
    def C0307(self, position):
        # type: (Logger, ClPosition) -> Dict[str, Any]
        """unions cannot have a base class"""
        return locals()

    #
    # Templates
    #
    def C0400(self, position, template_name, template_arguments):
        # type: (Logger, ClPosition, str, str) -> Dict[str, Any]
        """ambiguous partial specializations of {template_name}<{template_arguments}>"""
        return locals()

    @error
    def C0401(self, position):
        # type: (Logger, ClPosition) -> Dict[str, Any]
        """extraneous 'template<>' in parameter declaration"""
        return locals()

    @error
    def C0402(self, position):
        # type: (Logger, ClPosition) -> Dict[str, Any]
        """template specialization requires template<>"""
        return locals()

    @error
    def C0403(self, position, type_1, type_2):
        # type: (Logger, ClPosition, str, str) -> Dict[str, Any]
        """template parameter mismatch: {type_1} vs {type_2}"""
        return locals()

    #
    # Statements
    #
    def C1000(self, position, specifier):
        # type: (Logger, ClPosition, str) -> Dict[str, Any]
        """unexpected specifier: '{specifier}'"""
        return locals()

    #
    # High-level parse warnings
    #
    @warning('c-type', enabled_in=['most', 'all'])
    def W0000(self, position):
        # type: (Logger, ClPosition) -> Dict[str, Any]
        """C type used in CL kernel"""
        return locals()

    #
    # Struct
    #
    @warning('mismatched-tags', enabled_in=['most', 'all'])
    def W0100(self, position, struct_name, tag_old, tag_new):
        # type: (Logger, ClPosition, str, str, str) -> Dict[str, Any]
        """'{struct_name}' declared as {tag_new} here, but first declared as {tag_old}"""
        return locals()


logger = None

from be_typing import TYPE_CHECKING
if TYPE_CHECKING:
    from typing import Dict, Callable, TypeVar, Union, Any, List, Tuple
    from ..cl_position import ClPosition
    from argparse import ArgumentParser, Namespace, _ArgumentGroup
