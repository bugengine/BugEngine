@echo off

pushd %SrcDir%
%Python% debug:%Toolchain%:%Variant%
popd

