from be_typing import TYPE_CHECKING
from .lr0path import LR0Path


class LR0DominanceNode:
    def __init__(self, item_set, item, predecessors, all_dominators):
        # type: (Optional[LR0ItemSet], LR0Item, Dict[LR0DominanceNode, List[int]], List[LR0DominanceNode]) -> None
        self._item_set = item_set
        self._item = item
        self._dominators = set([self])
        self._post_dominators = set([self])
        self._predecessors = predecessors
        self._direct_predecessors = set([]) # type: Set[LR0DominanceNode]
        self._successors = set([])          # type: Set[LR0DominanceNode]
        self._direct_successors = set([])   # type: Set[LR0DominanceNode]
        self._parent = None                 # type: Optional[LR0DominanceNode]
        self._children = []                 # type: List[LR0DominanceNode]
        self._extended_successors = set([]) # type: Set[LR0DominanceNode]
        for p in predecessors.items():
            p[0]._successors.add(self)
        all_dominators.append(self)

    def find_core(self, parent_state_count, seen):
        # type: (int, Set[LR0DominanceNode]) -> List[LR0Path]
        if self in seen:
            return []
        seen.add(self)
        if parent_state_count == 0:
            return [LR0Path(self)]
        else:
            result = []    # type: List[LR0Path]
            for pred, reasons in self._predecessors.items():
                if pred._item_set == self._item_set:
                    result += [r.add(self, []) for r in pred.find_core(parent_state_count, seen)]
                else:
                    result += [r.add(self, reasons) for r in pred.find_core(parent_state_count - 1, seen)]
            return result


if TYPE_CHECKING:
    from be_typing import Dict, List, Optional, Set, Tuple
    from .lr0item import LR0Item
    from .lr0itemset import LR0ItemSet