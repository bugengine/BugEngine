from ply import lex
from .ir_position import IrPosition
from . import ir_grammar
from be_typing import TYPE_CHECKING
from copy import copy


DIGITS = set('0123456789')
ALPHABET = set('abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ_')

ID = DIGITS | ALPHABET
EXTENDED_ID = DIGITS | ALPHABET | set('-.')

KEYWORDS = set(ir_grammar.keywords)



class IrState:
    def __init__(self, token_name):
        # type: (str) -> None
        self._token_name = token_name

    def consume(self, char):
        # type: (str) -> Optional[IrState]
        raise NotImplementedError

    def token(self, lexer, start, end):
        # type: (IrLexer, int, int) -> Optional[lex.LexToken]
        result = lex.LexToken()
        result.type = self._token_name
        result.lexpos = start
        result.value = lexer._lexdata[start:end]
        return result


class IrError(IrState):
    def __init__(self):
        # type: () -> None
        IrState.__init__(self, 'ERROR')

    def consume(self, char):
        # type: (str) -> Optional[IrState]
        return None

    def token(self, lexer, start, end):
        # type: (IrLexer, int, int) -> Optional[lex.LexToken]
        token = IrState.token(self, lexer, start, end)
        assert token is not None
        lexer.logger.C0000(lexer.position(token), lexer._lexdata[start])
        return None


class IrStart(IrState):
    def __init__(self, sub_states):
        # type: (Dict[str, IrState]) -> None
        IrState.__init__(self, 'ERROR')
        self._sub_states = sub_states
        self._error_state = IrError()

    def consume(self, char):
        # type: (str) -> IrState
        return self._sub_states.get(char, self._error_state)

    def token(self, lexer, start, end):
        # type: (IrLexer, int, int) -> Optional[lex.LexToken]
        raise NotImplementedError


class IrComment(IrState):
    def __init__(self):
        # type: () -> None
        IrState.__init__(self, 'COMMENT')

    def consume(self, char):
        # type: (str) -> Optional[IrState]
        return self if char != '\n' else None
    
    def token(self, lexer, start, end):
        # type: (IrLexer, int, int) -> Optional[lex.LexToken]
        #print('comment')
        return None


class IrEOL(IrState):
    def __init__(self):
        # type: () -> None
        IrState.__init__(self, 'EOL')

    def consume(self, char):
        # type: (str) -> Optional[IrState]
        return self if char == '\n' else None

    def token(self, lexer, start, end):
        # type: (IrLexer, int, int) -> Optional[lex.LexToken]
        lexer._lineno += end - start
        return None


class IrWhitespace(IrState):
    def __init__(self):
        # type: () -> None
        IrState.__init__(self, 'WHITESPACE')

    def consume(self, char):
        # type: (str) -> Optional[IrState]
        return self if char in ' \t\r' else None

    def token(self, lexer, start, end):
        # type: (IrLexer, int, int) -> Optional[lex.LexToken]
        return None


class IrMetadataName(IrState):
    def __init__(self):
        # type: () -> None
        IrState.__init__(self, 'METADATA_NAME')

    def consume(self, char):
        # type: (str) -> Optional[IrState]
        return self if char in EXTENDED_ID else None


class IrMetadataRef(IrState):
    def __init__(self):
        # type: () -> None
        IrState.__init__(self, 'METADATA_REF')

    def consume(self, char):
        # type: (str) -> Optional[IrState]
        return self if char in DIGITS else None


class IrLabel(IrState):
    def __init__(self):
        # type: () -> None
        IrState.__init__(self, 'ID_LABEL')

    def consume(self, char):
        # type: (str) -> Optional[IrState]
        return self if char in EXTENDED_ID else None

    def token(self, lexer, start, end):
        # type: (IrLexer, int, int) -> Optional[lex.LexToken]
        result = IrState.token(self, lexer, start, end)
        if lexer._keywords_enabled:
            assert result is not None
            if result.value in KEYWORDS:
                result.type = result.value.upper()
        return result


class IrMetadata(IrState):
    def __init__(self):
        # type: () -> None
        IrState.__init__(self, 'METADATA_MARK')
        self._name = IrMetadataName()
        self._ref = IrMetadataRef()

    def consume(self, char):
        # type: (str) -> Optional[IrState]
        if char in ALPHABET:
            return self._name
        elif char in DIGITS:
            return self._ref
        else:
            return None


class IrCOMDAT(IrState):
    def __init__(self):
        # type: () -> None
        IrState.__init__(self, 'ID_COMDAT')

    def consume(self, char):
        # type: (str) -> Optional[IrState]
        return self if char in ID else None


class IrExtendedIDEnd(IrState):
    def __init__(self):
        # type: () -> None
        IrState.__init__(self, 'ID')

    def consume(self, char):
        # type: (str) -> Optional[IrState]
        return None


class IrExtendedID(IrState):
    def __init__(self):
        # type: () -> None
        IrState.__init__(self, 'ID')
        self._end = IrExtendedIDEnd()

    def consume(self, char):
        # type: (str) -> Optional[IrState]
        return self if char != '"' else self._end

    def token(self, lexer, start, end):
        # type: (IrLexer, int, int) -> Optional[lex.LexToken]
        raise NotImplementedError


class IrRegularID(IrState):
    def __init__(self):
        # type: () -> None
        IrState.__init__(self, 'ID')

    def consume(self, char):
        # type: (str) -> Optional[IrState]
        return self if char in EXTENDED_ID else None

class IrID(IrState):
    def __init__(self):
        # type: () -> None
        IrState.__init__(self, 'ID')
        self._regular = IrRegularID()
        self._extended = IrExtendedID()

    def consume(self, char):
        # type: (str) -> Optional[IrState]
        if char in EXTENDED_ID:
            return self._regular
        elif char == '"':
            return self._extended
        else:
            return None

    def token(self, lexer, start, end):
        # type: (IrLexer, int, int) -> Optional[lex.LexToken]
        temp = IrState.token(self, lexer, start, end)
        assert temp is not None
        lexer.logger.C0006(lexer.position(temp))
        return None


class IrDecimal(IrState):
    def __init__(self):
        # type: () -> None
        IrState.__init__(self, 'LITERAL_DECIMAL')

    def consume(self, char):
        # type: (str) -> Optional[IrState]
        return self if char in DIGITS else None

    def token(self, lexer, start, end):
        # type: (IrLexer, int, int) -> Optional[lex.LexToken]
        result = IrState.token(self, lexer, start, end)
        assert(result)
        result.parsed_value = int(result.value)
        return result


class IrStringEnd(IrState):
    def __init__(self):
        # type: () -> None
        IrState.__init__(self, 'LITERAL_STRING')

    def consume(self, char):
        # type: (str) -> Optional[IrState]
        return None

    def token(self, lexer, start, end):
        # type: (IrLexer, int, int) -> Optional[lex.LexToken]
        result = IrState.token(self, lexer, start, end)
        assert result is not None
        result.parsed_value = result.value[1:-1]
        return result


class IrString(IrState):
    def __init__(self):
        # type: () -> None
        IrState.__init__(self, 'LITERAL_STRING')
        self._end = IrStringEnd()
        self._skip = False

    def consume(self, char):
        # type: (str) -> Optional[IrState]
        if self._skip:
            self._skip = False
            return self
        if char == '\n':
            return None
        if char != '"':
            self._skip = char == '\\'
            return self
        else:
            return self._end

    def token(self, lexer, start, end):
        # type: (IrLexer, int, int) -> Optional[lex.LexToken]
        result = IrState.token(self, lexer, start, end)
        assert result is not None
        lexer.logger.C0005(lexer.position(result))
        return result


class IrAttributeGroup(IrState):
    def __init__(self):
        # type: () -> None
        IrState.__init__(self, 'ATTRIBUTE_GROUP')

    def consume(self, char):
        # type: (str) -> Optional[IrState]
        return self if char in DIGITS else None


class IrPunctuation(IrState):
    def __init__(self, token_name):
        # type: (str) -> None
        IrState.__init__(self, token_name)

    def consume(self, char):
        # type: (str) -> Optional[IrState]
        return None


class IrLexer:
    def __init__(self, filename, logger):
        # type: (str, Logger) -> None
        #self._lexer = lex.lex(module=ir_grammar.lex)
        #self._lexer.ir_lexer = self
        self._filename = filename
        self._lexdata = ''
        self._lineno = 1
        self._keywords_enabled = True
        self._last_token = None    # type: Optional[lex.LexToken]
        self.logger = logger
        states = {
            ';': IrComment(),
            ' ': IrWhitespace(),
            '\t': IrWhitespace(),
            '\r': IrWhitespace(),
            '\n': IrEOL(),
            '!': IrMetadata(),
            '@': IrID(),
            '%': IrID(),
            '$': IrCOMDAT(),
            '#': IrAttributeGroup(),
            '"': IrString(),
            '=': IrPunctuation('EQUAL'),
            '{': IrPunctuation('LBRACE'),
            '}': IrPunctuation('RBRACE'),
            ',': IrPunctuation('COMMA'),
            ':': IrPunctuation('COLON'),
            '[': IrPunctuation('LBRACKET'),
            ']': IrPunctuation('RBRACKET'),
            '(': IrPunctuation('LPAREN'),
            ')': IrPunctuation('RPAREN'),
            '<': IrPunctuation('LANGLE'),
            '>': IrPunctuation('RANGLE'),
            '*': IrPunctuation('STAR'),
            '|': IrPunctuation('PIPE'),
        }
        id_state = IrLabel()
        number_state = IrDecimal()
        for char in ALPHABET:
            states[char] = id_state
        for char in DIGITS:
            states[char] = number_state
        self._start_state = IrStart(states)
        self._state = self._start_state  # type: IrState

    def input(self, text):
        # type: (str) -> None
        #self._lexer.input(text)
        self._lexdata = text
        self._generator = self.input_parser()

    def input_parser(self):
        # type: () -> Generator[lex.LexToken, None, None]
        start = 0
        for end, character in enumerate(self._lexdata):
            new_state = self._state.consume(character)
            if new_state is None:
                token = self._state.token(self, start, end)
                if token is not None:
                    yield token
                start = end
                new_state = self._start_state.consume(character)
            self._state = new_state
        if self._state != self._start_state:
            token = self._state.token(self, start, end)
            if token is not None:
                yield token

    def token(self):
        # type: () -> Optional[lex.LexToken]
        if self._last_token is None or self._last_token.type != 'LPAREN':
            try:
                new_token = next(self._generator)
                #new_token = self._lexer.token()
            except StopIteration:
                return None
        else:
            new_token = copy(self._last_token)
            new_token.type = 'LPAREN_MARK'
        self._last_token = new_token
        return new_token

    def position(self, token):
        # type: (lex.LexToken) -> IrPosition
        return IrPosition(self._filename, self._lineno, token.lexpos, token.lexpos + len(token.value), self._lexdata)

    def eof_position(self):
        # type: () -> IrPosition
        return IrPosition(self._filename, self._lineno, len(self._lexdata), len(self._lexdata), self._lexdata)

    def enable_keywords(self):
        # type: () -> None
        self._keywords_enabled = True

    def disable_keywords(self):
        # type: () -> None
        self._keywords_enabled = False


if TYPE_CHECKING:
    from typing import Generator, Dict, Optional
    from .ir_messages import Logger
