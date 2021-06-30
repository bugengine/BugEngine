from . import parser
import sys


def run():
    # type: () -> None
    try:
        parser.Parser1()
        parser.Parser2()
        parser.Parser3()
        parser.Parser4()
    except SyntaxError as exception:
        sys.exit(255)
