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

    def run(self):
        bld = self.generator.bld
        root = bld.bldnode
        self.outputs[0].write(self.inputs[0].read())
        cmd = [self.env.AAPT, 'add', self.outputs[0].abspath()] + [i.path_from(root).replace('\\', '/') for i in self.inputs[1:]]
        return self.exec_command(cmd)


class jarsigner(Task.Task):
    """
    Signs jar file
    """
    color   = 'PINK'
    run_str = '${JARSIGNER} ${JARSIGNER_FLAGS} -signedjar ${TGT} ${SRC} ${JARSIGNER_KEY}'


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
