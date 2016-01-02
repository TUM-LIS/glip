/* Copyright (c) 2015-2016 by the author(s)
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in
 * all copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
 * THE SOFTWARE.
 *
 * =============================================================================
 *
 * UART Toplevel
 *
 * This module handles the UART interface and puts incoming bytes to a
 * FIFO and vice versa. The module only supports 8N1 UART, meaning 8
 * bit, no parity and one stop bit. All baud rates are supported, but
 * be careful with low frequencies and large baud rates that the
 * tolerance of the rounded bit divisor (rounding error of
 * FREQ/BAUD) is withint 2%.
 * 
 * Parameters:
 *  - FREQ: The frequency of the design, to match the second
 *  - BAUD: Interface baud rate
 *  - XILINX_TARGET_DEVICE: Xilinx device, allowed: "7SERIES"
 *
 * Author(s):
 *   Stefan Wallentowitz <stefan.wallentowitz@tum.de>
 */

module glip_uart_toplevel
  #(parameter FREQ = 32'hx,
    parameter BAUD = 115200,
    parameter XILINX_TARGET_DEVICE = "7SERIES")
   (
    // Clock & Reset
    input 	 clk_io,
    input 	 clk_logic,
    input 	 rst,

    // GLIP FIFO Interface
    input [7:0]  fifo_out_data,
    input 	 fifo_out_valid,
    output 	 fifo_out_ready,
    output [7:0] fifo_in_data,
    output 	 fifo_in_valid,
    input 	 fifo_in_ready,

    // UART Interface
    input 	 uart_rx,
    output 	 uart_tx,
    input 	 uart_cts,
    output 	 uart_rts,

    // Error signal if failure on the line
    output reg 	 error
    );

   wire [7:0] 	  in_data;
   wire 	  in_valid;
   wire 	  in_ready;
   wire [7:0] 	  out_data;
   wire 	  out_valid;
   wire 	  out_ready;   

   // Map FIFO signals to flow control
   wire 	  in_fifo_almostfull;
   wire 	  in_fifo_empty;
   wire 	  out_fifo_full;
   wire 	  out_fifo_empty;
   assign in_ready = ~in_almost_full;
   assign fifo_in_valid = ~in_fifo_empty;
   assign out_valid = ~out_fifo_empty;
   assign fifo_out_ready = ~out_fifo_full;

   // Ready to receive if FIFO has a few places left and no error
   // happened so far
   assign uart_rts = ~(in_ready & ~error);

   // Generate error. Sticky when an error occured. Currently we only
   // have receiver errors if the frame was incorrect
   wire 	  rcv_error;
   always @(posedge clk_io) begin
      if (rst) begin
	 error <= 0;
      end else begin
	 error <= error | rcv_error;
      end
   end
   
   /* glip_uart_receive AUTO_TEMPLATE(
    .clk (clk_io),
    .rx  (uart_rx),
    .enable (in_valid),
    .data   (in_data),
    .error  (rcv_error),
    ); */
   glip_uart_receive
     #(.DIVISOR(FREQ/BAUD))
   u_receive(/*AUTOINST*/
	     // Outputs
	     .enable			(in_valid),		 // Templated
	     .data			(in_data),		 // Templated
	     .error			(rcv_error),		 // Templated
	     // Inputs
	     .clk			(clk_io),		 // Templated
	     .rst			(rst),
	     .rx			(uart_rx));		 // Templated

   /* glip_uart_transmit AUTO_TEMPLATE(
    .clk    (clk_io),
    .tx     (uart_tx),
    .done   (out_ready),
    .enable (out_valid & ~uart_cts),
    .data   (out_data[]),
    ); */
   glip_uart_transmit
     #(.DIVISOR(FREQ/BAUD))
   u_transmit(/*AUTOINST*/
	      // Outputs
	      .tx			(uart_tx),		 // Templated
	      .done			(out_ready),		 // Templated
	      // Inputs
	      .clk			(clk_io),		 // Templated
	      .rst			(rst),
	      .data			(out_data[7:0]),	 // Templated
	      .enable			(out_valid & ~uart_cts)); // Templated
   
   // Clock domain crossing uart -> logic
   FIFO_DUALCLOCK_MACRO
     #(.ALMOST_FULL_OFFSET(9'h006), // Sets almost full threshold
       .ALMOST_EMPTY_OFFSET(9'h006),
       .DATA_WIDTH(8), // Valid values are 1-72 (37-72 only valid when FIFO_SIZE="36Kb")
       .DEVICE(XILINX_TARGET_DEVICE), // Target device: "VIRTEX5", "VIRTEX6", "7SERIES"
       .FIFO_SIZE("18Kb"), // Target BRAM: "18Kb" or "36Kb"
       .FIRST_WORD_FALL_THROUGH("TRUE") // Sets the FIfor FWFT to "TRUE" or "FALSE"
       )
   in_fifo
     (.ALMOSTEMPTY (),
      .ALMOSTFULL  (in_almost_full),
      .DO          (fifo_in_data[7:0]),
      .EMPTY       (in_fifo_empty),
      .FULL        (in_fifo_full),
      .RDCOUNT     (),
      .RDERR       (),
      .WRCOUNT     (),
      .WRERR       (),
      .DI          (in_data[7:0]),
      .RDCLK       (clk_logic),
      .RDEN        (fifo_in_ready & fifo_in_valid),
      .RST         (rst),
      .WRCLK       (clk_io),
      .WREN        (in_valid & in_ready)
      );
   
   // Clock domain crossing logic -> uart
   FIFO_DUALCLOCK_MACRO
     #(.ALMOST_EMPTY_OFFSET(9'h006), // Sets the almost empty threshold
       .ALMOST_FULL_OFFSET(9'h006),
       .DATA_WIDTH(8), // Valid values are 1-72 (37-72 only valid when FIFO_SIZE="36Kb")
       .DEVICE(XILINX_TARGET_DEVICE), // Target device: "VIRTEX5", "VIRTEX6", "7SERIES"
       .FIFO_SIZE("18Kb"), // Target BRAM: "18Kb" or "36Kb"
       .FIRST_WORD_FALL_THROUGH("TRUE") // Sets the FIfor FWFT to "TRUE" or "FALSE"
       )
   out_fifo
     (.ALMOSTEMPTY (),
      .ALMOSTFULL  (),
      .DO          (out_data),
      .EMPTY       (out_fifo_empty),
      .FULL        (out_fifo_full),
      .RDCOUNT     (),
      .RDERR       (),
      .WRCOUNT     (),
      .WRERR       (),
      .DI          (fifo_out_data[7:0]),
      .RDCLK       (clk_io),
      .RDEN        (out_ready & out_valid),
      .RST         (rst),
      .WRCLK       (clk_logic),
      .WREN        (fifo_out_valid & fifo_out_ready)
      );
   
endmodule // glip_uart_toplevel
