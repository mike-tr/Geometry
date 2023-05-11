#! /bin/sh/

echo $# 
if [ $# -ge 1 ] && [ "${1}" == "r" ]; then
    sh run.sh
else
    sh configure.sh
    sh build.sh
    if [ $? -eq 0 ]; then
        #echo build_success
        sh run.sh
    else
        echo FAIL
    fi
fi