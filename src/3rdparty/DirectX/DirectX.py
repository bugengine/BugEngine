import os

def options(opt):
    dx_sdks = []
    try:
        try:
            import _winreg as winreg
        except:
            import winreg
        try:
            sdks = winreg.OpenKey(winreg.HKEY_LOCAL_MACHINE, 'SOFTWARE\\Wow6432node\\Microsoft\\DirectX\\')
        except WindowsError:
            sdks = winreg.OpenKey(winreg.HKEY_LOCAL_MACHINE, 'SOFTWARE\\Microsoft\\DirectX\\')
        index = 0
        while 1:
            try:
                version = winreg.EnumKey(sdks, index)
            except WindowsError:
                break
            index = index+1
            try:
                sdk_version = winreg.OpenKey(sdks, version)
                version,type = winreg.QueryValueEx(sdk_version,'InstalledVersion')
                path,type = winreg.QueryValueEx(sdk_version,'InstallPath')
                dx_sdks.append((version, str(path)))
            except:
                pass
    except:
        pass
    dx_sdks.sort(key=lambda x: x[0])
    gr = opt.get_option_group('SDK paths and options')
    gr.add_option( '--directx-sdk',
                action='store',
                default=dx_sdks and dx_sdks[-1][1] or '',
                dest='dx_sdk',
                help='Location of the DirectX SDK')


def setup(conf):
    from waflib import Options
    if Options.options.dx_sdk:
        includes=[os.path.join(Options.options.dx_sdk, 'Include')]
        libdirs=[os.path.join(Options.options.dx_sdk, 'Lib', 'x86'), os.path.join(Options.options.dx_sdk, 'Lib', 'x64'), os.path.join(Options.options.dx_sdk, 'Lib', 'arm')]
    else:
        includes=[]
        libdirs=[]
    conf.check_lib('d3d9', var='dx9', libpath=libdirs, includepath=includes,
                   includes=['d3d9.h'],
                   functions=['Direct3DCreate9(0)'])
    conf.check_lib('d3d10', var='dx10', libpath=libdirs, includepath=includes,
                   includes=['d3d10.h'],
                   functions=['D3D10CreateDevice(0, D3D10_DRIVER_TYPE_HARDWARE, 0, 0, 0, 0)'])
    conf.check_lib('d3d11', var='dx11', libpath=libdirs, includepath=includes,
                   includes=['d3d11.h'],
                   functions=['D3D11CreateDevice(0, D3D_DRIVER_TYPE_HARDWARE, 0, 0, 0, 0, 0, 0, 0, 0)'])

