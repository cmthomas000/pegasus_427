-------------------------------------------------------------------------------
-- axi_vdma_s2mm_axis_dwidth_converter
-------------------------------------------------------------------------------
--
-- *************************************************************************
--
-- (c) Copyright 2010, 2011 Xilinx, Inc. All rights reserved.
--
-- This file contains confidential and proprietary information
-- of Xilinx, Inc. and is protected under U.S. and
-- international copyright and other intellectual property
-- laws.
--
-- DISCLAIMER
-- This disclaimer is not a license and does not grant any
-- rights to the materials distributed herewith. Except as
-- otherwise provided in a valid license issued to you by
-- Xilinx, and to the maximum extent permitted by applicable
-- law: (1) THESE MATERIALS ARE MADE AVAILABLE "AS IS" AND
-- WITH ALL FAULTS, AND XILINX HEREBY DISCLAIMS ALL WARRANTIES
-- AND CONDITIONS, EXPRESS, IMPLIED, OR STATUTORY, INCLUDING
-- BUT NOT LIMITED TO WARRANTIES OF MERCHANTABILITY, NON-
-- INFRINGEMENT, OR FITNESS FOR ANY PARTICULAR PURPOSE; and
-- (2) Xilinx shall not be liable (whether in contract or tort,
-- including negligence, or under any other theory of
-- liability) for any loss or damage of any kind or nature
-- related to, arising under or in connection with these
-- materials, including for any direct, or any indirect,
-- special, incidental, or consequential loss or damage
-- (including loss of data, profits, goodwill, or any type of
-- loss or damage suffered as a result of any action brought
-- by a third party) even if such damage or loss was
-- reasonably foreseeable or Xilinx had been advised of the
-- possibility of the same.
--
-- CRITICAL APPLICATIONS
-- Xilinx products are not designed or intended to be fail-
-- safe, or for use in any application requiring fail-safe
-- performance, such as life-support or safety devices or
-- systems, Class III medical devices, nuclear facilities,
-- applications related to the deployment of airbags, or any
-- other applications that could lead to death, personal
-- injury, or severe property or environmental damage
-- (individually and collectively, "Critical
-- Applications"). Customer assumes the sole risk and
-- liability of any use of Xilinx products in Critical
-- Applications, subject only to applicable laws and
-- regulations governing limitations on product liability.
--
-- THIS COPYRIGHT NOTICE AND DISCLAIMER MUST BE RETAINED AS
-- PART OF THIS FILE AT ALL TIMES.
--
-- *************************************************************************
--
-------------------------------------------------------------------------------
-- Filename:    axi_vdma_s2mm_axis_dwidth_converter.vhd
-- Description: This entity is the descriptor fetch command and status inteface
--              for the Scatter Gather Engine AXI DataMover.
--
-- VHDL-Standard:   VHDL'93
-------------------------------------------------------------------------------
-- Structure:
--                  axi_vdma.vhd
--                   |- axi_vdma_pkg.vhd
--                   |- axi_vdma_intrpt.vhd
--                   |- axi_vdma_rst_module.vhd
--                   |   |- axi_vdma_reset.vhd (mm2s)
--                   |   |   |- axi_vdma_cdc.vhd
--                   |   |- axi_vdma_reset.vhd (s2mm)
--                   |   |   |- axi_vdma_cdc.vhd
--                   |
--                   |- axi_vdma_reg_if.vhd
--                   |   |- axi_vdma_lite_if.vhd
--                   |   |- axi_vdma_cdc.vhd (mm2s)
--                   |   |- axi_vdma_cdc.vhd (s2mm)
--                   |
--                   |- axi_vdma_sg_cdc.vhd (mm2s)
--                   |- axi_vdma_vid_cdc.vhd (mm2s)
--                   |- axi_vdma_fsync_gen.vhd (mm2s)
--                   |- axi_vdma_sof_gen.vhd (mm2s)
--                   |- axi_vdma_reg_module.vhd (mm2s)
--                   |   |- axi_vdma_register.vhd (mm2s)
--                   |   |- axi_vdma_regdirect.vhd (mm2s)
--                   |- axi_vdma_mngr.vhd (mm2s)
--                   |   |- axi_vdma_sg_if.vhd (mm2s)
--                   |   |- axi_vdma_sm.vhd (mm2s)
--                   |   |- axi_vdma_cmdsts_if.vhd (mm2s)
--                   |   |- axi_vdma_vidreg_module.vhd (mm2s)
--                   |   |   |- axi_vdma_sgregister.vhd (mm2s)
--                   |   |   |- axi_vdma_vregister.vhd (mm2s)
--                   |   |   |- axi_vdma_vaddrreg_mux.vhd (mm2s)
--                   |   |   |- axi_vdma_blkmem.vhd (mm2s)
--                   |   |- axi_vdma_genlock_mngr.vhd (mm2s)
--                   |       |- axi_vdma_genlock_mux.vhd (mm2s)
--                   |       |- axi_vdma_greycoder.vhd (mm2s)
--                   |- axi_vdma_mm2s_linebuf.vhd (mm2s)
--                   |   |- axi_vdma_sfifo_autord.vhd (mm2s)
--                   |   |- axi_vdma_afifo_autord.vhd (mm2s)
--                   |   |- axi_vdma_skid_buf.vhd (mm2s)
--                   |   |- axi_vdma_cdc.vhd (mm2s)
--                   |
--                   |- axi_vdma_sg_cdc.vhd (s2mm)
--                   |- axi_vdma_vid_cdc.vhd (s2mm)
--                   |- axi_vdma_fsync_gen.vhd (s2mm)
--                   |- axi_vdma_sof_gen.vhd (s2mm)
--                   |- axi_vdma_reg_module.vhd (s2mm)
--                   |   |- axi_vdma_register.vhd (s2mm)
--                   |   |- axi_vdma_regdirect.vhd (s2mm)
--                   |- axi_vdma_mngr.vhd (s2mm)
--                   |   |- axi_vdma_sg_if.vhd (s2mm)
--                   |   |- axi_vdma_sm.vhd (s2mm)
--                   |   |- axi_vdma_cmdsts_if.vhd (s2mm)
--                   |   |- axi_vdma_vidreg_module.vhd (s2mm)
--                   |   |   |- axi_vdma_sgregister.vhd (s2mm)
--                   |   |   |- axi_vdma_vregister.vhd (s2mm)
--                   |   |   |- axi_vdma_vaddrreg_mux.vhd (s2mm)
--                   |   |   |- axi_vdma_blkmem.vhd (s2mm)
--                   |   |- axi_vdma_genlock_mngr.vhd (s2mm)
--                   |       |- axi_vdma_genlock_mux.vhd (s2mm)
--                   |       |- axi_vdma_greycoder.vhd (s2mm)
--                   |- axi_vdma_s2mm_linebuf.vhd (s2mm)
--                   |   |- axi_vdma_sfifo_autord.vhd (s2mm)
--                   |   |- axi_vdma_afifo_autord.vhd (s2mm)
--                   |   |- axi_vdma_skid_buf.vhd (s2mm)
--                   |   |- axi_vdma_cdc.vhd (s2mm)
--                   |
--                   |- axi_datamover_v3_00_a.axi_datamover.vhd (FULL)
--                   |- axi_sg_v3_00_a.axi_sg.vhd
--
-------------------------------------------------------------------------------
-------------------------------------------------------------------------------
library ieee;
use ieee.std_logic_1164.all;
use ieee.numeric_std.all;
use ieee.std_logic_misc.all;

library unisim;
use unisim.vcomponents.all;

library axi_vdma_v5_00_a;
use axi_vdma_v5_00_a.axi_vdma_pkg.all;


entity  axi_vdma_s2mm_axis_dwidth_converter is
     generic ( 	C_S_AXIS_S2MM_TDATA_WIDTH_CALCULATED         	: integer range 8 to 1024   	:= 32;
 		C_S_AXIS_S2MM_TDATA_WIDTH         		: integer range 8 to 1024   	:= 32;
 		C_S_AXIS_S2MM_TUSER_BITS             			: integer       		:= 1;
 		C_AXIS_TID_WIDTH             			: integer       		:= 1;
 		C_AXIS_TDEST_WIDTH           			: integer       		:= 1;
        	C_FAMILY                     			: string        	       	:= "virtex6"  );
     port ( 
      		ACLK                         :in  std_logic;
      		ARESETN                      :in  std_logic;
      		ACLKEN                       :in  std_logic;
      		S_AXIS_TVALID                :in  std_logic;
      		S_AXIS_TREADY                :out  std_logic;
      		S_AXIS_TDATA                 :in  std_logic_vector(C_S_AXIS_S2MM_TDATA_WIDTH-1 downto 0);
      		S_AXIS_TSTRB                 :in  std_logic_vector(C_S_AXIS_S2MM_TDATA_WIDTH/8-1 downto 0);
      		S_AXIS_TKEEP                 :in  std_logic_vector(C_S_AXIS_S2MM_TDATA_WIDTH/8-1 downto 0);
      		S_AXIS_TLAST                 :in  std_logic;
      		S_AXIS_TID                   :in  std_logic_vector(C_AXIS_TID_WIDTH-1 downto 0);
      		S_AXIS_TDEST                 :in  std_logic_vector(C_AXIS_TDEST_WIDTH-1 downto 0);
      		S_AXIS_TUSER                 :in  std_logic_vector(C_S_AXIS_S2MM_TUSER_BITS-1 downto 0);
      		M_AXIS_TVALID                :out  std_logic;
      		M_AXIS_TREADY                :in   std_logic;
      		M_AXIS_TDATA                 :out  std_logic_vector(C_S_AXIS_S2MM_TDATA_WIDTH_CALCULATED-1 downto 0);
      		M_AXIS_TSTRB                 :out  std_logic_vector(C_S_AXIS_S2MM_TDATA_WIDTH_CALCULATED/8-1 downto 0);
      		M_AXIS_TKEEP                 :out  std_logic_vector(C_S_AXIS_S2MM_TDATA_WIDTH_CALCULATED/8-1 downto 0);
      		M_AXIS_TLAST                 :out  std_logic;
      		M_AXIS_TID                   :out  std_logic_vector(C_AXIS_TID_WIDTH-1 downto 0);
      		M_AXIS_TDEST                 :out  std_logic_vector(C_AXIS_TDEST_WIDTH-1 downto 0);
      		M_AXIS_TUSER                 :out  std_logic_vector(C_S_AXIS_S2MM_TUSER_BITS-1 downto 0)
  );


end axi_vdma_s2mm_axis_dwidth_converter;



-------------------------------------------------------------------------------

-------------------------------------------------------------------------------
-- Architecture
-------------------------------------------------------------------------------
architecture implementation of axi_vdma_s2mm_axis_dwidth_converter is

-------------------------------------------------------------------------------
-- Functions
-------------------------------------------------------------------------------

-- No Functions Declared

-------------------------------------------------------------------------------
-- Constants Declarations
-------------------------------------------------------------------------------

constant ZERO_VALUE                 : std_logic_vector(255 downto 0)
                                        := (others => '0');

-------------------------------------------------------------------------------
-- Verilog module component declarations
-------------------------------------------------------------------------------

  component axis_dwidth_converter is
     generic ( 	C_S_AXIS_TDATA_WIDTH         	: integer   			:= 32;
 		C_M_AXIS_TDATA_WIDTH         	: integer   			:= 32;
 		C_AXIS_TID_WIDTH         	: integer       		:= 1;
 		C_AXIS_TDEST_WIDTH         	: integer       		:= 1;
 		C_S_AXIS_TUSER_WIDTH         	: integer   			:= 1;
 		C_M_AXIS_TUSER_WIDTH         	: integer   			:= 1;
 		C_AXIS_SIGNAL_SET            	: std_logic_vector(31 downto 0)	:= x"000000FF";
        	C_FAMILY                     	: string                    	:= "virtex6"  );
     port ( 
      		ACLK                         :in  std_logic;
      		ARESETN                      :in  std_logic;
      		ACLKEN                       :in  std_logic;
      		S_AXIS_TVALID                :in  std_logic;
      		S_AXIS_TREADY                :out  std_logic;
      		S_AXIS_TDATA                 :in  std_logic_vector(C_S_AXIS_S2MM_TDATA_WIDTH-1 downto 0);
      		S_AXIS_TSTRB                 :in  std_logic_vector(C_S_AXIS_S2MM_TDATA_WIDTH/8-1 downto 0);
      		S_AXIS_TKEEP                 :in  std_logic_vector(C_S_AXIS_S2MM_TDATA_WIDTH/8-1 downto 0);
      		S_AXIS_TLAST                 :in  std_logic;
      		S_AXIS_TID                   :in  std_logic_vector(C_AXIS_TID_WIDTH-1 downto 0);
      		S_AXIS_TDEST                 :in  std_logic_vector(C_AXIS_TDEST_WIDTH-1 downto 0);
      		S_AXIS_TUSER                 :in  std_logic_vector(C_S_AXIS_S2MM_TUSER_BITS-1 downto 0);
      		M_AXIS_TVALID                :out  std_logic;
      		M_AXIS_TREADY                :in   std_logic;
      		M_AXIS_TDATA                 :out  std_logic_vector(C_S_AXIS_S2MM_TDATA_WIDTH_CALCULATED-1 downto 0);
      		M_AXIS_TSTRB                 :out  std_logic_vector(C_S_AXIS_S2MM_TDATA_WIDTH_CALCULATED/8-1 downto 0);
      		M_AXIS_TKEEP                 :out  std_logic_vector(C_S_AXIS_S2MM_TDATA_WIDTH_CALCULATED/8-1 downto 0);
      		M_AXIS_TLAST                 :out  std_logic;
      		M_AXIS_TID                   :out  std_logic_vector(C_AXIS_TID_WIDTH-1 downto 0);
      		M_AXIS_TDEST                 :out  std_logic_vector(C_AXIS_TDEST_WIDTH-1 downto 0);
      		M_AXIS_TUSER                 :out  std_logic_vector(C_S_AXIS_S2MM_TUSER_BITS-1 downto 0)
  );

  end component;
-------------------------------------------------------------------------------
-- Signal / Type Declarations
-------------------------------------------------------------------------------

-- No Signals Declared

-------------------------------------------------------------------------------
-- Begin architecture logic
-------------------------------------------------------------------------------
begin


    S2MM_AXIS_DWIDTH_CONVERTER_I : axis_dwidth_converter
     generic map( 	C_S_AXIS_TDATA_WIDTH         =>		C_S_AXIS_S2MM_TDATA_WIDTH		, 
 		C_M_AXIS_TDATA_WIDTH         =>		C_S_AXIS_S2MM_TDATA_WIDTH_CALCULATED		, 
 		C_AXIS_TID_WIDTH             =>		C_AXIS_TID_WIDTH		, 
 		C_S_AXIS_TUSER_WIDTH             =>		C_S_AXIS_S2MM_TUSER_BITS		, 
 		C_M_AXIS_TUSER_WIDTH             =>		C_S_AXIS_S2MM_TUSER_BITS		, 
 		C_AXIS_TDEST_WIDTH           =>		C_AXIS_TDEST_WIDTH		, 
 		C_AXIS_SIGNAL_SET            =>		x"000000FF"		, 
        	C_FAMILY                     =>		C_FAMILY		   )
     port map( 
      		ACLK                         =>	ACLK                 			, 
      		ARESETN                      =>	ARESETN              			, 
      		ACLKEN                       =>	ACLKEN               			, 
      		S_AXIS_TVALID                =>	S_AXIS_TVALID        			, 
      		S_AXIS_TREADY                =>	S_AXIS_TREADY        			, 
      		S_AXIS_TDATA                 =>	S_AXIS_TDATA(C_S_AXIS_S2MM_TDATA_WIDTH-1 downto 0)         			, 
      		S_AXIS_TSTRB                 =>	S_AXIS_TSTRB(C_S_AXIS_S2MM_TDATA_WIDTH/8-1 downto 0)         			, 
      		S_AXIS_TKEEP                 =>	S_AXIS_TKEEP(C_S_AXIS_S2MM_TDATA_WIDTH/8-1 downto 0)         			, 
      		S_AXIS_TLAST                 =>	S_AXIS_TLAST         			, 
      		S_AXIS_TID                   =>	S_AXIS_TID(C_AXIS_TID_WIDTH-1 downto 0)           			, 
      		S_AXIS_TDEST                 =>	S_AXIS_TDEST(C_AXIS_TDEST_WIDTH-1 downto 0)         			, 
      		S_AXIS_TUSER                 =>	S_AXIS_TUSER(C_S_AXIS_S2MM_TUSER_BITS-1 downto 0)         			, 
      		M_AXIS_TVALID                =>	M_AXIS_TVALID        			, 
      		M_AXIS_TREADY                =>	M_AXIS_TREADY        			, 
      		M_AXIS_TDATA                 =>	M_AXIS_TDATA(C_S_AXIS_S2MM_TDATA_WIDTH_CALCULATED-1 downto 0)         			, 
      		M_AXIS_TSTRB                 =>	M_AXIS_TSTRB(C_S_AXIS_S2MM_TDATA_WIDTH_CALCULATED/8-1 downto 0)         			, 
      		M_AXIS_TKEEP                 =>	M_AXIS_TKEEP(C_S_AXIS_S2MM_TDATA_WIDTH_CALCULATED/8-1 downto 0)         			, 
      		M_AXIS_TLAST                 =>	M_AXIS_TLAST         			, 
      		M_AXIS_TID                   =>	M_AXIS_TID(C_AXIS_TID_WIDTH-1 downto 0)           			, 
      		M_AXIS_TDEST                 =>	M_AXIS_TDEST(C_AXIS_TDEST_WIDTH-1 downto 0)         			, 
      		M_AXIS_TUSER                 =>	M_AXIS_TUSER(C_S_AXIS_S2MM_TUSER_BITS-1 downto 0)        			
  );






end implementation;
