#!/bin/sh

if [ -z $BAMBOO_SOURCE ]
then
    BAMBOO_SOURCE=$PWD
fi

if [ -z $1 ]
then
    echo "please provide the detector set"
    exit
else
    det_set=$1
fi

if [ ! -d $BAMBOO_SOURCE/include/detector ]
then
   echo "Please make sure you are in the top level of the BambooMC directory or have set the variable of BAMBOO_SOURCE"
   exit
fi
   
cd $BAMBOO_SOURCE/include/detector

for a in $BAMBOO_SOURCE/detectors/$det_set/*.hh
do
    bname=$(basename $a)
    if [ ! -e "$bname" ]
    then
        echo "create symbolic link for $bname..."
        ln -s $a
    fi
done

cd $BAMBOO_SOURCE/src/detector

for a in $BAMBOO_SOURCE/detectors/$det_set/*.cc
do
    bname=$(basename $a)
    if [ ! -e "$bname" ]
    then
        echo "create symbolic link for $bname..."
        ln -s $a
    fi
done
