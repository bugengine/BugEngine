from waflib.TaskGen import feature, after_method


def build(bld):
    pass


@feature('cshlib', 'cxxshlib')
@after_method('apply_link')
def gcc_apply_implib(self):
    if self.env.CC_NAME == 'gcc' and 'windows' in self.env.VALID_PLATFORMS:
        implib = 'lib%s.a' % self.target_name
        out = self.link_task.outputs[0].parent.make_node(implib)
        self.env.append_unique('LINKFLAGS', ['-Wl,--out-implib,%s' % out.abspath()])
        self.link_task.outputs.append(out)
