from .production import Production
from .grammar import Grammar
from ..lex import Token
from be_typing import TYPE_CHECKING, TypeVar
from abc import abstractmethod
import os
import re
import hashlib
import zlib
import base64
try:
    import cPickle as pickle
except ImportError:
    import pickle  # type: ignore

LOAD_OPTIMIZED = 0
GENERATE = 1
LOAD_CACHE = 2


class Action(object):
    def __init__(self):
        # type: () -> None
        pass

    #@abstractmethod
    def __call__(self, parser):
        # type: (Parser) -> Callable[[Production], None]
        raise NotImplementedError


class _DirectAction(Action):
    def __init__(self, action_name):
        # type: (str) -> None
        Action.__init__(self)
        self._action_name = action_name

    def __call__(self, parser):
        # type: (Parser) -> Callable[[Production], None]
        return getattr(parser, self._action_name)


class _OptionalAction(Action):
    def __init__(self, action, optional_index):
        # type: (Action, int) -> None
        Action.__init__(self)
        self._action = action
        self._index = optional_index

    def __call__(self, parser):
        # type: (Parser) -> Callable[[Production], None]
        action = self._action(parser)

        def call(production):
            # type: (Production) -> Any
            production._insert(self._index, Token(1, '<empty>', production._position, None, []))
            action(production)

        return call


class _AcceptAction(Action):
    def __init__(self):
        # type: () -> None
        Action.__init__(self)

    def __call__(self, parser):
        # type: (Parser) -> Callable[[Production], None]
        return parser.accept


class Parser(object):
    AcceptAction = _AcceptAction

    class Stack(object):
        def __init__(self, stack, parent=None):
            # type: (List[Symbol], Optional[Parser.Stack]) -> None
            self._parent = parent
            self._stack = []   # type: List[Symbol]

        def __eq__(self, other):
            # type: (object) -> bool
            if isinstance(other, Parser.Stack):
                if self._parent == other._parent and len(self._stack) == len(other._stack):
                    for i1, i2 in zip(self._stack, other._stack):
                        if i1._id != i2._id:
                            break
                    else:
                        return True
            return False

        def clone(self):
            # type: () -> Parser.Stack
            return Parser.Stack(self._stack[:], self)

        def shift(self, symbol):
            # type: (Symbol) -> None
            self._stack.append(symbol)

        def reduce(self, prod_id, prod_name, prod_length, action):
            # type: (int, str, int, Callable[[Production], None]) -> None
            prod = self._stack[-prod_length:]
            self._stack[-prod_length:] = [Production(prod_id, prod_name, prod, action)]

    def __init__(self, lexer, start_symbol, temp_directory, output_directory, mode=LOAD_CACHE):
        # type: (Lexer, str, str, str, int) -> None
        self._lexer = lexer

        if mode == LOAD_OPTIMIZED:
            self._grammar = self._load_table(output_directory)
        else:
            h = hashlib.md5()
            for rule_action in dir(self):
                action = getattr(self, rule_action)
                for rule_string, _, _ in getattr(action, 'rules', []):
                    h.update(rule_string.encode())
            if mode == LOAD_CACHE:
                try:
                    self._grammar = self._load_table(output_directory)
                except Exception:
                    generate = True
                else:
                    generate = self._grammar._hash != h.hexdigest()
            else:
                generate = True
            if generate:
                self._grammar = self._generate_table(h.hexdigest(), start_symbol, temp_directory, output_directory)

    def _load_table(self, output_directory):
        # type: (str) -> Grammar
        with open(os.path.join(output_directory, self.__class__.__name__ + '.tbl'), 'rb') as table_file:
            return pickle.loads(zlib.decompress(base64.b64decode(table_file.read())))

    def _generate_table(self, rule_hash, start_symbol, temp_directory, output_directory):
        # type: (str, str, str, str) -> Grammar
        rules = []     # type: List[Tuple[str, Action, List[str], List[Tuple[str, List[str], int]], str, int]]
        for rule_action in dir(self):
            action = getattr(self, rule_action)
            for rule_string, filename, lineno in getattr(action, 'rules', []):
                rules += _parse_rule(rule_string, rule_action, filename, lineno)

        grammar = Grammar(
            self.__class__.__name__, rule_hash, self._lexer._terminals, rules, start_symbol, self, temp_directory
        )
        with open(os.path.join(output_directory, self.__class__.__name__ + '.tbl'), 'wb') as table_file:
            table_file.write(base64.b64encode(zlib.compress(pickle.dumps(grammar, protocol=0), 9)))
        return self._load_table(output_directory)

    def parse(self, filename):
        # type: (str) -> Any
        self._lexer.input(filename)

        s = Parser.Stack([])
        for token in self._lexer.token():
            s.shift(token)

    def accept(self, p):
        # type: (Production) -> None
        p[0] = p[1]


_id = r'([a-zA-Z_\-][a-zA-Z_\-0-9]*)'
_value = r'[a-zA-Z_\-0-9]+(:?\s*,\s*[a-zA-Z_\-0-9]+)*'
_single_quote = r"(?:'([^']*)')"
_double_quote = r'(?:"([^"]*)")'

_rule_id = re.compile(r'\s*%s' % _id, re.MULTILINE)
_rule_annotation = re.compile(r'\s*(:)\s*|\s*\[\s*%s\s*(\:\s*%s\s*)?\]' % (_id, _value), re.MULTILINE)
_production = re.compile(
    r'%s(\??)\s*|%s(\??)\s*|%s(\??)\s*|(\|)\s*|(;)\s*|($)|\[\s*%s\s*(\:\s*%s\s*)?\]\s*' %
    (_id, _single_quote, _double_quote, _id, _value), re.MULTILINE
)


def _parse_rule(rule_string, action, filename, lineno):
    # type: (str, str, str, int) -> List[Tuple[str, Action, List[str], List[Tuple[str, List[str], int]], str, int]]
    result = []    # type: List[Tuple[str, Action, List[str], List[Tuple[str, List[str], int]], str, int]]

    if rule_string:
        m = _rule_id.match(rule_string)
        if m is None:
            raise SyntaxError('unable to parse rule', (filename, lineno, 0, rule_string))
        id = m.group(1)

        annotations = []               # type: List[Tuple[str, List[str], int]]
        while m is not None:
            parse_start = m.end()
            m = _rule_annotation.match(rule_string, m.end())
            if m is None:
                raise SyntaxError('unable to parse rule', (filename, lineno, parse_start, rule_string))
            if m.lastindex == 1:       # :
                break
            elif m.lastindex == 2:     # annotation
                annotation = m.group(2)
                annotations.append((annotation, [], -1))
            elif m.lastindex == 3:     # annotation=value
                annotation = m.group(2)
                values = [v.strip() for v in m.group(3)[1:].split(',')]
                annotations.append((annotation, values, -1))

        while True:
            productions = [
                (_DirectAction(action), [], annotations[:])
            ]                                               # type: List[Tuple[Action, List[str], List[Tuple[str, List[str], int]]]]

            while m is not None:
                parse_start = m.end()
                m = _production.match(rule_string, m.end())
                if m is None:
                    raise SyntaxError('unable to parse rule', (filename, lineno, parse_start, rule_string))
                assert m.lastindex is not None
                if m.lastindex < 7:
                    for p in productions:
                        p[1].append(m.group(m.lastindex - 1))
                    if m.group(m.lastindex) == '?':
                        productions += [(_OptionalAction(p[0], len(p[1]) - 1), p[1][:-1], p[2][:]) for p in productions]
                    continue
                elif m.lastindex == 7: # |
                    for p in productions:
                        result.append((id, p[0], p[1], p[2], filename, lineno))
                    break
                elif m.lastindex == 8: # ;
                    for p in productions:
                        result.append((id, p[0], p[1], p[2], filename, lineno))
                    rule_string = rule_string[m.end():]
                    if rule_string:
                        m = _rule_id.match(rule_string)
                        if m is None:
                            raise SyntaxError('unable to parse rule', (filename, lineno, 0, rule_string))
                        id = m.group(1)

                        annotations = []
                        while m is not None:
                            parse_start = m.end()
                            m = _rule_annotation.match(rule_string, m.end())
                            if m is None:
                                raise SyntaxError('unable to parse rule', (filename, lineno, parse_start, rule_string))
                            if m.lastindex == 1:   # :
                                break
                            elif m.lastindex == 2: # annotation
                                annotation = m.group(2)
                                annotations.append((annotation, [], -1))
                            elif m.lastindex == 3: # annotation=value
                                annotation = m.group(2)
                                values = [v.strip() for v in m.group(3)[1:].split(',')]
                                annotations.append((annotation, values, -1))
                        break
                    else:
                        return result
                elif m.lastindex == 9:             # $
                    for p in productions:
                        result.append((id, p[0], p[1], p[2], filename, lineno))
                    return result
                elif m.lastindex == 10:            # annotation
                    annotation = m.group(10)
                    for p in productions:
                        p[2].append((annotation, [], len(p[1])))
                elif m.lastindex == 11:            # annotation=value
                    annotation = m.group(10)
                    values = [v.strip() for v in m.group(11)[1:].split(',')]
                    for p in productions:
                        p[2].append((annotation, values, len(p[1])))

    return result


T = TypeVar('T', bound=Parser)


def rule(rule_string):
    # type: (str) -> Callable[[Callable[[T, Production], None]], Callable[[T, Production], None]]
    def attach(method):
        # type: (Callable[[T, Production], None]) -> Callable[[T, Production], None]
        if not hasattr(method, 'rules'):
            setattr(method, 'rules', [])
        code = method.__code__
        getattr(method, 'rules').append((rule_string, code.co_filename, code.co_firstlineno))
        return method

    return attach


if TYPE_CHECKING:
    from typing import Any, Callable, List, Optional, Tuple, Type
    from ..lex import Lexer
    from ..symbol import Symbol