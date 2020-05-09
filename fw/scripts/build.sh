#!/bin/bash

west update
source ./zephyr/zephyr-env.sh
west build -b $1 ./app -- -DBOARD_ROOT=$(pwd)/app
