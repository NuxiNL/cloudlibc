#!/bin/sh
set -e

rm -Rf tmp
mkdir tmp
cd tmp

tar -xf ../tzdata2017c.tar.gz
tar -xf ../tzcode2017c.tar.gz

make zdump zic > /dev/null

mkdir output
./zic -d output africa antarctica asia australasia backward etcetera europe \
    northamerica pacificnew southamerica

cd output
i=0
curdir=`pwd`
find * -type f | sort | while read f; do
  echo "TEST_BEGIN(generated_$i, \"$f\")"
  ../zdump -v "$curdir/$f" | ../../convert_zdump_to_tests.py
  echo TEST_END
  i=$(($i+1))
done

cd ../..
rm -Rf tmp
