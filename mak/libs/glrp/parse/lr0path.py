from be_typing import TYPE_CHECKING


class LR0Path:
    def __init__(self, root_node):
        # type: (LR0DominanceNode) -> None
        self._node = root_node
        self._nodes = [(root_node, [])] # type: List[Tuple[LR0DominanceNode, List[int]]]

    def add(self, node, lookahead_list):
        # type: (LR0DominanceNode, List[int]) -> LR0Path
        self._nodes.append((node, lookahead_list))
        return self

    def extend(self, path):
        # type: (LR0Path) -> LR0Path
        self._nodes += path._nodes[1:]
        return self

    def log(self, logger, name_map):
        # type: (Logger, List[str]) -> None
        str = '->'
        indent = 0
        for node, lookaheads in self._nodes:
            if len(lookaheads) > 5:
                la = ','.join(['"%s"' % name_map[i] for i in lookaheads[:4]] + ['...'])
                str += ' ' + la
                indent += len(la)
            elif len(lookaheads) > 0:
                la = ','.join(['"%s"' % name_map[i] for i in lookaheads])
                str += ' ' + la
                indent += len(la)
            else:
                indent = len(str)
                str += ' ' + node._item._rule._prod_name
                logger.info(str)
                str = ' ' * indent + ' ` '
        logger.info(str)
        return


if TYPE_CHECKING:
    from be_typing import List, Tuple
    from .lr0dominancenode import LR0DominanceNode
    from ..log import Logger
