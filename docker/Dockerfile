# FROM debian@sha256:321341744acb788e251ebd374aecc1a42d60ce65da7bd4ee9207ff6be6686a62
FROM debian@sha256:9f3064e293387e18bb982af0d0a1eef29cbe4d1ad6bff01a2d55c2c033df6557

# Install build requirements to build cmake (g++ git libssl-dev make) and some additional tools
RUN apt-get update
RUN apt-get install --yes \
  bash \
  clangd=1:19.0-63 \
  cppcheck=2.16.0-1 \
  curl \
  g++ \
  g++-14=14.2.0-14 \
  gdb \
  git \
  libssl-dev \
  make \
  ninja-build=1.12.1-1 \
  sudo \
  vim

# Build/install cmake
RUN git clone --depth 1 --branch v3.31.3 https://gitlab.kitware.com/cmake/cmake.git /tmp/cmake-src && cd /tmp/cmake-src && ./bootstrap && make -j 20 && make install

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

RUN apt-get install --yes python3=3.13.1-2 python3-pip=24.3.1+dfsg-1
RUN pip install --break-system-packages --no-cache-dir setuptools

# TODO: WOrkaround. Instead of:
# RUN pip install --break-system-packages --no-cache-dir --requirement /ext/benchmark/tools/requirements.txt
# doing: (benchmark requirement file is outdated / not compat. with debian sid)
RUN pip install --break-system-packages --no-cache-dir numpy==1.26.4 scipy==1.15.1

RUN <<EOF
file=/etc/bash.bashrc
echo "shopt -s histappend" >> $file
echo 'PROMPT_COMMAND="history -a;$PROMPT_COMMAND"' >> $file
EOF
