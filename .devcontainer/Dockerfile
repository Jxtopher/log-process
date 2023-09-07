FROM gcc:13.2.0

ARG DEBIAN_FRONTEND=noninteractive

# Package installation 
RUN apt-get update -y && \ 
    apt-get install -y --no-install-recommends git build-essential \
    gdb \
    cmake \
    libgtest-dev \
    cppcheck \
    clang-tidy \
    clang-format \
    libboost-program-options-dev \
    ccache \
    gcovr \
    pkg-config \
    doxygen \
    graphviz \
    sudo \
    valgrind \
    && apt-get clean \
    && rm -rf /var/lib/apt/lists/* /tmp/* /var/tmp/* \
    && useradd -ms /bin/bash user \
    && chmod 660 /etc/sudoers && echo "user ALL=NOPASSWD: ALL" >> /etc/sudoers && chmod 400 /etc/sudoers

WORKDIR /app

ENTRYPOINT ["su", "user"]
