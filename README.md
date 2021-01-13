# Network traffic
**Real time network traffic monitor for the text console**

## Introduction

nload is a console application which monitors network traffic and bandwidth
usage in real time. It visualizes the in- and outgoing traffic using two
graphs and provides additional info like total amount of transfered data and
min/max network usage.

## Installing / Compiling nload

If you downloaded a snapshot from GitHub, you need to install automake and run `./run_autotools`.

Compiling is done by running `./configure && make`.

To install nload, you have to be root: `sudo make install`.

You can find more detailed information in the file INSTALL in this directory.

## Using nload

To learn about the command line switches and the usage of nload in general,
simply run `nload --help`. Alternatively, there is a man page available by
typing `man nload`.

## Uninstalling nload

If you want to uninstall nload run `make uninstall` in the build directory.


