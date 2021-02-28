from .lexer98 import CppLexer98
from .keyword import keywords_03


class CppLexer03(CppLexer98):
    keywords = CppLexer98.keywords + keywords_03
    tokens = CppLexer98.tokens + tuple('KW_' + kw.upper() for kw in keywords_03)