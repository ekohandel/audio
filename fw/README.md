# Prerequisites
The following tools are necessary as prerequisites:
- [Docker Desktop](https://www.docker.com/products/docker-desktop)
- [ST-Link Utility](https://github.com/texane/stlink)

# Build Environment
In order to simplify the build environment, a docker container is used.
This same container is used by the CI system which ensures a consistent build
environment is maintained.

## Using the Docker Container
To use the docker container, we first need to grab it from the repository. This
can be done using `docker pull electronshepherds/zephyr:latest`.

The container can then be run using
`docker run -it -v $(pwd):/audio electronshepherds/zephyr:latest`.

This should bring you to a prompt at the root of the image. The repository is
mapped to `/audio` directory.

## Building Firmware
The firmware is located in the `fw` directory. After navigating to this
directory the application can be built using the following commands:
```
west update
pip3 install --user -r ./zephyr/scripts/requirements.txt
source ./zephyr/zephyr-env.sh
west build -b nucleo_wb55rg ./app
```

## Debugging Firmware
The debugging process uses the standard GDB server/client configuration. The
GDB server can run on the host system using `st-util`. The GDB client can run
inside the docker container using `~/zephyr-sdk-0.11.2/arm-zephyr-eabi/bin/arm-zephyr-eabi-gdb --tui /audio/fw/build/zephyr/zephyr.elf`.
