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

## Known Issues

After the client sends the socket options to the server, the server DOES receive and parse them successfully and uses them to set the socket options for the server, but there is an issue with how the server prints the received message. The issue is that the server will print out the received lines randomly, so although it does receive the message, parse them successfully, and set the socket option, not all the lines are printed as LINE RECEIVED. This makes it appear as if the lines are not being received, but when looking at the socket options afterwards, the new socket options are set.

- This problem appears to be exacerbated when executing the client from a different server than where the server is executed (ie client on zeus and server on eros).

- NOTE: executing the client a few times will yield different printing, so one execution could print 9 lines received and the next could print 28, so I suggest multiple runs for more useful obeservation.

## Licensing & thanks

This project is released under the [MIT License](./LICENSE.txt).

## Changelog

- v1.0.0
