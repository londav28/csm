#
#
#

_null="/dev/null"
_home="${BASH_SOURCE[0]}"

function _get_script_loc() {
    local _path=$_home
    pushd . > _null
    if ([ -h "${_path}" ]); then
        while ([ -h "${_path}" ]); do cd `dirname "$_path"`;
        _path=`readlink "${_path}"`; done
    fi
    cd `dirname ${_path}` > _null
    local _result=`pwd` > _null
    popd > _null
}

local _cdir=$(_get_script_loc)

# Set home as the directory one above the script location.
echo "Script dir is $_cdir"
cd $_cdir > _null
pushd . > _null
cd .. > _null
export CSM_HOME=$(echo $PWD)
popd > _null
