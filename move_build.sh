#!/usr/bin/bash

set -x

pushd "/c/Users/Wisp/Desktop/Project+ v2.4.2 Netplay (Windows)/User/Wii"
imdisk -a -m 'O:' -v 1 -f "sd_debug.raw" -o rem
popd
rm -r '/o/Project+/codes'
mkdir -p '/o/Project+/codes'
cp build/Output/*  "/o/Project+/codes/"

pushd '/o/Project+' || exit

./GCTRealMate.exe ./NETPLAY.txt <<EOF

EOF
./GCTRealMate.exe ./RSBE01.txt <<EOF

EOF
popd

sleep 1
imdisk -D -m 'O:'