from ..log import Logger
from be_typing import TYPE_CHECKING, overload
from .lr0item import LR0Item
from . import lalr
import io
import os
import sys


class Grammar(object):
    class Rule(object):
        def __init__(self, id, prod_symbol, prod_name, production, action, annnotation_list, filename, lineno):
            # type: (int, int, str, Tuple[int,...], Action, List[Tuple[str, List[str], int]], str, int) -> None
            self._id = id
            self._prod_symbol = prod_symbol
            self._prod_name = prod_name
            self.production = production
            self.len = len(self.production)
            self._action = action
            self._filename = filename
            self._lineno = lineno
            predecessor = self.production[-1] if len(self.production) else None
            self._annotations = {}     # type: Dict[int, Dict[str, List[str]]]
            for annotation, values, index in annnotation_list:
                if index == len(production):
                    index = -1
                try:
                    self._annotations[index][annotation] = values
                except KeyError:
                    self._annotations[index] = {annotation: values}
            self._item = LR0Item(self, len(production), None, predecessor, [], set([-1]), {-1: 0})
            self._reduced = 0

        def to_string(self, name_map):
            # type: (List[str]) -> str
            return '%s -> %s' % (name_map[self._prod_symbol], ' '.join([name_map[p] for p in self.production]))

        def __iter__(self):
            # type: () -> Iterator[int]
            return iter(self.production)

    class Production(object):
        def __init__(self, prod_symbol, rule_list):
            # type: (int, List[Grammar.Rule]) -> None
            self._rule_list = rule_list
            self._prod_symbol = prod_symbol
            self._first = set([])  # type: Set[int]
            self._first_list = []  # type: List[int]
            self._empty = False
            self._nonterminal_count = 0
            for rule in rule_list:
                if rule.len == 0:
                    self._empty = True

        def add_rule(self, rule):
            # type: (Grammar.Rule) -> None
            self._rule_list.append(rule)
            if rule.len == 0:
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

    def __init__(self, name, rule_hash, terminals, rules, start_symbol, parser, temp_dir):
        # type: (str, str, Dict[str, Tuple[int, bool]], List[Tuple[str, Action, List[str], List[Tuple[str, List[str], int]], str, int]], str, Parser, str) -> None
        debug_filename = os.path.join(temp_dir, name + '.txt')
        conflict_filename = os.path.join(temp_dir, name + '-Conflicts.txt')
        index = {}
        for terminal, (i, _) in terminals.items():
            index[terminal] = i
        name_map = [''] * (len(terminals))
        log = Logger(io.open(debug_filename, 'w', encoding='utf-8'))
        stderr = Logger(io.open(sys.stderr.fileno(), 'w', encoding='utf-8', closefd=False))
        conflict_log = Logger(io.open(conflict_filename, 'w', encoding='utf-8'))
        dot_file = Logger(io.open(os.path.splitext(debug_filename)[0] + '.dot', 'w', encoding='utf-8'))
        for name, (i, _) in terminals.items():
            name_map[i] = name

        rules = rules + [
            ("%s'" % start_symbol, parser.__class__.AcceptAction(), [start_symbol, name_map[0]], [], '', 1)
        ]
        for nonterminal, _, _, _, _, _ in rules:
            if nonterminal not in index:
                i = len(index)
                index[nonterminal] = i
                name_map.append(nonterminal)
                assert name_map[i] == nonterminal

        name_map.append('<epsilon>')

        start_id = len(index) - 1
        productions, rule_table = _create_productions(rules, index, stderr, name_map, terminals, start_id)
        for prod_symbol, prod in productions.items():
            log.info(
                '%d %s {%s}', prod_symbol, name_map[prod_symbol], ', '.join([name_map[t] for t in prod._first_list])
            )
            for rule in prod:
                log.info('  %s', rule.to_string(name_map))

        _create_lr0_items(productions)
        tables = lalr.create_parser_table(
            productions, start_id, name_map, len(terminals), log, conflict_log, stderr, dot_file
        )
        self._action_table = tables._action_table
        self._goto_table = tables._goto_table
        self._rules = rule_table
        self._hash = rule_hash


def _create_productions(rules, index, log, name_map, terminals, start_id):
    # type: (List[Tuple[str, Action, List[str], List[Tuple[str, List[str], int]], str, int]], Dict[str, int], Logger, List[str], Dict[str, Tuple[int, bool]], int) -> Tuple[Dict[int, Grammar.Production], List[Tuple[int, Tuple[int,...], Action]]]
    rule_index = 1
    productions = {}   # type: Dict[int, Grammar.Production]
    rule_table = []

    symbol_usage = [0] * len(index)    # type: List[int]
    unreachable = []                   # type: List[Grammar.Production]
    errors = False

    for nonterminal, action, production, attribute_list, filename, lineno in rules:
        prod_symbol = index[nonterminal]
        try:
            symbols = tuple(index[s] for s in production)
        except KeyError as error:  # unknown rule or terminal
            log.error('%s:%d: Symbol %s used, but not defined as a token or a rule' % (filename, lineno, str(error)))
            errors = True
        else:
            rule_table.append((prod_symbol, symbols, action))
            rule = Grammar.Rule(rule_index, prod_symbol, nonterminal, symbols, action, attribute_list, filename, lineno)
            rule_index += 1
            for s in symbols:
                symbol_usage[s] += 1
            try:
                productions[prod_symbol].add_rule(rule)
            except KeyError:
                productions[prod_symbol] = Grammar.Production(prod_symbol, [rule])

    if not errors:
        pass

    if errors:
        raise SyntaxError('Unable to build grammar')

    terminal_count = len(terminals)
    for s, count in enumerate(symbol_usage[1:terminal_count]):
        if count == 0 and terminals[name_map[s + 1]][1]:
            log.warning('Token %s defined, but not used', name_map[s + 1])
    for s, count in enumerate(symbol_usage[terminal_count:]):
        if count == 0 and terminal_count + s != start_id:
            try:
                prod = productions[terminal_count + s]
            except KeyError:
                pass
            else:
                log.warning(
                    '%s:%d: Rule %r defined, but not used', prod[0]._filename, prod[0]._lineno, prod[0]._prod_name
                )

    changed = True
    while changed:
        changed = False
        for _, prods in productions.items():
            if not prods._empty:
                for rule in prods:
                    for x in rule:
                        try:
                            p = productions[x]
                        except KeyError:
                            break
                        else:
                            if not p._empty:
                                break
                    else:
                        prods._empty = True
                        changed = True
                        break

    queue = list(productions.keys())
    while queue:
        prod_symbol = queue.pop(0)
        prod = productions[prod_symbol]
        len_set_before = len(prod._first)
        if prod._empty:
            prod._first.add(-1)
        for rule in prod:
            i = 0
            found_epsilon = True
            while found_epsilon:
                found_epsilon = False
                try:
                    t = rule.production[i]
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
            prod._first_list = sorted(prod._first)
            queue.append(prod_symbol)

    return productions, rule_table


def _create_lr0_items(productions):
    # type: (Dict[int, Grammar.Production]) -> None
    for _, prods in productions.items():
        for rule in prods:
            i = rule.len - 1
            offset = 1
            follow_buffer = {-1: 0}
            while i >= 0:
                try:
                    production = productions[rule.production[i]]
                except KeyError:
                    # terminal
                    next = []
                    first = set([rule.production[i]])
                    follow = {rule.production[i]: offset}
                else:
                    next = list(production)
                    first = production._first

                    if -1 not in first:
                        follow = {}
                    else:
                        follow = dict(follow_buffer)
                    for terminal in production._first:
                        if terminal != -1:
                            follow[terminal] = offset
                predecessor = rule.production[i - 1] if i > 0 else None
                rule._item = LR0Item(rule, i, rule._item, predecessor, next, first, follow_buffer)
                i -= 1
                offset += 1
                follow_buffer = follow


if TYPE_CHECKING:
    from typing import Dict, Iterator, List, Sequence, Set, Tuple, Union
    from .parser import Parser, Action