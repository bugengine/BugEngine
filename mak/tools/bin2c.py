from waflib import Task


class bin2c(Task.Task):
    def sig_vars(self):
        self.m.update(self.var.encode('utf-8'))

    def run(self):
        input_lines = []
        with open(self.inputs[0].abspath(), 'rb') as input_file:
            while True:
                input_data = input_file.read(12)
                if input_data == b'':
                    break
                input_lines.append(', '.join('0x%.2X' % x for x in bytearray(input_data)))

        export = getattr(self, 'export', False)
        if getattr(self, 'zero_terminate', False):
            input_lines.append('0x00')
        with open(self.outputs[0].abspath(), 'w') as output_file:
            vars = {'var': self.var, 'data': ',\n    '.join(input_lines)}
            output_file.write(
                'extern const unsigned char s_%(var)s[] = {\n'
                '    %(data)s\n'
                '};\n'
                'extern const unsigned long s_%(var)s_size = sizeof(s_%(var)s);\n' % vars
            )
