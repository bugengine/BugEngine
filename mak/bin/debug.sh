#!/bin/sh

waf_debug()
{
  local current_dir=`pwd`
  cd ${SrcDir}
  ${Python} debug:${Toolchain}:${Variant}
  cd $local
}

waf_debug


