FROM debian@sha256:321341744acb788e251ebd374aecc1a42d60ce65da7bd4ee9207ff6be6686a62

# Install build requirements to build cmake (g++ git libssl-dev make) and some additional tools
RUN apt-get update && apt-get install --yes g++ git libssl-dev make sudo curl vim gdb bash

# Build/install cmake
RUN git clone --depth 1 --branch v3.31.3 https://gitlab.kitware.com/cmake/cmake.git /tmp/cmake-src && cd /tmp/cmake-src && ./bootstrap && make -j 20 && make install

RUN apt-get install --yes ninja-build=1.11.1-2~deb12u1

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

RUN apt-get install -y python3=3.11.2-1+b1 python3-pip=23.0.1+dfsg-1
RUN pip install --break-system-packages --no-cache-dir --upgrade pip && \
    pip install --break-system-packages --no-cache-dir --requirement /ext/benchmark/tools/requirements.txt

RUN apt-get install -y clangd=1:14.0-55.7~deb12u1

RUN apt-get install -y cppcheck=2.10-2

RUN <<EOF
file=/etc/bash.bashrc
echo "shopt -s histappend" >> $file
echo 'PROMPT_COMMAND="history -a;$PROMPT_COMMAND"' >> $file
EOF
