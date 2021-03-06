FROM debian:latest

ENV CC=/usr/bin/gcc \
    CXX=/usr/bin/g++ \
    EDITOR="code --wait" \
    PROMPT='%m %~ %# '

RUN apt-get update && apt-get install -y gcc g++ build-essential cmake git libc6-dev gdb lcov ninja-build meson curl nodejs npm zsh
RUN npm install --global http-server

RUN curl -L -o /usr/local/src/googletest-1.10.0.tar.gz https://github.com/google/googletest/archive/refs/tags/release-1.10.0.tar.gz \
 && tar -C /usr/local/src -xzf /usr/local/src/googletest-1.10.0.tar.gz \
 && cd /usr/local/src/googletest-release-1.10.0 \
 && cmake . \
 && make all install

RUN useradd -m -u 1000 -U josiahp
RUN chsh -s /bin/zsh josiahp
USER josiahp
RUN touch ~/.zshrc

WORKDIR /home/josiahp

CMD sleep infinity
