from waflib import Context, Build, TaskGen, Logs
import os, sys
from xml.dom.minidom import Document
from minixml import XmlNode, XmlDocument
import string
try:
    import cStringIO as StringIO
except ImportError:
    try:
        import io as StringIO
    except ImportError:
        import StringIO

try:
    import hashlib
    createMd5 = hashlib.md5
except:
    import md5
    createMd5 = md5.new


def _hexdigest(s):
    """Return a string as a string of hex characters.
    """
    # NOTE:  This routine is a method in the Python 2.0 interface
    # of the native md5 module, but we want SCons to operate all
    # the way back to at least Python 1.5.2, which doesn't have it.
    h = string.hexdigits
    r = ''
    for c in s:
        try:
            i = ord(c)
        except:
            i = c
        r = r + h[(i >> 4) & 0xF] + h[i & 0xF]
    return r


def generateGUID(name):
    """This generates a dummy GUID for the sln file to use.  It is
    based on the MD5 signatures of the sln filename plus the name of
    the project.  It basically just needs to be unique, and not
    change with each invocation."""
    d = _hexdigest(createMd5(str(name).encode()).digest()).upper()
    # convert most of the signature to GUID form (discard the rest)
    d = "{" + d[:8] + "-" + d[8:12] + "-" + d[12:16] + "-" + d[16:20] + "-" + d[20:32] + "}"
    return d


def unique(seq):
    seen = set()
    seen_add = seen.add
    return [x for x in seq if x not in seen and not seen_add(x)]


def gather_includes_defines(task_gen):
    defines = getattr(task_gen, 'defines', []) + getattr(task_gen, 'export_defines', []) + getattr(
        task_gen, 'extra_defines', [])
    includes = getattr(task_gen, 'includes', []) + getattr(task_gen, 'export_includes', []) + getattr(
        task_gen, 'extra_includes', [])
    seen = set([])
    use = getattr(task_gen, 'use', []) + getattr(task_gen, 'private_use', [])
    while use:
        name = use.pop()
        if name not in seen:
            seen.add(name)
            try:
                t = task_gen.bld.get_tgen_by_name(name)
            except:
                pass
            else:
                use = use + getattr(t, 'use', [])
                includes = includes + getattr(t, 'includes', []) + getattr(t, 'export_includes', []) + getattr(
                    task_gen, 'extra_includes', [])
                defines = defines + getattr(t, 'defines', []) + getattr(t, 'export_defines', []) + getattr(
                    task_gen, 'extra_defines', [])
    return unique(includes), unique(defines)


def path_from(path, bld):
    if isinstance(path, str):
        return path.replace('/', '\\')
    else:
        return '$(SolutionDir)%s' % path.path_from(bld.srcnode).replace('/', '\\')


class XmlFile:
    def __init__(self):
        self.document = Document()

    def _add(self, node, child_node, value=None):
        def setAttributes(node, attrs):
            for k, v in attrs.items():
                node.setAttribute(k, v)

        el = self.document.createElement(child_node)
        if (value):
            if type(value) == type(str()):
                el.appendChild(self.document.createTextNode(value))
            elif type(value) == type(dict()):
                setAttributes(el, value)
        node.appendChild(el)
        return el

    def write(self, node):
        try:
            xml = node.read()
        except IOError:
            xml = ''
        newxml = self.document.toprettyxml()
        if xml != newxml:
            Logs.pprint('NORMAL', 'writing %s' % node.name)
            node.write(newxml)


class Solution:
    def __init__(self, bld, appname, version_number, version_name, use_folders, vstudio_ide_version):
        self.header = '\xef\xbb\xbf\r\nMicrosoft Visual Studio Solution File, Format Version %s\r\n# %s' % (
            version_number, version_name)
        if vstudio_ide_version:
            self.header += '\r\nVisualStudioVersion = %s\r\nMinimumVIsualStudioVersion = %s' % (vstudio_ide_version,
                                                                                                vstudio_ide_version)
        self.projects = []
        self.project_configs = []
        self.configs = [
            '\t\t%s|%s = %s|%s' % (v, t, v, t) for v in bld.env.ALL_VARIANTS for t in bld.env.ALL_TOOLCHAINS
        ]
        self.folders = []
        self.folders_made = {}
        self.use_folders = use_folders
        self.master = ''

    def addFolder(self, name):
        names = name.split('.')[:-1]
        if names:
            folder_name = '.'.join(names)
            try:
                folder = self.folders_made[folder_name]
            except KeyError:
                folder = generateGUID(folder_name)
                self.folders_made[folder_name] = folder
                self.projects.append(
                    'Project("{2150E333-8FDC-42A3-9474-1A3956D46DE8}") = "%s", "%s", "%s"\r\nEndProject' %
                    (names[-1], names[-1], folder))
                parent = self.addFolder(folder_name)
                if parent:
                    self.folders.append((folder, parent))
            return folder
        else:
            return None

    def get_dependency(self, project):
        if self.master and project.GUID == VCproj.GUID:
            return "	ProjectSection(ProjectDependencies) = postProject\r\n		%s = %s\r\n	EndProjectSection\r\n" % (
                self.master, self.master)
        else:
            return ''

    def add(self, task_gen, project, project_path, build=False):
        self.projects.append('Project("%s") = "%s", "%s", "%s"\r\n%sEndProject' %
                             (project.GUID, project.name, project_path, project.guid, self.get_dependency(project)))
        project_config = []
        for t in task_gen.bld.env.ALL_TOOLCHAINS:
            env = task_gen.bld.all_envs[t]
            platform = task_gen.bld.get_platform(env.MS_PROJECT_PLATFORM)
            project_config += [
                '\t\t%s.%s|%s.ActiveCfg = %s-%s|%s' % (project.guid, v, t, t, v, platform)
                for v in task_gen.bld.env.ALL_VARIANTS
            ]
            if build:
                self.master = project.guid
                project_config += [
                    '\t\t%s.%s|%s.Build.0 = %s-%s|%s' % (project.guid, v, t, t, v, platform)
                    for v in task_gen.bld.env.ALL_VARIANTS
                ]
        self.project_configs += project_config
        if self.use_folders:
            parent = self.addFolder(task_gen.target)
            if parent:
                self.folders.append((project.guid, parent))

    def write(self, node):
        nested_projects = ''
        if self.use_folders:
            nested_projects = '\tGlobalSection(NestedProjects) = preSolution\r\n%s\n\tEndGlobalSection\r\n' % '\r\n'.join(
                ['\t\t%s = %s' % (project, parent) for project, parent in self.folders])
        newsolution = '%s\r\n%s\r\nGlobal\r\n\tGlobalSection(SolutionConfigurationPlatforms) = preSolution\r\n%s\r\n\tEndGlobalSection\r\n\tGlobalSection(ProjectConfigurationPlatforms) = postSolution\r\n%s\r\n\tEndGlobalSection\r\n%sEndGlobal\r\n' % (
            self.header, '\n'.join(self.projects), '\n'.join(self.configs), '\r\n'.join(
                self.project_configs), nested_projects)
        try:
            solution = node.read()
        except IOError:
            solution = ''
        if solution != newsolution:
            Logs.pprint('NORMAL', 'writing %s' % node.name)
            node.write(newsolution)


class VcprojNode:
    def __init__(self, name):
        self.subs = {}
        self.files = []

    def add(self, node):
        if os.path.isdir(node.abspath()):
            try:
                sub = self.subs[node.name]
            except KeyError:
                sub = self.subs[node.name] = VcprojNode(node.name)
            for f in node.listdir():
                sub.add(node.make_node(f))
        else:
            self.files.append(node)

    def write(self, xml, src_node):
        for name, sub in sorted(self.subs.items(), key=lambda x: x[0]):
            with XmlNode(xml, 'Filter', {'Name': name}) as filter:
                sub.write(filter, src_node)
        for file in sorted(self.files, key=lambda x: x.name):
            XmlNode(xml, 'File', {
                'RelativePath': '$(SolutionDir)%s' % file.path_from(src_node).replace('/', '\\')
            }).close()


class VCproj:
    GUID = '{8BC9CEB8-8B4A-11D0-8D11-00A0C91BC942}'
    extensions = ['vcproj']

    def __init__(self, task_gen, version, version_project, use_folders):
        if use_folders:
            self.name = task_gen.target.split('.')[-1]
        else:
            self.name = task_gen.target
        self.guid = generateGUID(task_gen.target)
        self.vcproj = XmlDocument(StringIO.StringIO(), 'UTF-8')
        with XmlNode(
                self.vcproj, 'VisualStudioProject', {
                    'ProjectType': 'Visual C++',
                    'Version': version_project,
                    'Name': self.name,
                    'ProjectGUID': self.guid,
                    'RootNamespace': task_gen.target,
                    'Keyword': 'Win32Proj'
                }) as project:
            with XmlNode(project, 'Platforms') as platforms:
                for p in task_gen.bld.__class__.platforms:
                    XmlNode(platforms, 'Platform', {'Name': p}).close()
            with XmlNode(project, 'Configurations') as configurations:
                includes, defines = gather_includes_defines(task_gen)
                for toolchain in task_gen.bld.env.ALL_TOOLCHAINS:
                    env = task_gen.bld.all_envs[toolchain]
                    if env.SUB_TOOLCHAINS:
                        sub_env = task_gen.bld.all_envs[env.SUB_TOOLCHAINS[0]]
                    else:
                        sub_env = env
                    for variant in task_gen.bld.env.ALL_VARIANTS:
                        platform = task_gen.bld.get_platform(env.MS_PROJECT_PLATFORM)
                        with XmlNode(
                                configurations, 'Configuration', {
                                    'Name': '%s-%s|%s' % (toolchain, variant, platform),
                                    'OutputDirectory': '$(SolutionDir)%s\\%s\\' % (sub_env.PREFIX, variant),
                                    'IntermediateDirectory': '$(SolutionDir)%s\\%s\\' % (sub_env.PREFIX, variant),
                                    'ConfigurationType': '0',
                                    'CharacterSet': '2'
                                }) as configuration:
                            tool = {'Name': 'VCNMakeTool'}
                            command = getattr(task_gen, 'command', '')
                            if command:
                                command = command % {'toolchain': toolchain, 'variant': variant}
                                tool['BuildCommandLine'] = 'cd $(SolutionDir) && %s %s %s' % (sys.executable,
                                                                                              sys.argv[0], command)
                            else:
                                tool['BuildCommandLine'] = 'cd $(SolutionDir) && %s %s build:%s:%s --targets=%s' % (
                                    sys.executable, sys.argv[0], toolchain, variant, task_gen.target)
                                tool['CleanCommandLine'] = 'cd $(SolutionDir) && %s %s clean:%s:%s --targets=%s' % (
                                    sys.executable, sys.argv[0], toolchain, variant, task_gen.target)
                                tool[
                                    'ReBuildCommandLine'] = 'cd $(SolutionDir) && %s %s clean:%s:%s instal:%s:%s --targets=%s' % (
                                        sys.executable, sys.argv[0], toolchain, variant, toolchain, variant,
                                        task_gen.target)
                            if 'cxxprogram' in task_gen.features:
                                tool['Output'] = '$(OutDir)\\%s\\%s' % (env.DEPLOY_BINDIR,
                                                                        sub_env.cxxprogram_PATTERN % task_gen.target)
                            elif 'game' in task_gen.features:
                                deps = task_gen.use[:]
                                seen = set([])
                                program = None
                                while (deps):
                                    dep = deps.pop()
                                    if dep not in seen:
                                        seen.add(dep)
                                        try:
                                            task_dep = task_gen.bld.get_tgen_by_name(dep)
                                            deps += getattr(task_dep, 'use', [])
                                            if 'cxxprogram' in task_dep.features:
                                                program = task_dep
                                        except:
                                            pass
                                if program:
                                    tool['Output'] = '$(OutDir)\\%s\\%s' % (env.DEPLOY_BINDIR,
                                                                            sub_env.cxxprogram_PATTERN % program.target)
                                    debug_command = '$(NMakeOutput)'
                                    debug_command_arguments = task_gen.target
                                else:
                                    tool['Output'] = '$(OutDir)\\%s\\%s' % (env.DEPLOY_BINDIR,
                                                                            sub_env.cxxprogram_PATTERN %
                                                                            task_gen.bld.launcher[0][0].target)
                            if float(version_project) >= 8:
                                tool['PreprocessorDefinitions'] = ';'.join(defines + sub_env.DEFINES +
                                                                           sub_env.SYSTEM_DEFINES)
                                tool['IncludeSearchPath'] = ';'.join([
                                    path_from(p, task_gen.bld) for p in includes + sub_env.INCLUDES +
                                    sub_env.SYSTEM_INCLUDES + [os.path.join(sub_env.SYSROOT or '', 'usr', 'include')]
                                ])
                            XmlNode(configuration, 'Tool', tool).close()
            XmlNode(project, 'References').close()
            with XmlNode(project, 'Files') as files:
                n = VcprojNode('root')
                for node in getattr(task_gen, 'source_nodes', []):
                    if os.path.isdir(node.abspath()):
                        for sub_node in node.listdir():
                            n.add(node.make_node(sub_node))
                n.write(files, task_gen.bld.srcnode)
            XmlNode(project, 'Globals').close()

    def write(self, nodes):
        content = self.vcproj.file.getvalue()
        try:
            original = nodes[0].read()
            if original != content:
                Logs.pprint('NORMAL', 'writing %s' % nodes[0].abspath())
                nodes[0].write(content)
        except Exception:
            nodes[0].write(content)
        self.vcproj.close()


class VCxproj:
    GUID = '{8BC9CEB8-8B4A-11D0-8D11-00A0C91BC942}'
    extensions = ['vcxproj', 'vcxproj.filters']

    def __init__(self, task_gen, version, version_project, use_folders):
        self.vcxproj = XmlFile()
        self.vcxfilters = XmlFile()
        if use_folders:
            self.name = task_gen.target.split('.')[-1]
        else:
            self.name = task_gen.target
        project = self.vcxfilters._add(
            self.vcxfilters.document, 'Project', {
                'DefaultTargets': 'Build',
                'ToolsVersion': version_project[2],
                'xmlns': 'http://schemas.microsoft.com/developer/msbuild/2003'
            })
        self.filter_nodes = self.vcxfilters._add(project, 'ItemGroup')
        self.file_nodes = self.vcxfilters._add(project, 'ItemGroup')

        self.guid = generateGUID(task_gen.target)
        project = self.vcxproj._add(
            self.vcxproj.document, 'Project', {
                'DefaultTargets': 'Build',
                'ToolsVersion': version_project[2],
                'xmlns': 'http://schemas.microsoft.com/developer/msbuild/2003'
            })
        configs = self.vcxproj._add(project, 'ItemGroup', {'Label': 'ProjectConfigurations'})
        for toolchain in task_gen.bld.env.ALL_TOOLCHAINS:
            env = task_gen.bld.all_envs[toolchain]
            platform = task_gen.bld.get_platform(env.MS_PROJECT_PLATFORM)
            for variant in task_gen.bld.env.ALL_VARIANTS:
                config = self.vcxproj._add(configs, 'ProjectConfiguration',
                                           {'Include': '%s-%s|%s' % (toolchain, variant, platform)})
                self.vcxproj._add(config, 'Configuration', '%s-%s' % (toolchain, variant))
                self.vcxproj._add(config, 'Platform', platform)
        for toolchain in task_gen.bld.env.ALL_TOOLCHAINS:
            env = task_gen.bld.all_envs[toolchain]
            if env.SUB_TOOLCHAINS:
                env = task_gen.bld.all_envs[env.SUB_TOOLCHAINS[0]]
            pgroup = self.vcxproj._add(project, 'PropertyGroup')
            self.vcxproj._add(pgroup, 'PlatformShortName', toolchain)
            self.vcxproj._add(pgroup, 'PlatformArchitecture', env.VALID_ARCHITECTURES[0])
            self.vcxproj._add(pgroup, 'PlatformTarget', toolchain)

        globals = self.vcxproj._add(project, 'PropertyGroup', {'Label': 'Globals'})
        self.vcxproj._add(globals, 'ProjectGUID', self.guid)
        #self.vcxproj._add(globals, 'TargetFrameworkVersion', 'v'+version_project[0])
        self.vcxproj._add(globals, 'RootNamespace', task_gen.target)
        self.vcxproj._add(globals, 'ProjectName', self.name)
        self.vcxproj._add(project, 'Import', {'Project': '$(VCTargetsPath)\\Microsoft.Cpp.Default.props'})
        for toolchain in task_gen.bld.env.ALL_TOOLCHAINS:
            env = task_gen.bld.all_envs[toolchain]
            if env.SUB_TOOLCHAINS:
                env = task_gen.bld.all_envs[env.SUB_TOOLCHAINS[0]]
            platform = task_gen.bld.get_platform(env.MS_PROJECT_PLATFORM)
            for prop in env.MS_PROJECT_IMPORT_PROPS:
                for variant in task_gen.bld.env.ALL_VARIANTS:
                    self.vcxproj._add(project, 'Import', {
                        'Condition': "'$(Configuration)'=='%s-%s'" % (toolchain, variant),
                        'Project': prop
                    })

        for toolchain in task_gen.bld.env.ALL_TOOLCHAINS:
            env = task_gen.bld.all_envs[toolchain]
            if env.SUB_TOOLCHAINS:
                env = task_gen.bld.all_envs[env.SUB_TOOLCHAINS[0]]
            platform = task_gen.bld.get_platform(env.MS_PROJECT_PLATFORM)
            for variant in task_gen.bld.env.ALL_VARIANTS:
                properties = self.vcxproj._add(
                    project, 'PropertyGroup',
                    {'Condition': "'$(Configuration)|$(Platform)'=='%s-%s|%s'" % (toolchain, variant, platform)})
                for var in [
                        'Prefix', 'TmpDir', 'Toolchain', 'Deploy_BinDir', 'Deploy_RunBinDir', 'Deploy_LibDir',
                        'Deploy_IncludeDir', 'Deploy_DataDir', 'Deploy_PluginDir', 'Deploy_KernelDir', 'Deploy_RootDir'
                ]:
                    self.vcxproj._add(properties, var, env[var.upper()].replace('/', '\\'))
                self.vcxproj._add(properties, 'Variant', variant)

        for toolchain in task_gen.bld.env.ALL_TOOLCHAINS:
            env = task_gen.bld.all_envs[toolchain]
            if env.SUB_TOOLCHAINS:
                env = task_gen.bld.all_envs[env.SUB_TOOLCHAINS[0]]
            platform = task_gen.bld.get_platform(env.MS_PROJECT_PLATFORM)
            for variant in task_gen.bld.env.ALL_VARIANTS:
                configuration = self.vcxproj._add(
                    project, 'PropertyGroup', {
                        'Label': 'Configuration',
                        'Condition': "'$(Configuration)|$(Platform)'=='%s-%s|%s'" % (toolchain, variant, platform)
                    })
                self.vcxproj._add(configuration, 'ConfigurationType', 'Makefile')
                self.vcxproj._add(configuration, 'PlatformToolset', 'v%d' % (float(version_project[1]) * 10))
                self.vcxproj._add(configuration, 'OutDir', '$(SolutionDir)$(Prefix)\\$(Variant)\\')
                self.vcxproj._add(configuration, 'IntDir', '$(TmpDir)\\$(Variant)\\')

        for toolchain in task_gen.bld.env.ALL_TOOLCHAINS:
            env = task_gen.bld.all_envs[toolchain]
            if env.SUB_TOOLCHAINS:
                env = task_gen.bld.all_envs[env.SUB_TOOLCHAINS[0]]
            platform = task_gen.bld.get_platform(env.MS_PROJECT_PLATFORM)
            for variant in task_gen.bld.env.ALL_VARIANTS:
                if env.MS_PROJECT_VARIABLES:
                    properties = self.vcxproj._add(
                        project, 'PropertyGroup',
                        {'Condition': "'$(Configuration)|$(Platform)'=='%s-%s|%s'" % (toolchain, variant, platform)})
                    for var, value in env.MS_PROJECT_VARIABLES:
                        self.vcxproj._add(properties, var, value)

        includes, defines = gather_includes_defines(task_gen)
        for toolchain in task_gen.bld.env.ALL_TOOLCHAINS:
            env = task_gen.bld.all_envs[toolchain]
            if env.SUB_TOOLCHAINS:
                sub_env = task_gen.bld.all_envs[env.SUB_TOOLCHAINS[0]]
            else:
                sub_env = env
            platform = task_gen.bld.get_platform(sub_env.MS_PROJECT_PLATFORM)
            for variant in task_gen.bld.env.ALL_VARIANTS:
                properties = self.vcxproj._add(
                    project, 'PropertyGroup',
                    {'Condition': "'$(Configuration)|$(Platform)'=='%s-%s|%s'" % (toolchain, variant, platform)})
                command = getattr(task_gen, 'command', '')
                if command:
                    command = command % {'toolchain': toolchain, 'variant': variant}
                    self.vcxproj._add(properties, 'NMakeBuildCommandLine',
                                      'cd $(SolutionDir) && %s %s %s' % (sys.executable, sys.argv[0], command))
                else:
                    self.vcxproj._add(
                        properties, 'NMakeBuildCommandLine', 'cd $(SolutionDir) && %s %s build:%s:%s --targets=%s' %
                        (sys.executable, sys.argv[0], toolchain, variant, task_gen.target))
                    self.vcxproj._add(
                        properties, 'NMakeReBuildCommandLine',
                        'cd $(SolutionDir) && %s %s clean:%s:%s build:%s:%s --targets=%s' %
                        (sys.executable, sys.argv[0], toolchain, variant, toolchain, variant, task_gen.target))
                    self.vcxproj._add(
                        properties, 'NMakeCleanCommandLine', 'cd $(SolutionDir) && %s %s clean:%s:%s --targets=%s' %
                        (sys.executable, sys.argv[0], toolchain, variant, task_gen.target))
                    if 'cxxprogram' in task_gen.features:
                        self.vcxproj._add(
                            properties, 'NMakeOutput',
                            '$(OutDir)\\%s\\%s' % (env.DEPLOY_BINDIR, sub_env.cxxprogram_PATTERN % task_gen.target))
                    elif 'game' in task_gen.features:
                        self.vcxproj._add(
                            properties, 'NMakeOutput', '$(OutDir)\\%s\\%s' %
                            (env.DEPLOY_BINDIR, sub_env.cxxprogram_PATTERN % task_gen.bld.launcher[0][0].target))
                        self.vcxproj._add(properties, 'LocalDebuggerCommand', '$(NMakeOutput)')
                        self.vcxproj._add(properties, 'LocalDebuggerCommandArguments', task_gen.target)
                    self.vcxproj._add(properties, 'NMakePreprocessorDefinitions',
                                      ';'.join(defines + sub_env.DEFINES + sub_env.SYSTEM_DEFINES))
                    if sub_env.SYS_ROOT:
                        includes.append('%s/usr/include' % sub_env.SYSROOT or '')
                    self.vcxproj._add(
                        properties, 'NMakeIncludeSearchPath',
                        ';'.join([path_from(i, task_gen.bld)
                                  for i in includes] + sub_env.INCLUDES + sub_env.SYSTEM_INCLUDES))
        self.vcxproj._add(project, 'Import', {'Project': '$(VCTargetsPath)\\Microsoft.Cpp.props'})
        files = self.vcxproj._add(project, 'ItemGroup')

        self.filters = {}
        for node in getattr(task_gen, 'source_nodes', []):
            self.add_node(task_gen.bld.srcnode, node, node, files)
        self.vcxproj._add(project, 'Import', {'Project': '$(VCTargetsPath)\\Microsoft.Cpp.targets'})
        for toolchain in task_gen.bld.env.ALL_TOOLCHAINS:
            env = task_gen.bld.all_envs[toolchain]
            if env.SUB_TOOLCHAINS:
                env = task_gen.bld.all_envs[env.SUB_TOOLCHAINS[0]]
            platform = task_gen.bld.get_platform(env.MS_PROJECT_PLATFORM)
            for target in env.MS_PROJECT_IMPORT_TARGETS:
                for variant in task_gen.bld.env.ALL_VARIANTS:
                    self.vcxproj._add(
                        project, 'Import', {
                            'Condition': "'$(Configuration)|$(Platform)'=='%s-%s|%s'" % (toolchain, variant, platform),
                            'Project': target
                        })

    def write(self, nodes):
        self.vcxproj.write(nodes[0])
        self.vcxfilters.write(nodes[1])

    def add_node(self, root_node, project_node, node, files):
        path = node.abspath()
        if os.path.isdir(path):
            if project_node != node:
                path = node.path_from(root_node).replace('/', '\\')
                try:
                    filter = self.filters[path]
                except KeyError:
                    filter = generateGUID(path)
                    n = self.vcxfilters._add(self.filter_nodes, 'Filter',
                                             {'Include': node.path_from(project_node).replace('/', '\\')})
                    self.vcxfilters._add(n, 'UniqueIdentifier', filter)
            for subdir in node.listdir():
                self.add_node(root_node, project_node, node.make_node(subdir), files)
        elif os.path.isfile(path):
            self.vcxproj._add(files, 'None',
                              {'Include': '$(SolutionDir)%s' % node.path_from(root_node).replace('/', '\\')})
            n = self.vcxfilters._add(self.file_nodes, 'None',
                                     {'Include': '$(SolutionDir)%s' % node.path_from(root_node).replace('/', '\\')})
            self.vcxfilters._add(n, 'Filter', node.parent.path_from(project_node).replace('/', '\\'))


class PyProj:
    GUID = '{888888A0-9F3D-457C-B088-3A5042F75D52}'

    def __init__(self, task_gen, version, version_project, use_folders):
        self.pyproj = XmlFile()
        self.guid = generateGUID(task_gen.target)
        if use_folders:
            self.name = task_gen.target.split('.')[-1]
        else:
            self.name = task_gen.target

        project = self.pyproj._add(
            self.pyproj.document, 'Project', {
                'DefaultTargets': 'Build',
                'ToolsVersion': '4.0',
                'xmlns': 'http://schemas.microsoft.com/developer/msbuild/2003'
            })
        propgroup = self.pyproj._add(project, 'PropertyGroup')
        self.pyproj._add(propgroup, 'SchemaVersion', '2.0')
        self.pyproj._add(propgroup, 'ProjectGuid', self.guid[1:-1])
        self.pyproj._add(propgroup, 'ProjectHome', '.')
        self.pyproj._add(propgroup, 'StartupFile',
                         task_gen.bld.bugenginenode.make_node('waf').abspath().replace('/', '\\'))
        self.pyproj._add(propgroup, 'SearchPath',
                         task_gen.bld.bugenginenode.make_node('mak').make_node('libs').abspath().replace('/', '\\'))
        self.pyproj._add(propgroup, 'WorkingDirectory', task_gen.bld.srcnode.abspath().replace('/', '\\'))
        self.pyproj._add(propgroup, 'OutputPath', '.')
        self.pyproj._add(propgroup, 'Name', self.name)
        self.pyproj._add(propgroup, 'RootNamespace', task_gen.target)
        self.pyproj._add(propgroup, 'LaunchProvider', 'Standard Python launcher')
        self.pyproj._add(propgroup, 'EnableNativeCodeDebugging', 'False')
        self.pyproj._add(propgroup, 'IsWindowsApplication', 'False')
        self.pyproj._add(propgroup, 'InterpreterId', 'MSBuild|env|$(MSBuildProjectFullPath)')
        for toolchain in task_gen.bld.env.ALL_TOOLCHAINS:
            for variant in task_gen.bld.env.ALL_VARIANTS:
                properties = self.pyproj._add(project, 'PropertyGroup',
                                              {'Condition': "'$(Configuration)'=='%s-%s'" % (toolchain, variant)})
                self.pyproj._add(properties, 'CommandLineArguments', 'build:%s:%s' % (toolchain, variant))
                self.pyproj._add(properties, 'DebugSymbols', 'true')
                self.pyproj._add(properties, 'EnableUnmanagedDebugging', 'false')

        self.pyproj._add(project, 'ItemGroup')
        ig_env = self.pyproj._add(project, 'ItemGroup')
        interpreter = self.pyproj._add(ig_env, 'Interpreter', {'Include': 'env\\'})
        self.pyproj._add(interpreter, 'Id', 'env')
        self.pyproj._add(interpreter, 'Version', '.'.join(str(i) for i in sys.version_info[0:2]))
        self.pyproj._add(interpreter, 'Description', 'Python used to generate solution')
        self.pyproj._add(interpreter, 'InterpreterPath', sys.executable)
        self.pyproj._add(interpreter, 'WindowsInterpreterPath', sys.executable)
        self.pyproj._add(interpreter, 'PathEnvironmentVariable', 'PYTHONPATH')
        self.pyproj._add(interpreter, 'Architecture', 'x64')
        self.pyproj._add(
            project, 'Import', {
                'Project':
                    "$(MSBuildExtensionsPath32)\\Microsoft\\VisualStudio\\v$(VisualStudioVersion)\\Python Tools\\Microsoft.PythonTools.targets"
            })
        self.pyproj._add(project, 'Target', {'Name': 'BeforeBuild'})
        self.pyproj._add(project, 'Target', {'Name': 'AfterBuild'})

    def write(self, node):
        self.pyproj.write(node)


class vs2003(Build.BuildContext):
    "creates projects for Visual Studio 2003"
    cmd = 'vs2003'
    fun = 'build'
    optim = 'debug'
    version = (('Visual Studio .NET 2003', '8.00', False, None), (VCproj, '7.10'))
    platforms = ['Win32']
    variant = 'projects/vs'

    def get_platform(self, platform_name):
        return platform_name if platform_name in self.__class__.platforms else self.__class__.platforms[0]

    def execute(self):
        """
        Entry point
        """
        self.restore()
        if not self.all_envs:
            self.load_envs()
        self.env.PROJECTS = [self.__class__.cmd]

        self.env.VARIANT = '$(Variant)'
        self.env.TOOLCHAIN = '$(Toolchain)'
        self.env.PREFIX = '$(Prefix)'
        self.env.TMPDIR = '$(TmpDir)'
        self.env.DEPLOY_ROOTDIR = '$(Deploy_RootDir)'
        self.env.DEPLOY_BINDIR = '$(Deploy_BinDir)'
        self.env.DEPLOY_RUNBINDIR = '$(Deploy_RunBinDir)'
        self.env.DEPLOY_LIBDIR = '$(Deploy_LibDir)'
        self.env.DEPLOY_INCLUDEDIR = '$(Deploy_IncludeDir)'
        self.env.DEPLOY_DATADIR = '$(Deploy_DataDir)'
        self.env.DEPLOY_PLUGINDIR = '$(Deploy_PluginDir)'
        self.env.DEPLOY_KERNELDIR = '$(Deploy_KernelDir)'
        self.features = ['GUI']

        self.recurse([self.run_dir])

        version = self.__class__.cmd
        version_name, version_number, folders, ide_version = self.__class__.version[0]
        klass, version_project = self.__class__.version[1]

        appname = getattr(Context.g_module, Context.APPNAME, self.srcnode.name)

        solution_node = self.srcnode.make_node(appname + '.' + version + '.sln')
        projects = self.srcnode.make_node('.build').make_node(version)
        projects.mkdir()

        solution = Solution(self, appname, version_number, version_name, folders, ide_version)

        for target, command, do_build in [('build.reconfigure', 'reconfigure', False),
                                          ('build.%s' % version, version, False),
                                          ('build.all', 'build:%(toolchain)s:%(variant)s', True)]:
            task_gen = lambda: None
            task_gen.target = target
            task_gen.command = command
            task_gen.bld = self
            task_gen.all_sources = []
            task_gen.features = []
            nodes = [projects.make_node("%s.%s" % (target, ext)) for ext in klass.extensions]
            project = klass(task_gen, version, version_project, folders)
            project.write(nodes)
            solution.add(task_gen, project, nodes[0].path_from(self.srcnode).replace('/', '\\'), do_build)

        pydbg_task_gen = lambda: None
        pydbg_task_gen.target = 'build.debug'
        pydbg_task_gen.command = command
        pydbg_task_gen.bld = self
        pydbg_task_gen.all_sources = []
        project = PyProj(pydbg_task_gen, version, version_project, folders)
        pydbg_node = projects.make_node('build.debug.pyproj')
        project.write(pydbg_node)
        solution.add(pydbg_task_gen, project, pydbg_node.path_from(self.srcnode).replace('/', '\\'), False)

        for g in self.groups:
            for tg in g:
                if not isinstance(tg, TaskGen.task_gen):
                    continue
                if not 'kernel' in tg.features:
                    tg.post()

                    nodes = [projects.make_node("%s.%s" % (tg.target, ext)) for ext in klass.extensions]
                    project = klass(tg, version, version_project, folders)
                    project.write(nodes)
                    solution.add(tg, project, nodes[0].path_from(self.srcnode).replace('/', '\\'))

        solution.write(solution_node)


class vs2005(vs2003):
    "creates projects for Visual Studio 2005"
    cmd = 'vs2005'
    fun = 'build'
    version = (('Visual Studio 2005', '9.00', True, None), (VCproj, '8.00'))
    platforms = ['Win32', 'x64']


class vs2005e(vs2003):
    "creates projects for Visual Studio 2005 Express"
    cmd = 'vs2005e'
    fun = 'build'
    version = (('Visual C++ Express 2005', '9.00', False, None), (VCproj, '8.00'))
    platforms = ['Win32', 'x64']


class vs2008(vs2003):
    "creates projects for Visual Studio 2008"
    cmd = 'vs2008'
    fun = 'build'
    version = (('Visual Studio 2008', '10.00', True, None), (VCproj, '9.00'))
    platforms = ['Win32', 'x64', 'Itanium']


class vs2008e(vs2003):
    "creates projects for Visual Studio 2008 Express"
    cmd = 'vs2008e'
    fun = 'build'
    version = (('Visual C++ Express 2008', '10.00', False, None), (VCproj, '9.00'))
    platforms = ['Win32', 'x64', 'Itanium']


class vs2010(vs2003):
    "creates projects for Visual Studio 2010"
    cmd = 'vs2010'
    fun = 'build'
    version = (('Visual Studio 2010', '11.00', True, None), (VCxproj, ('4.0', '10.0', '4.0')))
    platforms = ['Win32', 'x64', 'Itanium']


class vs2010e(vs2003):
    "creates projects for Visual Studio 2010 Express"
    cmd = 'vs2010e'
    fun = 'build'
    version = (('Visual C++ Express 2010', '11.00', False, None), (VCxproj, ('4.0', '10.0', '4.0')))
    platforms = ['Win32', 'x64', 'Itanium']


class vs11(vs2003):
    "creates projects for Visual Studio 2012"
    cmd = 'vs11'
    fun = 'build'
    version = (('Visual Studio 11', '12.00', True, None), (VCxproj, ('4.5', '11.0', '4.0')))
    platforms = ['Win32', 'x64', 'ARM', 'Itanium']


class vs2012(vs11):
    "creates projects for Visual Studio 2012"
    cmd = 'vs2012'
    fun = 'build'
    platforms = ['Win32', 'x64', 'ARM', 'Itanium']


class vs11e(vs2003):
    "creates projects for Visual Studio 2012 Express"
    cmd = 'vs11e'
    fun = 'build'
    version = (('Visual C++ Express 11', '12.00', False, '15.0.00000.0'), (VCxproj, ('4.5', '11.0', '4.0')))
    platforms = ['Win32', 'x64', 'ARM', 'Itanium']


class vs2012e(vs11e):
    "creates projects for Visual Studio 2012 Express"
    cmd = 'vs2012e'
    fun = 'build'
    platforms = ['Win32', 'x64', 'ARM', 'Itanium']


class vs2013e(vs2003):
    "creates projects for Visual Studio 2013 Express"
    cmd = 'vs2013e'
    fun = 'build'
    version = (('Visual C++ Express 12', '12.00', False, '15.0.00000.0'), (VCxproj, ('4.5', '12.0', '12.0')))
    platforms = ['Win32', 'x64', 'ARM', 'Itanium']


class vs2013(vs2003):
    "creates projects for Visual Studio 2013"
    cmd = 'vs2013'
    fun = 'build'
    version = (('Visual Studio 2013', '12.00', True, '12.0.00000.0'), (VCxproj, ('4.5', '12.0', '12.0')))
    platforms = ['Win32', 'x64', 'ARM', 'Itanium']


class vs2015(vs2003):
    "creates projects for Visual Studio 2015"
    cmd = 'vs2015'
    fun = 'build'
    version = (('Visual Studio 14', '12.00', True, '14.0.00000.0'), (VCxproj, ('4.5', '14.0', '14.0')))
    platforms = ['Win32', 'x64', 'ARM', 'Itanium']


class vs2017(vs2003):
    "creates projects for Visual Studio 2017"
    cmd = 'vs2017'
    fun = 'build'
    version = (('Visual Studio 15', '12.00', True, '15.0.00000.0'), (VCxproj, ('6.0', '14.1', '15.0')))
    platforms = ['Win32', 'x64', 'ARM', 'Itanium']


class vs2019(vs2003):
    "creates projects for Visual Studio 2019"
    cmd = 'vs2019'
    fun = 'build'
    version = (('Visual Studio 16', '12.00', True, '16.0.00000.0'), (VCxproj, ('6.0', '14.2', '16.0')))
    platforms = ['Win32', 'x64', 'ARM', 'Itanium']
