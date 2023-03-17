FROM --platform=$BUILDPLATFORM debian:buster-slim

RUN apt-get update && \
    apt-get install -y --no-install-recommends \
        build-essential \
        ca-certificates \
        crossbuild-essential-arm64 \
        crossbuild-essential-armel \
        crossbuild-essential-armhf \
        crossbuild-essential-i386 \
        devscripts \
        git \
        libssl-dev \
        libcrypto++-dev \
        libcurl4-openssl-dev \
        libcjson-dev \
        zlib1g-dev

WORKDIR /src
COPY . .

ENV DEB_BUILD_OPTIONS nocheck

ENTRYPOINT ["debuild", "-us", "-uc", "-ui", "--host-arch"]