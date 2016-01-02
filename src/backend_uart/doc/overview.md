@defgroup backend_uart UART
@ingroup backend

The UART backends (internal name: `uart`) connects to the target via a
UART device. Such UART devices are usually integrated on a board or
are otherwise are available as USB adapters. Nevertheless, only those
UART devices that have hardware flow control (`CTS` and `RTS` signal)
are supported. All standard baud rates of up to 4M are supported as
long the clock frequency is sufficiently larger than the baud rate.


Supported Features
==================
- Number of channels: 1 (fixed)
- FIFO width: 8 bit (fixed)


Components
==========

To make use of this backend, the standard linux terminal I/O is used.

For details and usage instructions for the individual components, see these
pages:
- @ref backend_uart-sw "libglip Backend"
- @ref backend_uart-logic "FPGA Logic"


Performance
==========

As UART is a relatively slow interface, the backend usually reaches
the maximum throughput. That is 10 symbols (8 bit payload plus start
and stop bit), meaning BAUD/10 symbols Byte/s. For example, With
115,200 Baud you get 11,520 Byte/s and with 3,000,000 Baud you get
300,000 Byte/s.
