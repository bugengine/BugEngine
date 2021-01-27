from .ircc_c_expressions import IrccCExpressions
from .. import IrccType
from be_typing import TYPE_CHECKING

_INDENT = '    '


class IrccCDefinition(IrccCExpressions):
    def __init__(self, file):
        # type: (TextIO) -> None
        IrccCExpressions.__init__(self, file)
        self._indent = ''

    def declare_type(self, type, name, ir_name):
        # type: (IrccType, str, str) -> None
        if type._declaration[0]:
            self._out_file.write('/* %s */\n%s;\n\n' % (ir_name, type.format(['%s_' % name, '', '', ''])))

    def begin_method(self, name, return_type, parameters, calling_convention, has_definition):
        # type: (str, IrccType, List[Tuple[IrccType, str]], str, bool) -> bool
        if has_definition:
            self._out_file.write(
                '%s %s(%s)\n{\n' % (
                    return_type.format(['', '', '', '']
                                       ), name, ', '.join(t.format(['', '', n, '']) for t, n in parameters)
                )
            )
            self._indent = _INDENT
            return True
        else:
            return False

    def end_method(self):
        # type: () -> None
        self._indent = ''
        self._out_file.write('}\n\n')

    def declare_variable(self, name, type):
        # type: (str, IrccType) -> None
        self._out_file.write('%s%s;\n' % (self._indent, type.format(['', '', name, ''])))

    def declare_label(self, name):
        # type: (str) -> None
        self._out_file.write('%s/* %s */\n' % (self._indent, name))

    def begin_loop(self, label):
        # type: (str) -> None
        indent = self._indent
        self._out_file.write('%s\n%sfor (;;)\n%s{\n' % (indent, indent, indent))
        self._indent += _INDENT

    def end_loop(self):
        # type: () -> None
        self._indent = self._indent[:-len(_INDENT)]
        self._out_file.write('%s}\n' % (self._indent))

    def begin_if(self, condition):
        # type: (IrccExpression) -> None
        indent = self._indent
        self._out_file.write('%sif (%s)\n%s{\n' % (indent, condition, indent))
        self._indent += _INDENT

    def begin_if_not(self, condition):
        # type: (IrccExpression) -> None
        indent = self._indent
        self._out_file.write('%sif (!(%s))\n%s{\n' % (indent, condition, indent))
        self._indent += _INDENT

    def begin_else(self):
        # type: () -> None
        indent = self._indent[:-len(_INDENT)]
        self._out_file.write('%s}\n%selse\n%s{\n' % (indent, indent, indent))

    def end_if(self):
        # type: () -> None
        self._indent = self._indent[:-len(_INDENT)]
        self._out_file.write('%s}\n' % (self._indent))

    def instruction_return_value(self, return_value):
        # type: (IrccExpression) -> None
        self._out_file.write('%sreturn %s;\n' % (self._indent, return_value))

    def instruction_break(self, label):
        # type: (str) -> None
        self._out_file.write('%sbreak; /* goto %s */\n' % (self._indent, label))

    def instruction_continue(self, label):
        # type: (str) -> None
        self._out_file.write('%scontinue; /* goto %s */\n' % (self._indent, label))

    def instruction_assign(self, name, value):
        # type: (str, IrccExpression) -> None
        self._out_file.write('%svar_%s = %s;\n' % (self._indent, name.replace('.', '_'), value))


if TYPE_CHECKING:
    from typing import List, TextIO, Tuple
    from .. import IrccType, IrccExpression