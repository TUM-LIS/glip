SUBDIRS = . tools

pkginclude_HEADERS = $(top_srcdir)/include/libglip.h

lib_LTLIBRARIES = libglip.la

libglip_la_SOURCES =\
	cbuf.c \
	glip.c \
	util.c

hwdir = $(pkgdatadir)/hardware
hwcreditdir = $(hwdir)/common/credit/verilog
hwifdir = $(hwdir)/common/interface/verilog
hwscalerdir = $(hwdir)/common/scaler/verilog

hwcredit_DATA = common/logic/credit/verilog/creditor.v \
	common/logic/credit/verilog/debtor.v
hwif_DATA = common/logic/interface/verilog/glip_channel.sv
hwscaler_DATA = common/logic/scaler/verilog/glip_upscale.v \
	common/logic/scaler/verilog/glip_downscale.v

if BACKEND_UART
libglip_la_SOURCES += backend_uart/sw/backend_uart.c
uarthwdir = $(hwdir)/backend_uart/verilog
uarthw_DATA= backend_uart/logic/verilog/glip_uart_toplevel.v \
	backend_uart/logic/verilog/glip_uart_receive.v \
	backend_uart/logic/verilog/glip_uart_transmit.v \
	backend_uart/logic/verilog/glip_uart_control.v \
	backend_uart/logic/verilog/glip_uart_control_ingress.v \
	backend_uart/logic/verilog/glip_uart_control_egress.v
endif

if BACKEND_CYPRESSFX2
libglip_la_SOURCES += backend_cypressfx2/sw/backend_cypressfx2.c
AM_CFLAGS += $(libusb_CFLAGS)
AM_LDFLAGS += $(libusb_LIBS)
fx2hwdir = $(hwdir)/backend_uart/verilog
fx2hw_DATA = backend_cypressfx2/logic/verilog/glip_cypressfx2_toplevel.v
endif

if BACKEND_TCP
libglip_la_SOURCES += backend_tcp/sw/backend_tcp.c
tcphwdpidir = $(hwdir)/backend_tcp/sv-dpi
tcphwdpi_DATA = backend_tcp/logic/dpi/glip_tcp_toplevel.sv \
	backend_tcp/logic/dpi/glip_tcp_dpi.cpp \
	backend_tcp/logic/dpi/GlipTcp.h \
	backend_tcp/logic/dpi/GlipTcp.cpp
tcphwsyscdir = $(hwdir)/backend_tcp/systemc
tcphwsysc_DATA = backend_tcp/logic/systemc/glip_tcp_toplevel.cpp \
	backend_tcp/logic/systemc/glip_tcp_toplevel.h
endif

if BACKEND_JTAG
libglip_la_SOURCES += backend_jtag/sw/backend_jtag.c

backend_jtag_dir = $(pkgdatadir)/backend_jtag
openocd_dir = $(backend_jtag_dir)/openocd
openocd__DATA = $(top_srcdir)/src/backend_jtag/openocd/glip.cfg

jtaghwdir = $(hwdir)/backend_jtag/verilog
jtaghw_DATA = backend_jtag/logic/verilog/glip_jtag_toplevel.v \
	backend_jtag/logic/verilog/glip_jtag_tap.v \
	backend_jtag/logic/verilog/glip_jtag_tap_defines.v \
	backend_jtag/logic/verilog/glip_jtag_fifo.v \
	backend_jtag/logic/verilog/glip_jtag_input_fsm.v \
	backend_jtag/logic/verilog/glip_jtag_output_fsm.v \
	backend_jtag/logic/verilog/glip_jtag_config_discovery_fsm.v
endif

libglip_la_LDFLAGS = $(AM_LDFLAGS) $(LTLDFLAGS)
libglip_la_CPPFLAGS = $(AM_CPPFLAGS) \
                      -I $(top_srcdir)/include \
                      -DPKGDATADIR='"$(pkgdatadir)"'
