# ByteBabble

Under Construction 🚧

# Instructions to run simple server/client

1. Open two terminals
2. Compile both files using `Make`
3. Run server using `./server <port>` use any port number instead of `<port>`, for example `./server 8080`
4. Run client using `./client <server-ip> <port>` use server ip and port number instead of `<server-ip>` and `<port>`, for example `./client loopback 8080`. **Use loopback for now because we are running the server on the same machine**

# Useful commands:

- 'make' to compile the code
- `fuser -k <port>/tcp` To clear the port if it is already in use (Use with caution, only for ports that you blocked yourself)

# Ideas:

- Allow client & server to communicate simultaneously
- Add a way to send files
- Add a way to send messages to multiple clients at once (broadcast)
- Add a way to send messages to specific clients
- Add a way for clients to send messages to each other
- Chatrooms?

#### Amazing resource that I totally didn't copy from: [Youtube](https://www.youtube.com/watch?v=_lQ-3S4fJ0U&list=PLPyaR5G9aNDvs6TtdpLcVO43_jvxp4emI&index=1)
