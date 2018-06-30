try:
    from itertools import zip_longest
except ImportError:
    from itertools import izip_longest as zip_longest


class Template:
    class InstanciationError(Exception):
        def __init__(self, msg):
            self.msg = msg

    def __init__(self, position):
        self.parameters = []
        self.specializations = []
        self.name = None
        self.position = position

    def get_token_type(self):
        return 'TEMPLATE_' + self.specializations[0][1].get_token_type()

    def find_nonrecursive(self, name):
        if self.name == name:
            return self

    def add_template_parameter(self, parameter):
        self.parameters.append(parameter)

    def find(self, name, is_current_scope):
        for p in self.parameters:
            if p.name == name:
                return p

    def create_instance(self, arguments):
        specialization, template_arguments = self.find_specialization(arguments)
        return specialization.instantiate(template_arguments)

    def instantiate(self, template_arguments):
        result = Template()
        for p in self.parameters:
            result.add_template_parameter(p.instantiate(template_arguments))
        for params, s in self.specializations:
            result.add((p.instantiate(template_values) for p in params), s.instantiate(template_values))
        return result

    def add(self, specialization):
        if not self.name:
            self.name = specialization.name
        self.specializations.append(([], specialization))

    def specialize(self, parameters, template_specialization):
        if self.name == template_specialization.name:
            self.specializations.append((parameters, template_specialization))
            return True

    def find_specialization(self, arguments):
        template_arguments = { }
        for parameter, argument in zip_longest(self.parameters, arguments):
            if not argument:
                raise Template.InstanciationError('Too many template parameters')
            if not parameter:
                if not agument.default_value:
                    raise Template.InstanciationError('Too few template parameters')
                else:
                    argument = parameter.default_value
            if not argument.is_valid(parameter):
                if parameter.name:
                    raise Template.InstanciationError('%s: Invalid template parameter' % parameter.name)
                else:
                    raise Template.InstanciationError('Invalid template parameter')
            if parameter.name:
                template_arguments[parameter.name] = { argument }
        return self.specializations[0][1], template_arguments

    def write_to(self, writer):
        pass
