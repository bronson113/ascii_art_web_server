# Ascii Art Web Server

This is a simple web server that serves ascii art. It is written in C.

## Requirements
There are several library required to build this server.
The curl library is used to retrieve images from the internet.
To set up the environment, run the commands below.
```sh
apt-get install libcurl4-openssl-dev
```


## Build

To build the server, run `make` in the root directory of the project. This will create a `server` in the project root directory.

## Run

To run the server, run `./server` in the root directory of the project. This will start the server on port 1234.

You can connect to the server using any browser by going to `http://localhost:1234`.

## TODO

- [X] Basic file structure
- [X] Request Handler
    - [X] Spawn threads
    - [X] Dispatch requests
    - [X] Thread safe queue
    - [X] Clean up thread on kill
    - [X] Parse and deal with requests
- [X] Response builder
- [X] Image download
- [X] Image conversion
- [X] Image display

