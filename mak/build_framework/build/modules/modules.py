def build(build_context):
    build_context.recurse('module.py')
    build_context.recurse('external.py')
    build_context.recurse('third_party.py')
    build_context.recurse('headers.py')
    build_context.recurse('library.py')
    build_context.recurse('static_library.py')
    build_context.recurse('shared_library.py')
    build_context.recurse('engine.py')
    build_context.recurse('game.py')
    build_context.recurse('plugin.py')
    build_context.recurse('package.py')