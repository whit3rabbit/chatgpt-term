# Replace "dockcross/linux-x64" with the appropriate dockcross image for your target architecture
FROM dockcross/linux-x64

# Install necessary dependencies
RUN apt-get update && \
    apt-get install -y --no-install-recommends \
        libssl-dev \
        libcrypto++-dev \
        zlib1g-dev \
        libcurl4-openssl-dev \
        libcjson-dev