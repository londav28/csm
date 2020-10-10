#
# Script to set $CSM_HOME environment variable. Naive!
#

# Find out the file path based on the shell, only supports BASH and ZSH.
if [ -n "${BASH_VERSION}" ]; then
    _file_path=${BASH_SOURCE[0]}
elif [ -n "${ZSH_VERSION}" ]; then
    _file_path=${(%):-%N}
else
    echo "Can only run setup script with BASH or ZSH!"
    return 1
fi

# Traverse to the root directory and get the path.
_this_dir=$(cd "$(dirname "${_file_path}")" && pwd)
_home=$(cd $_this_dir/../ && pwd)

# Blindly assume the root directory is home.
export CSM_HOME=${_home}
export CSM_BIN=$CSM_HOME/bin

