import os
import sys
sys.path.append(os.path.join(os.path.dirname(os.path.dirname(sys.argv[0])), 'libs'))

if __name__ == '__main__':
    import ircc
    ircc.run()