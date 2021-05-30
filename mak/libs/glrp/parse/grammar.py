from ..log import Logger
from be_typing import TYPE_CHECKING
import os
import sys


class Grammar(object):
    class Rule:
        def __init__(self, id, prod_index, prod_name, production, action, annnotation_list, filename, lineno):
            # type: (int, int, str, List[int], Parser.Action, List[Tuple[str, List[str], int]], str, int) -> None
            self._id = id
            self._prod_index = prod_index
            self._prod_name = prod_name
            self._production = production
            self._action = action
            self._filename = filename
            self._lineno = lineno
            predecessor = self._production[-1] if len(self._production) else None
            self._annotations = {}     # type: Dict[int, Dict[str, List[str]]]
            for annotation, values, index in annnotation_list:
                if index == len(production):
                    index = -1
                try:
                    self._annotations[index][annotation] = values
                except KeyError:
                    self._annotations[index] = {annotation: values}
            self._item = Grammar.LR0Item(self, len(production), None, predecessor, [], set([-1]))
            self._reduced = 0

        def to_string(self, name_map):
            # type: (List[str]) -> str
            return '%s -> %s' % (name_map[self._prod_index], ' '.join([name_map[p] for p in self._production]))

        def __iter__(self):
            # type: () -> Iterator[int]
            return iter(self._production)

        def __getitem__(self, index):
            # type: (int) -> int
            return self._production[index]

        def __len__(self):
            # type: () -> int
            return len(self._production)

    class Production:
        def __init__(self, prod_index, rule_list):
            # type: (int, List[Grammar.Rule]) -> None
            self._rule_list = rule_list
            self._prod_index = prod_index
            self._first = set([])  # type: Set[int]
            self._empty = False
            self._nonterminal_count = 0
            for rule in rule_list:
                if len(rule._production) == 0:
                    self._empty = True

        def add_rule(self, rule):
            # type: (Grammar.Rule) -> None
            self._rule_list.append(rule)
            if len(rule._production) == 0:
                self._empty = True

        def __iter__(self):
            # type: () -> Iterator[Grammar.Rule]
            return iter(self._rule_list)

        def __getitem__(self, index):
            # type: (int) -> Grammar.Rule
            return self._rule_list[index]

        def __len__(self):
            # type: () -> int
            return len(self._rule_list)

    class LR0Item:
        class DominatorNode:
            def __init__(self, item_set_id, item, predecessors, all_dominators):
                # type: (int, Grammar.LR0Item, List[Grammar.LR0Item.DominatorNode], List[Grammar.LR0Item.DominatorNode]) -> None
                self._item_set = item_set_id
                self._item = item
                self._dominators = set([self])
                self._post_dominators = set([self])
                self._predecessors = predecessors
                self._direct_predecessors = set([]) # type: Set[Grammar.LR0Item.DominatorNode]
                self._successors = set([])          # type: Set[Grammar.LR0Item.DominatorNode]
                self._direct_successors = set([])   # type: Set[Grammar.LR0Item.DominatorNode]
                self._parent = None                 # type: Optional[Grammar.LR0Item.DominatorNode]
                self._children = []                 # type: List[Grammar.LR0Item.DominatorNode]
                self._extended_successors = set([]) # type: Set[Grammar.LR0Item.DominatorNode]
                for p in predecessors:
                    p._successors.add(self)
                all_dominators.append(self)

            def find_core(self, parent_state_count, seen):
                # type: (int, Set[Grammar.LR0Item.DominatorNode]) -> List[Grammar.LR0Item.DominatorNode]
                if self in seen:
                    return []
                seen.add(self)
                result = []    # type: List[Grammar.LR0Item.DominatorNode]
                if parent_state_count == 0:
                    result.append(self)
                else:
                    for pred in self._predecessors:
                        if pred._item_set == self._item_set:
                            result += pred.find_core(parent_state_count, seen)
                        else:
                            result += pred.find_core(parent_state_count - 1, seen)
                return result

        def __init__(self, rule, index, next, predecessor, successors, follow):
            # type: (Grammar.Rule, int, Optional[Grammar.LR0Item], Optional[int], List[Grammar.Rule], Set[int]) -> None
            self._rule = rule
            self._index = index
            self._next = next
            self._before = predecessor
            self._after = successors
            self._symbols = set(rule._production)
            self._follow = follow
            self._lookaheads = {}      # type: Dict[int, List[int]]
            self._precedence = None    # type: Optional[Tuple[str, int]]
            self._split = False
            self._dominators = {}      # type: Dict[int, Grammar.LR0Item.DominatorNode]

            if index == len(rule):
                index = -1
            annotations = rule._annotations.get(index, {})
            for annotation, values in annotations.items():
                if annotation == "prec":
                    if len(values) == 1:
                        try:
                            precedence = int(values[0])
                        except ValueError:
                            raise SyntaxError(
                                'incorrect precedence: value should be an integer or a pair (string,integer), got %s' %
                                values[0], (rule._filename, rule._lineno, 0, '')
                            )
                        else:
                            self._precedence = ('nonassoc', precedence)
                    elif len(values) == 2:
                        if values[0] not in ('left', 'right', 'nonassoc'):
                            raise SyntaxError(
                                'incorrect precedence: value should be an integer or a pair (string,integer), got %s' %
                                ', '.join(values), (rule._filename, rule._lineno, 0, '')
                            )
                        try:
                            precedence = int(values[1])
                        except ValueError:
                            raise SyntaxError(
                                'incorrect precedence: value should be an integer or a pair (string,integer), got %s' %
                                ', '.join(values), (rule._filename, rule._lineno, 0, '')
                            )
                        else:
                            self._precedence = (values[0], precedence)
                    else:
                        raise SyntaxError(
                            'incorrect precedence: value should be an integer or a pair (string,integer), got %s' %
                            ', '.join(values), (rule._filename, rule._lineno, 0, '')
                        )
                elif annotation == "split":
                    if len(values) != 0:
                        raise SyntaxError(
                            'incorrect annotation: split does not take any argument, got %s' % ','.join(values),
                            (rule._filename, rule._lineno, 0, '')
                        )
                    self._split = True
                else:
                    raise SyntaxError('unknown annotation %s' % annotation, (rule._filename, rule._lineno, 0, ''))

        def _annotations(self):
            # type: () -> str
            result = ''
            if self._precedence is not None:
                result += '[prec:%s,%d]' % self._precedence
            return result

        def to_string(self, name_map):
            # type: (List[str]) -> str
            return '%s -> %s' % (
                name_map[self._rule._prod_index], ' '.join(
                    [name_map[p] for p in self._rule._production[:self._index]] + ['.%s' % self._annotations()] +
                    [name_map[p] for p in self._rule._production[self._index:]]
                )
            )

        def is_reduction_item(self):
            # type: () -> bool
            return self._index == len(self._rule)

    class LR0ItemSet:
        def __init__(self, core, all_dominators):
            # type: (List[Tuple[Grammar.LR0Item, Grammar.LR0Item.DominatorNode]], List[Grammar.LR0Item.DominatorNode]) -> None
            self._core = set(c for c, _ in core)
            self._items = list(self._core)
            self.add_caller(core, all_dominators)
            self._close(all_dominators)

        def __iter__(self):
            # type: () -> Iterator[Grammar.LR0Item]
            return iter(self._items)

        def to_strings(self, name_map):
            # type: (List[str]) -> List[str]
            return [i.to_string(name_map) for i in self._items]

        def add_caller(self, core, all_dominators):
            # type: (List[Tuple[Grammar.LR0Item, Grammar.LR0Item.DominatorNode]], List[Grammar.LR0Item.DominatorNode]) -> None
            item_set_id = id(self)
            for item, node in core:
                try:
                    target_node = item._dominators[item_set_id]
                except KeyError:
                    item._dominators[item_set_id] = Grammar.LR0Item.DominatorNode(
                        item_set_id, item, [node], all_dominators
                    )
                else:
                    if node not in target_node._predecessors:
                        target_node._predecessors.append(node)
                        node._successors.add(item._dominators[item_set_id])

        def _close(self, all_dominators):
            # type: (List[Grammar.LR0Item.DominatorNode]) -> None
            item_set_id = id(self)
            size = 0
            while size != len(self._items):
                old_size = len(self._items)
                for item in self._items[size:]:
                    dn = item._dominators[item_set_id]
                    for x in item._after:
                        try:
                            successor = x._item._dominators[item_set_id]
                        except KeyError:
                            successor = x._item._dominators[item_set_id] = Grammar.LR0Item.DominatorNode(
                                item_set_id, x._item, [dn], all_dominators
                            )
                            self._items.append(x._item)
                        dn._successors.add(successor)
                        if successor not in dn._direct_successors:
                            dn._direct_successors.add(successor)
                            dn._direct_successors.update(successor._direct_successors)
                            for node in dn._direct_predecessors:
                                node._direct_successors.add(successor)
                                node._direct_successors.update(successor._direct_successors)

                            successor._predecessors.append(dn)
                            successor._direct_predecessors.add(dn)
                            successor._direct_predecessors.update(dn._direct_predecessors)
                            for node in successor._direct_successors:
                                node._direct_predecessors.add(dn)
                                node._direct_predecessors.update(dn._direct_predecessors)
                size = old_size

    def __init__(self, terminals, rules, start_symbol, parser, tab_filename, debug_filename):
        # type: (Dict[str, int], List[Tuple[str, Parser.Action, List[str], List[Tuple[str, List[str], int]], str, int]], str, Parser, str, str) -> None
        index = dict(terminals)
        name_map = [''] * (1 + len(terminals))
        log = Logger(open(debug_filename, 'w'))
        stderr = Logger(sys.stderr)
        dot_file = Logger(open(os.path.splitext(debug_filename)[0] + '.dot', 'w'))
        for name, i in terminals.items():
            name_map[i] = name

        rules = rules + [
            ("%s'" % start_symbol, parser.__class__.AcceptAction(), [start_symbol, name_map[0]], [], '', 1)
        ]
        for nonterminal, _, _, _, _, _ in rules:
            if nonterminal not in index:
                i = 1 + len(index)
                index[nonterminal] = i
                name_map.append(nonterminal)
                assert name_map[i] == nonterminal

        name_map.append('<epsilon>')

        start_id = len(index)
        productions = _create_productions(rules, index, log)
        for prod_index, prod in productions.items():
            log.info('%d %s {%s}', prod_index, name_map[prod_index], ', '.join([name_map[t] for t in prod._first]))
            for rule in prod:
                log.info('  %s', rule.to_string(name_map))

        _create_lr0_items(productions)
        _create_parser_table(productions, start_id, name_map, len(terminals), log, stderr, dot_file)

        #with open('cxx.y', 'w') as yaccfile:
        #    for index, terminal in enumerate(name_map[2:1 + len(terminals)]):
        #        yaccfile.write('%%token _%d "%s"\n' % (index - 1, terminal))
        #    yaccfile.write('\n%%start %s\n\n%%%%\n' % start_symbol)
        #    for prod_index, prod in productions.items():
        #        print(name_map[prod_index])
        #        if prod_index == start_id:
        #            continue
        #        for rule in prod:
        #            if len(rule) == 0:
        #                yaccfile.write('\n%s: ;\n' % (name_map[prod_index]))
        #            else:
        #                yaccfile.write(
        #                    '\n%s:\n        %s\n    ;\n' % (
        #                        name_map[prod_index],
        #                        ' '.join(name_map[i] if i > len(terminals) else '"%s"' % name_map[i] for i in rule)
        #                    )
        #                )
        #    yaccfile.write('\n%%%%\n')


def _create_productions(rules, index, log):
    # type: (List[Tuple[str, Parser.Action, List[str], List[Tuple[str, List[str], int]], str, int]], Dict[str, int], Logger) -> Dict[int, Grammar.Production]
    rule_index = 0
    productions = {}   # type: Dict[int, Grammar.Production]

    for nonterminal, action, production, attribute_list, filename, lineno in rules:
        prod_index = index[nonterminal]
        try:
            rule = Grammar.Rule(
                rule_index, prod_index, nonterminal, [index[s] for s in production], action, attribute_list, filename,
                lineno
            )
        except KeyError as error:  # unknown rule or terminal
            log.error('unknown object used in rule: %s', str(error))
        else:
            rule_index += 1
            try:
                productions[prod_index].add_rule(rule)
            except KeyError:
                productions[prod_index] = Grammar.Production(prod_index, [rule])

    queue = list(productions.keys())
    while queue:
        prod_index = queue.pop(0)
        prod = productions[prod_index]
        len_set_before = len(prod._first)
        if prod._empty:
            prod._first.add(-1)
        for rule in prod:
            i = 0
            found_epsilon = True
            while found_epsilon:
                found_epsilon = False
                try:
                    t = rule[i]
                except IndexError:
                    break
                else:
                    i += 1
                    try:
                        p = productions[t]
                    except KeyError:
                        prod._first.add(t)
                    else:
                        for t in p._first:
                            if t != -1:
                                prod._first.add(t)
                            else:
                                found_epsilon = True
        len_set_after = len(prod._first)
        if len_set_before != len_set_after or len_set_after == 0:
            queue.append(prod_index)

    return productions


def _create_lr0_items(productions):
    # type: (Dict[int, Grammar.Production]) -> None
    for _, prods in productions.items():
        for rule in prods:
            follow = set([-1])
            i = len(rule) - 1
            while i >= 0:
                try:
                    production = productions[rule._production[i]]
                except KeyError:
                    # terminal
                    next = []
                    follow_buffer = set([rule._production[i]])
                else:
                    next = list(production)
                    follow_buffer = production._first
                    if -1 in follow_buffer:
                        follow_buffer = set(follow_buffer)
                        follow_buffer.remove(-1)
                        follow_buffer = follow_buffer.union(follow)
                predecessor = rule._production[i - 1] if i > 0 else None
                rule._item = Grammar.LR0Item(rule, i, rule._item, predecessor, next, follow)
                follow = follow_buffer
                i -= 1


def _create_parser_table(productions, start_id, name_map, terminal_count, log, error_log, dot_file):
    # type: (Dict[int, Grammar.Production], int, List[str], int, Logger, Logger, Logger) -> None
    cidhash = {}
    goto_cache = {}        # type: Dict[Tuple[int, int], Optional[Grammar.LR0ItemSet]]
    goto_cache_2 = {}      # type: Dict[int, Any]
    dominator_nodes = []   # type: List[Grammar.LR0Item.DominatorNode]

    def goto(item_set, lookahead):
        # type: (Grammar.LR0ItemSet, int) -> Optional[Grammar.LR0ItemSet]
        # First we look for a previously cached entry
        item_set_id = id(item_set)
        result = goto_cache.get((item_set_id, lookahead), None)
        if result is not None:
            return result

        s = goto_cache_2.get(lookahead)
        if not s:
            s = {}
            goto_cache_2[lookahead] = s

        gs = []
        for item in item_set:
            next = item._next
            if next and next._before == lookahead:
                s1 = s.get(id(next))
                if not s1:
                    s1 = {}
                    s[id(next)] = s1
                gs.append((next, item._dominators[item_set_id]))
                s = s1

        result = s.get(0, None)
        if result is None:
            if gs:
                result = Grammar.LR0ItemSet(gs, dominator_nodes)
                s[0] = result
            else:
                s[0] = None
        else:
            result.add_caller(gs, dominator_nodes)

        goto_cache[(item_set_id, lookahead)] = result
        return result

    def create_item_sets():
        # type: () -> Tuple[List[Grammar.LR0Item.DominatorNode], List[Grammar.LR0ItemSet]]
        assert len(productions[start_id]) == 1
        assert len(dominator_nodes) == 0
        root_node = Grammar.LR0Item.DominatorNode(0, productions[start_id][0]._item, [], dominator_nodes)
        states = [Grammar.LR0ItemSet([(productions[start_id][0]._item, root_node)], dominator_nodes)]
        cidhash[id(states[0])] = 0

        # Loop over the items in C and each grammar symbols
        i = 0
        while i < len(states):
            state = states[i]
            i += 1

            # Collect all of the symbols that could possibly be in the goto(I,X) sets
            asyms = set([])
            for item in state:
                for s in item._symbols:
                    asyms.add(s)

            for x in asyms:
                g = goto(state, x)
                if not g or id(g) in cidhash:
                    continue
                cidhash[id(g)] = len(states)
                states.append(g)

        return dominator_nodes, states

    def add_lalr_lookahead(states):
        # type: (List[Grammar.LR0ItemSet]) -> None

        def traverse(x, N, stack, F, X, R, FP):
            # type: (Tuple[int, int], Dict[Tuple[int, int], int], List[Tuple[int, int]], Dict[Tuple[int, int], List[int]], List[Tuple[int, int]], Callable[[Tuple[int, int]], List[Tuple[int, int]]], Callable[[Tuple[int, int]], List[int]]) -> None
            stack.append(x)
            d = len(stack)
            N[x] = d
            F[x] = FP(x)   # F(X) <- F'(x)

            rel = R(x)     # Get y's related to x
            for y in rel:
                if N[y] == 0:
                    traverse(y, N, stack, F, X, R, FP)
                N[x] = min(N[x], N[y])
                for a in F.get(y, []):
                    if a not in F[x]:
                        F[x].append(a)
            if N[x] == d:
                N[stack[-1]] = sys.maxsize
                F[stack[-1]] = F[x]
                element = stack.pop()
                while element != x:
                    N[stack[-1]] = sys.maxsize
                    F[stack[-1]] = F[x]
                    element = stack.pop()

        def digraph(X, R, FP):
            # type: (List[Tuple[int, int]], Callable[[Tuple[int, int]], List[Tuple[int, int]]], Callable[[Tuple[int, int]], List[int]]) -> Dict[Tuple[int, int], List[int]]
            N = {}
            for x in X:
                N[x] = 0
            stack = []     # type: List[Tuple[int, int]]
            F = {}         # type: Dict[Tuple[int, int], List[int]]
            for x in X:
                if N[x] == 0:
                    traverse(x, N, stack, F, X, R, FP)
            return F

        def dr_relation(trans):
            # type: (Tuple[int, int]) -> List[int]
            state, N = trans
            terms = []

            item_set = goto(states[state], N)
            assert item_set is not None
            for item in item_set:
                if item._index < len(item._rule):
                    a = item._rule._production[item._index]
                    if a < terminal_count:
                        if a not in terms:
                            terms.append(a)

            # This extra bit is to handle the start state
            if state == start_id and N == productions[start_id][0][0]:
                terms.append(0)

            return terms

        def reads_relation(trans, empty):
            # type: (Tuple[int, int], Set[int]) -> List[Tuple[int, int]]
            # Look for empty transitions
            rel = []
            state, N = trans

            item_set = goto(states[state], N)
            assert item_set is not None
            j = cidhash[id(item_set)]
            for item in item_set:
                if item._index < len(item._rule):
                    a = item._rule[item._index]
                    if a in empty:
                        rel.append((j, a))

            return rel

        def compute_read_sets(trans, nullable):
            # type: (List[Tuple[int, int]], Set[int]) -> Dict[Tuple[int, int], List[int]]
            FP = lambda x: dr_relation(x)
            R = lambda x: reads_relation(x, nullable)
            F = digraph(trans, R, FP)
            return F

        def compute_lookback_includes(trans, nullable):
            # type: (List[Tuple[int, int]], Set[int]) -> Tuple[Dict[Tuple[int, int], List[Tuple[int, Grammar.LR0Item]]], Dict[Tuple[int, int], List[Tuple[int, int]]]]
            lookdict = {}      # Dictionary of lookback relations
            includedict = {}   # type: Dict[Tuple[int, int], List[Tuple[int, int]]]

            # Make a dictionary of non-terminal transitions
            dtrans = {}
            for t1 in trans:
                dtrans[t1] = 1

            # Loop over all transitions and compute lookbacks and includes
            for state, N in trans:
                lookb = []
                includes = []
                for p in states[state]:
                    if p._rule._prod_index != N:
                        continue

                    # Okay, we have a name match.  We now follow the production all the way
                    # through the state machine until we get the . on the right hand side

                    lr_index = p._index
                    j = state
                    while lr_index < len(p._rule):
                        t = p._rule[lr_index]
                        lr_index = lr_index + 1

                        # Check to see if this symbol and state are a non-terminal transition
                        if (j, t) in dtrans:
                            # Yes.  Okay, there is some chance that this is an includes relation
                            # the only way to know for certain is whether the rest of the
                            # production derives empty

                            li = lr_index
                            while li < len(p._rule):
                                if p._rule[li] < terminal_count:
                                    break # No forget it
                                if p._rule[li] not in nullable:
                                    break
                                li = li + 1
                            else:
                                          # Appears to be a relation between (j,t) and (state,N)
                                includes.append((j, t))

                        g = goto(states[j], t) # Go to next set
                        j = cidhash[id(g)]     # Go to next state

                    # When we get here, j is the final state, now we have to locate the production
                    for r in states[j]:
                        if r._rule._prod_index != p._rule._prod_index:
                            continue
                        if len(r._rule) != len(p._rule):
                            continue
                        # i = 0
                        # This look is comparing a production ". A B C" with "A B C ."
                        # while i < r._index:
                        #    if r._rule[i] != p._rule[i + 1]:
                        #        break
                        #    i = i + 1
                        #else:
                        #    lookb.append((j, r))
                        if p._index == 0 and r._rule._production[:r._index] == p._rule._production[:r._index]:
                            lookb.append((j, r))
                for ii in includes:
                    if ii not in includedict:
                        includedict[ii] = []
                    includedict[ii].append((state, N))
                lookdict[(state, N)] = lookb

            return lookdict, includedict

        def compute_follow_sets(ntrans, readsets, inclsets):
            # type: (List[Tuple[int, int]], Dict[Tuple[int, int], List[int]], Dict[Tuple[int, int], List[Tuple[int, int]]]) -> Dict[Tuple[int, int], List[int]]
            FP = lambda x: readsets[x]
            R = lambda x: inclsets.get(x, [])
            F = digraph(ntrans, R, FP)
            return F

        def add_lookaheads(lookbacks, followset):
            # type: (Dict[Tuple[int, int], List[Tuple[int, Grammar.LR0Item]]], Dict[Tuple[int, int], List[int]]) -> None
            for trans, lb in lookbacks.items():
                # Loop over productions in lookback
                for state, p in lb:
                    if state not in p._lookaheads:
                        p._lookaheads[state] = []
                    l = p._lookaheads[state]
                    f = followset.get(trans, [])
                    for a in f:
                        if a not in l:
                            l.append(a)

        # Determine all of the nullable nonterminals
        nullable = set([])
        for prod_index, prod in productions.items():
            if prod._empty:
                nullable.add(prod_index)

        # Find all non-terminal transitions
        trans = []
        for stateno, state in enumerate(states):
            for item in state:
                if item._index < len(item._rule):
                    t = (stateno, item._rule[item._index])
                    if t[1] >= terminal_count:
                        if t not in trans:
                            trans.append(t)

        # Compute read sets
        readsets = compute_read_sets(trans, nullable)

        # Compute lookback/includes relations
        lookd, included = compute_lookback_includes(trans, nullable)

        # Compute LALR FOLLOW sets
        followsets = compute_follow_sets(trans, readsets, included)

        # Add all of the lookaheads
        add_lookaheads(lookd, followsets)

    goto_table = {}    # Goto array
    action = {}        # Action array
    actionp = {}       # Action production array (temporary)

    # Build the parser table, state by state
    dominator_nodes, states = create_item_sets()
    add_lalr_lookahead(states)

    # build dominance tree for items
    all_nodes = set(dominator_nodes)
    for node in dominator_nodes[1:]:
        node._dominators = all_nodes
    changed = True
    while changed:
        changed = False
        for node in dominator_nodes[1:]:
            dominators = set([])   # type: Set[Grammar.LR0Item.DominatorNode]
            dominators.update(node._predecessors[0]._dominators)
            for pred in node._predecessors[1:]:
                dominators.intersection_update(pred._dominators)
            dominators.add(node)
            if node._dominators != dominators:
                changed = True
                node._dominators = dominators

    for node in dominator_nodes[1:]:
        parent = None  # type: Optional[Grammar.LR0Item.DominatorNode]
        for dom_node in node._dominators:
            if dom_node == node:
                continue
            elif parent is None:
                parent = dom_node
            elif dom_node in parent._dominators:
                assert parent not in dom_node._dominators
            else:
                assert parent in dom_node._dominators
                parent = dom_node
        node._parent = parent
        if parent is not None:
            parent._children.append(node)

    st = 0

    num_missing_annotations = 0
    num_rr = 0

    dot_file.info('digraph Grammar {')
    for item_group in states:
        dot_file.info("  subgraph cluster_%d {", st)
        dot_file.info('    label="State %d";', st)
        dot_file.info('    style=filled;')
        dot_file.info('    color=lightgray;')
        dot_file.info('    node[style=filled;color=white];')
        state_id = id(item_group)
        for item in item_group:
            dot_file.info('    %d[label="%s"];', id(item._dominators[state_id]), item.to_string(name_map))
        for item in item_group:
            dnode = item._dominators[id(item_group)]
            for predecessor in dnode._predecessors:
                if predecessor._item_set == state_id:
                    dot_file.info('    %d -> %d;', id(predecessor), id(dnode))
        dot_file.info("  }")
        st += 1
    dot_file.info('  %d[label="start"];', id(dominator_nodes[0]))

    st = 0
    for item_group in states:
        # Loop over each production
        st_action = {}     # type: Dict[int, List[Tuple[int, Grammar.LR0Item]]]
        st_goto = {}
        log.info('state %d:', st)
        log.info('')
        for item in item_group:
            log.info('    (%d) %s', item._rule._prod_index, item.to_string(name_map))
        log.info('')

        for item in item_group:
            if len(item._rule) == item._index:
                if item._rule._prod_index == start_id:
                    # Start symbol. Accept!
                    st_action[0] = st_action.get(0, []) + [(0, item)]
                    item._rule._reduced += 1
                else:
                    # We are at the end of a production.  Reduce!
                    for a in item._lookaheads[st]:
                        st_action[a] = st_action.get(a, []) + [(-item._rule._id, item)]
                        item._rule._reduced += 1
            else:
                i = item._index
                a = item._rule[i]  # Get symbol right after the "."
                if a < terminal_count:
                    g = goto(item_group, a)
                    j = cidhash[id(g)]
                    if j >= 0:
                        st_action[a] = st_action.get(a, []) + [(j, item)]

        for a, actions in st_action.items():
            action_dest = {}   # type: Dict[int, List[Grammar.LR0Item]]
            for i, item in actions:
                try:
                    action_dest[i].append(item)
                except KeyError:
                    action_dest[i] = [item]

            accepted_actions = {}  # type: Dict[int, List[Grammar.LR0Item]]

            if len(action_dest) > 1:
                # looks like a potential conflict, look at precedence
                actions = sorted(actions, key=lambda x: x[1]._precedence[1] if x[1]._precedence is not None else -1)
                precedence = actions[-1][1]._precedence[1] if actions[-1][1]._precedence is not None else -1
                associativity = actions[-1][1]._precedence[0] if actions[-1][1]._precedence is not None else 'left'
                for _, item in actions:
                    if item._precedence is None:
                        log.warning('  ** %s [%s] needs a precedence annotation', item.to_string(name_map), name_map[a])
                        error_log.warning(
                            '  ** %s [%s] needs a precedence annotation', item.to_string(name_map), name_map[a]
                        )
                        num_missing_annotations += 1
                assoc_conflict = False
                for j, item in actions:
                    if item._precedence is not None:
                        if item._precedence[1] < precedence:
                            log.info('  -- %s is not used', item.to_string(name_map))
                        elif item._precedence[0] != associativity:
                            log.error('  *** associativity conflicts!')
                            log.error('  *** %s', item.to_string(name_map))
                            error_log.error('  *** associativity conflicts!')
                            error_log.error('  *** %s', item.to_string(name_map))
                            assoc_conflict = True
                        elif associativity == 'left' and j >= 0:
                            try:
                                accepted_actions[j].append(item)
                            except KeyError:
                                accepted_actions[j] = [item]
                        elif associativity == 'right' and j < 0:
                            try:
                                accepted_actions[j].append(item)
                            except KeyError:
                                accepted_actions[j] = [item]
                        elif associativity == 'nonassoc':
                            try:
                                accepted_actions[j].append(item)
                            except KeyError:
                                accepted_actions[j] = [item]
                        else:
                            log.info('  -- %s is not used', item.to_string(name_map))
                    else:
                        try:
                            accepted_actions[j].append(item)
                        except KeyError:
                            accepted_actions[j] = [item]

                if assoc_conflict:
                    log.error('  *** %s', actions[-1][1].to_string(name_map))
                    log.error('  *** using %s', actions[-1][1].to_string(name_map))
                    error_log.error('  *** %s', actions[-1][1].to_string(name_map))
                    error_log.error('  *** using %s', actions[-1][1].to_string(name_map))
            else:
                accepted_actions = action_dest

            for target_group, item_list in accepted_actions.items():
                if target_group >= 0:
                    for item1 in item_list:
                        node1 = item1._dominators[id(item_group)]
                        for item2 in states[target_group]:
                            node2 = item2._dominators[id(states[target_group])]
                            if node1 in node2._predecessors:
                                dot_file.info('  %d -> %d[label="%s"];', id(node1), id(node2), name_map[a])
            if len(accepted_actions) > 1:
                # get the longest reduce action
                dom_nodes = {}     # type: Dict[int, Set[Grammar.LR0Item.DominatorNode]]
                longest_reduction = 0
                conflict_nodes = set([])
                for _, item_list in accepted_actions.items():
                    for item in item_list:
                        longest_reduction = max(longest_reduction, item._index)
                for _, item_list in accepted_actions.items():
                    for item in item_list:
                        for core_node in item._dominators[id(item_group)].find_core(longest_reduction, set([])):
                            conflict_nodes.add(core_node)
                    for node in conflict_nodes:
                        try:
                            dom_nodes[node._item_set].add(node)
                        except:
                            dom_nodes[node._item_set] = set([node])

                conflict_symbols = set([])
                for _, item_list in accepted_actions.items():
                    for item in item_list:
                        if not item._split:
                            log.warning('  ** %s [%s] needs a split annotation', item.to_string(name_map), name_map[a])
                            error_log.warning(
                                '  ** %s [%s] needs a split annotation', item.to_string(name_map), name_map[a]
                            )
                            num_missing_annotations += 1
                        conflict_symbols.add(item._rule._prod_index)

                # reduce until...
                reduced = True
                while reduced:
                    new_dom_nodes = {} # type: Dict[int, Set[Grammar.LR0Item.DominatorNode]]
                    reduced = False

                    #error_log.warning('  ** state %d', st)
                    #for _, nodes in dom_nodes.items():
                    #    for node in nodes:
                    #        log.warning('  ** %s needs a merge annotation', node._item.to_string(name_map))
                    #        error_log.warning('  ** %s needs a merge annotation', node._item.to_string(name_map))
                    #        num_missing_annotations += 1

                    for item_group_id, nodes in dom_nodes.items():
                        conflict_nodes = set([])
                        n1 = nodes.pop()
                        longest_reduction = n1._item._index
                        origin = set(n1._direct_predecessors)
                        for node in nodes:
                            longest_reduction = max(longest_reduction, node._item._index)
                            origin.intersection_update(node._direct_predecessors)
                        nodes.add(n1)
                        if longest_reduction == 0 and len(origin) == 0:
                            for node in nodes:
                                for pred in node._direct_predecessors:
                                    if pred._item._index != 0:
                                        origin.add(pred)
                            nodes = origin
                            longest_reduction = 0
                            for node in nodes:
                                longest_reduction = max(longest_reduction, node._item._index)
                        if longest_reduction != 0:
                            reduced = True
                            for node in nodes:
                                for core_node in node.find_core(longest_reduction, set([])):
                                    conflict_nodes.add(core_node)
                            for node in conflict_nodes:
                                try:
                                    new_dom_nodes[node._item_set].add(node)
                                except:
                                    new_dom_nodes[node._item_set] = set([node])
                        else:
                            new_dom_nodes[item_group_id] = nodes
                    dom_nodes = new_dom_nodes

                # find the deepest nodes of each state that reach only one of the conflicts
                #error_log.warning('  ** state %d', st)
                #for _, nodes in dom_nodes.items():
                #    for node in nodes:
                #        log.warning('  ** %s needs a merge annotation', node._item.to_string(name_map))
                #        error_log.warning('  ** %s needs a merge annotation', node._item.to_string(name_map))
                #        num_missing_annotations += 1

                for item_group_id, nodes in dom_nodes.items():
                    leaves = set(nodes)
                    conflict_nodes = set(nodes)
                    node = nodes.pop()
                    origin = set(node._direct_predecessors)
                    while nodes:
                        origin.intersection_update(nodes.pop()._direct_predecessors)
                    for o in origin:
                        for successor in o._direct_successors:
                            inter = leaves.intersection(successor._direct_successors)
                            if len(inter) > 0 and len(inter) < len(leaves):
                                conflict_nodes.add(successor)
                    for n1 in conflict_nodes:
                        for n2 in conflict_nodes:
                            if n2 in n1._direct_predecessors:
                                break
                        else:
                            log.warning('  ** %s needs a merge annotation', n1._item.to_string(name_map))
                            error_log.warning('  ** %s needs a merge annotation', n1._item.to_string(name_map))
                            num_missing_annotations += 1

                # find the deepest nodes of each state that reach only one of the conflicts
                #error_log.warning('  ** state %d', st)
                #for _, nodes in dom_nodes.items():
                #    for node in nodes:
                #        log.warning('  ** %s needs a merge annotation', node._item.to_string(name_map))
                #        error_log.warning('  ** %s needs a merge annotation', node._item.to_string(name_map))
                #        num_missing_annotations += 1

        nkeys = set([])
        for item in item_group:
            node1 = item._dominators[id(item_group)]
            for s in item._symbols:
                if s > terminal_count:
                    g = goto(item_group, s)
                    j = cidhash.get(id(g), -1)
                    if j >= 0:
                        for item2 in states[j]:
                            dnode = item2._dominators[id(states[j])]
                            if node1 in dnode._predecessors:
                                dot_file.info('  %d->%d[label="%s"];', id(node1), id(dnode), name_map[s])
                        if s not in nkeys:
                            nkeys.add(s)
                            #st_goto[n] = j
                            log.info('    %-30s shift and go to state %d', name_map[s], j)

        st += 1
        """
        # Print the actions associated with each terminal
        _actprint = {}
        for a, p, m in actlist:
            if a in st_action:
                if p is st_actionp[a]:
                    log.info('    %-15s %s', name_map[a], m)
                    _actprint[(a, m)] = 1
        log.info('')
        # Print the actions that were not used. (debugging)
        not_used = 0
        for a, p, m in actlist:
            if a in st_action:
                if p is not st_actionp[a]:
                    if not (a, m) in _actprint:
                        log.note('  ! %-15s [ %s ]', name_map[a], m)
                        not_used = 1
                        _actprint[(a, m)] = 1
        if not_used:
            log.note('')

        # Construct the goto table for this state

        nkeys = set([])    # type: Set[int]
        for item in item_group:
            for s in item._symbols:
                if s > terminal_count:
                    nkeys.add(s)
        for n in nkeys:
            g = goto(item_group, n)
            j = cidhash.get(id(g), -1)
            if j >= 0:
                st_goto[n] = j
                log.info('    %-30s shift and go to state %d', name_map[n], j)

        action[st] = st_action
        goto_table[st] = st_goto
        st += 1

        for state, tok, rule, resolution in sr_conflicts:
            log.warning('shift/reduce conflict for %s in state %d resolved as %s', name_map[tok], state, resolution)
            log.warning('  reduce rule %s', rule.to_string(name_map))
            error_log.warning(
                'shift/reduce conflict for %s in state %d resolved as %s', name_map[tok], state, resolution
            )
            error_log.warning('  reduce rule %s', rule.to_string(name_map))

        already_reported = set()
        for state, rule, rejected in rr_conflicts:
            if (state, id(rule), id(rejected)) in already_reported:
                continue
            log.warning('reduce/reduce conflict in state %d resolved using rule (%s)', state, rule.to_string(name_map))
            log.warning('rejected rule (%s) in state %d', rejected.to_string(name_map), state)
            error_log.warning(
                'reduce/reduce conflict in state %d resolved using rule (%s)', state, rule.to_string(name_map)
            )
            error_log.warning('rejected rule (%s) in state %d', rejected.to_string(name_map), state)
            already_reported.add((state, id(rule), id(rejected)))
        """
    dot_file.info('}')

    # Report shift/reduce and reduce/reduce conflicts
    if num_missing_annotations == 1:
        log.warning('1 missing annotation')
        error_log.warning('1 missing annotation')
    elif num_missing_annotations > 1:
        log.warning('%d missing annotations', num_missing_annotations)
        error_log.warning('%d missing annotations', num_missing_annotations)

    if num_rr == 1:
        log.warning('1 reduce/reduce conflict')
    elif num_rr > 1:
        log.warning('%d reduce/reduce conflicts', num_rr)
        error_log.warning('%d reduce/reduce conflicts', num_rr)

    for _, production in productions.items():
        for rule in production:
            if rule._reduced == 0:
                log.warning('Rule (%s) is never reduced', rule.to_string(name_map))


if TYPE_CHECKING:
    from typing import Any, Callable, Dict, Iterator, List, Optional, Set, Tuple
    from .parser import Parser
    from .production import Production