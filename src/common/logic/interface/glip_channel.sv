
interface glip_channel
  #(parameter WIDTH=16)
   (input clk);

   logic [WIDTH-1:0] data;
   logic             valid;
   logic             ready;

   modport master(output data, output valid, input ready);

   modport slave(input data, input valid, output ready);

   // a helper function to ease the assembly of interface signals
   function logic assemble ( input logic [15:0] m_data,
                             input logic m_valid
                             );
      data = m_data;
      valid = m_valid;
      return ready;
   endfunction // assemble
   
endinterface // glip_channel
