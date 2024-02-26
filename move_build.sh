#!/usr/bin/bash

SOURCE=${BASH_SOURCE[0]}
while [ -L "$SOURCE" ]; do # resolve $SOURCE until the file is no longer a symlink
  DIR=$( cd -P "$( dirname "$SOURCE" )" >/dev/null 2>&1 && pwd )
  SOURCE=$(readlink "$SOURCE")
  [[ $SOURCE != /* ]] && SOURCE=$DIR/$SOURCE # if $SOURCE was a relative symlink, we need to resolve it relative to the path where the symlink file was located
done
SOURCE_DIR=$( cd -P "$( dirname "$SOURCE" )" >/dev/null 2>&1 && pwd )

set -x

pushd ~/Documents/WiiSDSync/Project+ || exit
rm -r 'codes'
mkdir 'codes'
cp ${SOURCE_DIR}/build/Output/*  "codes/"

./GCTRealMate.exe ./NETPLAY.txt <<EOF

EOF
./GCTRealMate.exe ./RSBE01.txt <<EOF

EOF
popd

cd "${SOURCE_DIR}"
cp build/Disassembly/Symbols.map "${HOME}/Documents/Dolphin Emulator/Maps/RSBE01.map"