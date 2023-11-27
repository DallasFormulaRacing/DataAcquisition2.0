# Fetch the Ubuntu image as a base
FROM ubuntu:22.04


# A layer is made per RUN command.
# `apt-get update` to update the package manager.
# Try to put as many Ubuntu commands into a single RUN command
#   to reduce layers and speed up the image building process later.
# `build-essential` includes compilers (gcc and g++), make, etc.
RUN \
    apt-get update && \
    apt-get install -y python3 && \
    apt-get install -y build-essential && \
    apt-get install ninja-build && \
    apt-get install -y sudo && \
    sudo apt install -y cmake
