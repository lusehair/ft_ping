# ft_ping

A simple implementation of the `ping` command in C that sends ICMP echo requests to network hosts.

## Description

`ft_ping` is a custom version of the standard `ping` utility. It constructs ICMP echo requests and sends them to the specified host, then listens for ICMP echo replies. It reports packet loss and round-trip time statistics.

## Features

- Sends ICMP ECHO_REQUEST packets to network hosts
- Displays round-trip time for each packet
- Provides statistics on packet loss and latency

## Requirements

- Unix-like operating system
- C compiler (e.g., `gcc`)
- Root privileges (required for raw sockets)

## Installation

Clone the repository and build the executable using the provided Makefile:

```bash
git clone https://github.com/yourusername/ft_ping.git
cd ft_ping
make
```

## Usage

Run the `ft_ping` executable with the destination host:

```bash
sudo ./ft_ping [options] destination
```

### Options

- `-h`: Display help and usage information
- `-v`: Enable verbose output

### Examples

Ping a host:

```bash
sudo ./ft_ping google.com
```

Display help:

```bash
./ft_ping -h
```

## Makefile Targets

- `make`: Build the `ft_ping` executable
- `make clean`: Remove object files
- `make fclean`: Remove object files and the executable
- `make re`: Rebuild the project from scratch

## Project Structure

- `main.c`: Entry point and argument parsing
- `socket.c`: Socket creation and ping loop
- `ft_ping.h`: Definitions and function prototypes
- `Makefile`: Build instructions
