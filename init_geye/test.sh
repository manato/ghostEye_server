#!/bin/sh

set -e
#trap 'echo "init_geye.sh failed..."; exit 1' ERR
trap 'echo -e "\n init_geye.sh failed...\n"' ERR

function test() 
{
    return 1
}


test
echo "hi there"
