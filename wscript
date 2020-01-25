#! /usr/bin/env python
"""
    Main wscript for the BugENgine
"""


VERSION = "0.2.0"
APPNAME = "BugEngine"

top = '.'           #pylint: disable=invalid-name
out = 'bld/.waf'    #pylint: disable=invalid-name

import build_framework

def options(option_context):
    "recursively declare options to the parser"
    build_framework.options(option_context)


def configure(configuration_context):
    "entry point for build system configuration"
    build_framework.configure(configuration_context)


def setup(configuration_context):
    "setup a platform environment in the current configuration context"
    build_framework.setup(configuration_context)


def build(build_context):
    "set up build targets and executes the build"
    build_framework.build(build_context)
    build_context.recurse('src/build.py')
    build_framework.plugins(build_context)


def deploy(build_context):
    "deploy a build to a device"
    build_framework.deploy(build_context)


def run(build_context):
    "run a deployed build on a device"
    build_framework.run(build_context)


def debug(build_context):
    "start a debugging session on a device"
    build_framework.debug(build_context)
