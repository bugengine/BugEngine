@echo off

pushd %SrcDir%
%Python% run:%Toolchain%:%Variant%
popd

