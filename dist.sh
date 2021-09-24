#!/bin/bash

cd `dirname $0`
version=`cat version.txt`
filename=simpson-${version}.zip
echo "writing $filename"
git archive --format=zip -o ${filename} --prefix=simpson/ master
