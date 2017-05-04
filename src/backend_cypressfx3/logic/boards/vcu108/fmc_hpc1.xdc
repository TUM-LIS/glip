# FX3 Evaluation Board connected to the FMC HPC1 (J2) connector on the VCU108
# board

# FX3: A0, FMC: G37, FPGA: AG33
set_property PACKAGE_PIN AG33 [get_ports {fx3_a[0]}]
# FX3: A1, FMC: G36, FPGA: AG32
set_property PACKAGE_PIN AG32 [get_ports {fx3_a[1]}]

# FX3: COM_RST, FMC: C11, FPGA: AN34
set_property PACKAGE_PIN AN34 [get_ports fx3_com_rst]
# FX3: LOGIC_RST, FMC: C10, FPGA: AN35
set_property PACKAGE_PIN AN35 [get_ports fx3_logic_rst]

# FX3: FLAGA, FMC: G25, FPGA: AL31
set_property PACKAGE_PIN AL31 [get_ports fx3_flaga_n]
# FX3: FLAGB, FMC: G27, FPGA: AP36
set_property PACKAGE_PIN AP36 [get_ports fx3_flagb_n]
# FX3: FLAGC, FMC: G28, FPGA: AP37
set_property PACKAGE_PIN AP37 [get_ports fx3_flagc_n]
# FX3: FLAGD, FMC: G31, FPGA: AR38
set_property PACKAGE_PIN AR38 [get_ports fx3_flagd_n]

# FX3: PKTEND, FMC: G30, FPGA: AP38
set_property PACKAGE_PIN AP38 [get_ports fx3_pktend_n]

# FX3: SLCS, FMC: G19, FPGA: U32
set_property PACKAGE_PIN U32 [get_ports fx3_slcs_n]
# FX3: SLOE, FMC: G22, FPGA: AT37
set_property PACKAGE_PIN AT37 [get_ports fx3_sloe_n]
# FX3: SLRD, FMC: G24, FPGA: AL30
set_property PACKAGE_PIN AL30 [get_ports fx3_slrd_n]
# FX3: SLWR, FMC: G21, FPGA: AR37
set_property PACKAGE_PIN AR37 [get_ports fx3_slwr_n]


# FX3: PCLK, FMC: G6, FPGA: T33
set_property PACKAGE_PIN T33 [get_ports fx3_pclk]


# FX3: DQ0, FMC: H4, FPGA: R32
set_property PACKAGE_PIN R32 [get_ports {fx3_dq[0]}]
# FX3: DQ1, FMC: H7, FPGA: N33
set_property PACKAGE_PIN H24 [get_ports {fx3_dq[1]}]
# FX3: DQ2, FMC: H8, FPGA: M33
set_property PACKAGE_PIN M33 [get_ports {fx3_dq[2]}]
# FX3: DQ3, FMC: H10, FPGA: M37
set_property PACKAGE_PIN M37 [get_ports {fx3_dq[3]}]
# FX3: DQ4, FMC: H11, FPGA: L38
set_property PACKAGE_PIN L38 [get_ports {fx3_dq[4]}]
# FX3: DQ5, FMC: H13, FPGA: L34
set_property PACKAGE_PIN L34 [get_ports {fx3_dq[5]}]
# FX3: DQ6, FMC: H14, FPGA: K34
set_property PACKAGE_PIN K34 [get_ports {fx3_dq[6]}]
# FX3: DQ7, FMC: H16, FPGA: Y31
set_property PACKAGE_PIN Y31 [get_ports {fx3_dq[7]}]
# FX3: DQ8, FMC: H17, FPGA: W31
set_property PACKAGE_PIN W31 [get_ports {fx3_dq[8]}]
# FX3: DQ9, FMC: H19, FPGA: T34
set_property PACKAGE_PIN T34 [get_ports {fx3_dq[9]}]
# FX3: DQ10, FMC: H20, FPGA: T35
set_property PACKAGE_PIN T35 [get_ports {fx3_dq[10]}]
# FX3: DQ11, FMC: H22, FPGA: AT39
set_property PACKAGE_PIN AT39 [get_ports {fx3_dq[11]}]
# FX3: DQ12, FMC: H23, FPGA: AT40
set_property PACKAGE_PIN AT40 [get_ports {fx3_dq[12]}]
# FX3: DQ13, FMC: H25, FPGA: AT35
set_property PACKAGE_PIN AT35 [get_ports {fx3_dq[13]}]
# FX3: DQ14, FMC: H26, FPGA: AT36
set_property PACKAGE_PIN AT36 [get_ports {fx3_dq[14]}]
# FX3: DQ15, FMC: H28, FPGA: AM36
set_property PACKAGE_PIN AM36 [get_ports {fx3_dq[15]}]
# FX3: DQ16, FMC: H29, FPGA: AN36
set_property PACKAGE_PIN AN36 [get_ports {fx3_dq[16]}]
# FX3: DQ17, FMC: H31, FPGA: AL35
set_property PACKAGE_PIN AL35 [get_ports {fx3_dq[17]}]
# FX3: DQ18, FMC: H32, FPGA: AL36
set_property PACKAGE_PIN AL36 [get_ports {fx3_dq[18]}]
# FX3: DQ19, FMC: H34, FPGA: AJ30
set_property PACKAGE_PIN AJ30 [get_ports {fx3_dq[19]}]
# FX3: DQ20, FMC: H35, FPGA: AJ31
set_property PACKAGE_PIN AJ31 [get_ports {fx3_dq[20]}]
# FX3: DQ21, FMC: H37, FPGA: AG31
set_property PACKAGE_PIN AG31 [get_ports {fx3_dq[21]}]
# FX3: DQ22, FMC: H38, FPGA: AH31
set_property PACKAGE_PIN AH31 [get_ports {fx3_dq[22]}]
# FX3: DQ23, FMC: G2, FPGA: AK34
set_property PACKAGE_PIN AK34 [get_ports {fx3_dq[23]}]
# FX3: DQ24, FMC: G3, FPGA: AL34
set_property PACKAGE_PIN AL34 [get_ports {fx3_dq[24]}]
# FX3: DQ25, FMC: G9, FPGA: N34
set_property PACKAGE_PIN N34 [get_ports {fx3_dq[25]}]
# FX3: DQ26, FMC: G10, FPGA: N35
set_property PACKAGE_PIN N35 [get_ports {fx3_dq[26]}]
# FX3: DQ27, FMC: G12, FPGA: M35
set_property PACKAGE_PIN M35 [get_ports {fx3_dq[27]}]
# FX3: DQ28, FMC: G13, FPGA: L35
set_property PACKAGE_PIN L35 [get_ports {fx3_dq[28]}]
# FX3: DQ29, FMC: G15, FPGA: R31
set_property PACKAGE_PIN R31 [get_ports {fx3_dq[29]}]
# FX3: DQ30, FMC: G16, FPGA: P31
set_property PACKAGE_PIN P31 [get_ports {fx3_dq[30]}]
# FX3: DQ31, FMC: G18, FPGA: U31
set_property PACKAGE_PIN U31 [get_ports {fx3_dq[31]}]

# FX3: PMODE0, FMC: D11, FPGA: N38
set_property PACKAGE_PIN N38 [get_ports {fx3_pmode[0]}]
# FX3: PMODE1, FMC: D12, FPGA: M38
set_property PACKAGE_PIN M38 [get_ports {fx3_pmode[1]}]
# FX3: PMODE2, FMC: D14, FPGA: M36
set_property PACKAGE_PIN M36 [get_ports {fx3_pmode[2]}]


set_property IOSTANDARD LVCMOS18 [get_ports {fx3_pmode[0]}]
set_property IOSTANDARD LVCMOS18 [get_ports {fx3_pmode[1]}]
set_property IOSTANDARD LVCMOS18 [get_ports {fx3_pmode[2]}]
set_property IOSTANDARD LVCMOS18 [get_ports fx3_flaga_n]
set_property IOSTANDARD LVCMOS18 [get_ports fx3_flagb_n]
set_property IOSTANDARD LVCMOS18 [get_ports fx3_flagc_n]
set_property IOSTANDARD LVCMOS18 [get_ports fx3_flagd_n]
set_property IOSTANDARD LVCMOS18 [get_ports fx3_com_rst]
set_property IOSTANDARD LVCMOS18 [get_ports fx3_logic_rst]
set_property IOSTANDARD LVCMOS18 [get_ports fx3_pclk]
set_property IOSTANDARD LVCMOS18 [get_ports fx3_pktend_n]
set_property IOSTANDARD LVCMOS18 [get_ports fx3_slcs_n]
set_property IOSTANDARD LVCMOS18 [get_ports fx3_sloe_n]
set_property IOSTANDARD LVCMOS18 [get_ports fx3_slrd_n]
set_property IOSTANDARD LVCMOS18 [get_ports fx3_slwr_n]

set_property IOSTANDARD LVCMOS18 [get_ports {fx3_dq[16]}]
set_property IOSTANDARD LVCMOS18 [get_ports {fx3_dq[17]}]
set_property IOSTANDARD LVCMOS18 [get_ports {fx3_dq[18]}]
set_property IOSTANDARD LVCMOS18 [get_ports {fx3_dq[19]}]
set_property IOSTANDARD LVCMOS18 [get_ports {fx3_dq[20]}]
set_property IOSTANDARD LVCMOS18 [get_ports {fx3_dq[21]}]
set_property IOSTANDARD LVCMOS18 [get_ports {fx3_dq[22]}]
set_property IOSTANDARD LVCMOS18 [get_ports {fx3_dq[23]}]
set_property IOSTANDARD LVCMOS18 [get_ports {fx3_dq[24]}]
set_property IOSTANDARD LVCMOS18 [get_ports {fx3_dq[25]}]
set_property IOSTANDARD LVCMOS18 [get_ports {fx3_dq[26]}]
set_property IOSTANDARD LVCMOS18 [get_ports {fx3_dq[27]}]
set_property IOSTANDARD LVCMOS18 [get_ports {fx3_dq[28]}]
set_property IOSTANDARD LVCMOS18 [get_ports {fx3_dq[29]}]
set_property IOSTANDARD LVCMOS18 [get_ports {fx3_dq[30]}]
set_property IOSTANDARD LVCMOS18 [get_ports {fx3_dq[31]}]
set_property IOSTANDARD LVCMOS18 [get_ports {fx3_a[1]}]
set_property IOSTANDARD LVCMOS18 [get_ports {fx3_a[0]}]
set_property IOSTANDARD LVCMOS18 [get_ports {fx3_dq[15]}]
set_property IOSTANDARD LVCMOS18 [get_ports {fx3_dq[14]}]
set_property IOSTANDARD LVCMOS18 [get_ports {fx3_dq[13]}]
set_property IOSTANDARD LVCMOS18 [get_ports {fx3_dq[12]}]
set_property IOSTANDARD LVCMOS18 [get_ports {fx3_dq[11]}]
set_property IOSTANDARD LVCMOS18 [get_ports {fx3_dq[10]}]
set_property IOSTANDARD LVCMOS18 [get_ports {fx3_dq[9]}]
set_property IOSTANDARD LVCMOS18 [get_ports {fx3_dq[8]}]
set_property IOSTANDARD LVCMOS18 [get_ports {fx3_dq[7]}]
set_property IOSTANDARD LVCMOS18 [get_ports {fx3_dq[6]}]
set_property IOSTANDARD LVCMOS18 [get_ports {fx3_dq[5]}]
set_property IOSTANDARD LVCMOS18 [get_ports {fx3_dq[4]}]
set_property IOSTANDARD LVCMOS18 [get_ports {fx3_dq[3]}]
set_property IOSTANDARD LVCMOS18 [get_ports {fx3_dq[2]}]
set_property IOSTANDARD LVCMOS18 [get_ports {fx3_dq[1]}]
set_property IOSTANDARD LVCMOS18 [get_ports {fx3_dq[0]}]








create_debug_core u_ila_0 ila
set_property ALL_PROBE_SAME_MU true [get_debug_cores u_ila_0]
set_property ALL_PROBE_SAME_MU_CNT 1 [get_debug_cores u_ila_0]
set_property C_ADV_TRIGGER false [get_debug_cores u_ila_0]
set_property C_DATA_DEPTH 32768 [get_debug_cores u_ila_0]
set_property C_EN_STRG_QUAL false [get_debug_cores u_ila_0]
set_property C_INPUT_PIPE_STAGES 0 [get_debug_cores u_ila_0]
set_property C_TRIGIN_EN false [get_debug_cores u_ila_0]
set_property C_TRIGOUT_EN false [get_debug_cores u_ila_0]
set_property port_width 1 [get_debug_ports u_ila_0/clk]
connect_debug_port u_ila_0/clk [get_nets [list u_clock/clk_out]]
set_property PROBE_TYPE DATA_AND_TRIGGER [get_debug_ports u_ila_0/probe0]
set_property port_width 16 [get_debug_ports u_ila_0/probe0]
connect_debug_port u_ila_0/probe0 [get_nets [list {u_glib_cypressfx3/ingress_logic_buffer_din[0]} {u_glib_cypressfx3/ingress_logic_buffer_din[1]} {u_glib_cypressfx3/ingress_logic_buffer_din[2]} {u_glib_cypressfx3/ingress_logic_buffer_din[3]} {u_glib_cypressfx3/ingress_logic_buffer_din[4]} {u_glib_cypressfx3/ingress_logic_buffer_din[5]} {u_glib_cypressfx3/ingress_logic_buffer_din[6]} {u_glib_cypressfx3/ingress_logic_buffer_din[7]} {u_glib_cypressfx3/ingress_logic_buffer_din[8]} {u_glib_cypressfx3/ingress_logic_buffer_din[9]} {u_glib_cypressfx3/ingress_logic_buffer_din[10]} {u_glib_cypressfx3/ingress_logic_buffer_din[11]} {u_glib_cypressfx3/ingress_logic_buffer_din[12]} {u_glib_cypressfx3/ingress_logic_buffer_din[13]} {u_glib_cypressfx3/ingress_logic_buffer_din[14]} {u_glib_cypressfx3/ingress_logic_buffer_din[15]}]]
create_debug_port u_ila_0 probe
set_property PROBE_TYPE DATA_AND_TRIGGER [get_debug_ports u_ila_0/probe1]
set_property port_width 32 [get_debug_ports u_ila_0/probe1]
connect_debug_port u_ila_0/probe1 [get_nets [list {u_glib_cypressfx3/ingress_fx3_buffer_din[0]} {u_glib_cypressfx3/ingress_fx3_buffer_din[1]} {u_glib_cypressfx3/ingress_fx3_buffer_din[2]} {u_glib_cypressfx3/ingress_fx3_buffer_din[3]} {u_glib_cypressfx3/ingress_fx3_buffer_din[4]} {u_glib_cypressfx3/ingress_fx3_buffer_din[5]} {u_glib_cypressfx3/ingress_fx3_buffer_din[6]} {u_glib_cypressfx3/ingress_fx3_buffer_din[7]} {u_glib_cypressfx3/ingress_fx3_buffer_din[8]} {u_glib_cypressfx3/ingress_fx3_buffer_din[9]} {u_glib_cypressfx3/ingress_fx3_buffer_din[10]} {u_glib_cypressfx3/ingress_fx3_buffer_din[11]} {u_glib_cypressfx3/ingress_fx3_buffer_din[12]} {u_glib_cypressfx3/ingress_fx3_buffer_din[13]} {u_glib_cypressfx3/ingress_fx3_buffer_din[14]} {u_glib_cypressfx3/ingress_fx3_buffer_din[15]} {u_glib_cypressfx3/ingress_fx3_buffer_din[16]} {u_glib_cypressfx3/ingress_fx3_buffer_din[17]} {u_glib_cypressfx3/ingress_fx3_buffer_din[18]} {u_glib_cypressfx3/ingress_fx3_buffer_din[19]} {u_glib_cypressfx3/ingress_fx3_buffer_din[20]} {u_glib_cypressfx3/ingress_fx3_buffer_din[21]} {u_glib_cypressfx3/ingress_fx3_buffer_din[22]} {u_glib_cypressfx3/ingress_fx3_buffer_din[23]} {u_glib_cypressfx3/ingress_fx3_buffer_din[24]} {u_glib_cypressfx3/ingress_fx3_buffer_din[25]} {u_glib_cypressfx3/ingress_fx3_buffer_din[26]} {u_glib_cypressfx3/ingress_fx3_buffer_din[27]} {u_glib_cypressfx3/ingress_fx3_buffer_din[28]} {u_glib_cypressfx3/ingress_fx3_buffer_din[29]} {u_glib_cypressfx3/ingress_fx3_buffer_din[30]} {u_glib_cypressfx3/ingress_fx3_buffer_din[31]}]]
create_debug_port u_ila_0 probe
set_property PROBE_TYPE DATA_AND_TRIGGER [get_debug_ports u_ila_0/probe2]
set_property port_width 32 [get_debug_ports u_ila_0/probe2]
connect_debug_port u_ila_0/probe2 [get_nets [list {u_glib_cypressfx3/ingress_cdc_rd_data[0]} {u_glib_cypressfx3/ingress_cdc_rd_data[1]} {u_glib_cypressfx3/ingress_cdc_rd_data[2]} {u_glib_cypressfx3/ingress_cdc_rd_data[3]} {u_glib_cypressfx3/ingress_cdc_rd_data[4]} {u_glib_cypressfx3/ingress_cdc_rd_data[5]} {u_glib_cypressfx3/ingress_cdc_rd_data[6]} {u_glib_cypressfx3/ingress_cdc_rd_data[7]} {u_glib_cypressfx3/ingress_cdc_rd_data[8]} {u_glib_cypressfx3/ingress_cdc_rd_data[9]} {u_glib_cypressfx3/ingress_cdc_rd_data[10]} {u_glib_cypressfx3/ingress_cdc_rd_data[11]} {u_glib_cypressfx3/ingress_cdc_rd_data[12]} {u_glib_cypressfx3/ingress_cdc_rd_data[13]} {u_glib_cypressfx3/ingress_cdc_rd_data[14]} {u_glib_cypressfx3/ingress_cdc_rd_data[15]} {u_glib_cypressfx3/ingress_cdc_rd_data[16]} {u_glib_cypressfx3/ingress_cdc_rd_data[17]} {u_glib_cypressfx3/ingress_cdc_rd_data[18]} {u_glib_cypressfx3/ingress_cdc_rd_data[19]} {u_glib_cypressfx3/ingress_cdc_rd_data[20]} {u_glib_cypressfx3/ingress_cdc_rd_data[21]} {u_glib_cypressfx3/ingress_cdc_rd_data[22]} {u_glib_cypressfx3/ingress_cdc_rd_data[23]} {u_glib_cypressfx3/ingress_cdc_rd_data[24]} {u_glib_cypressfx3/ingress_cdc_rd_data[25]} {u_glib_cypressfx3/ingress_cdc_rd_data[26]} {u_glib_cypressfx3/ingress_cdc_rd_data[27]} {u_glib_cypressfx3/ingress_cdc_rd_data[28]} {u_glib_cypressfx3/ingress_cdc_rd_data[29]} {u_glib_cypressfx3/ingress_cdc_rd_data[30]} {u_glib_cypressfx3/ingress_cdc_rd_data[31]}]]
create_debug_port u_ila_0 probe
set_property PROBE_TYPE DATA_AND_TRIGGER [get_debug_ports u_ila_0/probe3]
set_property port_width 32 [get_debug_ports u_ila_0/probe3]
connect_debug_port u_ila_0/probe3 [get_nets [list {u_glib_cypressfx3/ingress_fx3_buffer_dout[0]} {u_glib_cypressfx3/ingress_fx3_buffer_dout[1]} {u_glib_cypressfx3/ingress_fx3_buffer_dout[2]} {u_glib_cypressfx3/ingress_fx3_buffer_dout[3]} {u_glib_cypressfx3/ingress_fx3_buffer_dout[4]} {u_glib_cypressfx3/ingress_fx3_buffer_dout[5]} {u_glib_cypressfx3/ingress_fx3_buffer_dout[6]} {u_glib_cypressfx3/ingress_fx3_buffer_dout[7]} {u_glib_cypressfx3/ingress_fx3_buffer_dout[8]} {u_glib_cypressfx3/ingress_fx3_buffer_dout[9]} {u_glib_cypressfx3/ingress_fx3_buffer_dout[10]} {u_glib_cypressfx3/ingress_fx3_buffer_dout[11]} {u_glib_cypressfx3/ingress_fx3_buffer_dout[12]} {u_glib_cypressfx3/ingress_fx3_buffer_dout[13]} {u_glib_cypressfx3/ingress_fx3_buffer_dout[14]} {u_glib_cypressfx3/ingress_fx3_buffer_dout[15]} {u_glib_cypressfx3/ingress_fx3_buffer_dout[16]} {u_glib_cypressfx3/ingress_fx3_buffer_dout[17]} {u_glib_cypressfx3/ingress_fx3_buffer_dout[18]} {u_glib_cypressfx3/ingress_fx3_buffer_dout[19]} {u_glib_cypressfx3/ingress_fx3_buffer_dout[20]} {u_glib_cypressfx3/ingress_fx3_buffer_dout[21]} {u_glib_cypressfx3/ingress_fx3_buffer_dout[22]} {u_glib_cypressfx3/ingress_fx3_buffer_dout[23]} {u_glib_cypressfx3/ingress_fx3_buffer_dout[24]} {u_glib_cypressfx3/ingress_fx3_buffer_dout[25]} {u_glib_cypressfx3/ingress_fx3_buffer_dout[26]} {u_glib_cypressfx3/ingress_fx3_buffer_dout[27]} {u_glib_cypressfx3/ingress_fx3_buffer_dout[28]} {u_glib_cypressfx3/ingress_fx3_buffer_dout[29]} {u_glib_cypressfx3/ingress_fx3_buffer_dout[30]} {u_glib_cypressfx3/ingress_fx3_buffer_dout[31]}]]
create_debug_port u_ila_0 probe
set_property PROBE_TYPE DATA_AND_TRIGGER [get_debug_ports u_ila_0/probe4]
set_property port_width 16 [get_debug_ports u_ila_0/probe4]
connect_debug_port u_ila_0/probe4 [get_nets [list {u_glib_cypressfx3/ingress_logic_buffer_dout[0]} {u_glib_cypressfx3/ingress_logic_buffer_dout[1]} {u_glib_cypressfx3/ingress_logic_buffer_dout[2]} {u_glib_cypressfx3/ingress_logic_buffer_dout[3]} {u_glib_cypressfx3/ingress_logic_buffer_dout[4]} {u_glib_cypressfx3/ingress_logic_buffer_dout[5]} {u_glib_cypressfx3/ingress_logic_buffer_dout[6]} {u_glib_cypressfx3/ingress_logic_buffer_dout[7]} {u_glib_cypressfx3/ingress_logic_buffer_dout[8]} {u_glib_cypressfx3/ingress_logic_buffer_dout[9]} {u_glib_cypressfx3/ingress_logic_buffer_dout[10]} {u_glib_cypressfx3/ingress_logic_buffer_dout[11]} {u_glib_cypressfx3/ingress_logic_buffer_dout[12]} {u_glib_cypressfx3/ingress_logic_buffer_dout[13]} {u_glib_cypressfx3/ingress_logic_buffer_dout[14]} {u_glib_cypressfx3/ingress_logic_buffer_dout[15]}]]
create_debug_port u_ila_0 probe
set_property PROBE_TYPE DATA_AND_TRIGGER [get_debug_ports u_ila_0/probe5]
set_property port_width 32 [get_debug_ports u_ila_0/probe5]
connect_debug_port u_ila_0/probe5 [get_nets [list {u_glib_cypressfx3/ingress_cdc_wr_data[0]} {u_glib_cypressfx3/ingress_cdc_wr_data[1]} {u_glib_cypressfx3/ingress_cdc_wr_data[2]} {u_glib_cypressfx3/ingress_cdc_wr_data[3]} {u_glib_cypressfx3/ingress_cdc_wr_data[4]} {u_glib_cypressfx3/ingress_cdc_wr_data[5]} {u_glib_cypressfx3/ingress_cdc_wr_data[6]} {u_glib_cypressfx3/ingress_cdc_wr_data[7]} {u_glib_cypressfx3/ingress_cdc_wr_data[8]} {u_glib_cypressfx3/ingress_cdc_wr_data[9]} {u_glib_cypressfx3/ingress_cdc_wr_data[10]} {u_glib_cypressfx3/ingress_cdc_wr_data[11]} {u_glib_cypressfx3/ingress_cdc_wr_data[12]} {u_glib_cypressfx3/ingress_cdc_wr_data[13]} {u_glib_cypressfx3/ingress_cdc_wr_data[14]} {u_glib_cypressfx3/ingress_cdc_wr_data[15]} {u_glib_cypressfx3/ingress_cdc_wr_data[16]} {u_glib_cypressfx3/ingress_cdc_wr_data[17]} {u_glib_cypressfx3/ingress_cdc_wr_data[18]} {u_glib_cypressfx3/ingress_cdc_wr_data[19]} {u_glib_cypressfx3/ingress_cdc_wr_data[20]} {u_glib_cypressfx3/ingress_cdc_wr_data[21]} {u_glib_cypressfx3/ingress_cdc_wr_data[22]} {u_glib_cypressfx3/ingress_cdc_wr_data[23]} {u_glib_cypressfx3/ingress_cdc_wr_data[24]} {u_glib_cypressfx3/ingress_cdc_wr_data[25]} {u_glib_cypressfx3/ingress_cdc_wr_data[26]} {u_glib_cypressfx3/ingress_cdc_wr_data[27]} {u_glib_cypressfx3/ingress_cdc_wr_data[28]} {u_glib_cypressfx3/ingress_cdc_wr_data[29]} {u_glib_cypressfx3/ingress_cdc_wr_data[30]} {u_glib_cypressfx3/ingress_cdc_wr_data[31]}]]
create_debug_port u_ila_0 probe
set_property PROBE_TYPE DATA_AND_TRIGGER [get_debug_ports u_ila_0/probe6]
set_property port_width 5 [get_debug_ports u_ila_0/probe6]
connect_debug_port u_ila_0/probe6 [get_nets [list {u_glib_cypressfx3/state[0]} {u_glib_cypressfx3/state[1]} {u_glib_cypressfx3/state[2]} {u_glib_cypressfx3/state[3]} {u_glib_cypressfx3/state[4]}]]
create_debug_port u_ila_0 probe
set_property PROBE_TYPE DATA_AND_TRIGGER [get_debug_ports u_ila_0/probe7]
set_property port_width 14 [get_debug_ports u_ila_0/probe7]
connect_debug_port u_ila_0/probe7 [get_nets [list {u_glib_cypressfx3/idle_counter[0]} {u_glib_cypressfx3/idle_counter[1]} {u_glib_cypressfx3/idle_counter[2]} {u_glib_cypressfx3/idle_counter[3]} {u_glib_cypressfx3/idle_counter[4]} {u_glib_cypressfx3/idle_counter[5]} {u_glib_cypressfx3/idle_counter[6]} {u_glib_cypressfx3/idle_counter[7]} {u_glib_cypressfx3/idle_counter[8]} {u_glib_cypressfx3/idle_counter[9]} {u_glib_cypressfx3/idle_counter[10]} {u_glib_cypressfx3/idle_counter[11]} {u_glib_cypressfx3/idle_counter[12]} {u_glib_cypressfx3/idle_counter[13]}]]
create_debug_port u_ila_0 probe
set_property PROBE_TYPE DATA_AND_TRIGGER [get_debug_ports u_ila_0/probe8]
set_property port_width 2 [get_debug_ports u_ila_0/probe8]
connect_debug_port u_ila_0/probe8 [get_nets [list {u_glib_cypressfx3/fifoadr[0]} {u_glib_cypressfx3/fifoadr[1]}]]
create_debug_port u_ila_0 probe
set_property PROBE_TYPE DATA_AND_TRIGGER [get_debug_ports u_ila_0/probe9]
set_property port_width 32 [get_debug_ports u_ila_0/probe9]
connect_debug_port u_ila_0/probe9 [get_nets [list {u_glib_cypressfx3/fx3_dq_out[0]} {u_glib_cypressfx3/fx3_dq_out[1]} {u_glib_cypressfx3/fx3_dq_out[2]} {u_glib_cypressfx3/fx3_dq_out[3]} {u_glib_cypressfx3/fx3_dq_out[4]} {u_glib_cypressfx3/fx3_dq_out[5]} {u_glib_cypressfx3/fx3_dq_out[6]} {u_glib_cypressfx3/fx3_dq_out[7]} {u_glib_cypressfx3/fx3_dq_out[8]} {u_glib_cypressfx3/fx3_dq_out[9]} {u_glib_cypressfx3/fx3_dq_out[10]} {u_glib_cypressfx3/fx3_dq_out[11]} {u_glib_cypressfx3/fx3_dq_out[12]} {u_glib_cypressfx3/fx3_dq_out[13]} {u_glib_cypressfx3/fx3_dq_out[14]} {u_glib_cypressfx3/fx3_dq_out[15]} {u_glib_cypressfx3/fx3_dq_out[16]} {u_glib_cypressfx3/fx3_dq_out[17]} {u_glib_cypressfx3/fx3_dq_out[18]} {u_glib_cypressfx3/fx3_dq_out[19]} {u_glib_cypressfx3/fx3_dq_out[20]} {u_glib_cypressfx3/fx3_dq_out[21]} {u_glib_cypressfx3/fx3_dq_out[22]} {u_glib_cypressfx3/fx3_dq_out[23]} {u_glib_cypressfx3/fx3_dq_out[24]} {u_glib_cypressfx3/fx3_dq_out[25]} {u_glib_cypressfx3/fx3_dq_out[26]} {u_glib_cypressfx3/fx3_dq_out[27]} {u_glib_cypressfx3/fx3_dq_out[28]} {u_glib_cypressfx3/fx3_dq_out[29]} {u_glib_cypressfx3/fx3_dq_out[30]} {u_glib_cypressfx3/fx3_dq_out[31]}]]
create_debug_port u_ila_0 probe
set_property PROBE_TYPE DATA_AND_TRIGGER [get_debug_ports u_ila_0/probe10]
set_property port_width 32 [get_debug_ports u_ila_0/probe10]
connect_debug_port u_ila_0/probe10 [get_nets [list {u_glib_cypressfx3/fx3_dq_in[0]} {u_glib_cypressfx3/fx3_dq_in[1]} {u_glib_cypressfx3/fx3_dq_in[2]} {u_glib_cypressfx3/fx3_dq_in[3]} {u_glib_cypressfx3/fx3_dq_in[4]} {u_glib_cypressfx3/fx3_dq_in[5]} {u_glib_cypressfx3/fx3_dq_in[6]} {u_glib_cypressfx3/fx3_dq_in[7]} {u_glib_cypressfx3/fx3_dq_in[8]} {u_glib_cypressfx3/fx3_dq_in[9]} {u_glib_cypressfx3/fx3_dq_in[10]} {u_glib_cypressfx3/fx3_dq_in[11]} {u_glib_cypressfx3/fx3_dq_in[12]} {u_glib_cypressfx3/fx3_dq_in[13]} {u_glib_cypressfx3/fx3_dq_in[14]} {u_glib_cypressfx3/fx3_dq_in[15]} {u_glib_cypressfx3/fx3_dq_in[16]} {u_glib_cypressfx3/fx3_dq_in[17]} {u_glib_cypressfx3/fx3_dq_in[18]} {u_glib_cypressfx3/fx3_dq_in[19]} {u_glib_cypressfx3/fx3_dq_in[20]} {u_glib_cypressfx3/fx3_dq_in[21]} {u_glib_cypressfx3/fx3_dq_in[22]} {u_glib_cypressfx3/fx3_dq_in[23]} {u_glib_cypressfx3/fx3_dq_in[24]} {u_glib_cypressfx3/fx3_dq_in[25]} {u_glib_cypressfx3/fx3_dq_in[26]} {u_glib_cypressfx3/fx3_dq_in[27]} {u_glib_cypressfx3/fx3_dq_in[28]} {u_glib_cypressfx3/fx3_dq_in[29]} {u_glib_cypressfx3/fx3_dq_in[30]} {u_glib_cypressfx3/fx3_dq_in[31]}]]
create_debug_port u_ila_0 probe
set_property PROBE_TYPE DATA_AND_TRIGGER [get_debug_ports u_ila_0/probe11]
set_property port_width 16 [get_debug_ports u_ila_0/probe11]
connect_debug_port u_ila_0/probe11 [get_nets [list {u_glib_cypressfx3/egress_upscale_in_data[0]} {u_glib_cypressfx3/egress_upscale_in_data[1]} {u_glib_cypressfx3/egress_upscale_in_data[2]} {u_glib_cypressfx3/egress_upscale_in_data[3]} {u_glib_cypressfx3/egress_upscale_in_data[4]} {u_glib_cypressfx3/egress_upscale_in_data[5]} {u_glib_cypressfx3/egress_upscale_in_data[6]} {u_glib_cypressfx3/egress_upscale_in_data[7]} {u_glib_cypressfx3/egress_upscale_in_data[8]} {u_glib_cypressfx3/egress_upscale_in_data[9]} {u_glib_cypressfx3/egress_upscale_in_data[10]} {u_glib_cypressfx3/egress_upscale_in_data[11]} {u_glib_cypressfx3/egress_upscale_in_data[12]} {u_glib_cypressfx3/egress_upscale_in_data[13]} {u_glib_cypressfx3/egress_upscale_in_data[14]} {u_glib_cypressfx3/egress_upscale_in_data[15]}]]
create_debug_port u_ila_0 probe
set_property PROBE_TYPE DATA_AND_TRIGGER [get_debug_ports u_ila_0/probe12]
set_property port_width 16 [get_debug_ports u_ila_0/probe12]
connect_debug_port u_ila_0/probe12 [get_nets [list {u_glib_cypressfx3/egress_logic_buffer_din[0]} {u_glib_cypressfx3/egress_logic_buffer_din[1]} {u_glib_cypressfx3/egress_logic_buffer_din[2]} {u_glib_cypressfx3/egress_logic_buffer_din[3]} {u_glib_cypressfx3/egress_logic_buffer_din[4]} {u_glib_cypressfx3/egress_logic_buffer_din[5]} {u_glib_cypressfx3/egress_logic_buffer_din[6]} {u_glib_cypressfx3/egress_logic_buffer_din[7]} {u_glib_cypressfx3/egress_logic_buffer_din[8]} {u_glib_cypressfx3/egress_logic_buffer_din[9]} {u_glib_cypressfx3/egress_logic_buffer_din[10]} {u_glib_cypressfx3/egress_logic_buffer_din[11]} {u_glib_cypressfx3/egress_logic_buffer_din[12]} {u_glib_cypressfx3/egress_logic_buffer_din[13]} {u_glib_cypressfx3/egress_logic_buffer_din[14]} {u_glib_cypressfx3/egress_logic_buffer_din[15]}]]
create_debug_port u_ila_0 probe
set_property PROBE_TYPE DATA_AND_TRIGGER [get_debug_ports u_ila_0/probe13]
set_property port_width 32 [get_debug_ports u_ila_0/probe13]
connect_debug_port u_ila_0/probe13 [get_nets [list {u_glib_cypressfx3/egress_fx3_buffer_dout[0]} {u_glib_cypressfx3/egress_fx3_buffer_dout[1]} {u_glib_cypressfx3/egress_fx3_buffer_dout[2]} {u_glib_cypressfx3/egress_fx3_buffer_dout[3]} {u_glib_cypressfx3/egress_fx3_buffer_dout[4]} {u_glib_cypressfx3/egress_fx3_buffer_dout[5]} {u_glib_cypressfx3/egress_fx3_buffer_dout[6]} {u_glib_cypressfx3/egress_fx3_buffer_dout[7]} {u_glib_cypressfx3/egress_fx3_buffer_dout[8]} {u_glib_cypressfx3/egress_fx3_buffer_dout[9]} {u_glib_cypressfx3/egress_fx3_buffer_dout[10]} {u_glib_cypressfx3/egress_fx3_buffer_dout[11]} {u_glib_cypressfx3/egress_fx3_buffer_dout[12]} {u_glib_cypressfx3/egress_fx3_buffer_dout[13]} {u_glib_cypressfx3/egress_fx3_buffer_dout[14]} {u_glib_cypressfx3/egress_fx3_buffer_dout[15]} {u_glib_cypressfx3/egress_fx3_buffer_dout[16]} {u_glib_cypressfx3/egress_fx3_buffer_dout[17]} {u_glib_cypressfx3/egress_fx3_buffer_dout[18]} {u_glib_cypressfx3/egress_fx3_buffer_dout[19]} {u_glib_cypressfx3/egress_fx3_buffer_dout[20]} {u_glib_cypressfx3/egress_fx3_buffer_dout[21]} {u_glib_cypressfx3/egress_fx3_buffer_dout[22]} {u_glib_cypressfx3/egress_fx3_buffer_dout[23]} {u_glib_cypressfx3/egress_fx3_buffer_dout[24]} {u_glib_cypressfx3/egress_fx3_buffer_dout[25]} {u_glib_cypressfx3/egress_fx3_buffer_dout[26]} {u_glib_cypressfx3/egress_fx3_buffer_dout[27]} {u_glib_cypressfx3/egress_fx3_buffer_dout[28]} {u_glib_cypressfx3/egress_fx3_buffer_dout[29]} {u_glib_cypressfx3/egress_fx3_buffer_dout[30]} {u_glib_cypressfx3/egress_fx3_buffer_dout[31]}]]
create_debug_port u_ila_0 probe
set_property PROBE_TYPE DATA_AND_TRIGGER [get_debug_ports u_ila_0/probe14]
set_property port_width 32 [get_debug_ports u_ila_0/probe14]
connect_debug_port u_ila_0/probe14 [get_nets [list {u_glib_cypressfx3/egress_fx3_buffer_din[0]} {u_glib_cypressfx3/egress_fx3_buffer_din[1]} {u_glib_cypressfx3/egress_fx3_buffer_din[2]} {u_glib_cypressfx3/egress_fx3_buffer_din[3]} {u_glib_cypressfx3/egress_fx3_buffer_din[4]} {u_glib_cypressfx3/egress_fx3_buffer_din[5]} {u_glib_cypressfx3/egress_fx3_buffer_din[6]} {u_glib_cypressfx3/egress_fx3_buffer_din[7]} {u_glib_cypressfx3/egress_fx3_buffer_din[8]} {u_glib_cypressfx3/egress_fx3_buffer_din[9]} {u_glib_cypressfx3/egress_fx3_buffer_din[10]} {u_glib_cypressfx3/egress_fx3_buffer_din[11]} {u_glib_cypressfx3/egress_fx3_buffer_din[12]} {u_glib_cypressfx3/egress_fx3_buffer_din[13]} {u_glib_cypressfx3/egress_fx3_buffer_din[14]} {u_glib_cypressfx3/egress_fx3_buffer_din[15]} {u_glib_cypressfx3/egress_fx3_buffer_din[16]} {u_glib_cypressfx3/egress_fx3_buffer_din[17]} {u_glib_cypressfx3/egress_fx3_buffer_din[18]} {u_glib_cypressfx3/egress_fx3_buffer_din[19]} {u_glib_cypressfx3/egress_fx3_buffer_din[20]} {u_glib_cypressfx3/egress_fx3_buffer_din[21]} {u_glib_cypressfx3/egress_fx3_buffer_din[22]} {u_glib_cypressfx3/egress_fx3_buffer_din[23]} {u_glib_cypressfx3/egress_fx3_buffer_din[24]} {u_glib_cypressfx3/egress_fx3_buffer_din[25]} {u_glib_cypressfx3/egress_fx3_buffer_din[26]} {u_glib_cypressfx3/egress_fx3_buffer_din[27]} {u_glib_cypressfx3/egress_fx3_buffer_din[28]} {u_glib_cypressfx3/egress_fx3_buffer_din[29]} {u_glib_cypressfx3/egress_fx3_buffer_din[30]} {u_glib_cypressfx3/egress_fx3_buffer_din[31]}]]
create_debug_port u_ila_0 probe
set_property PROBE_TYPE DATA_AND_TRIGGER [get_debug_ports u_ila_0/probe15]
set_property port_width 32 [get_debug_ports u_ila_0/probe15]
connect_debug_port u_ila_0/probe15 [get_nets [list {u_glib_cypressfx3/egress_upscale_out_data[0]} {u_glib_cypressfx3/egress_upscale_out_data[1]} {u_glib_cypressfx3/egress_upscale_out_data[2]} {u_glib_cypressfx3/egress_upscale_out_data[3]} {u_glib_cypressfx3/egress_upscale_out_data[4]} {u_glib_cypressfx3/egress_upscale_out_data[5]} {u_glib_cypressfx3/egress_upscale_out_data[6]} {u_glib_cypressfx3/egress_upscale_out_data[7]} {u_glib_cypressfx3/egress_upscale_out_data[8]} {u_glib_cypressfx3/egress_upscale_out_data[9]} {u_glib_cypressfx3/egress_upscale_out_data[10]} {u_glib_cypressfx3/egress_upscale_out_data[11]} {u_glib_cypressfx3/egress_upscale_out_data[12]} {u_glib_cypressfx3/egress_upscale_out_data[13]} {u_glib_cypressfx3/egress_upscale_out_data[14]} {u_glib_cypressfx3/egress_upscale_out_data[15]} {u_glib_cypressfx3/egress_upscale_out_data[16]} {u_glib_cypressfx3/egress_upscale_out_data[17]} {u_glib_cypressfx3/egress_upscale_out_data[18]} {u_glib_cypressfx3/egress_upscale_out_data[19]} {u_glib_cypressfx3/egress_upscale_out_data[20]} {u_glib_cypressfx3/egress_upscale_out_data[21]} {u_glib_cypressfx3/egress_upscale_out_data[22]} {u_glib_cypressfx3/egress_upscale_out_data[23]} {u_glib_cypressfx3/egress_upscale_out_data[24]} {u_glib_cypressfx3/egress_upscale_out_data[25]} {u_glib_cypressfx3/egress_upscale_out_data[26]} {u_glib_cypressfx3/egress_upscale_out_data[27]} {u_glib_cypressfx3/egress_upscale_out_data[28]} {u_glib_cypressfx3/egress_upscale_out_data[29]} {u_glib_cypressfx3/egress_upscale_out_data[30]} {u_glib_cypressfx3/egress_upscale_out_data[31]}]]
create_debug_port u_ila_0 probe
set_property PROBE_TYPE DATA_AND_TRIGGER [get_debug_ports u_ila_0/probe16]
set_property port_width 32 [get_debug_ports u_ila_0/probe16]
connect_debug_port u_ila_0/probe16 [get_nets [list {u_glib_cypressfx3/dbg_word_cnt[0]} {u_glib_cypressfx3/dbg_word_cnt[1]} {u_glib_cypressfx3/dbg_word_cnt[2]} {u_glib_cypressfx3/dbg_word_cnt[3]} {u_glib_cypressfx3/dbg_word_cnt[4]} {u_glib_cypressfx3/dbg_word_cnt[5]} {u_glib_cypressfx3/dbg_word_cnt[6]} {u_glib_cypressfx3/dbg_word_cnt[7]} {u_glib_cypressfx3/dbg_word_cnt[8]} {u_glib_cypressfx3/dbg_word_cnt[9]} {u_glib_cypressfx3/dbg_word_cnt[10]} {u_glib_cypressfx3/dbg_word_cnt[11]} {u_glib_cypressfx3/dbg_word_cnt[12]} {u_glib_cypressfx3/dbg_word_cnt[13]} {u_glib_cypressfx3/dbg_word_cnt[14]} {u_glib_cypressfx3/dbg_word_cnt[15]} {u_glib_cypressfx3/dbg_word_cnt[16]} {u_glib_cypressfx3/dbg_word_cnt[17]} {u_glib_cypressfx3/dbg_word_cnt[18]} {u_glib_cypressfx3/dbg_word_cnt[19]} {u_glib_cypressfx3/dbg_word_cnt[20]} {u_glib_cypressfx3/dbg_word_cnt[21]} {u_glib_cypressfx3/dbg_word_cnt[22]} {u_glib_cypressfx3/dbg_word_cnt[23]} {u_glib_cypressfx3/dbg_word_cnt[24]} {u_glib_cypressfx3/dbg_word_cnt[25]} {u_glib_cypressfx3/dbg_word_cnt[26]} {u_glib_cypressfx3/dbg_word_cnt[27]} {u_glib_cypressfx3/dbg_word_cnt[28]} {u_glib_cypressfx3/dbg_word_cnt[29]} {u_glib_cypressfx3/dbg_word_cnt[30]} {u_glib_cypressfx3/dbg_word_cnt[31]}]]
create_debug_port u_ila_0 probe
set_property PROBE_TYPE DATA_AND_TRIGGER [get_debug_ports u_ila_0/probe17]
set_property port_width 16 [get_debug_ports u_ila_0/probe17]
connect_debug_port u_ila_0/probe17 [get_nets [list {u_glib_cypressfx3/fifo_in_data[0]} {u_glib_cypressfx3/fifo_in_data[1]} {u_glib_cypressfx3/fifo_in_data[2]} {u_glib_cypressfx3/fifo_in_data[3]} {u_glib_cypressfx3/fifo_in_data[4]} {u_glib_cypressfx3/fifo_in_data[5]} {u_glib_cypressfx3/fifo_in_data[6]} {u_glib_cypressfx3/fifo_in_data[7]} {u_glib_cypressfx3/fifo_in_data[8]} {u_glib_cypressfx3/fifo_in_data[9]} {u_glib_cypressfx3/fifo_in_data[10]} {u_glib_cypressfx3/fifo_in_data[11]} {u_glib_cypressfx3/fifo_in_data[12]} {u_glib_cypressfx3/fifo_in_data[13]} {u_glib_cypressfx3/fifo_in_data[14]} {u_glib_cypressfx3/fifo_in_data[15]}]]
create_debug_port u_ila_0 probe
set_property PROBE_TYPE DATA_AND_TRIGGER [get_debug_ports u_ila_0/probe18]
set_property port_width 16 [get_debug_ports u_ila_0/probe18]
connect_debug_port u_ila_0/probe18 [get_nets [list {u_glib_cypressfx3/egress_logic_buffer_dout[0]} {u_glib_cypressfx3/egress_logic_buffer_dout[1]} {u_glib_cypressfx3/egress_logic_buffer_dout[2]} {u_glib_cypressfx3/egress_logic_buffer_dout[3]} {u_glib_cypressfx3/egress_logic_buffer_dout[4]} {u_glib_cypressfx3/egress_logic_buffer_dout[5]} {u_glib_cypressfx3/egress_logic_buffer_dout[6]} {u_glib_cypressfx3/egress_logic_buffer_dout[7]} {u_glib_cypressfx3/egress_logic_buffer_dout[8]} {u_glib_cypressfx3/egress_logic_buffer_dout[9]} {u_glib_cypressfx3/egress_logic_buffer_dout[10]} {u_glib_cypressfx3/egress_logic_buffer_dout[11]} {u_glib_cypressfx3/egress_logic_buffer_dout[12]} {u_glib_cypressfx3/egress_logic_buffer_dout[13]} {u_glib_cypressfx3/egress_logic_buffer_dout[14]} {u_glib_cypressfx3/egress_logic_buffer_dout[15]}]]
create_debug_port u_ila_0 probe
set_property PROBE_TYPE DATA_AND_TRIGGER [get_debug_ports u_ila_0/probe19]
set_property port_width 1 [get_debug_ports u_ila_0/probe19]
connect_debug_port u_ila_0/probe19 [get_nets [list u_glib_cypressfx3/egress_fx3_buffer_empty]]
create_debug_port u_ila_0 probe
set_property PROBE_TYPE DATA_AND_TRIGGER [get_debug_ports u_ila_0/probe20]
set_property port_width 1 [get_debug_ports u_ila_0/probe20]
connect_debug_port u_ila_0/probe20 [get_nets [list u_glib_cypressfx3/egress_fx3_buffer_full]]
create_debug_port u_ila_0 probe
set_property PROBE_TYPE DATA_AND_TRIGGER [get_debug_ports u_ila_0/probe21]
set_property port_width 1 [get_debug_ports u_ila_0/probe21]
connect_debug_port u_ila_0/probe21 [get_nets [list u_glib_cypressfx3/egress_fx3_buffer_rd_en]]
create_debug_port u_ila_0 probe
set_property PROBE_TYPE DATA_AND_TRIGGER [get_debug_ports u_ila_0/probe22]
set_property port_width 1 [get_debug_ports u_ila_0/probe22]
connect_debug_port u_ila_0/probe22 [get_nets [list u_glib_cypressfx3/egress_fx3_buffer_wr_en]]
create_debug_port u_ila_0 probe
set_property PROBE_TYPE DATA_AND_TRIGGER [get_debug_ports u_ila_0/probe23]
set_property port_width 1 [get_debug_ports u_ila_0/probe23]
connect_debug_port u_ila_0/probe23 [get_nets [list u_glib_cypressfx3/egress_logic_buffer_empty]]
create_debug_port u_ila_0 probe
set_property PROBE_TYPE DATA_AND_TRIGGER [get_debug_ports u_ila_0/probe24]
set_property port_width 1 [get_debug_ports u_ila_0/probe24]
connect_debug_port u_ila_0/probe24 [get_nets [list u_glib_cypressfx3/egress_logic_buffer_full]]
create_debug_port u_ila_0 probe
set_property PROBE_TYPE DATA_AND_TRIGGER [get_debug_ports u_ila_0/probe25]
set_property port_width 1 [get_debug_ports u_ila_0/probe25]
connect_debug_port u_ila_0/probe25 [get_nets [list u_glib_cypressfx3/egress_logic_buffer_rd_en]]
create_debug_port u_ila_0 probe
set_property PROBE_TYPE DATA_AND_TRIGGER [get_debug_ports u_ila_0/probe26]
set_property port_width 1 [get_debug_ports u_ila_0/probe26]
connect_debug_port u_ila_0/probe26 [get_nets [list u_glib_cypressfx3/egress_logic_buffer_wr_en]]
create_debug_port u_ila_0 probe
set_property PROBE_TYPE DATA_AND_TRIGGER [get_debug_ports u_ila_0/probe27]
set_property port_width 1 [get_debug_ports u_ila_0/probe27]
connect_debug_port u_ila_0/probe27 [get_nets [list u_glib_cypressfx3/egress_upscale_in_ready]]
create_debug_port u_ila_0 probe
set_property PROBE_TYPE DATA_AND_TRIGGER [get_debug_ports u_ila_0/probe28]
set_property port_width 1 [get_debug_ports u_ila_0/probe28]
connect_debug_port u_ila_0/probe28 [get_nets [list u_glib_cypressfx3/egress_upscale_in_valid]]
create_debug_port u_ila_0 probe
set_property PROBE_TYPE DATA_AND_TRIGGER [get_debug_ports u_ila_0/probe29]
set_property port_width 1 [get_debug_ports u_ila_0/probe29]
connect_debug_port u_ila_0/probe29 [get_nets [list u_glib_cypressfx3/egress_upscale_out_ready]]
create_debug_port u_ila_0 probe
set_property PROBE_TYPE DATA_AND_TRIGGER [get_debug_ports u_ila_0/probe30]
set_property port_width 1 [get_debug_ports u_ila_0/probe30]
connect_debug_port u_ila_0/probe30 [get_nets [list u_glib_cypressfx3/egress_upscale_out_valid]]
create_debug_port u_ila_0 probe
set_property PROBE_TYPE DATA_AND_TRIGGER [get_debug_ports u_ila_0/probe31]
set_property port_width 1 [get_debug_ports u_ila_0/probe31]
connect_debug_port u_ila_0/probe31 [get_nets [list u_glib_cypressfx3/fifo_in_valid]]
create_debug_port u_ila_0 probe
set_property PROBE_TYPE DATA_AND_TRIGGER [get_debug_ports u_ila_0/probe32]
set_property port_width 1 [get_debug_ports u_ila_0/probe32]
connect_debug_port u_ila_0/probe32 [get_nets [list u_glib_cypressfx3/fifo_out_ready]]
create_debug_port u_ila_0 probe
set_property PROBE_TYPE DATA_AND_TRIGGER [get_debug_ports u_ila_0/probe33]
set_property port_width 1 [get_debug_ports u_ila_0/probe33]
connect_debug_port u_ila_0/probe33 [get_nets [list u_glib_cypressfx3/flush]]
create_debug_port u_ila_0 probe
set_property PROBE_TYPE DATA_AND_TRIGGER [get_debug_ports u_ila_0/probe34]
set_property port_width 1 [get_debug_ports u_ila_0/probe34]
connect_debug_port u_ila_0/probe34 [get_nets [list u_glib_cypressfx3/fx3_in_almost_full]]
create_debug_port u_ila_0 probe
set_property PROBE_TYPE DATA_AND_TRIGGER [get_debug_ports u_ila_0/probe35]
set_property port_width 1 [get_debug_ports u_ila_0/probe35]
connect_debug_port u_ila_0/probe35 [get_nets [list u_glib_cypressfx3/fx3_in_full]]
create_debug_port u_ila_0 probe
set_property PROBE_TYPE DATA_AND_TRIGGER [get_debug_ports u_ila_0/probe36]
set_property port_width 1 [get_debug_ports u_ila_0/probe36]
connect_debug_port u_ila_0/probe36 [get_nets [list u_glib_cypressfx3/fx3_out_almost_empty]]
create_debug_port u_ila_0 probe
set_property PROBE_TYPE DATA_AND_TRIGGER [get_debug_ports u_ila_0/probe37]
set_property port_width 1 [get_debug_ports u_ila_0/probe37]
connect_debug_port u_ila_0/probe37 [get_nets [list u_glib_cypressfx3/fx3_out_empty]]
create_debug_port u_ila_0 probe
set_property PROBE_TYPE DATA_AND_TRIGGER [get_debug_ports u_ila_0/probe38]
set_property port_width 1 [get_debug_ports u_ila_0/probe38]
connect_debug_port u_ila_0/probe38 [get_nets [list u_glib_cypressfx3/ingress_cdc_rd_empty]]
create_debug_port u_ila_0 probe
set_property PROBE_TYPE DATA_AND_TRIGGER [get_debug_ports u_ila_0/probe39]
set_property port_width 1 [get_debug_ports u_ila_0/probe39]
connect_debug_port u_ila_0/probe39 [get_nets [list u_glib_cypressfx3/ingress_cdc_rd_en]]
create_debug_port u_ila_0 probe
set_property PROBE_TYPE DATA_AND_TRIGGER [get_debug_ports u_ila_0/probe40]
set_property port_width 1 [get_debug_ports u_ila_0/probe40]
connect_debug_port u_ila_0/probe40 [get_nets [list u_glib_cypressfx3/ingress_cdc_wr_en]]
create_debug_port u_ila_0 probe
set_property PROBE_TYPE DATA_AND_TRIGGER [get_debug_ports u_ila_0/probe41]
set_property port_width 1 [get_debug_ports u_ila_0/probe41]
connect_debug_port u_ila_0/probe41 [get_nets [list u_glib_cypressfx3/ingress_cdc_wr_full]]
create_debug_port u_ila_0 probe
set_property PROBE_TYPE DATA_AND_TRIGGER [get_debug_ports u_ila_0/probe42]
set_property port_width 1 [get_debug_ports u_ila_0/probe42]
connect_debug_port u_ila_0/probe42 [get_nets [list u_glib_cypressfx3/ingress_fx3_buffer_empty]]
create_debug_port u_ila_0 probe
set_property PROBE_TYPE DATA_AND_TRIGGER [get_debug_ports u_ila_0/probe43]
set_property port_width 1 [get_debug_ports u_ila_0/probe43]
connect_debug_port u_ila_0/probe43 [get_nets [list u_glib_cypressfx3/ingress_fx3_buffer_full]]
create_debug_port u_ila_0 probe
set_property PROBE_TYPE DATA_AND_TRIGGER [get_debug_ports u_ila_0/probe44]
set_property port_width 1 [get_debug_ports u_ila_0/probe44]
connect_debug_port u_ila_0/probe44 [get_nets [list u_glib_cypressfx3/ingress_fx3_buffer_rd_en]]
create_debug_port u_ila_0 probe
set_property PROBE_TYPE DATA_AND_TRIGGER [get_debug_ports u_ila_0/probe45]
set_property port_width 1 [get_debug_ports u_ila_0/probe45]
connect_debug_port u_ila_0/probe45 [get_nets [list u_glib_cypressfx3/ingress_fx3_buffer_wr_en]]
create_debug_port u_ila_0 probe
set_property PROBE_TYPE DATA_AND_TRIGGER [get_debug_ports u_ila_0/probe46]
set_property port_width 1 [get_debug_ports u_ila_0/probe46]
connect_debug_port u_ila_0/probe46 [get_nets [list u_glib_cypressfx3/ingress_logic_buffer_empty]]
create_debug_port u_ila_0 probe
set_property PROBE_TYPE DATA_AND_TRIGGER [get_debug_ports u_ila_0/probe47]
set_property port_width 1 [get_debug_ports u_ila_0/probe47]
connect_debug_port u_ila_0/probe47 [get_nets [list u_glib_cypressfx3/ingress_logic_buffer_full]]
create_debug_port u_ila_0 probe
set_property PROBE_TYPE DATA_AND_TRIGGER [get_debug_ports u_ila_0/probe48]
set_property port_width 1 [get_debug_ports u_ila_0/probe48]
connect_debug_port u_ila_0/probe48 [get_nets [list u_glib_cypressfx3/ingress_logic_buffer_rd_en]]
create_debug_port u_ila_0 probe
set_property PROBE_TYPE DATA_AND_TRIGGER [get_debug_ports u_ila_0/probe49]
set_property port_width 1 [get_debug_ports u_ila_0/probe49]
connect_debug_port u_ila_0/probe49 [get_nets [list u_glib_cypressfx3/ingress_logic_buffer_wr_en]]
create_debug_port u_ila_0 probe
set_property PROBE_TYPE DATA_AND_TRIGGER [get_debug_ports u_ila_0/probe50]
set_property port_width 1 [get_debug_ports u_ila_0/probe50]
connect_debug_port u_ila_0/probe50 [get_nets [list u_glib_cypressfx3/rst]]
set_property C_CLK_INPUT_FREQ_HZ 300000000 [get_debug_cores dbg_hub]
set_property C_ENABLE_CLK_DIVIDER false [get_debug_cores dbg_hub]
set_property C_USER_SCAN_CHAIN 1 [get_debug_cores dbg_hub]
connect_debug_port dbg_hub/clk [get_nets fx3_pclk_OBUF]
