class Logger:
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

    def __init__(self, out_file):
        # type: (IO[str]) -> None
        self._out_file = out_file
        self._error_color = out_file.isatty()

    def _msg(self, error_type, message):
        # type: (str, str) -> None
        if self._error_color:
            (color_error_type, color_filename, color_message, color_caret,
             color_off) = self.COLOR_PATTERN.get(error_type, self.DEFAULT_COLOR_PATTERN)
        else:
            color_error_type = ''
            color_filename = ''
            color_message = ''
            color_caret = ''
            color_off = ''

        self._out_file.write('{color_message}{message}{color_off}\n'.format(**locals()))

    def note(self, message, *args):
        # type: (str, *Union[str, int]) -> None
        self._msg('note', message % args)

    def info(self, message, *args):
        # type: (str, *Union[str, int]) -> None
        self._msg('info', message % args)

    def warning(self, message, *args):
        # type: (str, *Union[str, int]) -> None
        self._msg('warning', message % args)

    def error(self, message, *args):
        # type: (str, *Union[str, int]) -> None
        self._msg('error', message % args)


from be_typing import TYPE_CHECKING
if TYPE_CHECKING:
    from typing import IO, Union