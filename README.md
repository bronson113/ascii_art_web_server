# Ascii Art Web Server

This is a simple web server that serves ascii art. It is written in C.

## Requirements
There are several library required to build this server.
The curl library is used to retrieve images from the internet.
To set up the environment, run the commands below.
```sh
apt-get install libcurl-dev
```


## Build

To build the server, run `make` in the root directory of the project. This will create a `server` in the project root directory.

## Run

To run the server, run `./server` in the root directory of the project. This will start the server on port 8080.

You can connect to the server using any browser by going to `http://localhost:8080`.

## TODO

- [X] Basic file structure
- [ ] Request Handler
    - [X] Spawn threads
    - [X] Dispatch requests
    - [X] Thread safe queue
    - [ ] Clean up thread on kill
    - [ ] Parse and deal with requests
- [ ] Response builder
- [X] Image download
- [ ] Image conversion
- [ ] Image display

