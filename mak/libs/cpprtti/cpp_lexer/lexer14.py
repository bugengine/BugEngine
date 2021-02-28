from .lexer11 import CppLexer11
from .keyword import keywords_14


class CppLexer14(CppLexer11):
    keywords = CppLexer11.keywords + keywords_14
    tokens = CppLexer11.tokens + tuple('KW_' + kw.upper() for kw in keywords_14)