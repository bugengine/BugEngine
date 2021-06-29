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


if TYPE_CHECKING:
    from argparse import ArgumentParser, Namespace
