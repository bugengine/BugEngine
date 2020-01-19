from . import (aarch64, amd64, arm, arm64, armv6, armv7, armv7a, armv7k, armv7s, i386, ia64, mips, mips64, mips64el,
               mipsel, mipsel64, powerpc, powerpc64, ppc, ppc64, x64, x86, x86_64)

def configure(configuration_context, arch):
    globals()[arch].configure(configuration_context)
