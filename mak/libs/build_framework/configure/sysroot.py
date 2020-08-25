from waflib import Options
import os
from elftools.elf.elffile import ELFFile
from elftools.common.exceptions import ELFError

PLATFORMS = {
    'ELFOSABI_LINUX': 'linux',
    'ELFOSABI_FREEBSD': 'pc-freebsd',
    'ELFOSABI_SOLARIS': 'solaris',
}
ARCHITECTURES = {
    'EM_386': 'i386',
    'EM_X86_64': 'x86_64',
    'EM_PPC': 'ppc',
    'EM_PPC64': 'ppc64',
    'EM_ARM': 'arm',
    'EM_AARCH64': 'aarch64',
}

LITTLE_ENDIAN = {
    'arm': 'armel',
    'aarch64': 'aarch64',
    'ppc64': 'ppc64le',
}

BIG_ENDIAN = {
    'arm': 'armbe',
}

ARM_NAMES = {
    0: 'Pre-v4',
    1: 'v4',
    2: 'v4T',
    3: 'v5T',
    4: 'v5TE',
    5: 'v5TEJ',
    6: 'v6',
    7: 'v6KZ',
    8: 'v6T2',
    9: 'v6K',
    10: 'v7',
    11: 'v6-M',
    12: 'v6S-M',
    13: 'v7E-M',
    14: 'v8',
    15: 'v8-R',
    16: 'v8-M.baseline',
    17: 'v8-M.mainline',
}

MULTILIB_DIRS = {
    'arm': {
        'v5': 'armv5',
        'v6': 'armv6',
        'v7': 'armv7',
    },
    'x86':
        {
            '64': 'x86_64',
            'amd64': 'x86_64',
            'x86_64': 'x86_64',
            'i386': 'i386',
            'i486': 'i486',
            'i586': 'i586',
            'i686': 'i686',
        },
    'i386': {
        '64': 'x86_64',
        'amd64': 'x86_64',
        'x86_64': 'x86_64',
        'i486': 'i486',
        'i586': 'i586',
        'i686': 'i686',
    },
    'i486': {
        '64': 'x86_64',
        'amd64': 'x86_64',
        'x86_64': 'x86_64',
        'i386': 'i386',
        'i586': 'i586',
        'i686': 'i686',
    },
    'i586': {
        '64': 'x86_64',
        'amd64': 'x86_64',
        'x86_64': 'x86_64',
        'i386': 'i386',
        'i486': 'i486',
        'i686': 'i686',
    },
    'i686': {
        '64': 'x86_64',
        'amd64': 'x86_64',
        'x86_64': 'x86_64',
        'i386': 'i386',
        'i486': 'i486',
        'i586': 'i586',
    },
    'x86_64': {
        '32': 'i386',
        'i386': 'i386',
        'i486': 'i486',
        'i586': 'i586',
        'i686': 'i686',
    },
    'ppc': {
        '64': 'ppc64',
        'ppc64': 'ppc64',
        'powerpc64': 'powerpc64'
    },
    'powerpc': {
        '64': 'powerpc64',
        'ppc64': 'ppc64',
        'powerpc64': 'powerpc64'
    },
    'ppc64': {
        '32': 'ppc',
        'ppc': 'ppc',
        'powerpc': 'powerpc'
    },
    'powerpc64': {
        '32': 'powerpc',
        'ppc': 'ppc',
        'powerpc': 'powerpc'
    }
}


def get_sysroot_libpaths(sysroot):
    try:
        confs = os.listdir(os.path.join(sysroot, 'etc', 'ld.so.conf.d'))
    except OSError:
        return [os.path.join(sysroot, 'usr', x) for x in ('lib32', 'lib64', 'libx32', 'lib')]
    else:
        libpaths = []
        for conf in confs:
            try:
                f = open(os.path.join(sysroot, 'etc', 'ld.so.conf.d', conf), 'r')
            except OSError:
                pass
            else:
                for line in f:
                    line = line.split('#')[0].strip()
                    if not line:
                        continue
                    elif line.startswith('include'):
                        continue
                    elif os.path.isdir(os.path.join(sysroot, line)):
                        libpaths.append(os.path.join(sysroot, line))
        return libpaths


def configure(configuration_context):
    configuration_context.env.SYSROOTS = []
    for sysroot in Options.options.sysroots:
        targets = []
        configuration_context.start_msg('Checking sysroot %s' % sysroot)
        try:
            # try to find GCC triples
            gcc_targets = os.listdir(os.path.join(sysroot, 'usr', 'lib', 'gcc'))
        except OSError as e:
            for libpath in get_sysroot_libpaths(sysroot):
                try:
                    content = os.listdir(libpath)
                except OSError:
                    pass
                else:
                    platform = None
                    arch = None
                    abi = None
                    suffix = None
                    for item in content:
                        path = os.path.join(libpath, item)
                        if os.path.isfile(path):
                            with open(path, 'rb') as elffile:
                                try:
                                    elf = ELFFile(elffile)
                                except ELFError as e:
                                    pass
                                else:
                                    platform = PLATFORMS.get(elf.header.e_ident['EI_OSABI'], platform)
                                    arch = ARCHITECTURES.get(elf.header.e_machine, arch)
                                    if arch == 'arm':
                                        abi_flags = elf.header.e_flags >> 24
                                        if abi_flags != 0:
                                            abi = 'gnueabi'
                                        for section in elf.iter_sections():
                                            if section['sh_type'] == 'SHT_ARM_ATTRIBUTES':
                                                for subsection in section.iter_subsections():
                                                    for subsubsection in subsection.iter_subsubsections():
                                                        for attribute in subsubsection.attributes:
                                                            if attribute.tag == 'TAG_ABI_VFP_ARGS':
                                                                suffix = 'hf'
                                                            if attribute.tag == 'TAG_CPU_ARCH':
                                                                arch += ARM_NAMES[attribute.value]
                                    if elf.header.e_ident['EI_DATA'] == 'ELFDATA2LSB':
                                        arch = LITTLE_ENDIAN.get(arch, arch)
                                    elif elf.header.e_ident['EI_DATA'] == 'ELFDATA2MSB':
                                        arch = BIG_ENDIAN.get(arch, arch)
                                    if suffix is not None:
                                        #arch = arch + suffix
                                        if abi is not None:
                                            abi = abi + suffix
                                    elif elf.header.e_flags & 0x00000400:
                                        arch = arch + 'hf'
                                    if platform is not None and arch is not None:
                                        if abi is not None:
                                            targets.append('%s-%s-%s' % (arch, platform, abi))
                                        else:
                                            targets.append('%s-%s' % (arch, platform))
                                    break
        else:
            for target in gcc_targets:
                gcc_target_path = os.path.join(sysroot, 'usr', 'lib', 'gcc', target)
                target_arch = target.split('-')[0]

                for version in os.listdir(gcc_target_path):
                    if target not in targets:
                        targets.append(target)
                    gcc_target_version_path = os.path.join(gcc_target_path, version)
                    for multilib in os.listdir(gcc_target_version_path):
                        if os.path.isfile(os.path.join(gcc_target_version_path, multilib, 'libgcc.a')):
                            try:
                                multilib_arch = MULTILIB_DIRS[target_arch][multilib]
                            except KeyError:
                                print('unknown multilib: %s/%s' % (arch, multilib))
                            else:
                                multilib_target = '-'.join([multilib_arch] + target.split('-')[1:])
                                if multilib_target not in targets:
                                    targets.append(multilib_target)

        configuration_context.env.append_value('SYSROOTS', [(sysroot, targets)])
        configuration_context.end_msg(', '.join(targets))
