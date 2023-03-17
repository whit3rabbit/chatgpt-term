FROM debian:buster

RUN apt-get update && \
    apt-get install -y build-essential \
                       libssl-dev \
                       libcrypto++-dev \
                       zlib1g-dev \
                       libcurl4-openssl-dev \
                       libcjson-dev \
                       crossbuild-essential-arm64 \
                       crossbuild-essential-armhf

COPY . /src
WORKDIR /src

ENTRYPOINT ["/bin/bash", "-c"]
