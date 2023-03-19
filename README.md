# ChatGPT

A command-line tool to interact with OpenAI's GPT-3.5-turbo model using the OpenAI API.

## Prerequisites

- A Unix-based system (Linux or macOS)
- GCC compiler
- `libcurl`, `cJSON`, and their development files
- An OpenAI API key

## Download

Click the releases link on the right.  There are deb files or a static binary for 64bit system.
Install on debian based OS using:
```
dpkg -i chatgpt*.deb
```

## Usage

1. Run the ChatGPT tool with a question:
```
./chatgpt "tell me a haiku about market crash"
Stocks tumble, prices fall
Panic grips the market floor
Losses mount, hope fades.
```
2. Alternatively, you can run the ChatGPT tool and enter the question when prompted:
```
./chatgpt 
Enter your question: capital of france
The capital of France is Paris.
```

## Building the ChatGPT tool

1. Clone the repository:
```
sudo apt-get install libssl-dev libcrypto++-dev zlib1g-dev libcurl4-openssl-dev libcjson-dev
git clone https://github.com/whit3rabbit/chatgpt-term.git
```
2. Change into the cloned directory:

```
cd chatgpt
```
3. (optional) Build cJSON from source
```
git clone https://github.com/DaveGamble/cJSON.git
cd cJSON
mkdir build
cd build
cmake -DBUILD_SHARED_LIBS=OFF ..
make
sudo make install
```

4. Build the ChatGPT tool:

```
make all
```
4. (Optional) Build the static version of ChatGPT:

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

# Inside chatgpt-term folder
make static
```

## Installation

1. Install the ChatGPT tool:
```
make install
```
2. Set up the configuration file in `/etc/openai/chatgpt.config` with your OpenAI API key:
```
API_KEY=your_api_key_here
MODEL=gpt-3.5-turbo
```
3. (Optional) Install the ChatGPT tool locally:
```
make local
```

## Clean up

To clean up the compiled files, run:

```
make clean
```
