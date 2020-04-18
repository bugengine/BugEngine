from be_typing import TYPE_CHECKING


def build(build_context):
    # type: (Build.BuildContext) -> None
    build_context.thirdparty('3rdparty.graphics.DirectX9', var='dx9', feature='DirectX9')
    build_context.thirdparty('3rdparty.graphics.DirectX10', var='dx10', feature='DirectX10')
    build_context.thirdparty('3rdparty.graphics.DirectX11', var='dx11', feature='DirectX11')


if TYPE_CHECKING:
    from waflib import Build