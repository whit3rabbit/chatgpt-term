## Running

Set key three different ways:

Terminal
```
export OPENAI_API_KEY="your_api_key"
```

Hardocded
```
# Modify api_key.h and build normally
# #define API_KEY "your_hardcoded_api_key"
```

Set in file
```
# Add key to file:
# /etc/openai/chatgpt.config
```

## Development Build

```
sudo apt-get update
sudo apt-get install libcurl4-openssl-dev libidn2-dev

# Build cJSON
git clone https://github.com/DaveGamble/cJSON.git
cd cJSON
mkdir build
cd build
cmake -DBUILD_SHARED_LIBS=OFF ..
make
sudo make install
```

For static building:
```
# Build curl without LDAP
wget https://curl.se/download/curl-7.88.1.tar.gz
tar xvf curl*
cd curl-7.88.1
./configure --disable-ldap --disable-ldaps --with-openssl
make
sudo make install
sudo ldconfig

# Build libunistring
wget https://ftp.gnu.org/gnu/libunistring/libunistring-latest.tar.xz
tar xf libunistring-latest.tar.xz
cd libunistring-*/
./configure --prefix=/usr/local
make
sudo make install
```

To build a non-static executable (default), run:

make

To build a static executable, run:
make static

To install the executable to /usr/local/bin (default), run:
sudo make install

To install the executable to a local bin folder, run:
make local