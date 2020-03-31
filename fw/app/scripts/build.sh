#!/bin/bash

west update
west zephyr-export
pip3 install --user -r ./zephyr/scripts/requirements.txt
source ./zephyr/zephyr-env.sh
west build -p auto -b $1 ./app -- -DBOARD_ROOT=$(pwd)/app
