import os
from waflib import Options
from waflib.Logs import pprint
from be_typing import TYPE_CHECKING


def setup(configuration_context):
    # type: (Configure.ConfigurationContext) -> None
    if 'windows' in configuration_context.env.VALID_PLATFORMS:
        if Options.options.dx_sdk:
            includes=[os.path.join(Options.options.dx_sdk, 'Include')]
            libdirs = []
            for arch in configuration_context.env.VALID_ARCHITECTURES:
                p = os.path.join(Options.options.dx_sdk, 'Lib', arch)
                if os.path.isdir(p):
                    libdirs.append(p)
        else:
            includes=[]
            libdirs=[]
        configuration_context.check_lib('d3d9', var='dx9', libpath=libdirs, includepath=includes,
                                        includes=['d3d9.h'],
                                        functions=['Direct3DCreate9']) or pprint('YELLOW', '-dx9', sep=' ')
        configuration_context.check_lib('d3d10', var='dx10', libpath=libdirs, includepath=includes,
                                        includes=['d3d10.h'],
                                        functions=['D3D10CreateDevice']) or pprint('YELLOW', '-dx10', sep=' ')
        configuration_context.check_lib('d3d11', var='dx11', libpath=libdirs, includepath=includes,
                                        includes=['d3d11.h'],
                                        functions=['D3D11CreateDevice']) or pprint('YELLOW', '-dx11', sep=' ')


if TYPE_CHECKING:
    from waflib import Configure