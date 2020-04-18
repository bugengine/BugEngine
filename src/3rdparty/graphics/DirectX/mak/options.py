from be_typing import TYPE_CHECKING


def options(opt):
    # type: (Options.OptionsContext) -> None
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


if TYPE_CHECKING:
    from waflib import Options
