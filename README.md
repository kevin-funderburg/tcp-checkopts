# Advanced Network Programming - Socket Options 

This project is an exploration of socket options within a TCP client/server interface.

## About

The goal of this project is to send to a server all the socket options of a client, and based upon the values of the socket options received fromt the client, the server will set new values accordingly.

## Configuration

To compile, navigate to `unpv13e/tcpcliserv`

### Make commands

```shell
make        # compile and create executables
make clean  # reset executables and object files
```

Once compiled, begin server with `./checkoptsserv` then the client with `./checkoptscli <IPADDRESS>` where `<IPADDRESS>` will be IP address where the server is running, these IP addresses are listed bellow.g

1. eros - 147.26.231.153
2. zeus - 147.26.231.156
3. localhost - 127.0.0.1

## Examples

```shell
#if both the client and server are running on the same server
./checkoptscli 127.0.0.1

#if server is on eros and client is elsewhere
./checkoptscli 147.26.231.153

#if server is on zues and client is elsewhere
./checkoptscli 147.26.231.156
```

## Licensing & thanks

This project is released under the [MIT License](./LICENSE.txt).

## Changelog

- v1.0.0
