# Fetch the Ubuntu image as a base
FROM ubuntu:22.04

# Install dependencies
#   Downloading Sphinx prompts the user for additional input. This is
#   disabled with DEBIAN_FRONTEND='noninteractive'
RUN \
    apt-get update && \
    apt-get install -y git && \
    apt-get install -y build-essential && \
    apt-get install -y flex && \
    apt-get install -y bison && \
    apt-get install -y cmake && \
    apt-get install -y python3 && \
    apt-get install -y python3-pip && \
    pip install breathe && \
    pip install furo && \
    DEBIAN_FRONTEND='noninteractive' apt-get install -y python3-sphinx && \
    pip install myst-parser && \
    pip install sphinx-autobuild

# Install Doxygen
ARG DOXYGEN_REPO_URL=https://github.com/doxygen/doxygen.git
RUN \
    git clone ${DOXYGEN_REPO_URL} && \
    cd doxygen && \
    mkdir build && \
    cd build && \
    cmake -G "Unix Makefiles" .. && \
    make install && \
    doxygen --version

EXPOSE 8000
WORKDIR "/Project"
CMD ["sphinx-autobuild", "--re-ignore", "xml/", "--port", "8000", "--host", "0.0.0.0", "Docs", "Docs/_build/html"]
