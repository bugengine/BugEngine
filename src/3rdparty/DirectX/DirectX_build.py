import os

def build(bld):
    bld.thirdparty('3rdparty.DirectX9', var='dx9', feature='DirectX9')
    bld.thirdparty('3rdparty.DirectX10', var='dx10', feature='DirectX10')
    bld.thirdparty('3rdparty.DirectX11', var='dx11', feature='DirectX11')
