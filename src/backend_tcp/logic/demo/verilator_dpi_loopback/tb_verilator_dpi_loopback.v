

module tb_verilator_dpi_loopback
  (
   input clk,
   input rst
   );

   parameter WIDTH = 16;
   
   wire [WIDTH-1:0] data;
   wire             valid;
   wire             ready;
   
   wire             logic_rst;
   wire             com_rst;
   
   /* glip_tcp_toplevel AUTO_TEMPLATE(
    .fifo_.*_valid (valid),
    .fifo_.*_ready (ready),
    .fifo_.*_data  (data),
    .clk_io        (clk),
    .clk_logic     (clk),
    ); */
   glip_tcp_toplevel
     #(.WIDTH(WIDTH))
     u_glip(/*AUTOINST*/
            // Outputs
            .fifo_out_ready             (ready),                 // Templated
            .fifo_in_data               (data),                  // Templated
            .fifo_in_valid              (valid),                 // Templated
            .logic_rst                  (logic_rst),
            .com_rst                    (com_rst),
            // Inputs
            .clk_io                     (clk),                   // Templated
            .clk_logic                  (clk),                   // Templated
            .rst                        (rst),
            .fifo_out_data              (data),                  // Templated
            .fifo_out_valid             (valid),                 // Templated
            .fifo_in_ready              (ready));                // Templated

endmodule

// Local Variables:
// verilog-library-directories:("." "../../dpi")
// End:
