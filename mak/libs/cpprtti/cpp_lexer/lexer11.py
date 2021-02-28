from .lexer03 import CppLexer03
from .keyword import keywords_11
from be_typing import TYPE_CHECKING


class CppLexer11(CppLexer03):
    keywords = CppLexer03.keywords + keywords_11
    tokens = CppLexer03.tokens + ('LDOUBLEBRACKET', ) + tuple('KW_' + kw.upper() for kw in keywords_11)

    def token(self):
        # type: () -> Optional[LexToken]
        token = CppLexer03.token(self)
        if token is not None:
            if token.type == 'LBRACKET':
                if getattr(self.look_ahead(0), 'type', '') == 'LBRACKET':
                    token.type = 'LDOUBLEBRACKET'
        return token


if TYPE_CHECKING:
    from typing import Optional
    from ply.lex import LexToken