#!/bin/sh

waf_run()
{
  local current_dir=`pwd`
  cd ${SrcDir}
  ${Python} run:${Toolchain}:${Variant}
  cd $local
}

waf_run


