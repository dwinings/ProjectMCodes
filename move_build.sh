#!/usr/bin/bash

set -x

pushd "/c/Users/Wisp/Desktop/P+ builds/Project+ v2.4.2 Netplay (Windows)/User/Wii"
imdisk -a -m 'Q:' -v 1 -f "sd_debug.raw" -o rem
popd
rm -r '/q/Project+/codes'
mkdir -p '/q/Project+/codes'
cp build/Output/*  "/q/Project+/codes/"

pushd '/q/Project+' || exit

./GCTRealMate.exe ./NETPLAY.txt <<EOF

EOF
./GCTRealMate.exe ./RSBE01.txt <<EOF

EOF
popd

sleep 1
imdisk -D -m 'q:'