import os
import sys
sys.path.insert(0, os.path.join(os.path.dirname(os.path.dirname(os.path.dirname(sys.argv[0]))), 'libs'))

if __name__ == '__main__':
    import cxx
    argument_context = cxx.init_arguments()
    cxx.run(argument_context)
