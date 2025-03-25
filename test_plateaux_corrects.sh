#!/bin/sh
make test_plateau
PLATEAUX_CORRECTS=$(ls plateaux/*.txt)
NB_TESTS_OK=0
NB_TESTS_KO=0

for PLATEAU_CORRECT in $PLATEAUX_CORRECTS
do
    echo "\033[33m[+] Test $PLATEAU_CORRECT:\033[0m"
    ./test_plateau $PLATEAU_CORRECT > /dev/null
    if [ $? -eq 0 ]
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

if [ $NB_TESTS_KO -eq 0 ]
then
    exit 0
else
    echo "\033[30;41m$NB_TESTS_KO tests KO\033[0m"
    exit 1
fi
