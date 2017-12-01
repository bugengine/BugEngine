from waflib import Task, Errors


DEX_RE = '**/*.class'

class aapt_create(Task.Task):
    """
    Create an apk file
    """
    color   = 'PINK'
    run_str = '${AAPT} package ${AAPTFLAGS} -M ${MANIFEST} -S ${RESOURCE_PATH} -F ${TGT}'


class copy(Task.Task):
    """
    Copy file from input to output
    """
    color   = 'PINK'

    def run(self):
        self.outputs[0].write(self.inputs[0].read())
        return 0


class aapt_package(Task.Task):
    """
    Store files in an apk file
    """
    color   = 'PINK'

    @staticmethod
    def filter_output(prog, output):
        return [i for i in output if i[-3:] != '...']

    def run(self):
        bld = self.generator.bld
        root = bld.bldnode
        self.outputs[0].write(self.inputs[0].read())
        if self.env._7Z:
            compression_level = 2 if bld.__class__.optim != 'final' else 9
            cmd = [self.env._7Z, 'a', '-tzip', '-mx%d'%compression_level,
                   self.outputs[0].abspath()] + [i.path_from(root).replace('\\', '/') for i in self.inputs[1:]]
            with open(self.outputs[0].change_ext('.tmp').abspath(), 'w') as stdout:
                return self.exec_command(cmd, stdout=stdout)
        else:
            cmd = [self.env.AAPT, 'add', self.outputs[0].abspath()] + [i.path_from(root).replace('\\', '/') for i in self.inputs[1:]]
            return self.exec_command(cmd, filter=self.filter_aapt_output)


class jarsigner(Task.Task):
    """
    Signs jar file
    """
    color   = 'PINK'
    #run_str = '${JARSIGNER} ${JARSIGNER_FLAGS} -signedjar ${TGT} ${SRC} ${JARSIGNER_KEY}'

    def run(self):
        cmd = [self.env.JARSIGNER] + self.env.JARSIGNER_FLAGS + ['-signedjar', self.outputs[0].abspath(),
                                                                 self.inputs[0].abspath(), self.env.JARSIGNER_KEY]
        with open(self.outputs[0].change_ext('.tmp').abspath(), 'w') as stdout:
            return self.exec_command(cmd, stdout=stdout)




class zipalign(Task.Task):
    """
    Align zip file on 4
    """
    color   = 'PINK'
    run_str = '${ZIPALIGN} -f 4 ${SRC} ${TGT}'


class dex(Task.Task):
    """
    Create a dex file
    """
    color   = 'GREEN'
    run_str = '${JAVA} -jar ${DEX} ${DEXCREATE} ${DEX_TGT_PATTERN:OUTPUT_FILES} ${DEXOPTS} ${INPUT_FILES}'

    def runnable_status(self):
        """
        Wait for dependent tasks to be executed, then read the
        files to update the list of inputs.
        """
        for t in self.run_after:
            if not t.hasrun:
                return Task.ASK_LATER
        if not self.inputs:
            global DEX_RE
            try:
                self.inputs = [x for x in self.outdir.ant_glob(DEX_RE, remove=False) if id(x) != id(self.outputs[0])]
                self.env.INPUT_FILES = [x.path_from(self.basedir) for x in self.inputs]
                self.env.OUTPUT_FILES = [x.path_from(self.basedir) for x in self.outputs]
            except Exception:
                raise Errors.WafError('Could not find the basedir %r for %r' % (self.basedir, self))
        return super(dex, self).runnable_status()


def build(bld):
    pass
