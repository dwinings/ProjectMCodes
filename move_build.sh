#!/usr/bin/bash

set -x

pushd ~/Documents/WiiSDSync/Project+ || exit
rm -r 'codes'
mkdir 'codes'
cp ~/Code/ProjectMCodes/build/Output/*  "codes/"

./GCTRealMate.exe ./NETPLAY.txt <<EOF

EOF
./GCTRealMate.exe ./RSBE01.txt <<EOF

EOF
popd

cd ~/Code/ProjectMCodes
cp build/Disassembly/Symbols.map "${HOME}/Documents/Dolphin Emulator/Maps/RSBE01.map"