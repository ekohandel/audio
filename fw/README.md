# Prerequisites
The following tools are necessary as prerequisites:
- [Docker Desktop](https://www.docker.com/products/docker-desktop)
- [ST-Link Utility](https://github.com/texane/stlink)

# Build Environment
In order to simplify the build environment, a docker container is used.
This same container is used by the CI system which ensures a consistent build
environment is maintained.

If you are running the docker container on linux, you will need to enable the
[user namespace remap](https://docs.docker.com/engine/security/userns-remap)
functionality.

Basically you need to create the mapping in `/ect/subuid` and `/etc/subgid`:
```
<username>:1000:65536
```
and then configure the docker daemon by adding the following to
`/etc/docker/daemon.json`:
```
{
  "userns-remap": "<username>"
}
```

The container can then be run using
`docker run -it --rm -v $(pwd):/audio -w /audio electronshepherds/zephyr:latest`.

This should bring you to a prompt at the `/audio` directory of the image.

## Building Firmware
The firmware is located in the `fw` directory. After navigating to this
directory the application can be built using the following commands:
`scripts/build.sh <board name>`

## Debugging Firmware
The debugging process uses the standard GDB server/client configuration. The
GDB server can run on the host system using `st-util` or other appropriate GDB
server programs.

The GDB client can run inside the docker container using
`/opt/zephyr-sdk-0.11.2/arm-zephyr-eabi/bin/arm-zephyr-eabi-gdb --tui /audio/fw/build/zephyr/zephyr.elf`.
