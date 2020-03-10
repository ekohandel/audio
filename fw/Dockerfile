FROM ubuntu:18.04

ENV DEBIAN_FRONTEND noninteractive

RUN ln -fs /usr/share/zoneinfo/America/New_York /etc/localtime

# Update package repository
RUN apt-get update -y
RUN apt-get upgrade -y

# Install dependencies
RUN apt-get install -y --no-install-recommends git cmake ninja-build gperf \
    ccache dfu-util device-tree-compiler wget \
    python3-pip python3-setuptools python3-tk python3-wheel xz-utils file \
    make gcc gcc-multilib g++-multilib libsdl2-dev \
    gnupg2 software-properties-common

# Upgrade CMake
RUN wget -O - https://apt.kitware.com/keys/kitware-archive-latest.asc 2>/dev/null | apt-key add -
RUN apt-add-repository 'deb https://apt.kitware.com/ubuntu/ bionic main'
RUN apt-get update -y
RUN apt-get install -y cmake

# Install west
RUN pip3 install --user -U west
ENV PATH ${PATH}:~/.local/bin

# Install toolchain
RUN wget -O ~/.zephyr-sdk-0.11.2-setup.run https://github.com/zephyrproject-rtos/sdk-ng/releases/download/v0.11.2/zephyr-sdk-0.11.2-setup.run 2>/dev/null
RUN chmod +x ~/.zephyr-sdk-0.11.2-setup.run
RUN ~/.zephyr-sdk-0.11.2-setup.run -- -d ~/zephyr-sdk-0.11.2
RUN rm ~/.zephyr-sdk-0.11.2-setup.run
ENV ZEPHYR_TOOLCHAIN_VARIANT zephyr
ENV ZEPHYR_SDK_INSTALL_DIR ~/zephyr-sdk-0.11.2
