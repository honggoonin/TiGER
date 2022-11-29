#!/usr/bin/env bash

algname="TiGER"
params="128 192 256"
for param in $params
do
    echo KAT ${algname}${param}
    mkdir KAT_VALUES/KEM/${algname}${param}
    cp ../Reference_Implementation/${algname}${param}/*.c ./${algname}-KAT-ref/KAT-${algname}${param}

    cd ${algname}-KAT-ref/KAT-${algname}${param}
    rm *.req *.rsp
    make new
    cp *.req ../../KAT_VALUES/KEM/${algname}${param}
    cp *.rsp ../../KAT_VALUES/KEM/${algname}${param}

    cd ../../
done

