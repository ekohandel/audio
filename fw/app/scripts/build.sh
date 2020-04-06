#!/bin/bash

west update
pip3 install --user -r ./zephyr/scripts/requirements.txt
source ./zephyr/zephyr-env.sh
west build -b $1 ./app -- -DBOARD_ROOT=$(pwd)/app
