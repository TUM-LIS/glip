
interface glip_channel
  #(parameter WIDTH=16)
   (input clk);

   logic [WIDTH-1:0] data;
   logic             valid;
   logic             ready;

   modport master(output data, output valid, input ready);

   modport slave(input data, input valid, output ready);
   
endinterface // glip_channel
