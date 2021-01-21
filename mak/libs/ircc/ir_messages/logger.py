import sys
import argparse
from be_typing import TypeVar, cast

T = TypeVar('T', bound='Callable[..., Dict[str, Any]]')


def diagnostic(func):
    # type: (T) -> T
    def call(self, position, *args, **kw_args):
        # type: (Logger, IrPosition, Union[str, int], Union[str, int]) -> None
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
            # type: (Logger, IrPosition, *Union[str, int], **Union[str, int]) -> None
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
        # type: (Logger, IrPosition, *Union[str, int], **Union[str, int]) -> None
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
            '{color_filename}{filename}{color_off}({line:d},{column:d}) : {color_error_type}{error_type}{color_off}: {color_message}{message}{color_off}\n',
        'unix':
            '{color_filename}{filename}{color_off}:{line:d}:{column:d}: {color_error_type}{error_type}{color_off}: {color_message}{message}{color_off}\n',
        'vi':
            '{color_filename}{filename}{color_off} +{line:d}:{column:d}: {color_error_type}{error_type}{color_off}: {color_message}{message}{color_off}\n',
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
        # type: (str, IrPosition, str) -> None
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

        if position.lexdata:
            while offset > 0 and position.lexdata[offset - 1] != '\n':
                offset -= 1
            while end < len(position.lexdata) and position.lexdata[end] != '\n':
                end += 1

        column = position.start_position - offset + 1
        sys.stderr.write(self._diagnostics_format.format(**locals()))
        if position.lexdata:
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
        # type: (IrPosition) -> Dict[str, Any]
        """first declared here"""
        return locals()

    @diagnostic
    def I0001(self, position):
        # type: (IrPosition) -> Dict[str, Any]
        """first defined here"""
        return locals()

    #
    # High-level parse errors
    #
    @error
    def C0000(self, position, char):
        # type: (IrPosition, str) -> Dict[str, Any]
        """invalid character '{char}' in input stream"""
        return locals()

    @error
    def C0001(self, position, option_context):
        # type: (IrPosition, ArgumentParser) -> Dict[str, Any]
        """invalid command line\n{usage}"""
        usage = option_context.usage
        return locals()

    @error
    def C0002(self, position):
        # type: (IrPosition) -> Dict[str, Any]
        """warning treated as error"""
        return locals()

    @error
    def C0003(self, position, token, type):
        # type: (IrPosition, str, str) -> Dict[str, Any]
        """unexpected token {token} ({type})"""
        return locals()

    @error
    def C0004(self, position):
        # type: (IrPosition) -> Dict[str, Any]
        """unexpected end of file"""
        return locals()

    @error
    def C0005(self, position):
        # type: (IrPosition) -> Dict[str, Any]
        """unmatched " before end of line"""
        return locals()

    @error
    def C0006(self, position):
        # type: (IrPosition) -> Dict[str, Any]
        """invalid ID"""
        return locals()

    @error
    def C0010(self, position, calling_convention):
        # type: (IrPosition, str) -> Dict[str, Any]
        """unsupported calling convention {calling_convention}"""
        return locals()

    @error
    def C0100(self, position, type, message):
        # type: (IrPosition, str, str) -> Dict[str, Any]
        """could not deduce address space of type {type}: {message}"""
        return locals()

    @diagnostic
    def C0101(self, position, address_space):
        # type: (IrPosition, str) -> Dict[str, Any]
        """location of deduction as '{address_space}'"""
        return locals()


logger = None

from be_typing import TYPE_CHECKING
if TYPE_CHECKING:
    from typing import Dict, Callable, TypeVar, Union, Any, List, Tuple
    from ..ir_position import IrPosition
    from argparse import ArgumentParser, Namespace, _ArgumentGroup
