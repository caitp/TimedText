#!/bin/sh
#cvs -d:pserver:anonymous@expat.cvs.sourceforge.net:/cvsroot/expat login
repo="-d:pserver:anonymous@expat.cvs.sourceforge.net:/cvsroot/expat"
tag="HEAD"
module="expat/lib"

start=$PWD
script=$(cd "$(dirname "$0")" && pwd)
root=$(cd "$script/.." && pwd)
framework=$root/src/expat
cd $start
dir=$$expat$$

cvs $repo export -r $tag -d $dir $module || exit 1

mkdir -p $framework
rm -rf $framework/*.hpp $framework/*.cpp $framework/*.c $framework/*.h

mv $dir/*.c $framework/
mv $dir/*.h $framework/
mv $dir/*.def $framework/

cd $start
rm -rf $dir
