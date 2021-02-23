"""
preprocessing-token:
  	header-name
  	identifier
  	pp-number
  	character-literal                           [not post-preprocessor]
  	user-defined-character-literal     C++0x    [not post-preprocessor]
  	string-literal                              [not post-preprocessor]
  	user-defined-string-literal     C++0x       [not post-preprocessor]
  	preprocessing-op-or-punc                    [not post-preprocessor]
"""
from be_typing import TYPE_CHECKING


def t_preprocessor(t):
    # type: (LexToken) -> Optional[LexToken]
    r'\#([^\\\n]|(\\.)|(\\\n))*'
    t.lexer.lineno += t.value.count('\n')
    #if t.value.find('include') != -1:
    #    t.lexer.includes.append(t.value)
    return None


if TYPE_CHECKING:
    from typing import Optional
    from ply.lex import LexToken