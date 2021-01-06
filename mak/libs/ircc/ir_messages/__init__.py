import sys
from .logger import Logger
from be_typing import TYPE_CHECKING


def init_arguments(argument_context):
    # type: (ArgumentParser) -> None
    group = argument_context.add_argument_group('Diagnostics options')
    Logger.init_diagnostic_flags(group)


def load_arguments(argument_context):
    # type: (Namespace) -> Logger
    logger = Logger(argument_context)
    return logger


class IrAddressSpaceResolutionError(Exception):
    def __init__(self, position, type, message, parent=None):
        # type: (IrPosition, str, str, Optional[IrAddressSpaceResolutionError]) -> None
        Exception.__init__(self, message)
        self.position = position
        self.type = type
        self.parent = parent


if TYPE_CHECKING:
    from be_typing import Optional
    from argparse import ArgumentParser, Namespace
    from ..ir_position import IrPosition
