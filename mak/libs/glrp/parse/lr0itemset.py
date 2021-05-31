from .lr0item import LR0Item
from .lr0dominancenode import LR0DominanceNode
from be_typing import TYPE_CHECKING


class LR0ItemSet:
    def __init__(self, core, all_dominators):
        # type: (List[Tuple[LR0Item, LR0DominanceNode, int]], List[LR0DominanceNode]) -> None
        self._core = {}    # type: Dict[LR0Item, Tuple[LR0DominanceNode, List[int]]]
        self._items = {}   # type: Dict[LR0Item, LR0DominanceNode]
        self.add_caller(core, all_dominators)
        self._close(all_dominators)

    def __iter__(self):
        # type: () -> Iterator[LR0Item]
        return iter(self._items)

    def to_strings(self, name_map):
        # type: (List[str]) -> List[str]
        return [i.to_string(name_map) for i in self._items]

    def add_caller(self, core, all_dominators):
        # type: (List[Tuple[LR0Item, LR0DominanceNode, int]], List[LR0DominanceNode]) -> None
        for item, node, lookahead in core:
            try:
                target_node, lookaheads = self._core[item]
            except KeyError:
                target_node = LR0DominanceNode(self, item, {node: [lookahead]}, all_dominators)
                self._core[item] = (target_node, [lookahead])
                self._items[item] = target_node
            else:
                lookaheads.append(lookahead)
                try:
                    target_node._predecessors[node].append(lookahead)
                except KeyError:
                    target_node._predecessors[node] = [lookahead]
                    node._successors.add(target_node)

    def _close(self, all_dominators):
        # type: (List[LR0DominanceNode]) -> None
        item_set_id = id(self)
        size = 0
        new_items = self._items
        self._items = {}
        while new_items:
            self._items.update(new_items)
            new_items = {}
            for item, dn in self._items.items():
                for x in item._after:
                    try:
                        successor = self._items[x._item]
                    except KeyError:
                        try:
                            successor = new_items[x._item]
                        except KeyError:
                            successor = LR0DominanceNode(self, x._item, {dn: []}, all_dominators)
                            new_items[x._item] = successor
                    dn._successors.add(successor)
                    successor._predecessors[dn] = []
                    if successor not in dn._direct_successors:
                        dn._direct_successors.add(successor)
                        dn._direct_successors.update(successor._direct_successors)
                        for node in dn._direct_predecessors:
                            node._direct_successors.add(successor)
                            node._direct_successors.update(successor._direct_successors)

                        successor._direct_predecessors.add(dn)
                        successor._direct_predecessors.update(dn._direct_predecessors)
                        for node in successor._direct_successors:
                            node._direct_predecessors.add(dn)
                            node._direct_predecessors.update(dn._direct_predecessors)
                    else:
                        assert len(successor._predecessors[dn]) == 0


if TYPE_CHECKING:
    from typing import Any, Callable, Dict, Iterator, List, Optional, Set, Tuple
