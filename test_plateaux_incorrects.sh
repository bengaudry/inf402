#!/bin/sh

PLATEAUX_INCORRECTS=$(ls plateaux/incorrects/*.txt)
NB_TESTS_OK=0
NB_TESTS_KO=0

for PLATEAU_INCORRECT in $PLATEAUX_INCORRECTS
do
    echo "\033[33m[+] Test $PLATEAU_INCORRECT:\033[0m"
    ./test_plateau $PLATEAU_INCORRECT > /dev/null
    if [ $? -eq 1 ]
    then
        echo "\033[30;42mOK\033[0m"
        NB_TESTS_OK=$((NB_TESTS_OK+1))
    else
        echo "\033[30;41mKO\033[0m"
        NB_TESTS_KO=$((NB_TESTS_KO+1))
    fi
    echo
done

echo "\033[33m[+] Résultat des tests:\033[0m"
echo "\033[30;42m$NB_TESTS_OK tests OK\033[0m"
echo "\033[30;41m$NB_TESTS_KO tests KO\033[0m"
