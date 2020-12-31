"""
identifier:
  	identifier-nondigit     C++0x
  	identifier identifier-nondigit     C++0x
  	identifier digit
identifier-nondigit:
  	nondigit     C++0x
  	universal-character-name     C++0x                  [not implemented]
  	other implementation-defined characters     C++0x   [not implemented]
"""

identifier = '[a-zA-z_][a-zA-Z_0-9]*'