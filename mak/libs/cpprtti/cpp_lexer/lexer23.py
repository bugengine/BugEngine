from .lexer20 import CppLexer20
from .keyword import keywords_23


class CppLexer23(CppLexer20):
    keywords = CppLexer20.keywords + keywords_23
    tokens = CppLexer20.tokens + tuple('KW_' + kw.upper() for kw in keywords_23)