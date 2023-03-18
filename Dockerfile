FROM ubuntu:latest

RUN apt-get update && \
    DEBIAN_FRONTEND=noninteractive apt-get install -y --no-install-recommends \
    build-essential \
    libssl-dev \
    libcrypto++-dev \
    zlib1g-dev \
    libcurl4-openssl-dev \
    libcjson-dev \
    && rm -rf /var/lib/apt/lists/*

WORKDIR /app

COPY . .

RUN ./configure && \
    make && \
    make install

# Create the Debian package
RUN apt-get update && \
    DEBIAN_FRONTEND=noninteractive apt-get install -y --no-install-recommends \
    devscripts \
    debhelper \
    fakeroot \
    && rm -rf /var/lib/apt/lists/*

CMD dpkg-buildpackage -us -uc
