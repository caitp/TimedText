#!/bin/sh
repo=http://googletest.googlecode.com/svn/trunk/
start=$PWD
script=$(cd "$(dirname "$0")" && pwd)
root=$(cd "$script/.." && pwd)
framework=$root/test/gtest
dir=$framework/gtest-repo

svn checkout $repo $dir

rm -rf $framework/gtest $framework/*.cc
cd $dir/scripts
python fuse_gtest_files.py $framework
mv $framework/gtest/*.cc $framework/
mv $dir/src/gtest_main.cc $framework/
cd $framework
rm -rf $dir
cd $start
