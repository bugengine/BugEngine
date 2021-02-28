from .lexer14 import CppLexer14
from .keyword import keywords_17


class CppLexer17(CppLexer14):
    keywords = CppLexer14.keywords + keywords_17
    tokens = CppLexer14.tokens + tuple('KW_' + kw.upper() for kw in keywords_17)