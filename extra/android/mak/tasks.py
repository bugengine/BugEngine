from waflib import Task, Errors, Context



class android_manifest(Task.Task):
    """
    Create an apk file
    """
    color   = 'PINK'
    def run(self):
        MANIFEST_SKELETON = """<?xml version="1.0" encoding="utf-8"?>
<manifest xmlns:android="http://schemas.android.com/apk/res/android"
    package="com.%(publisher)s.%(app)s"
    android:versionCode="%(version_code)d"
    android:versionName="%(version)s">
    <application
            android:label="@string/bugengine_activity"
            android:debuggable="true">
        <activity android:name="com.bugengine.BugEngineActivity"
                  android:enabled="false"
                  android:theme="@android:style/Theme.NoTitleBar.Fullscreen"
                  android:launchMode="singleTask"
                  android:configChanges="orientation|keyboardHidden">
        </activity>
        %(activities)s
    </application>
    <uses-feature android:glEsVersion="0x00020000"/>
    <uses-sdk android:minSdkVersion="5"/>
</manifest>
"""

        ACTIVITY_SKELETON = """<activity-alias android:label="%(task_gen)s"
                android:name=".%(task_gen)s"
                android:targetActivity="com.bugengine.BugEngineActivity">
            <intent-filter>
                <action android:name="android.intent.action.MAIN" />
                <category android:name="android.intent.category.LAUNCHER" />
            </intent-filter>
            <meta-data android:name="app"
                       android:value="%(task_gen)s" />
        </activity-alias>"""


        appname = getattr(Context.g_module, Context.APPNAME, self.generator.bld.srcnode.name)
        publisher = getattr(Context.g_module, 'PUBLISHER', 'Unknown')
        version = getattr(Context.g_module, Context.VERSION, '1.0')
        version_code = 1 # TODO
        apps = []
        multiarch = len(self.generator.bld.multiarch_envs) > 1
        for group in self.generator.bld.groups:
            for task_gen in group:
                if multiarch:
                    if 'multiarch' in task_gen.features:
                        if 'game' in self.generator.bld.get_tgen_by_name(task_gen.use[0]).features:
                            apps.append(task_gen)
                else:
                    if 'game' in task_gen.features:
                        apps.append(task_gen)
        values = {
            'app': appname.lower(),
            'publisher': publisher.lower().replace(' ', ''),
            'version': version,
            'version_code': version_code,
            'activities': '\n        '.join([ACTIVITY_SKELETON % {'task_gen':tg.name} for tg in apps])
        }
        self.outputs[0].write(MANIFEST_SKELETON % values)


class aapt_create(Task.Task):
    """
    Create an apk file
    """
    color   = 'PINK'
    run_str = '${AAPT} package -f ${AAPTFLAGS} -M ${MANIFEST} -S ${RESOURCE_PATH} -F ${TGT}'


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
    DEX_RE = '**/*.class'
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
            try:
                self.inputs = [x for x in self.outdir.ant_glob(self.DEX_RE, remove=False) if id(x) != id(self.outputs[0])]
                self.env.INPUT_FILES = [x.path_from(self.basedir) for x in self.inputs]
                self.env.OUTPUT_FILES = [x.path_from(self.basedir) for x in self.outputs]
            except Exception:
                raise Errors.WafError('Could not find the basedir %r for %r' % (self.basedir, self))
        return super(dex, self).runnable_status()


def build(bld):
    pass
