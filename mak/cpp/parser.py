import cpp


spec = cpp.yacc.Spec(
                [
                    cpp.tokens,
                    cpp.grammar.unit,
                    cpp.grammar.exprs,
                    cpp.grammar.namespace,
                    cpp.grammar.name,
                    cpp.grammar.using,
                    cpp.grammar.tag,
                    cpp.grammar.comment,
                    cpp.grammar.struct,
                    cpp.grammar.enum,
                    cpp.grammar.method,
                    cpp.grammar.variable,
                    cpp.grammar.type,
                    cpp.grammar.template,
                    cpp.grammar.keywords,
                    cpp.grammar.value,
                    cpp.grammar.skip,
                ],
                pickleFile="../../cpp.pickle",
                logFile="../../cpp.log",
                graphFile="../../cpp.dot",
                verbose=False)

class Parser(cpp.yacc.Lr):
    def __init__(self, filename, instancesname, docname, plugin, source, pch):
        cpp.yacc.Lr.__init__(self, spec)
        self.filename = filename
        self.instancesname = instancesname
        self.doc = docname
        self.plugin = plugin
        self.source = source
        self.pch = pch


    def parse(self, input, lexer):
        lexer.input(input)
        t=lexer.token()
        try:
            while t:
                tok = cpp.tokens.__dict__[t.type](self)
                tok.value = t.value
                tok.lineno = t.lineno
                self.token(tok)
                t=lexer.token()
            self.eoi()
        except SyntaxError as e:
            print("%s:%d %s" % (self.source, t.lineno, str(e)))
            raise
        except Exception as e:
            print("%s:%d %s" % (self.source, t.lineno, str(e)))
            raise

    def dump(self):
        try:
            with open(self.filename, 'w') as implementation:
                with open(self.instancesname, 'w') as instances:
                    with open(self.doc, 'w') as doc:
                        if self.pch:
                            implementation.write("#include    <%s>\n" % self.pch)
                            instances.write("#include    <%s>\n" % self.pch)
                        self.root.dump(implementation, instances, doc)
        except IOError as e:
            raise Exception("cannot open output file : %s" % str(e))

