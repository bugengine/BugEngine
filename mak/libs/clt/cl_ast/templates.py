
class Template:
    def __init__(self):
        self.parameters = []
        self.specializations = []
        self.name = None

    def get_token_type(self):
        return 'TEMPLATE_' + self.specializations[0][1].get_token_type()

    def find_nonrecursive(self, name):
        if self.name == name:
            return self

    def add_template_parameter(self, parameter):
        self.parameters.append(parameter)

    def find(self, name):
        for p in self.parameters:
            if p.name == name:
                return p

    def create_instance(self, arguments):
        template_arguments = {}
        return self.specializations[0][1].instantiate(template_arguments)

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
