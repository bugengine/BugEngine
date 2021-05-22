from ..log import Logger
from be_typing import TYPE_CHECKING
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

    class LR0ItemSet:
        def __init__(self, items):
            # type: (List[Grammar.LR0Item]) -> None
            self._items = items

        def __iter__(self):
            # type: () -> Iterator[Grammar.LR0Item]
            return iter(self._items)

        def to_strings(self, name_map):
            # type: (List[str]) -> List[str]
            return [i.to_string(name_map) for i in self._items]

    def __init__(self, terminals, rules, start_symbol, parser, tab_filename, debug_filename):
        # type: (Dict[str, int], List[Tuple[str, Parser.Action, List[str], List[Tuple[str, List[str], int]], str, int]], str, Parser, str, str) -> None
        index = dict(terminals)
        name_map = [''] * (1 + len(terminals))
        log = Logger(open(debug_filename, 'w'))
        stderr = Logger(sys.stderr)
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
        _create_parser_table(productions, start_id, name_map, len(terminals), log, stderr)

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


def _closure(item_set):
    # type: (List[Grammar.LR0Item]) -> Grammar.LR0ItemSet
    result = item_set[:]
    seen = set([])
    added = True
    while added:
        added = False
        for item in result:
            for x in item._after:
                if x in seen:
                    continue
                seen.add(x)
                added = True
                result.append(x._item)
    return Grammar.LR0ItemSet(result)


def _create_parser_table(productions, start_id, name_map, terminal_count, log, error_log):
    # type: (Dict[int, Grammar.Production], int, List[str], int, Logger, Logger) -> None
    cidhash = {}
    goto_cache = {}    # type: Dict[Tuple[int, int], Optional[Grammar.LR0ItemSet]]
    goto_cache_2 = {}  # type: Dict[int, Any]

    def goto(item_set, lookahead):
        # type: (Grammar.LR0ItemSet, int) -> Optional[Grammar.LR0ItemSet]
        # First we look for a previously cached entry
        result = goto_cache.get((id(item_set), lookahead), None)
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
                gs.append(next)
                s = s1

        result = s.get(0, None)
        if result is None:
            if gs:
                result = _closure(gs)
                s[0] = result
            else:
                s[0] = None

        goto_cache[(id(item_set), lookahead)] = result
        return result

    def create_item_sets():
        # type: () -> List[Grammar.LR0ItemSet]
        assert len(productions[start_id]) == 1
        states = [_closure([productions[start_id][0]._item])]
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

        return states

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
    states = create_item_sets()
    add_lalr_lookahead(states)
    st = 0

    num_missing_annotations = 0
    num_rr = 0

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
                for _, item in actions:
                    if item._precedence is not None and item._precedence[1] < precedence:
                        log.info('  -- %s is not used', item.to_string(name_map))
                assoc_conflict = False
                for _, item in actions:
                    if item._precedence is not None and item._precedence[0] != associativity:
                        log.error('  *** associativity conflicts!')
                        log.error('  *** %s', item.to_string(name_map))
                        error_log.error('  *** associativity conflicts!')
                        error_log.error('  *** %s', item.to_string(name_map))
                        assoc_conflict = True
                if assoc_conflict:
                    log.error('  *** %s', actions[-1][1].to_string(name_map))
                    log.error('  *** using %s', actions[-1][1].to_string(name_map))
                    error_log.error('  *** %s', actions[-1][1].to_string(name_map))
                    error_log.error('  *** using %s', actions[-1][1].to_string(name_map))

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