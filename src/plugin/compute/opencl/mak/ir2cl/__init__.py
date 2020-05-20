from .cl_generator import ClGenerator
from be_typing import TYPE_CHECKING


def generators(file):
    # type: (TextIO) -> Generator[IrCodeGenerator, None, None]
    yield ClGenerator(file)


if TYPE_CHECKING:
    from typing import Generator, TextIO
    from ircc import IrCodeGenerator