class CppObject:
    def __init__(self, parent, position):
        from .templates import Template
        self.parser = parent and parent.parser
        self.position = position
        self.parent = parent
        self.templates = []
        while parent:
            if isinstance(parent, Template):
                self.templates.append(parent)
            parent = parent.parent
        self.instances = []

    def instantiate(self, parent, template_arguments, *args):
        from .templates import Template
        for template_position, t in enumerate(self.templates):
            if t.root_template == template_arguments[0].root_template:
                break
        else:
            return self
        templates = []
        p = parent
        while p:
            if isinstance(p, Template):
                templates.insert(0, p)
            p = p.parent
        #for i in range(0, template_position):
        #    assert(self.templates[i].root_template == templates[i].root_template)
        for arguments, instance in self.instances:
            if self.match(arguments, template_arguments[1]):
                return instance
        else:
            #print('creating instance of %s %s (%s)' % (self.__class__.__name__,
            #                                           getattr(self, 'name', ''),
            #                                           self.position))
            instance = self._instantiate(parent, template_arguments, *args)
            assert instance, '%s did not return an instance' % self.__class__.__name__
            self.instances.append((template_arguments[1], instance))
            self._instantiate_content(instance, template_arguments)
            #print('done')
            return instance

    def _instantiate_content(self, instance, template_arguments):
        pass

    def match(self, arguments1, arguments2):
        for arg1, arg2 in zip(arguments1, arguments2):
            if arg1[1] != arg2[1]:
                if not arg1[1].matches(arg2[1]):
                    return False
        return True

    def lookup_by_name(self, name):
        while self:
            obj = self.find(name, False)
            if obj:
                return (self, obj)
            self = self.parent
        return (None, None)

    def _error(self, error_message):
        self.parser.lexer._error(error_message, self.position)

    def wrap_template_parameter(self):
        return self