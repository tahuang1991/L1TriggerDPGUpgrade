#!/bin/bash
# Here we check out any dependencies we need for the package itself
pushd $CMSSW_BASE/src
cvs co -rV00-07-05 CommonTools/Utils
popd