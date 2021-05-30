import os
import sys
sys.path.insert(0, os.path.join(os.path.dirname(os.path.dirname(os.path.dirname(sys.argv[0]))), 'libs'))

if __name__ == '__main__':
    import glrp_test
    glrp_test.run()
