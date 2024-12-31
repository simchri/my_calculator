FROM debian:bullseye-slim

# Install build requirements to build cmake (g++ git libssl-dev make) and some additional tools
RUN apt-get update && apt-get install --yes g++ git libssl-dev make sudo curl vim gdb bash

# Build/install cmake
RUN git clone --depth 1 --branch v3.31.3 https://gitlab.kitware.com/cmake/cmake.git /tmp/cmake-src && cd /tmp/cmake-src && ./bootstrap && make -j 20 && make install

RUN apt-get install --yes ninja-build=1.10.1-1

ENTRYPOINT ["/bin/bash"]
ENV TERM=xterm-256color

RUN git clone --depth 1 --branch v1.15.2 https://github.com/google/googletest.git /ext/googletest \
&& cd /ext/googletest \
&& mkdir build \
&& cd build \
&& cmake .. -DCMAKE_BUILD_TYPE=Release -GNinja && cmake --build . --target install -j10

RUN git clone --depth 1 --branch v1.9.1 https://github.com/google/benchmark.git /ext/benchmark \
&& cd /ext/benchmark  \
&& mkdir build \
&& cd build \
&& cmake .. -DCMAKE_BUILD_TYPE=Release -DGOOGLETEST_PATH=/ext/googletest -GNinja && cmake --build . --config Release --target install -j10

RUN git clone --depth 1 --branch v5.0.0 https://github.com/ArthurSonzogni/FTXUI.git /ext/FTXUI \
&& cd /ext/FTXUI  \
&& mkdir build \
&& cd build \
&& cmake .. -DCMAKE_BUILD_TYPE=Release -GNinja && cmake --build . --target install -j10

RUN apt-get install -y python3=3.9.2-3 python3-pip=20.3.4-4+deb11u1
RUN pip install --no-cache-dir --upgrade pip && \
    pip install --no-cache-dir --requirement /ext/benchmark/tools/requirements.txt

RUN apt-get install -y clangd=1:11.0-51+nmu5

RUN apt-get install -y cppcheck=2.3-1
