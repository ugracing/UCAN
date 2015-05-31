#! /bin/sh

BuildSrc=`pwd`
BuildRAM="/tmp/build"
BuildCleanup="rm -rf *.o *.a *.as *.ppu"
BuildIncludeFolders="Src/"
BuildIncludeFiles="*.pas *.inc"
#use this line for valgrind (pg mode, gprof compat)
#BuildCommand=`echo "fpc -fPIC -pg -gl " " $1"`

#use this line for normal gdb support
BuildCommand=`echo "fpc -fPIC -gl " " $1"`

echo "Preparing area in /tmp"
mkdir $BuildRAM

echo "Copying data to RAM..."
#cp -rf $BuildTarget $BuildRAM
cp -rvf $BuildIncludeFolders $BuildRAM
cp -rvf $BuildIncludeFiles $BuildRAM
#cp -rvf $BuildCleanup $BuildRAM

echo "Compiling..."
cd $BuildRAM
echo $BuildCommand
echo "`$BuildCommand`"

echo -n "Cleaning... "
`$BuildCleanup`
cp -rf * $BuildSrc
cd $BuildSrc
rm -rf $BuildRAM
./clean.sh

echo "Done."
exit
