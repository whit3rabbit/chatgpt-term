# ChatGPT

A command-line tool to interact with OpenAI's GPT-3.5-turbo model using the OpenAI API.

## Prerequisites

- A Unix-based system (Linux or macOS)
- GCC compiler
- `libcurl`, `cJSON`, and their development files
- An OpenAI API key

## Building the ChatGPT tool

1. Clone the repository:
```
git clone https://github.com/whit3rabbit/chatgpt-term.git
```
2. Change into the cloned directory:

```
cd chatgpt
```
3. Build the ChatGPT tool:

```
make all
```
4. (Optional) Build the static version of ChatGPT:

```
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
## Clean up

To clean up the compiled files, run:

```
make clean
```
