#! /usr/bin/env python
"""
    Main wscript for the BugENgine
"""

VERSION = "0.2.0"
APPNAME = "BugEngine"

top = '.'          #pylint: disable=invalid-name
out = 'bld/.waf'   #pylint: disable=invalid-name


def options(option_context):
    "recursively declare options to the parser"
    option_context.bugenginenode = option_context.path
    option_context.recurse('mak/libs/build_framework/options/options.py')
    option_context.recurse('src/options.py')


def configure(configuration_context):
    "entry point for build system configuration"
    configuration_context.bugenginenode = configuration_context.path
    configuration_context.recurse('mak/libs/build_framework/configure/configure.py')
    configuration_context.recurse('src/configure.py')


def multiarch_setup(configuration_context):
    "setup a set of toolchains in a multi-arch environment (e.g. Android, MacOS)"
    configuration_context.bugenginenode = configuration_context.path
    configuration_context.recurse('mak/libs/build_framework/setup/setup.py')

def setup(configuration_context):
    "setup a platform environment in the current configuration context"
    configuration_context.bugenginenode = configuration_context.path
    configuration_context.recurse('mak/libs/build_framework/setup/setup.py', once=False)
    configuration_context.recurse('src/setup.py', once=False)


def build(build_context):
    "set up build targets and executes the build"
    build_context.bugenginenode = build_context.path
    build_context.recurse('mak/libs/build_framework/build/build.py')
    build_context.recurse('src/build.py')


def deploy(build_context):
    "deploy a build to a device"
    build_context.bugenginenode = build_context.path
    #build_context.recurse('mak/libs/build_framework/deploy/deploy.py')


def run(build_context):
    "run a deployed build on a device"
    build_context.bugenginenode = build_context.path
    build_context.recurse('mak/libs/build_framework/run/run.py')


def debug(build_context):
    "start a debugging session on a device"
    build_context.bugenginenode = build_context.path
    build_context.recurse('mak/libs/build_framework/debug/debug.py')
