#
# Run all the tests in the $(CSM_HOME)/test directory. Each test should
# have exactly one .out file which represents the output it should
# produce.
#

import sys
import os
import path

def get_env_var(name):
    result = os.environ[name]
    return result

def main(argv):
    pass

if __name__ == '__main__':
    main(sys.argv)

