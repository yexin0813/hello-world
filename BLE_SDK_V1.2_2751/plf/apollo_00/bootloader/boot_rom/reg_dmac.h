//
// Created by             :
// Filename               :reg_dmac.h
// Author                 :
// Created On             :2016-04-20 18:11:36
// Last Modified          :
// Update Count           :2016-04-20 18:11:36
// Description            :
//
//======================================================================
#ifndef __REG_DMAC_H__
#define __REG_DMAC_H__
//Auto-gen by reg_gen
#include "_reg_base_addr.h"
//ch0_sar_L
typedef union
{
    uint32_t val;
    struct
    {
        uint32_t                            sar : 32; /*31: 0, Current Source Address of DMA transfer. Updated after each sourcetransfer. The SINC field in the CTLx register determines whether the address increments, decrements, or is left unchanged on every source transfer throughout the block transfer.*/
    } bit_field;
} T_DMAC_CH0_SAR_L;

//ch0_sar_H
typedef union
{
    uint32_t val;
    struct
    {
        uint32_t                     reserved_0 : 32; /*31: 0,                       Reserved*/
    } bit_field;
} T_DMAC_CH0_SAR_H;

//ch0_dar_L
typedef union
{
    uint32_t val;
    struct
    {
        uint32_t                            dar : 32; /*31: 0, Current Destination address of DMA transfer. Updated after eachdestination transfer. The DINC field in the CTLx register determines whetherthe address increments, decrements, or is left unchanged on everydestination transfer throughout the block transfer.*/
    } bit_field;
} T_DMAC_CH0_DAR_L;

//ch0_dar_H
typedef union
{
    uint32_t val;
    struct
    {
        uint32_t                     reserved_0 : 32; /*31: 0,                       Reserved*/
    } bit_field;
} T_DMAC_CH0_DAR_H;

//ch0_ctrl_L
typedef union
{
    uint32_t val;
    struct
    {
        uint32_t                         int_en :  1; /* 0: 0, Interrupt Enable Bit. If set, then all interrupt-generating sources areenabled. Functions as a global mask bit for all interrupts for thechannel; raw* interrupt registers still assert if CTLx.INT_EN = 0.Reset Value: 0x1*/
        uint32_t                   dst_tr_width :  3; /* 3: 1, Destination Transfer Width. Table 6-3 on page 175 lists thedecoding for this field. Mapped to AHB bus ※hsize.§ For anon-memory peripheral, typically rgw peripheral (destination) FIFOwidth.This value must be less than or equal to DMAH_Mk_HDATA_WIDTH,where k is the AHB layer 1 to 4 where the destination resides.Reset Value: Encoded value; refer to Table 6-3 on page 175.Dependencies: This field does not exist if DMAH_CHx_DTW(page 129) is hardcoded. In this case, the read-back value is alwaysthe hardcoded destination transfer width, DMAH_CHx_DTW.*/
        uint32_t                   src_tr_width :  3; /* 6: 4, Source Transfer Width. Table 6-3 on page 175 lists the decoding forthis field.Mapped to AHB bus ※hsize.§ For a non-memory peripheral, typicallythe peripheral (source) FIFO width.This value must be less than or equal to DMAH_Mx_HDATA_WIDTH,where x is the AHB layer 1 to 4 where the source resides.Reset Value: Encoded value; refer to Table 6-3 on page 175.Dependencies: This field does not exist if the parameterDMAH_CHx_STW (page 129) is hardcoded. In this case, theread-back value is always the hardcoded source transfer width,DMAH_CHx_STW.*/
        uint32_t                           dinc :  2; /* 8: 7, Destination Address Increment. Indicates whether to increment ordecrement the destination address on every destination transfer. Ifyour device is writing data to a destination peripheral FIFO with afixed address, then set this field to ※No change.§00 = Increment01 = Decrement1x = No changeNOTE: Incrementing or decrementing is done for alignment to thenext CTLx.DST_TR_WIDTH boundary.Reset Value: 0x0*/
        uint32_t                           sinc :  2; /*10: 9, Source Address Increment. Indicates whether to increment ordecrement the source address on every source transfer. If the deviceis fetching data from a source peripheral FIFO with a fixed address,then set this field to ※No change.§00 = Increment01 = Decrement1x = No changeNOTE: Incrementing or decrementing is done for alignment to thenext CTLx.SRC_TR_WIDTH boundary.Reset Value: 0x0*/
        uint32_t                     dest_msize :  3; /*13:11, Destination Burst Transaction Length. Number of data items, eachof width CTLx.DST_TR_WIDTH, to be written to the destinationevery time a destination burst transaction request is made from eitherthe corresponding hardware or software handshaking interface.NOTE: This value is not related to the AHB bus master HBURSTbus.Reset Value: 0x1Dependencies: DMAH_CHx_MAX_MULT_SIZE determines the bit width of this field. All surplus bits in this field do not exist and read back as 0.*/
        uint32_t                      src_msize :  3; /*16:14, Source Burst Transaction Length. Number of data items, each ofwidth CTLx.SRC_TR_WIDTH, to be read from the source every timea source burst transaction request is made from either thecorresponding hardware or software handshaking interface.Table 6-2 on page 174 lists the decoding for this field; refer to※Choosing the Receive Watermark level§ on page 101.NOTE: This value is not related to the AHB bus master HBURSTbus.Reset Value: 0x1Dependencies: The configuration parameter DMAH_CHx_MAX_MULT_SIZE determines the bit widthof this field. All remaining bits in this field do not exist and read backas 0.*/
        uint32_t                  src_gather_en :  1; /*17:17, Source gather enable bit:0 = Gather disabled1 = Gather enabledGather on the source side is applicable only when the CTLx.SINC bitindicates an incrementing or decrementing address control.Reset Value: 0x0Dependencies: This field does not exist ifDMAH_CHx_SRC_GAT_EN is not selected; in this case, the read-back value is always 0.*/
        uint32_t                 dst_scatter_en :  1; /*18:18, Destination scatter enable bit:0 = Scatter disabled1 = Scatter enabledScatter on the destination side is applicable only when theCTLx.DINC bit indicates an incrementing or decrementing addresscontrol.Reset Value: 0x0Dependencies: This field does not exist if DMAH_CHx_DST_SCA_EN is not selected; in this case, the read-back value is always 0.*/
        uint32_t                     reserved_1 :  1; /*19:19,                       Reserved*/
        uint32_t                          tt_fc :  2; /*21:20, Transfer Type and Flow Control. The following transfer types aresupported.? Memory to Memory? Memory to Peripheral? Peripheral to Memory? Peripheral to PeripheralFlow Control can be assigned to the DW_ahb_dmac, the sourceperipheral, or the destination peripheral. Reset Value: Configuration dependent:TT_FC[0] = 1＊b1TT_FC[1] = DMAH_CHx_FC[1] || (!DMAH_CHx_FC[0])TT_FC[2] = DMAH_CHx_FC[1] ^ DMAH_CHx_FC[0]Dependencies: If the configuration parameter DMAH_CHx_FCis set to DMA_FC_ONLY, then TT_FC[2] does not existand TT_FC[2] always reads back 0.If DMAH_CHx_FC is set to SRC_FC_ONLY, then TT_FC[2:1] doesnot exist and TT_FC[2:1] always reads back 2＊b10.If DMAH_CHx_FC is set to DST_FC_ONLY, then TT_FC[2:1] doesnot exist and TT_FC[2:1] always reads back 2＊b11.For multi-block transfers using linked list operation, TT_FC must beconstant for all blocks of this multi-block transfer.*/
        uint32_t                            dms :  3; /*24:22, Destination Master Select. Identifies the Master Interface layerwhere the destination device (peripheral or memory) resides.00 = AHB master 101 = AHB master 210 = AHB master 311 = AHB master 4The maximum value of this field that can be read back isDMAH_NUM_MASTER_INT 每 1.Reset Value: DMAH_CHx_DMS[1:0]Dependencies: This field does not exist if the configurationparameter DMAH_CHx_DMS (page 129) is hardcoded; in this case,the read-back value is always the hardcoded value.*/
        uint32_t                            sms :  2; /*26:25, Source Master Select. Identifies the Master Interface layer fromwhich the source device (peripheral or memory) is accessed.00 = AHB master 101 = AHB master 210 = AHB master 311 = AHB master 4The maximum value of this field that can be read back isDMAH_NUM_MASTER_INT 每 1.Reset Value: DMAH_CHx_SMS[1:0]Dependencies: This field does not exist if the configurationparameter DMAH_CHx_SMS is hardcoded; in this case,the read-back value is always the hardcoded value.*/
        uint32_t                     llp_dst_en :  1; /*27:27, Block chaining is enabled on the destination side only if theLLP_DST_EN field is high and LLPx.LOC is non-zero.Reset Value: 0x0Dependencies: This field does not exist if the configurationparameter DMAH_CHx_MULTI_BLK_EN is not selected or if DMAH_CHx_HC_LLP is selected; in this case, the read-back value is always 0.*/
        uint32_t                     llp_src_en :  1; /*28:28, Block chaining is enabled on the source side only if theLLP_SRC_EN field is high and LLPx.LOC is non-zeroReset Value: 0x0Dependencies: This field does not exist if the configurationparameter DMAH_CHx_MULTI_BLK_EN is not selected or if DMAH_CHx_HC_LLP is selected; in this case, the read-back value is always 0.*/
        uint32_t                     reserved_0 :  3; /*31:29,                       Reserved*/
    } bit_field;
} T_DMAC_CH0_CTRL_L;

//ch0_ctrl_H
typedef union
{
    uint32_t val;
    struct
    {
        uint32_t                       block_ts : 12; /*11: 0, Block Transfer Size.When the DW_ahb_dmac is the flow controller, the user writes thisfield before the channel is enabled in order to indicate the block size.The number programmed into BLOCK_TS indicates the total numberof single transactions to perform for every block transfer; a singletransaction is mapped to a single AMBA beat.Width: The width of the single transaction is determined byCTLx.SRC_TR_WIDTH. For further information on setting this field,refer to Table 6-3 on page 175 and ※Transfer Operation§ on page 61.Once the transfer starts, the read-back value is the total number ofdata items already read from the source peripheral, regardless ofwhat is the flow controller.When the source or destination peripheral is assigned as the flowcontroller, then the maximum block size that can be read backsaturates at DMAH_CHx_MAX_BLK_SIZE, but the actual block sizecan be greater. Refer to Table 8-1 on page 249.b = log2(DMAH_CHx_MAX_BLK_SIZE + 1) + 31Bits 43:b+1 do not exist and return 0 on a read.Reset Value: 0x2*/
        uint32_t                           done :  1; /*12:12, Done bitIf status write-back is enabled, the upper word of the control register,CTLx[63:32], is written to the control register location of the LinkedList Item (LLI) in system memory at the end of the block transfer withthe done bit set.Software can poll the LLI CTLx.DONE bit to see when a blocktransfer is complete. The LLI CTLx.DONE bit should be clearedwhen the linked lists are set up in memory prior to enabling thechannel.LLI accesses are always 32-bit accesses (Hsize = 2) aligned to 32-bitboundaries and cannot be changed or programmed to anything otherthan 32-bit. For more information, refer to ※Multi-Block Transfers§ onpage 247.Reset Value: 0x0*/
        uint32_t                     reserved_0 : 19; /*31:13,                       Reserved*/
    } bit_field;
} T_DMAC_CH0_CTRL_H;

//ch0_cfg_L
typedef union
{
    uint32_t val;
    struct
    {
        uint32_t                     reserved_0 :  5; /* 4: 0,                       Reserved*/
        uint32_t                       ch_prior :  3; /* 7: 5, Channel priority. A priority of 7 is the highest priority, and 0 isthe lowest. This field must be programmed within the followingrange:0: (DMAH_NUM_CHANNELS 每 1)A programmed value outside this range will cause erroneousbehavior.*/
        uint32_t                        ch_susp :  1; /* 8: 8, Channel Suspend. Suspends all DMA data transfers from thesource until this bit is cleared. There is no guarantee that thecurrent transaction will complete. Can also be used in conjunctionwith CFGx.FIFO_EMPTY to cleanly disable a channel withoutlosing any data.0 = Not suspended.1 = Suspend DMA transfer from the source.For more information, refer to ※Disabling a Channel Prior toTransfer Completion§ on page 282.*/
        uint32_t                     fifo_empty :  1; /* 9: 9, Indicates if there is data left in the channel FIFO. Can be used inconjunction with CFGx.CH_SUSP to cleanly disable a channel.For more information, refer to ※Disabling a Channel Prior toTransfer Completion§ on page 282.1 = Channel FIFO empty0 = Channel FIFO not empty*/
        uint32_t                     hs_sel_dst :  1; /*10:10, Destination Software or Hardware Handshaking Select. Thisregister selects which of the handshaking interfaces 每 hardwareor software 每 is active for destination requests on this channel.0 = Hardware handshaking interface. Software-initiatedtransaction requests are ignored.1 = Software handshaking interface. Hardware- initiatedtransaction requests are ignored.If the destination peripheral is memory, then this bit is ignored.*/
        uint32_t                     hs_sel_src :  1; /*11:11, Source Software or Hardware Handshaking Select. Thisregister selects which of the handshaking interfaces 每 hardwareor software 每 is active for source requests on this channel.0 = Hardware handshaking interface. Software-initiatedtransaction requests are ignored.1 = Software handshaking interface. Hardware-initiatedtransaction requests are ignored.If the source peripheral is memory, then this bit is ignored.*/
        uint32_t                      lock_ch_l :  2; /*13:12, Channel Lock Level. Indicates the duration over whichCFGx.LOCK_CH bit applies.00 = Over complete DMA transfer01 = Over complete DMA block transfer1x = Over complete DMA transactionThis field does not exist if the configuration parameterDMAH_CHx_LOCK_EN is set to False (page 128); in this case,the read-back value is always 0.*/
        uint32_t                       lock_b_l :  2; /*15:14, Bus Lock Level. Indicates the duration over whichCFGx.LOCK_B bit applies.00 = Over complete DMA transfer01 = Over complete DMA block transfer1x = Over complete DMA transactionThis field does not exist if the parameter DMAH_CHx_LOCK_ENis set to False (page 128); in this case, the read-back value isalways 0.*/
        uint32_t                        lock_ch :  1; /*16:16, Bus Lock Bit. When active, the AHB bus master signal hlock isasserted for the duration specified in CFGx.LOCK_B_L. For moreinformation, refer to ※Locked DMA Transfers§ on page 103.This field does not exist if the configuration parameterDMAH_CHx_LOCK_EN is set to False (page 128); in this case,the read-back value is always 0.*/
        uint32_t                         lock_b :  1; /*17:17, Bus Lock Bit. When active, the AHB bus master signal hlock isasserted for the duration specified in CFGx.LOCK_B_L. For moreinformation, refer to ※Locked DMA Transfers§ on page 103.This field does not exist if the configuration parameterDMAH_CHx_LOCK_EN is set to False (page 128); in this case,the read-back value is always 0.*/
        uint32_t                     dst_hs_pol :  1; /*18:18, Source Handshaking Interface Polarity.0 = Active high1 = Active lowFor information on this, refer to ※Handshaking Interface§ onpage 42.*/
        uint32_t                     src_hs_pol :  1; /*19:19, Source Handshaking Interface Polarity.0 = Active high1 = Active lowFor information on this, refer to ※Handshaking Interface§ onpage 42.*/
        uint32_t                      max_abrst : 10; /*29:20, Maximum AMBA Burst Length. Maximum AMBA burst lengththat is used for DMA transfers on this channel.A value of 0 indicates that software is not limiting the maximumAMBA burst length for DMA transfers on this channel.This field does not exist if the configuration parameterDMAH_MABRST (page 121) is not selected; in this case, theread-back value is always 0, and the maximum AMBA burstlength cannot be limited by software.*/
        uint32_t                     reload_src :  1; /*30:30, Automatic Source Reload. The SARx register can beautomatically reloaded from its initial value at the end of everyblock for multi-block transfers. A new block transfer is theninitiated. For conditions under which this occurs, refer to Table 8-1on page 249.This field does not exist if the configuration parameterDMAH_CHx_MULTI_BLK_EN (page 130) is not selected; in thiscase, the read-back value is always 0.*/
        uint32_t                     reload_dst :  1; /*31:31, Automatic Destination Reload. The DARx register can beautomatically reloaded from its initial value at the end of everyblock for multi-block transfers. A new block transfer is theninitiated. For conditions under which this occurs, refer to Table 8-1on page 249.This register does not exist if the configuration parameterDMAH_CHx_MULTI_BLK_EN (page 130) is not selected; in thiscase, the read-back value is always 0.*/
    } bit_field;
} T_DMAC_CH0_CFG_L;

//ch0_cfg_H
typedef union
{
    uint32_t val;
    struct
    {
        uint32_t                        fc_mode :  1; /* 0: 0, Flow Control Mode. Determines when source transactionrequests are serviced when the Destination Peripheral is the flowcontroller.0 = Source transaction requests are serviced when they occur.Data pre-fetching is enabled.1 = Source transaction requests are not serviced until adestination transaction request occurs. In this mode, the amountof data transferred from the source is limited so that it isguaranteed to be transferred to the destination prior to blocktermination by the destination. Data pre-fetching is disabled.*/
        uint32_t                      fifo_mode :  1; /* 1: 1, FIFO Mode Select. Determines how much space or data needsto be available in the FIFO before a burst transaction request isserviced.0 = Space/data available for single AHB transfer of the specifiedtransfer width.1 = Data available is greater than or equal to half the FIFO depthfor destination transfers and space available is greater than halfthe fifo depth for source transfers. The exceptions are at the endof a burst transaction request or at the end of a block transfer.*/
        uint32_t                        protctl :  3; /* 4: 2, Protection Control bits used to drive the AHB HPROT[3:1] bus.The AMBA Specification recommends that the default value ofHPROT indicates a non-cached, non-buffered, privileged dataaccess. The reset value is used to indicate such an access.HPROT[0] is tied high because all transfers are data accesses,as there are no opcode fetches.There is a one-to-one mapping of these register bits to theHPROT[3:1] master interface signals. Table 6-5 on page 185shows the mapping of bits in this field to the AHB HPROT[3:1]bus.*/
        uint32_t                      ds_upd_en :  1; /* 5: 5, Destination Status Update Enable. Destination statusinformation is fetched only from the location pointed to by theDSTATARx register, stored in the DSTATx register and written outto the DSTATx location of the LLI (refer to Figure 8-3 on page250) if DS_UPD_EN is high.This field does not exist if the configuration parameterDMAH_CHx_STAT_DST is set to False; in this case, theread-back value is always 0.*/
        uint32_t                      ss_upd_en :  1; /* 6: 6, Source Status Update Enable. Source status information isfetched only from the location pointed to by the SSTATARxregister, stored in the SSTATx register and written out to theSSTATx location of the LLI (refer to Figure 8-3 on page 250) ifSS_UPD_EN is high.NOTE: This enable is applicable only if DMAH_CHx_STAT_SRC(page 132) is set to True.This field does not exist if the configuration parameterDMAH_CHx_STAT_SRC is set to False; in this case, theread-back value is always 0.*/
        uint32_t                        src_per :  4; /*10: 7, Assigns a hardware handshaking interface(0 - DMAH_NUM_HS_INT-1) to the source of channel x if theCFGx.HS_SEL_SRC field is 0; otherwise, this field is ignored.The channel can then communicate with the source peripheralconnected to that interface through the assigned hardwarehandshaking interface.NOTE1: For correct DW_ahb_dmac operation, only oneperipheral (source or destination) should be assigned to thesame handshaking interface.NOTE2: This field does not exist if the configuration parameterDMAH_NUM_HS_INT is set to 0.NOTE3:← b = 39 if DMAH_NUM_HS_INT is 1← b = ceil(log2(DMAH_NUM_HS_INT)) + 38 ifDMAH_NUM_HS_INT is greater than 1Bits 42:(b+1) do not exist and return 0 on a read.*/
        uint32_t                       dest_per :  4; /*14:11, Assigns a hardware handshaking interface(0 - DMAH_NUM_HS_INT-1) to the destination of channel x if theCFGx.HS_SEL_DST field is 0; otherwise, this field is ignored.The channel can then communicate with the destinationperipheral connected to that interface through the assignedhardware handshaking interface.NOTE1: For correct DMA operation, only one peripheral (sourceor destination) should be assigned to the same handshakinginterface.NOTE2: This field does not exist if the configuration parameterDMAH_NUM_HS_INT is set to 0.NOTE3:← b = 43 if DMAH_NUM_HS_INT is 1← b = ceil(log2(DMAH_NUM_HS_INT)) + 42 ifDMAH_NUM_HS_INT is greater than 1Bits 46:(b+1) do not exist and return 0 on a read.*/
        uint32_t                     reserved_0 : 17; /*31:15,                       Reserved*/
    } bit_field;
} T_DMAC_CH0_CFG_H;

//ch1_sar_L
typedef union
{
    uint32_t val;
    struct
    {
        uint32_t                            sar : 32; /*31: 0, Current Source Address of DMA transfer. Updated after each sourcetransfer. The SINC field in the CTLx register determines whether the address increments, decrements, or is left unchanged on every source transfer throughout the block transfer.*/
    } bit_field;
} T_DMAC_CH1_SAR_L;

//ch1_sar_H
typedef union
{
    uint32_t val;
    struct
    {
        uint32_t                     reserved_0 : 32; /*31: 0,                       Reserved*/
    } bit_field;
} T_DMAC_CH1_SAR_H;

//ch1_dar_L
typedef union
{
    uint32_t val;
    struct
    {
        uint32_t                            dar : 32; /*31: 0, Current Destination address of DMA transfer. Updated after eachdestination transfer. The DINC field in the CTLx register determines whetherthe address increments, decrements, or is left unchanged on everydestination transfer throughout the block transfer.*/
    } bit_field;
} T_DMAC_CH1_DAR_L;

//ch1_dar_H
typedef union
{
    uint32_t val;
    struct
    {
        uint32_t                     reserved_0 : 32; /*31: 0,                       Reserved*/
    } bit_field;
} T_DMAC_CH1_DAR_H;

//ch1_ctrl_L
typedef union
{
    uint32_t val;
    struct
    {
        uint32_t                         int_en :  1; /* 0: 0, Interrupt Enable Bit. If set, then all interrupt-generating sources areenabled. Functions as a global mask bit for all interrupts for thechannel; raw* interrupt registers still assert if CTLx.INT_EN = 0.Reset Value: 0x1*/
        uint32_t                   dst_tr_width :  3; /* 3: 1, Destination Transfer Width. Table 6-3 on page 175 lists thedecoding for this field. Mapped to AHB bus ※hsize.§ For anon-memory peripheral, typically rgw peripheral (destination) FIFOwidth.This value must be less than or equal to DMAH_Mk_HDATA_WIDTH,where k is the AHB layer 1 to 4 where the destination resides.Reset Value: Encoded value; refer to Table 6-3 on page 175.Dependencies: This field does not exist if DMAH_CHx_DTW(page 129) is hardcoded. In this case, the read-back value is alwaysthe hardcoded destination transfer width, DMAH_CHx_DTW.*/
        uint32_t                   src_tr_width :  3; /* 6: 4, Source Transfer Width. Table 6-3 on page 175 lists the decoding forthis field.Mapped to AHB bus ※hsize.§ For a non-memory peripheral, typicallythe peripheral (source) FIFO width.This value must be less than or equal to DMAH_Mx_HDATA_WIDTH,where x is the AHB layer 1 to 4 where the source resides.Reset Value: Encoded value; refer to Table 6-3 on page 175.Dependencies: This field does not exist if the parameterDMAH_CHx_STW (page 129) is hardcoded. In this case, theread-back value is always the hardcoded source transfer width,DMAH_CHx_STW.*/
        uint32_t                           dinc :  2; /* 8: 7, Destination Address Increment. Indicates whether to increment ordecrement the destination address on every destination transfer. Ifyour device is writing data to a destination peripheral FIFO with afixed address, then set this field to ※No change.§00 = Increment01 = Decrement1x = No changeNOTE: Incrementing or decrementing is done for alignment to thenext CTLx.DST_TR_WIDTH boundary.Reset Value: 0x0*/
        uint32_t                           sinc :  2; /*10: 9, Source Address Increment. Indicates whether to increment ordecrement the source address on every source transfer. If the deviceis fetching data from a source peripheral FIFO with a fixed address,then set this field to ※No change.§00 = Increment01 = Decrement1x = No changeNOTE: Incrementing or decrementing is done for alignment to thenext CTLx.SRC_TR_WIDTH boundary.Reset Value: 0x0*/
        uint32_t                     dest_msize :  3; /*13:11, Destination Burst Transaction Length. Number of data items, eachof width CTLx.DST_TR_WIDTH, to be written to the destinationevery time a destination burst transaction request is made from eitherthe corresponding hardware or software handshaking interface.NOTE: This value is not related to the AHB bus master HBURSTbus.Reset Value: 0x1Dependencies: DMAH_CHx_MAX_MULT_SIZE determines the bit width of this field. All surplus bits in this field do not exist and read back as 0.*/
        uint32_t                      src_msize :  3; /*16:14, Source Burst Transaction Length. Number of data items, each ofwidth CTLx.SRC_TR_WIDTH, to be read from the source every timea source burst transaction request is made from either thecorresponding hardware or software handshaking interface.Table 6-2 on page 174 lists the decoding for this field; refer to※Choosing the Receive Watermark level§ on page 101.NOTE: This value is not related to the AHB bus master HBURSTbus.Reset Value: 0x1Dependencies: The configuration parameter DMAH_CHx_MAX_MULT_SIZE determines the bit widthof this field. All remaining bits in this field do not exist and read backas 0.*/
        uint32_t                  src_gather_en :  1; /*17:17, Source gather enable bit:0 = Gather disabled1 = Gather enabledGather on the source side is applicable only when the CTLx.SINC bitindicates an incrementing or decrementing address control.Reset Value: 0x0Dependencies: This field does not exist ifDMAH_CHx_SRC_GAT_EN is not selected; in this case, the read-back value is always 0.*/
        uint32_t                 dst_scatter_en :  1; /*18:18, Destination scatter enable bit:0 = Scatter disabled1 = Scatter enabledScatter on the destination side is applicable only when theCTLx.DINC bit indicates an incrementing or decrementing addresscontrol.Reset Value: 0x0Dependencies: This field does not exist if DMAH_CHx_DST_SCA_EN is not selected; in this case, the read-back value is always 0.*/
        uint32_t                     reserved_1 :  1; /*19:19,                       Reserved*/
        uint32_t                          tt_fc :  2; /*21:20, Transfer Type and Flow Control. The following transfer types aresupported.? Memory to Memory? Memory to Peripheral? Peripheral to Memory? Peripheral to PeripheralFlow Control can be assigned to the DW_ahb_dmac, the sourceperipheral, or the destination peripheral. Reset Value: Configuration dependent:TT_FC[0] = 1＊b1TT_FC[1] = DMAH_CHx_FC[1] || (!DMAH_CHx_FC[0])TT_FC[2] = DMAH_CHx_FC[1] ^ DMAH_CHx_FC[0]Dependencies: If the configuration parameter DMAH_CHx_FCis set to DMA_FC_ONLY, then TT_FC[2] does not existand TT_FC[2] always reads back 0.If DMAH_CHx_FC is set to SRC_FC_ONLY, then TT_FC[2:1] doesnot exist and TT_FC[2:1] always reads back 2＊b10.If DMAH_CHx_FC is set to DST_FC_ONLY, then TT_FC[2:1] doesnot exist and TT_FC[2:1] always reads back 2＊b11.For multi-block transfers using linked list operation, TT_FC must beconstant for all blocks of this multi-block transfer.*/
        uint32_t                            dms :  3; /*24:22, Destination Master Select. Identifies the Master Interface layerwhere the destination device (peripheral or memory) resides.00 = AHB master 101 = AHB master 210 = AHB master 311 = AHB master 4The maximum value of this field that can be read back isDMAH_NUM_MASTER_INT 每 1.Reset Value: DMAH_CHx_DMS[1:0]Dependencies: This field does not exist if the configurationparameter DMAH_CHx_DMS (page 129) is hardcoded; in this case,the read-back value is always the hardcoded value.*/
        uint32_t                            sms :  2; /*26:25, Source Master Select. Identifies the Master Interface layer fromwhich the source device (peripheral or memory) is accessed.00 = AHB master 101 = AHB master 210 = AHB master 311 = AHB master 4The maximum value of this field that can be read back isDMAH_NUM_MASTER_INT 每 1.Reset Value: DMAH_CHx_SMS[1:0]Dependencies: This field does not exist if the configurationparameter DMAH_CHx_SMS is hardcoded; in this case,the read-back value is always the hardcoded value.*/
        uint32_t                     llp_dst_en :  1; /*27:27, Block chaining is enabled on the destination side only if theLLP_DST_EN field is high and LLPx.LOC is non-zero.Reset Value: 0x0Dependencies: This field does not exist if the configurationparameter DMAH_CHx_MULTI_BLK_EN is not selected or if DMAH_CHx_HC_LLP is selected; in this case, the read-back value is always 0.*/
        uint32_t                     llp_src_en :  1; /*28:28, Block chaining is enabled on the source side only if theLLP_SRC_EN field is high and LLPx.LOC is non-zeroReset Value: 0x0Dependencies: This field does not exist if the configurationparameter DMAH_CHx_MULTI_BLK_EN is not selected or if DMAH_CHx_HC_LLP is selected; in this case, the read-back value is always 0.*/
        uint32_t                     reserved_0 :  3; /*31:29,                       Reserved*/
    } bit_field;
} T_DMAC_CH1_CTRL_L;

//ch1_ctrl_H
typedef union
{
    uint32_t val;
    struct
    {
        uint32_t                       block_ts : 12; /*11: 0, Block Transfer Size.When the DW_ahb_dmac is the flow controller, the user writes thisfield before the channel is enabled in order to indicate the block size.The number programmed into BLOCK_TS indicates the total numberof single transactions to perform for every block transfer; a singletransaction is mapped to a single AMBA beat.Width: The width of the single transaction is determined byCTLx.SRC_TR_WIDTH. For further information on setting this field,refer to Table 6-3 on page 175 and ※Transfer Operation§ on page 61.Once the transfer starts, the read-back value is the total number ofdata items already read from the source peripheral, regardless ofwhat is the flow controller.When the source or destination peripheral is assigned as the flowcontroller, then the maximum block size that can be read backsaturates at DMAH_CHx_MAX_BLK_SIZE, but the actual block sizecan be greater. Refer to Table 8-1 on page 249.b = log2(DMAH_CHx_MAX_BLK_SIZE + 1) + 31Bits 43:b+1 do not exist and return 0 on a read.Reset Value: 0x2*/
        uint32_t                           done :  1; /*12:12, Done bitIf status write-back is enabled, the upper word of the control register,CTLx[63:32], is written to the control register location of the LinkedList Item (LLI) in system memory at the end of the block transfer withthe done bit set.Software can poll the LLI CTLx.DONE bit to see when a blocktransfer is complete. The LLI CTLx.DONE bit should be clearedwhen the linked lists are set up in memory prior to enabling thechannel.LLI accesses are always 32-bit accesses (Hsize = 2) aligned to 32-bitboundaries and cannot be changed or programmed to anything otherthan 32-bit. For more information, refer to ※Multi-Block Transfers§ onpage 247.Reset Value: 0x0*/
        uint32_t                     reserved_0 : 19; /*31:13,                       Reserved*/
    } bit_field;
} T_DMAC_CH1_CTRL_H;

//ch1_cfg_L
typedef union
{
    uint32_t val;
    struct
    {
        uint32_t                     reserved_0 :  5; /* 4: 0,                       Reserved*/
        uint32_t                       ch_prior :  3; /* 7: 5, Channel priority. A priority of 7 is the highest priority, and 0 isthe lowest. This field must be programmed within the followingrange:0: (DMAH_NUM_CHANNELS 每 1)A programmed value outside this range will cause erroneousbehavior.*/
        uint32_t                        ch_susp :  1; /* 8: 8, Channel Suspend. Suspends all DMA data transfers from thesource until this bit is cleared. There is no guarantee that thecurrent transaction will complete. Can also be used in conjunctionwith CFGx.FIFO_EMPTY to cleanly disable a channel withoutlosing any data.0 = Not suspended.1 = Suspend DMA transfer from the source.For more information, refer to ※Disabling a Channel Prior toTransfer Completion§ on page 282.*/
        uint32_t                     fifo_empty :  1; /* 9: 9, Indicates if there is data left in the channel FIFO. Can be used inconjunction with CFGx.CH_SUSP to cleanly disable a channel.For more information, refer to ※Disabling a Channel Prior toTransfer Completion§ on page 282.1 = Channel FIFO empty0 = Channel FIFO not empty*/
        uint32_t                     hs_sel_dst :  1; /*10:10, Destination Software or Hardware Handshaking Select. Thisregister selects which of the handshaking interfaces 每 hardwareor software 每 is active for destination requests on this channel.0 = Hardware handshaking interface. Software-initiatedtransaction requests are ignored.1 = Software handshaking interface. Hardware- initiatedtransaction requests are ignored.If the destination peripheral is memory, then this bit is ignored.*/
        uint32_t                     hs_sel_src :  1; /*11:11, Source Software or Hardware Handshaking Select. Thisregister selects which of the handshaking interfaces 每 hardwareor software 每 is active for source requests on this channel.0 = Hardware handshaking interface. Software-initiatedtransaction requests are ignored.1 = Software handshaking interface. Hardware-initiatedtransaction requests are ignored.If the source peripheral is memory, then this bit is ignored.*/
        uint32_t                      lock_ch_l :  2; /*13:12, Channel Lock Level. Indicates the duration over whichCFGx.LOCK_CH bit applies.00 = Over complete DMA transfer01 = Over complete DMA block transfer1x = Over complete DMA transactionThis field does not exist if the configuration parameterDMAH_CHx_LOCK_EN is set to False (page 128); in this case,the read-back value is always 0.*/
        uint32_t                       lock_b_l :  2; /*15:14, Bus Lock Level. Indicates the duration over whichCFGx.LOCK_B bit applies.00 = Over complete DMA transfer01 = Over complete DMA block transfer1x = Over complete DMA transactionThis field does not exist if the parameter DMAH_CHx_LOCK_ENis set to False (page 128); in this case, the read-back value isalways 0.*/
        uint32_t                        lock_ch :  1; /*16:16, Bus Lock Bit. When active, the AHB bus master signal hlock isasserted for the duration specified in CFGx.LOCK_B_L. For moreinformation, refer to ※Locked DMA Transfers§ on page 103.This field does not exist if the configuration parameterDMAH_CHx_LOCK_EN is set to False (page 128); in this case,the read-back value is always 0.*/
        uint32_t                         lock_b :  1; /*17:17, Bus Lock Bit. When active, the AHB bus master signal hlock isasserted for the duration specified in CFGx.LOCK_B_L. For moreinformation, refer to ※Locked DMA Transfers§ on page 103.This field does not exist if the configuration parameterDMAH_CHx_LOCK_EN is set to False (page 128); in this case,the read-back value is always 0.*/
        uint32_t                     dst_hs_pol :  1; /*18:18, Source Handshaking Interface Polarity.0 = Active high1 = Active lowFor information on this, refer to ※Handshaking Interface§ onpage 42.*/
        uint32_t                     src_hs_pol :  1; /*19:19, Source Handshaking Interface Polarity.0 = Active high1 = Active lowFor information on this, refer to ※Handshaking Interface§ onpage 42.*/
        uint32_t                      max_abrst : 10; /*29:20, Maximum AMBA Burst Length. Maximum AMBA burst lengththat is used for DMA transfers on this channel.A value of 0 indicates that software is not limiting the maximumAMBA burst length for DMA transfers on this channel.This field does not exist if the configuration parameterDMAH_MABRST (page 121) is not selected; in this case, theread-back value is always 0, and the maximum AMBA burstlength cannot be limited by software.*/
        uint32_t                     reload_src :  1; /*30:30, Automatic Source Reload. The SARx register can beautomatically reloaded from its initial value at the end of everyblock for multi-block transfers. A new block transfer is theninitiated. For conditions under which this occurs, refer to Table 8-1on page 249.This field does not exist if the configuration parameterDMAH_CHx_MULTI_BLK_EN (page 130) is not selected; in thiscase, the read-back value is always 0.*/
        uint32_t                     reload_dst :  1; /*31:31, Automatic Destination Reload. The DARx register can beautomatically reloaded from its initial value at the end of everyblock for multi-block transfers. A new block transfer is theninitiated. For conditions under which this occurs, refer to Table 8-1on page 249.This register does not exist if the configuration parameterDMAH_CHx_MULTI_BLK_EN (page 130) is not selected; in thiscase, the read-back value is always 0.*/
    } bit_field;
} T_DMAC_CH1_CFG_L;

//ch1_cfg_H
typedef union
{
    uint32_t val;
    struct
    {
        uint32_t                        fc_mode :  1; /* 0: 0, Flow Control Mode. Determines when source transactionrequests are serviced when the Destination Peripheral is the flowcontroller.0 = Source transaction requests are serviced when they occur.Data pre-fetching is enabled.1 = Source transaction requests are not serviced until adestination transaction request occurs. In this mode, the amountof data transferred from the source is limited so that it isguaranteed to be transferred to the destination prior to blocktermination by the destination. Data pre-fetching is disabled.*/
        uint32_t                      fifo_mode :  1; /* 1: 1, FIFO Mode Select. Determines how much space or data needsto be available in the FIFO before a burst transaction request isserviced.0 = Space/data available for single AHB transfer of the specifiedtransfer width.1 = Data available is greater than or equal to half the FIFO depthfor destination transfers and space available is greater than halfthe fifo depth for source transfers. The exceptions are at the endof a burst transaction request or at the end of a block transfer.*/
        uint32_t                        protctl :  3; /* 4: 2, Protection Control bits used to drive the AHB HPROT[3:1] bus.The AMBA Specification recommends that the default value ofHPROT indicates a non-cached, non-buffered, privileged dataaccess. The reset value is used to indicate such an access.HPROT[0] is tied high because all transfers are data accesses,as there are no opcode fetches.There is a one-to-one mapping of these register bits to theHPROT[3:1] master interface signals. Table 6-5 on page 185shows the mapping of bits in this field to the AHB HPROT[3:1]bus.*/
        uint32_t                      ds_upd_en :  1; /* 5: 5, Destination Status Update Enable. Destination statusinformation is fetched only from the location pointed to by theDSTATARx register, stored in the DSTATx register and written outto the DSTATx location of the LLI (refer to Figure 8-3 on page250) if DS_UPD_EN is high.This field does not exist if the configuration parameterDMAH_CHx_STAT_DST is set to False; in this case, theread-back value is always 0.*/
        uint32_t                      ss_upd_en :  1; /* 6: 6, Source Status Update Enable. Source status information isfetched only from the location pointed to by the SSTATARxregister, stored in the SSTATx register and written out to theSSTATx location of the LLI (refer to Figure 8-3 on page 250) ifSS_UPD_EN is high.NOTE: This enable is applicable only if DMAH_CHx_STAT_SRC(page 132) is set to True.This field does not exist if the configuration parameterDMAH_CHx_STAT_SRC is set to False; in this case, theread-back value is always 0.*/
        uint32_t                        src_per :  4; /*10: 7, Assigns a hardware handshaking interface(0 - DMAH_NUM_HS_INT-1) to the source of channel x if theCFGx.HS_SEL_SRC field is 0; otherwise, this field is ignored.The channel can then communicate with the source peripheralconnected to that interface through the assigned hardwarehandshaking interface.NOTE1: For correct DW_ahb_dmac operation, only oneperipheral (source or destination) should be assigned to thesame handshaking interface.NOTE2: This field does not exist if the configuration parameterDMAH_NUM_HS_INT is set to 0.NOTE3:← b = 39 if DMAH_NUM_HS_INT is 1← b = ceil(log2(DMAH_NUM_HS_INT)) + 38 ifDMAH_NUM_HS_INT is greater than 1Bits 42:(b+1) do not exist and return 0 on a read.*/
        uint32_t                       dest_per :  4; /*14:11, Assigns a hardware handshaking interface(0 - DMAH_NUM_HS_INT-1) to the destination of channel x if theCFGx.HS_SEL_DST field is 0; otherwise, this field is ignored.The channel can then communicate with the destinationperipheral connected to that interface through the assignedhardware handshaking interface.NOTE1: For correct DMA operation, only one peripheral (sourceor destination) should be assigned to the same handshakinginterface.NOTE2: This field does not exist if the configuration parameterDMAH_NUM_HS_INT is set to 0.NOTE3:← b = 43 if DMAH_NUM_HS_INT is 1← b = ceil(log2(DMAH_NUM_HS_INT)) + 42 ifDMAH_NUM_HS_INT is greater than 1Bits 46:(b+1) do not exist and return 0 on a read.*/
        uint32_t                     reserved_0 : 17; /*31:15,                       Reserved*/
    } bit_field;
} T_DMAC_CH1_CFG_H;

//ch2_sar_L
typedef union
{
    uint32_t val;
    struct
    {
        uint32_t                            sar : 32; /*31: 0, Current Source Address of DMA transfer. Updated after each sourcetransfer. The SINC field in the CTLx register determines whether the address increments, decrements, or is left unchanged on every source transfer throughout the block transfer.*/
    } bit_field;
} T_DMAC_CH2_SAR_L;

//ch2_sar_H
typedef union
{
    uint32_t val;
    struct
    {
        uint32_t                     reserved_0 : 32; /*31: 0,                       Reserved*/
    } bit_field;
} T_DMAC_CH2_SAR_H;

//ch2_dar_L
typedef union
{
    uint32_t val;
    struct
    {
        uint32_t                            dar : 32; /*31: 0, Current Destination address of DMA transfer. Updated after eachdestination transfer. The DINC field in the CTLx register determines whetherthe address increments, decrements, or is left unchanged on everydestination transfer throughout the block transfer.*/
    } bit_field;
} T_DMAC_CH2_DAR_L;

//ch2_dar_H
typedef union
{
    uint32_t val;
    struct
    {
        uint32_t                     reserved_0 : 32; /*31: 0,                       Reserved*/
    } bit_field;
} T_DMAC_CH2_DAR_H;

//ch2_ctrl_L
typedef union
{
    uint32_t val;
    struct
    {
        uint32_t                         int_en :  1; /* 0: 0, Interrupt Enable Bit. If set, then all interrupt-generating sources areenabled. Functions as a global mask bit for all interrupts for thechannel; raw* interrupt registers still assert if CTLx.INT_EN = 0.Reset Value: 0x1*/
        uint32_t                   dst_tr_width :  3; /* 3: 1, Destination Transfer Width. Table 6-3 on page 175 lists thedecoding for this field. Mapped to AHB bus ※hsize.§ For anon-memory peripheral, typically rgw peripheral (destination) FIFOwidth.This value must be less than or equal to DMAH_Mk_HDATA_WIDTH,where k is the AHB layer 1 to 4 where the destination resides.Reset Value: Encoded value; refer to Table 6-3 on page 175.Dependencies: This field does not exist if DMAH_CHx_DTW(page 129) is hardcoded. In this case, the read-back value is alwaysthe hardcoded destination transfer width, DMAH_CHx_DTW.*/
        uint32_t                   src_tr_width :  3; /* 6: 4, Source Transfer Width. Table 6-3 on page 175 lists the decoding forthis field.Mapped to AHB bus ※hsize.§ For a non-memory peripheral, typicallythe peripheral (source) FIFO width.This value must be less than or equal to DMAH_Mx_HDATA_WIDTH,where x is the AHB layer 1 to 4 where the source resides.Reset Value: Encoded value; refer to Table 6-3 on page 175.Dependencies: This field does not exist if the parameterDMAH_CHx_STW (page 129) is hardcoded. In this case, theread-back value is always the hardcoded source transfer width,DMAH_CHx_STW.*/
        uint32_t                           dinc :  2; /* 8: 7, Destination Address Increment. Indicates whether to increment ordecrement the destination address on every destination transfer. Ifyour device is writing data to a destination peripheral FIFO with afixed address, then set this field to ※No change.§00 = Increment01 = Decrement1x = No changeNOTE: Incrementing or decrementing is done for alignment to thenext CTLx.DST_TR_WIDTH boundary.Reset Value: 0x0*/
        uint32_t                           sinc :  2; /*10: 9, Source Address Increment. Indicates whether to increment ordecrement the source address on every source transfer. If the deviceis fetching data from a source peripheral FIFO with a fixed address,then set this field to ※No change.§00 = Increment01 = Decrement1x = No changeNOTE: Incrementing or decrementing is done for alignment to thenext CTLx.SRC_TR_WIDTH boundary.Reset Value: 0x0*/
        uint32_t                     dest_msize :  3; /*13:11, Destination Burst Transaction Length. Number of data items, eachof width CTLx.DST_TR_WIDTH, to be written to the destinationevery time a destination burst transaction request is made from eitherthe corresponding hardware or software handshaking interface.NOTE: This value is not related to the AHB bus master HBURSTbus.Reset Value: 0x1Dependencies: DMAH_CHx_MAX_MULT_SIZE determines the bit width of this field. All surplus bits in this field do not exist and read back as 0.*/
        uint32_t                      src_msize :  3; /*16:14, Source Burst Transaction Length. Number of data items, each ofwidth CTLx.SRC_TR_WIDTH, to be read from the source every timea source burst transaction request is made from either thecorresponding hardware or software handshaking interface.Table 6-2 on page 174 lists the decoding for this field; refer to※Choosing the Receive Watermark level§ on page 101.NOTE: This value is not related to the AHB bus master HBURSTbus.Reset Value: 0x1Dependencies: The configuration parameter DMAH_CHx_MAX_MULT_SIZE determines the bit widthof this field. All remaining bits in this field do not exist and read backas 0.*/
        uint32_t                  src_gather_en :  1; /*17:17, Source gather enable bit:0 = Gather disabled1 = Gather enabledGather on the source side is applicable only when the CTLx.SINC bitindicates an incrementing or decrementing address control.Reset Value: 0x0Dependencies: This field does not exist ifDMAH_CHx_SRC_GAT_EN is not selected; in this case, the read-back value is always 0.*/
        uint32_t                 dst_scatter_en :  1; /*18:18, Destination scatter enable bit:0 = Scatter disabled1 = Scatter enabledScatter on the destination side is applicable only when theCTLx.DINC bit indicates an incrementing or decrementing addresscontrol.Reset Value: 0x0Dependencies: This field does not exist if DMAH_CHx_DST_SCA_EN is not selected; in this case, the read-back value is always 0.*/
        uint32_t                     reserved_1 :  1; /*19:19,                       Reserved*/
        uint32_t                          tt_fc :  2; /*21:20, Transfer Type and Flow Control. The following transfer types aresupported.? Memory to Memory? Memory to Peripheral? Peripheral to Memory? Peripheral to PeripheralFlow Control can be assigned to the DW_ahb_dmac, the sourceperipheral, or the destination peripheral. Reset Value: Configuration dependent:TT_FC[0] = 1＊b1TT_FC[1] = DMAH_CHx_FC[1] || (!DMAH_CHx_FC[0])TT_FC[2] = DMAH_CHx_FC[1] ^ DMAH_CHx_FC[0]Dependencies: If the configuration parameter DMAH_CHx_FCis set to DMA_FC_ONLY, then TT_FC[2] does not existand TT_FC[2] always reads back 0.If DMAH_CHx_FC is set to SRC_FC_ONLY, then TT_FC[2:1] doesnot exist and TT_FC[2:1] always reads back 2＊b10.If DMAH_CHx_FC is set to DST_FC_ONLY, then TT_FC[2:1] doesnot exist and TT_FC[2:1] always reads back 2＊b11.For multi-block transfers using linked list operation, TT_FC must beconstant for all blocks of this multi-block transfer.*/
        uint32_t                            dms :  3; /*24:22, Destination Master Select. Identifies the Master Interface layerwhere the destination device (peripheral or memory) resides.00 = AHB master 101 = AHB master 210 = AHB master 311 = AHB master 4The maximum value of this field that can be read back isDMAH_NUM_MASTER_INT 每 1.Reset Value: DMAH_CHx_DMS[1:0]Dependencies: This field does not exist if the configurationparameter DMAH_CHx_DMS (page 129) is hardcoded; in this case,the read-back value is always the hardcoded value.*/
        uint32_t                            sms :  2; /*26:25, Source Master Select. Identifies the Master Interface layer fromwhich the source device (peripheral or memory) is accessed.00 = AHB master 101 = AHB master 210 = AHB master 311 = AHB master 4The maximum value of this field that can be read back isDMAH_NUM_MASTER_INT 每 1.Reset Value: DMAH_CHx_SMS[1:0]Dependencies: This field does not exist if the configurationparameter DMAH_CHx_SMS is hardcoded; in this case,the read-back value is always the hardcoded value.*/
        uint32_t                     llp_dst_en :  1; /*27:27, Block chaining is enabled on the destination side only if theLLP_DST_EN field is high and LLPx.LOC is non-zero.Reset Value: 0x0Dependencies: This field does not exist if the configurationparameter DMAH_CHx_MULTI_BLK_EN is not selected or if DMAH_CHx_HC_LLP is selected; in this case, the read-back value is always 0.*/
        uint32_t                     llp_src_en :  1; /*28:28, Block chaining is enabled on the source side only if theLLP_SRC_EN field is high and LLPx.LOC is non-zeroReset Value: 0x0Dependencies: This field does not exist if the configurationparameter DMAH_CHx_MULTI_BLK_EN is not selected or if DMAH_CHx_HC_LLP is selected; in this case, the read-back value is always 0.*/
        uint32_t                     reserved_0 :  3; /*31:29,                       Reserved*/
    } bit_field;
} T_DMAC_CH2_CTRL_L;

//ch2_ctrl_H
typedef union
{
    uint32_t val;
    struct
    {
        uint32_t                       block_ts : 12; /*11: 0, Block Transfer Size.When the DW_ahb_dmac is the flow controller, the user writes thisfield before the channel is enabled in order to indicate the block size.The number programmed into BLOCK_TS indicates the total numberof single transactions to perform for every block transfer; a singletransaction is mapped to a single AMBA beat.Width: The width of the single transaction is determined byCTLx.SRC_TR_WIDTH. For further information on setting this field,refer to Table 6-3 on page 175 and ※Transfer Operation§ on page 61.Once the transfer starts, the read-back value is the total number ofdata items already read from the source peripheral, regardless ofwhat is the flow controller.When the source or destination peripheral is assigned as the flowcontroller, then the maximum block size that can be read backsaturates at DMAH_CHx_MAX_BLK_SIZE, but the actual block sizecan be greater. Refer to Table 8-1 on page 249.b = log2(DMAH_CHx_MAX_BLK_SIZE + 1) + 31Bits 43:b+1 do not exist and return 0 on a read.Reset Value: 0x2*/
        uint32_t                           done :  1; /*12:12, Done bitIf status write-back is enabled, the upper word of the control register,CTLx[63:32], is written to the control register location of the LinkedList Item (LLI) in system memory at the end of the block transfer withthe done bit set.Software can poll the LLI CTLx.DONE bit to see when a blocktransfer is complete. The LLI CTLx.DONE bit should be clearedwhen the linked lists are set up in memory prior to enabling thechannel.LLI accesses are always 32-bit accesses (Hsize = 2) aligned to 32-bitboundaries and cannot be changed or programmed to anything otherthan 32-bit. For more information, refer to ※Multi-Block Transfers§ onpage 247.Reset Value: 0x0*/
        uint32_t                     reserved_0 : 19; /*31:13,                       Reserved*/
    } bit_field;
} T_DMAC_CH2_CTRL_H;

//ch2_cfg_L
typedef union
{
    uint32_t val;
    struct
    {
        uint32_t                     reserved_0 :  5; /* 4: 0,                       Reserved*/
        uint32_t                       ch_prior :  3; /* 7: 5, Channel priority. A priority of 7 is the highest priority, and 0 isthe lowest. This field must be programmed within the followingrange:0: (DMAH_NUM_CHANNELS 每 1)A programmed value outside this range will cause erroneousbehavior.*/
        uint32_t                        ch_susp :  1; /* 8: 8, Channel Suspend. Suspends all DMA data transfers from thesource until this bit is cleared. There is no guarantee that thecurrent transaction will complete. Can also be used in conjunctionwith CFGx.FIFO_EMPTY to cleanly disable a channel withoutlosing any data.0 = Not suspended.1 = Suspend DMA transfer from the source.For more information, refer to ※Disabling a Channel Prior toTransfer Completion§ on page 282.*/
        uint32_t                     fifo_empty :  1; /* 9: 9, Indicates if there is data left in the channel FIFO. Can be used inconjunction with CFGx.CH_SUSP to cleanly disable a channel.For more information, refer to ※Disabling a Channel Prior toTransfer Completion§ on page 282.1 = Channel FIFO empty0 = Channel FIFO not empty*/
        uint32_t                     hs_sel_dst :  1; /*10:10, Destination Software or Hardware Handshaking Select. Thisregister selects which of the handshaking interfaces 每 hardwareor software 每 is active for destination requests on this channel.0 = Hardware handshaking interface. Software-initiatedtransaction requests are ignored.1 = Software handshaking interface. Hardware- initiatedtransaction requests are ignored.If the destination peripheral is memory, then this bit is ignored.*/
        uint32_t                     hs_sel_src :  1; /*11:11, Source Software or Hardware Handshaking Select. Thisregister selects which of the handshaking interfaces 每 hardwareor software 每 is active for source requests on this channel.0 = Hardware handshaking interface. Software-initiatedtransaction requests are ignored.1 = Software handshaking interface. Hardware-initiatedtransaction requests are ignored.If the source peripheral is memory, then this bit is ignored.*/
        uint32_t                      lock_ch_l :  2; /*13:12, Channel Lock Level. Indicates the duration over whichCFGx.LOCK_CH bit applies.00 = Over complete DMA transfer01 = Over complete DMA block transfer1x = Over complete DMA transactionThis field does not exist if the configuration parameterDMAH_CHx_LOCK_EN is set to False (page 128); in this case,the read-back value is always 0.*/
        uint32_t                       lock_b_l :  2; /*15:14, Bus Lock Level. Indicates the duration over whichCFGx.LOCK_B bit applies.00 = Over complete DMA transfer01 = Over complete DMA block transfer1x = Over complete DMA transactionThis field does not exist if the parameter DMAH_CHx_LOCK_ENis set to False (page 128); in this case, the read-back value isalways 0.*/
        uint32_t                        lock_ch :  1; /*16:16, Bus Lock Bit. When active, the AHB bus master signal hlock isasserted for the duration specified in CFGx.LOCK_B_L. For moreinformation, refer to ※Locked DMA Transfers§ on page 103.This field does not exist if the configuration parameterDMAH_CHx_LOCK_EN is set to False (page 128); in this case,the read-back value is always 0.*/
        uint32_t                         lock_b :  1; /*17:17, Bus Lock Bit. When active, the AHB bus master signal hlock isasserted for the duration specified in CFGx.LOCK_B_L. For moreinformation, refer to ※Locked DMA Transfers§ on page 103.This field does not exist if the configuration parameterDMAH_CHx_LOCK_EN is set to False (page 128); in this case,the read-back value is always 0.*/
        uint32_t                     dst_hs_pol :  1; /*18:18, Source Handshaking Interface Polarity.0 = Active high1 = Active lowFor information on this, refer to ※Handshaking Interface§ onpage 42.*/
        uint32_t                     src_hs_pol :  1; /*19:19, Source Handshaking Interface Polarity.0 = Active high1 = Active lowFor information on this, refer to ※Handshaking Interface§ onpage 42.*/
        uint32_t                      max_abrst : 10; /*29:20, Maximum AMBA Burst Length. Maximum AMBA burst lengththat is used for DMA transfers on this channel.A value of 0 indicates that software is not limiting the maximumAMBA burst length for DMA transfers on this channel.This field does not exist if the configuration parameterDMAH_MABRST (page 121) is not selected; in this case, theread-back value is always 0, and the maximum AMBA burstlength cannot be limited by software.*/
        uint32_t                     reload_src :  1; /*30:30, Automatic Source Reload. The SARx register can beautomatically reloaded from its initial value at the end of everyblock for multi-block transfers. A new block transfer is theninitiated. For conditions under which this occurs, refer to Table 8-1on page 249.This field does not exist if the configuration parameterDMAH_CHx_MULTI_BLK_EN (page 130) is not selected; in thiscase, the read-back value is always 0.*/
        uint32_t                     reload_dst :  1; /*31:31, Automatic Destination Reload. The DARx register can beautomatically reloaded from its initial value at the end of everyblock for multi-block transfers. A new block transfer is theninitiated. For conditions under which this occurs, refer to Table 8-1on page 249.This register does not exist if the configuration parameterDMAH_CHx_MULTI_BLK_EN (page 130) is not selected; in thiscase, the read-back value is always 0.*/
    } bit_field;
} T_DMAC_CH2_CFG_L;

//ch2_cfg_H
typedef union
{
    uint32_t val;
    struct
    {
        uint32_t                        fc_mode :  1; /* 0: 0, Flow Control Mode. Determines when source transactionrequests are serviced when the Destination Peripheral is the flowcontroller.0 = Source transaction requests are serviced when they occur.Data pre-fetching is enabled.1 = Source transaction requests are not serviced until adestination transaction request occurs. In this mode, the amountof data transferred from the source is limited so that it isguaranteed to be transferred to the destination prior to blocktermination by the destination. Data pre-fetching is disabled.*/
        uint32_t                      fifo_mode :  1; /* 1: 1, FIFO Mode Select. Determines how much space or data needsto be available in the FIFO before a burst transaction request isserviced.0 = Space/data available for single AHB transfer of the specifiedtransfer width.1 = Data available is greater than or equal to half the FIFO depthfor destination transfers and space available is greater than halfthe fifo depth for source transfers. The exceptions are at the endof a burst transaction request or at the end of a block transfer.*/
        uint32_t                        protctl :  3; /* 4: 2, Protection Control bits used to drive the AHB HPROT[3:1] bus.The AMBA Specification recommends that the default value ofHPROT indicates a non-cached, non-buffered, privileged dataaccess. The reset value is used to indicate such an access.HPROT[0] is tied high because all transfers are data accesses,as there are no opcode fetches.There is a one-to-one mapping of these register bits to theHPROT[3:1] master interface signals. Table 6-5 on page 185shows the mapping of bits in this field to the AHB HPROT[3:1]bus.*/
        uint32_t                      ds_upd_en :  1; /* 5: 5, Destination Status Update Enable. Destination statusinformation is fetched only from the location pointed to by theDSTATARx register, stored in the DSTATx register and written outto the DSTATx location of the LLI (refer to Figure 8-3 on page250) if DS_UPD_EN is high.This field does not exist if the configuration parameterDMAH_CHx_STAT_DST is set to False; in this case, theread-back value is always 0.*/
        uint32_t                      ss_upd_en :  1; /* 6: 6, Source Status Update Enable. Source status information isfetched only from the location pointed to by the SSTATARxregister, stored in the SSTATx register and written out to theSSTATx location of the LLI (refer to Figure 8-3 on page 250) ifSS_UPD_EN is high.NOTE: This enable is applicable only if DMAH_CHx_STAT_SRC(page 132) is set to True.This field does not exist if the configuration parameterDMAH_CHx_STAT_SRC is set to False; in this case, theread-back value is always 0.*/
        uint32_t                        src_per :  4; /*10: 7, Assigns a hardware handshaking interface(0 - DMAH_NUM_HS_INT-1) to the source of channel x if theCFGx.HS_SEL_SRC field is 0; otherwise, this field is ignored.The channel can then communicate with the source peripheralconnected to that interface through the assigned hardwarehandshaking interface.NOTE1: For correct DW_ahb_dmac operation, only oneperipheral (source or destination) should be assigned to thesame handshaking interface.NOTE2: This field does not exist if the configuration parameterDMAH_NUM_HS_INT is set to 0.NOTE3:← b = 39 if DMAH_NUM_HS_INT is 1← b = ceil(log2(DMAH_NUM_HS_INT)) + 38 ifDMAH_NUM_HS_INT is greater than 1Bits 42:(b+1) do not exist and return 0 on a read.*/
        uint32_t                       dest_per :  4; /*14:11, Assigns a hardware handshaking interface(0 - DMAH_NUM_HS_INT-1) to the destination of channel x if theCFGx.HS_SEL_DST field is 0; otherwise, this field is ignored.The channel can then communicate with the destinationperipheral connected to that interface through the assignedhardware handshaking interface.NOTE1: For correct DMA operation, only one peripheral (sourceor destination) should be assigned to the same handshakinginterface.NOTE2: This field does not exist if the configuration parameterDMAH_NUM_HS_INT is set to 0.NOTE3:← b = 43 if DMAH_NUM_HS_INT is 1← b = ceil(log2(DMAH_NUM_HS_INT)) + 42 ifDMAH_NUM_HS_INT is greater than 1Bits 46:(b+1) do not exist and return 0 on a read.*/
        uint32_t                     reserved_0 : 17; /*31:15,                       Reserved*/
    } bit_field;
} T_DMAC_CH2_CFG_H;

//ch3_sar_L
typedef union
{
    uint32_t val;
    struct
    {
        uint32_t                            sar : 32; /*31: 0, Current Source Address of DMA transfer. Updated after each sourcetransfer. The SINC field in the CTLx register determines whether the address increments, decrements, or is left unchanged on every source transfer throughout the block transfer.*/
    } bit_field;
} T_DMAC_CH3_SAR_L;

//ch3_sar_H
typedef union
{
    uint32_t val;
    struct
    {
        uint32_t                     reserved_0 : 32; /*31: 0,                       Reserved*/
    } bit_field;
} T_DMAC_CH3_SAR_H;

//ch3_dar_L
typedef union
{
    uint32_t val;
    struct
    {
        uint32_t                            dar : 32; /*31: 0, Current Destination address of DMA transfer. Updated after eachdestination transfer. The DINC field in the CTLx register determines whetherthe address increments, decrements, or is left unchanged on everydestination transfer throughout the block transfer.*/
    } bit_field;
} T_DMAC_CH3_DAR_L;

//ch3_dar_H
typedef union
{
    uint32_t val;
    struct
    {
        uint32_t                     reserved_0 : 32; /*31: 0,                       Reserved*/
    } bit_field;
} T_DMAC_CH3_DAR_H;

//ch3_ctrl_L
typedef union
{
    uint32_t val;
    struct
    {
        uint32_t                         int_en :  1; /* 0: 0, Interrupt Enable Bit. If set, then all interrupt-generating sources areenabled. Functions as a global mask bit for all interrupts for thechannel; raw* interrupt registers still assert if CTLx.INT_EN = 0.Reset Value: 0x1*/
        uint32_t                   dst_tr_width :  3; /* 3: 1, Destination Transfer Width. Table 6-3 on page 175 lists thedecoding for this field. Mapped to AHB bus ※hsize.§ For anon-memory peripheral, typically rgw peripheral (destination) FIFOwidth.This value must be less than or equal to DMAH_Mk_HDATA_WIDTH,where k is the AHB layer 1 to 4 where the destination resides.Reset Value: Encoded value; refer to Table 6-3 on page 175.Dependencies: This field does not exist if DMAH_CHx_DTW(page 129) is hardcoded. In this case, the read-back value is alwaysthe hardcoded destination transfer width, DMAH_CHx_DTW.*/
        uint32_t                   src_tr_width :  3; /* 6: 4, Source Transfer Width. Table 6-3 on page 175 lists the decoding forthis field.Mapped to AHB bus ※hsize.§ For a non-memory peripheral, typicallythe peripheral (source) FIFO width.This value must be less than or equal to DMAH_Mx_HDATA_WIDTH,where x is the AHB layer 1 to 4 where the source resides.Reset Value: Encoded value; refer to Table 6-3 on page 175.Dependencies: This field does not exist if the parameterDMAH_CHx_STW (page 129) is hardcoded. In this case, theread-back value is always the hardcoded source transfer width,DMAH_CHx_STW.*/
        uint32_t                           dinc :  2; /* 8: 7, Destination Address Increment. Indicates whether to increment ordecrement the destination address on every destination transfer. Ifyour device is writing data to a destination peripheral FIFO with afixed address, then set this field to ※No change.§00 = Increment01 = Decrement1x = No changeNOTE: Incrementing or decrementing is done for alignment to thenext CTLx.DST_TR_WIDTH boundary.Reset Value: 0x0*/
        uint32_t                           sinc :  2; /*10: 9, Source Address Increment. Indicates whether to increment ordecrement the source address on every source transfer. If the deviceis fetching data from a source peripheral FIFO with a fixed address,then set this field to ※No change.§00 = Increment01 = Decrement1x = No changeNOTE: Incrementing or decrementing is done for alignment to thenext CTLx.SRC_TR_WIDTH boundary.Reset Value: 0x0*/
        uint32_t                     dest_msize :  3; /*13:11, Destination Burst Transaction Length. Number of data items, eachof width CTLx.DST_TR_WIDTH, to be written to the destinationevery time a destination burst transaction request is made from eitherthe corresponding hardware or software handshaking interface.NOTE: This value is not related to the AHB bus master HBURSTbus.Reset Value: 0x1Dependencies: DMAH_CHx_MAX_MULT_SIZE determines the bit width of this field. All surplus bits in this field do not exist and read back as 0.*/
        uint32_t                      src_msize :  3; /*16:14, Source Burst Transaction Length. Number of data items, each ofwidth CTLx.SRC_TR_WIDTH, to be read from the source every timea source burst transaction request is made from either thecorresponding hardware or software handshaking interface.Table 6-2 on page 174 lists the decoding for this field; refer to※Choosing the Receive Watermark level§ on page 101.NOTE: This value is not related to the AHB bus master HBURSTbus.Reset Value: 0x1Dependencies: The configuration parameter DMAH_CHx_MAX_MULT_SIZE determines the bit widthof this field. All remaining bits in this field do not exist and read backas 0.*/
        uint32_t                  src_gather_en :  1; /*17:17, Source gather enable bit:0 = Gather disabled1 = Gather enabledGather on the source side is applicable only when the CTLx.SINC bitindicates an incrementing or decrementing address control.Reset Value: 0x0Dependencies: This field does not exist ifDMAH_CHx_SRC_GAT_EN is not selected; in this case, the read-back value is always 0.*/
        uint32_t                 dst_scatter_en :  1; /*18:18, Destination scatter enable bit:0 = Scatter disabled1 = Scatter enabledScatter on the destination side is applicable only when theCTLx.DINC bit indicates an incrementing or decrementing addresscontrol.Reset Value: 0x0Dependencies: This field does not exist if DMAH_CHx_DST_SCA_EN is not selected; in this case, the read-back value is always 0.*/
        uint32_t                     reserved_1 :  1; /*19:19,                       Reserved*/
        uint32_t                          tt_fc :  2; /*21:20, Transfer Type and Flow Control. The following transfer types aresupported.? Memory to Memory? Memory to Peripheral? Peripheral to Memory? Peripheral to PeripheralFlow Control can be assigned to the DW_ahb_dmac, the sourceperipheral, or the destination peripheral. Reset Value: Configuration dependent:TT_FC[0] = 1＊b1TT_FC[1] = DMAH_CHx_FC[1] || (!DMAH_CHx_FC[0])TT_FC[2] = DMAH_CHx_FC[1] ^ DMAH_CHx_FC[0]Dependencies: If the configuration parameter DMAH_CHx_FCis set to DMA_FC_ONLY, then TT_FC[2] does not existand TT_FC[2] always reads back 0.If DMAH_CHx_FC is set to SRC_FC_ONLY, then TT_FC[2:1] doesnot exist and TT_FC[2:1] always reads back 2＊b10.If DMAH_CHx_FC is set to DST_FC_ONLY, then TT_FC[2:1] doesnot exist and TT_FC[2:1] always reads back 2＊b11.For multi-block transfers using linked list operation, TT_FC must beconstant for all blocks of this multi-block transfer.*/
        uint32_t                            dms :  3; /*24:22, Destination Master Select. Identifies the Master Interface layerwhere the destination device (peripheral or memory) resides.00 = AHB master 101 = AHB master 210 = AHB master 311 = AHB master 4The maximum value of this field that can be read back isDMAH_NUM_MASTER_INT 每 1.Reset Value: DMAH_CHx_DMS[1:0]Dependencies: This field does not exist if the configurationparameter DMAH_CHx_DMS (page 129) is hardcoded; in this case,the read-back value is always the hardcoded value.*/
        uint32_t                            sms :  2; /*26:25, Source Master Select. Identifies the Master Interface layer fromwhich the source device (peripheral or memory) is accessed.00 = AHB master 101 = AHB master 210 = AHB master 311 = AHB master 4The maximum value of this field that can be read back isDMAH_NUM_MASTER_INT 每 1.Reset Value: DMAH_CHx_SMS[1:0]Dependencies: This field does not exist if the configurationparameter DMAH_CHx_SMS is hardcoded; in this case,the read-back value is always the hardcoded value.*/
        uint32_t                     llp_dst_en :  1; /*27:27, Block chaining is enabled on the destination side only if theLLP_DST_EN field is high and LLPx.LOC is non-zero.Reset Value: 0x0Dependencies: This field does not exist if the configurationparameter DMAH_CHx_MULTI_BLK_EN is not selected or if DMAH_CHx_HC_LLP is selected; in this case, the read-back value is always 0.*/
        uint32_t                     llp_src_en :  1; /*28:28, Block chaining is enabled on the source side only if theLLP_SRC_EN field is high and LLPx.LOC is non-zeroReset Value: 0x0Dependencies: This field does not exist if the configurationparameter DMAH_CHx_MULTI_BLK_EN is not selected or if DMAH_CHx_HC_LLP is selected; in this case, the read-back value is always 0.*/
        uint32_t                     reserved_0 :  3; /*31:29,                       Reserved*/
    } bit_field;
} T_DMAC_CH3_CTRL_L;

//ch3_ctrl_H
typedef union
{
    uint32_t val;
    struct
    {
        uint32_t                       block_ts : 12; /*11: 0, Block Transfer Size.When the DW_ahb_dmac is the flow controller, the user writes thisfield before the channel is enabled in order to indicate the block size.The number programmed into BLOCK_TS indicates the total numberof single transactions to perform for every block transfer; a singletransaction is mapped to a single AMBA beat.Width: The width of the single transaction is determined byCTLx.SRC_TR_WIDTH. For further information on setting this field,refer to Table 6-3 on page 175 and ※Transfer Operation§ on page 61.Once the transfer starts, the read-back value is the total number ofdata items already read from the source peripheral, regardless ofwhat is the flow controller.When the source or destination peripheral is assigned as the flowcontroller, then the maximum block size that can be read backsaturates at DMAH_CHx_MAX_BLK_SIZE, but the actual block sizecan be greater. Refer to Table 8-1 on page 249.b = log2(DMAH_CHx_MAX_BLK_SIZE + 1) + 31Bits 43:b+1 do not exist and return 0 on a read.Reset Value: 0x2*/
        uint32_t                           done :  1; /*12:12, Done bitIf status write-back is enabled, the upper word of the control register,CTLx[63:32], is written to the control register location of the LinkedList Item (LLI) in system memory at the end of the block transfer withthe done bit set.Software can poll the LLI CTLx.DONE bit to see when a blocktransfer is complete. The LLI CTLx.DONE bit should be clearedwhen the linked lists are set up in memory prior to enabling thechannel.LLI accesses are always 32-bit accesses (Hsize = 2) aligned to 32-bitboundaries and cannot be changed or programmed to anything otherthan 32-bit. For more information, refer to ※Multi-Block Transfers§ onpage 247.Reset Value: 0x0*/
        uint32_t                     reserved_0 : 19; /*31:13,                       Reserved*/
    } bit_field;
} T_DMAC_CH3_CTRL_H;

//ch3_cfg_L
typedef union
{
    uint32_t val;
    struct
    {
        uint32_t                     reserved_0 :  5; /* 4: 0,                       Reserved*/
        uint32_t                       ch_prior :  3; /* 7: 5, Channel priority. A priority of 7 is the highest priority, and 0 isthe lowest. This field must be programmed within the followingrange:0: (DMAH_NUM_CHANNELS 每 1)A programmed value outside this range will cause erroneousbehavior.*/
        uint32_t                        ch_susp :  1; /* 8: 8, Channel Suspend. Suspends all DMA data transfers from thesource until this bit is cleared. There is no guarantee that thecurrent transaction will complete. Can also be used in conjunctionwith CFGx.FIFO_EMPTY to cleanly disable a channel withoutlosing any data.0 = Not suspended.1 = Suspend DMA transfer from the source.For more information, refer to ※Disabling a Channel Prior toTransfer Completion§ on page 282.*/
        uint32_t                     fifo_empty :  1; /* 9: 9, Indicates if there is data left in the channel FIFO. Can be used inconjunction with CFGx.CH_SUSP to cleanly disable a channel.For more information, refer to ※Disabling a Channel Prior toTransfer Completion§ on page 282.1 = Channel FIFO empty0 = Channel FIFO not empty*/
        uint32_t                     hs_sel_dst :  1; /*10:10, Destination Software or Hardware Handshaking Select. Thisregister selects which of the handshaking interfaces 每 hardwareor software 每 is active for destination requests on this channel.0 = Hardware handshaking interface. Software-initiatedtransaction requests are ignored.1 = Software handshaking interface. Hardware- initiatedtransaction requests are ignored.If the destination peripheral is memory, then this bit is ignored.*/
        uint32_t                     hs_sel_src :  1; /*11:11, Source Software or Hardware Handshaking Select. Thisregister selects which of the handshaking interfaces 每 hardwareor software 每 is active for source requests on this channel.0 = Hardware handshaking interface. Software-initiatedtransaction requests are ignored.1 = Software handshaking interface. Hardware-initiatedtransaction requests are ignored.If the source peripheral is memory, then this bit is ignored.*/
        uint32_t                      lock_ch_l :  2; /*13:12, Channel Lock Level. Indicates the duration over whichCFGx.LOCK_CH bit applies.00 = Over complete DMA transfer01 = Over complete DMA block transfer1x = Over complete DMA transactionThis field does not exist if the configuration parameterDMAH_CHx_LOCK_EN is set to False (page 128); in this case,the read-back value is always 0.*/
        uint32_t                       lock_b_l :  2; /*15:14, Bus Lock Level. Indicates the duration over whichCFGx.LOCK_B bit applies.00 = Over complete DMA transfer01 = Over complete DMA block transfer1x = Over complete DMA transactionThis field does not exist if the parameter DMAH_CHx_LOCK_ENis set to False (page 128); in this case, the read-back value isalways 0.*/
        uint32_t                        lock_ch :  1; /*16:16, Bus Lock Bit. When active, the AHB bus master signal hlock isasserted for the duration specified in CFGx.LOCK_B_L. For moreinformation, refer to ※Locked DMA Transfers§ on page 103.This field does not exist if the configuration parameterDMAH_CHx_LOCK_EN is set to False (page 128); in this case,the read-back value is always 0.*/
        uint32_t                         lock_b :  1; /*17:17, Bus Lock Bit. When active, the AHB bus master signal hlock isasserted for the duration specified in CFGx.LOCK_B_L. For moreinformation, refer to ※Locked DMA Transfers§ on page 103.This field does not exist if the configuration parameterDMAH_CHx_LOCK_EN is set to False (page 128); in this case,the read-back value is always 0.*/
        uint32_t                     dst_hs_pol :  1; /*18:18, Source Handshaking Interface Polarity.0 = Active high1 = Active lowFor information on this, refer to ※Handshaking Interface§ onpage 42.*/
        uint32_t                     src_hs_pol :  1; /*19:19, Source Handshaking Interface Polarity.0 = Active high1 = Active lowFor information on this, refer to ※Handshaking Interface§ onpage 42.*/
        uint32_t                      max_abrst : 10; /*29:20, Maximum AMBA Burst Length. Maximum AMBA burst lengththat is used for DMA transfers on this channel.A value of 0 indicates that software is not limiting the maximumAMBA burst length for DMA transfers on this channel.This field does not exist if the configuration parameterDMAH_MABRST (page 121) is not selected; in this case, theread-back value is always 0, and the maximum AMBA burstlength cannot be limited by software.*/
        uint32_t                     reload_src :  1; /*30:30, Automatic Source Reload. The SARx register can beautomatically reloaded from its initial value at the end of everyblock for multi-block transfers. A new block transfer is theninitiated. For conditions under which this occurs, refer to Table 8-1on page 249.This field does not exist if the configuration parameterDMAH_CHx_MULTI_BLK_EN (page 130) is not selected; in thiscase, the read-back value is always 0.*/
        uint32_t                     reload_dst :  1; /*31:31, Automatic Destination Reload. The DARx register can beautomatically reloaded from its initial value at the end of everyblock for multi-block transfers. A new block transfer is theninitiated. For conditions under which this occurs, refer to Table 8-1on page 249.This register does not exist if the configuration parameterDMAH_CHx_MULTI_BLK_EN (page 130) is not selected; in thiscase, the read-back value is always 0.*/
    } bit_field;
} T_DMAC_CH3_CFG_L;

//ch3_cfg_H
typedef union
{
    uint32_t val;
    struct
    {
        uint32_t                        fc_mode :  1; /* 0: 0, Flow Control Mode. Determines when source transactionrequests are serviced when the Destination Peripheral is the flowcontroller.0 = Source transaction requests are serviced when they occur.Data pre-fetching is enabled.1 = Source transaction requests are not serviced until adestination transaction request occurs. In this mode, the amountof data transferred from the source is limited so that it isguaranteed to be transferred to the destination prior to blocktermination by the destination. Data pre-fetching is disabled.*/
        uint32_t                      fifo_mode :  1; /* 1: 1, FIFO Mode Select. Determines how much space or data needsto be available in the FIFO before a burst transaction request isserviced.0 = Space/data available for single AHB transfer of the specifiedtransfer width.1 = Data available is greater than or equal to half the FIFO depthfor destination transfers and space available is greater than halfthe fifo depth for source transfers. The exceptions are at the endof a burst transaction request or at the end of a block transfer.*/
        uint32_t                        protctl :  3; /* 4: 2, Protection Control bits used to drive the AHB HPROT[3:1] bus.The AMBA Specification recommends that the default value ofHPROT indicates a non-cached, non-buffered, privileged dataaccess. The reset value is used to indicate such an access.HPROT[0] is tied high because all transfers are data accesses,as there are no opcode fetches.There is a one-to-one mapping of these register bits to theHPROT[3:1] master interface signals. Table 6-5 on page 185shows the mapping of bits in this field to the AHB HPROT[3:1]bus.*/
        uint32_t                      ds_upd_en :  1; /* 5: 5, Destination Status Update Enable. Destination statusinformation is fetched only from the location pointed to by theDSTATARx register, stored in the DSTATx register and written outto the DSTATx location of the LLI (refer to Figure 8-3 on page250) if DS_UPD_EN is high.This field does not exist if the configuration parameterDMAH_CHx_STAT_DST is set to False; in this case, theread-back value is always 0.*/
        uint32_t                      ss_upd_en :  1; /* 6: 6, Source Status Update Enable. Source status information isfetched only from the location pointed to by the SSTATARxregister, stored in the SSTATx register and written out to theSSTATx location of the LLI (refer to Figure 8-3 on page 250) ifSS_UPD_EN is high.NOTE: This enable is applicable only if DMAH_CHx_STAT_SRC(page 132) is set to True.This field does not exist if the configuration parameterDMAH_CHx_STAT_SRC is set to False; in this case, theread-back value is always 0.*/
        uint32_t                        src_per :  4; /*10: 7, Assigns a hardware handshaking interface(0 - DMAH_NUM_HS_INT-1) to the source of channel x if theCFGx.HS_SEL_SRC field is 0; otherwise, this field is ignored.The channel can then communicate with the source peripheralconnected to that interface through the assigned hardwarehandshaking interface.NOTE1: For correct DW_ahb_dmac operation, only oneperipheral (source or destination) should be assigned to thesame handshaking interface.NOTE2: This field does not exist if the configuration parameterDMAH_NUM_HS_INT is set to 0.NOTE3:← b = 39 if DMAH_NUM_HS_INT is 1← b = ceil(log2(DMAH_NUM_HS_INT)) + 38 ifDMAH_NUM_HS_INT is greater than 1Bits 42:(b+1) do not exist and return 0 on a read.*/
        uint32_t                       dest_per :  4; /*14:11, Assigns a hardware handshaking interface(0 - DMAH_NUM_HS_INT-1) to the destination of channel x if theCFGx.HS_SEL_DST field is 0; otherwise, this field is ignored.The channel can then communicate with the destinationperipheral connected to that interface through the assignedhardware handshaking interface.NOTE1: For correct DMA operation, only one peripheral (sourceor destination) should be assigned to the same handshakinginterface.NOTE2: This field does not exist if the configuration parameterDMAH_NUM_HS_INT is set to 0.NOTE3:← b = 43 if DMAH_NUM_HS_INT is 1← b = ceil(log2(DMAH_NUM_HS_INT)) + 42 ifDMAH_NUM_HS_INT is greater than 1Bits 46:(b+1) do not exist and return 0 on a read.*/
        uint32_t                     reserved_0 : 17; /*31:15,                       Reserved*/
    } bit_field;
} T_DMAC_CH3_CFG_H;

//ch4_sar_L
typedef union
{
    uint32_t val;
    struct
    {
        uint32_t                            sar : 32; /*31: 0, Current Source Address of DMA transfer. Updated after each sourcetransfer. The SINC field in the CTLx register determines whether the address increments, decrements, or is left unchanged on every source transfer throughout the block transfer.*/
    } bit_field;
} T_DMAC_CH4_SAR_L;

//ch4_sar_H
typedef union
{
    uint32_t val;
    struct
    {
        uint32_t                     reserved_0 : 32; /*31: 0,                       Reserved*/
    } bit_field;
} T_DMAC_CH4_SAR_H;

//ch4_dar_L
typedef union
{
    uint32_t val;
    struct
    {
        uint32_t                            dar : 32; /*31: 0, Current Destination address of DMA transfer. Updated after eachdestination transfer. The DINC field in the CTLx register determines whetherthe address increments, decrements, or is left unchanged on everydestination transfer throughout the block transfer.*/
    } bit_field;
} T_DMAC_CH4_DAR_L;

//ch4_dar_H
typedef union
{
    uint32_t val;
    struct
    {
        uint32_t                     reserved_0 : 32; /*31: 0,                       Reserved*/
    } bit_field;
} T_DMAC_CH4_DAR_H;

//ch4_ctrl_L
typedef union
{
    uint32_t val;
    struct
    {
        uint32_t                         int_en :  1; /* 0: 0, Interrupt Enable Bit. If set, then all interrupt-generating sources areenabled. Functions as a global mask bit for all interrupts for thechannel; raw* interrupt registers still assert if CTLx.INT_EN = 0.Reset Value: 0x1*/
        uint32_t                   dst_tr_width :  3; /* 3: 1, Destination Transfer Width. Table 6-3 on page 175 lists thedecoding for this field. Mapped to AHB bus ※hsize.§ For anon-memory peripheral, typically rgw peripheral (destination) FIFOwidth.This value must be less than or equal to DMAH_Mk_HDATA_WIDTH,where k is the AHB layer 1 to 4 where the destination resides.Reset Value: Encoded value; refer to Table 6-3 on page 175.Dependencies: This field does not exist if DMAH_CHx_DTW(page 129) is hardcoded. In this case, the read-back value is alwaysthe hardcoded destination transfer width, DMAH_CHx_DTW.*/
        uint32_t                   src_tr_width :  3; /* 6: 4, Source Transfer Width. Table 6-3 on page 175 lists the decoding forthis field.Mapped to AHB bus ※hsize.§ For a non-memory peripheral, typicallythe peripheral (source) FIFO width.This value must be less than or equal to DMAH_Mx_HDATA_WIDTH,where x is the AHB layer 1 to 4 where the source resides.Reset Value: Encoded value; refer to Table 6-3 on page 175.Dependencies: This field does not exist if the parameterDMAH_CHx_STW (page 129) is hardcoded. In this case, theread-back value is always the hardcoded source transfer width,DMAH_CHx_STW.*/
        uint32_t                           dinc :  2; /* 8: 7, Destination Address Increment. Indicates whether to increment ordecrement the destination address on every destination transfer. Ifyour device is writing data to a destination peripheral FIFO with afixed address, then set this field to ※No change.§00 = Increment01 = Decrement1x = No changeNOTE: Incrementing or decrementing is done for alignment to thenext CTLx.DST_TR_WIDTH boundary.Reset Value: 0x0*/
        uint32_t                           sinc :  2; /*10: 9, Source Address Increment. Indicates whether to increment ordecrement the source address on every source transfer. If the deviceis fetching data from a source peripheral FIFO with a fixed address,then set this field to ※No change.§00 = Increment01 = Decrement1x = No changeNOTE: Incrementing or decrementing is done for alignment to thenext CTLx.SRC_TR_WIDTH boundary.Reset Value: 0x0*/
        uint32_t                     dest_msize :  3; /*13:11, Destination Burst Transaction Length. Number of data items, eachof width CTLx.DST_TR_WIDTH, to be written to the destinationevery time a destination burst transaction request is made from eitherthe corresponding hardware or software handshaking interface.NOTE: This value is not related to the AHB bus master HBURSTbus.Reset Value: 0x1Dependencies: DMAH_CHx_MAX_MULT_SIZE determines the bit width of this field. All surplus bits in this field do not exist and read back as 0.*/
        uint32_t                      src_msize :  3; /*16:14, Source Burst Transaction Length. Number of data items, each ofwidth CTLx.SRC_TR_WIDTH, to be read from the source every timea source burst transaction request is made from either thecorresponding hardware or software handshaking interface.Table 6-2 on page 174 lists the decoding for this field; refer to※Choosing the Receive Watermark level§ on page 101.NOTE: This value is not related to the AHB bus master HBURSTbus.Reset Value: 0x1Dependencies: The configuration parameter DMAH_CHx_MAX_MULT_SIZE determines the bit widthof this field. All remaining bits in this field do not exist and read backas 0.*/
        uint32_t                  src_gather_en :  1; /*17:17, Source gather enable bit:0 = Gather disabled1 = Gather enabledGather on the source side is applicable only when the CTLx.SINC bitindicates an incrementing or decrementing address control.Reset Value: 0x0Dependencies: This field does not exist ifDMAH_CHx_SRC_GAT_EN is not selected; in this case, the read-back value is always 0.*/
        uint32_t                 dst_scatter_en :  1; /*18:18, Destination scatter enable bit:0 = Scatter disabled1 = Scatter enabledScatter on the destination side is applicable only when theCTLx.DINC bit indicates an incrementing or decrementing addresscontrol.Reset Value: 0x0Dependencies: This field does not exist if DMAH_CHx_DST_SCA_EN is not selected; in this case, the read-back value is always 0.*/
        uint32_t                     reserved_1 :  1; /*19:19,                       Reserved*/
        uint32_t                          tt_fc :  2; /*21:20, Transfer Type and Flow Control. The following transfer types aresupported.? Memory to Memory? Memory to Peripheral? Peripheral to Memory? Peripheral to PeripheralFlow Control can be assigned to the DW_ahb_dmac, the sourceperipheral, or the destination peripheral. Reset Value: Configuration dependent:TT_FC[0] = 1＊b1TT_FC[1] = DMAH_CHx_FC[1] || (!DMAH_CHx_FC[0])TT_FC[2] = DMAH_CHx_FC[1] ^ DMAH_CHx_FC[0]Dependencies: If the configuration parameter DMAH_CHx_FCis set to DMA_FC_ONLY, then TT_FC[2] does not existand TT_FC[2] always reads back 0.If DMAH_CHx_FC is set to SRC_FC_ONLY, then TT_FC[2:1] doesnot exist and TT_FC[2:1] always reads back 2＊b10.If DMAH_CHx_FC is set to DST_FC_ONLY, then TT_FC[2:1] doesnot exist and TT_FC[2:1] always reads back 2＊b11.For multi-block transfers using linked list operation, TT_FC must beconstant for all blocks of this multi-block transfer.*/
        uint32_t                            dms :  3; /*24:22, Destination Master Select. Identifies the Master Interface layerwhere the destination device (peripheral or memory) resides.00 = AHB master 101 = AHB master 210 = AHB master 311 = AHB master 4The maximum value of this field that can be read back isDMAH_NUM_MASTER_INT 每 1.Reset Value: DMAH_CHx_DMS[1:0]Dependencies: This field does not exist if the configurationparameter DMAH_CHx_DMS (page 129) is hardcoded; in this case,the read-back value is always the hardcoded value.*/
        uint32_t                            sms :  2; /*26:25, Source Master Select. Identifies the Master Interface layer fromwhich the source device (peripheral or memory) is accessed.00 = AHB master 101 = AHB master 210 = AHB master 311 = AHB master 4The maximum value of this field that can be read back isDMAH_NUM_MASTER_INT 每 1.Reset Value: DMAH_CHx_SMS[1:0]Dependencies: This field does not exist if the configurationparameter DMAH_CHx_SMS is hardcoded; in this case,the read-back value is always the hardcoded value.*/
        uint32_t                     llp_dst_en :  1; /*27:27, Block chaining is enabled on the destination side only if theLLP_DST_EN field is high and LLPx.LOC is non-zero.Reset Value: 0x0Dependencies: This field does not exist if the configurationparameter DMAH_CHx_MULTI_BLK_EN is not selected or if DMAH_CHx_HC_LLP is selected; in this case, the read-back value is always 0.*/
        uint32_t                     llp_src_en :  1; /*28:28, Block chaining is enabled on the source side only if theLLP_SRC_EN field is high and LLPx.LOC is non-zeroReset Value: 0x0Dependencies: This field does not exist if the configurationparameter DMAH_CHx_MULTI_BLK_EN is not selected or if DMAH_CHx_HC_LLP is selected; in this case, the read-back value is always 0.*/
        uint32_t                     reserved_0 :  3; /*31:29,                       Reserved*/
    } bit_field;
} T_DMAC_CH4_CTRL_L;

//ch4_ctrl_H
typedef union
{
    uint32_t val;
    struct
    {
        uint32_t                       block_ts : 12; /*11: 0, Block Transfer Size.When the DW_ahb_dmac is the flow controller, the user writes thisfield before the channel is enabled in order to indicate the block size.The number programmed into BLOCK_TS indicates the total numberof single transactions to perform for every block transfer; a singletransaction is mapped to a single AMBA beat.Width: The width of the single transaction is determined byCTLx.SRC_TR_WIDTH. For further information on setting this field,refer to Table 6-3 on page 175 and ※Transfer Operation§ on page 61.Once the transfer starts, the read-back value is the total number ofdata items already read from the source peripheral, regardless ofwhat is the flow controller.When the source or destination peripheral is assigned as the flowcontroller, then the maximum block size that can be read backsaturates at DMAH_CHx_MAX_BLK_SIZE, but the actual block sizecan be greater. Refer to Table 8-1 on page 249.b = log2(DMAH_CHx_MAX_BLK_SIZE + 1) + 31Bits 43:b+1 do not exist and return 0 on a read.Reset Value: 0x2*/
        uint32_t                           done :  1; /*12:12, Done bitIf status write-back is enabled, the upper word of the control register,CTLx[63:32], is written to the control register location of the LinkedList Item (LLI) in system memory at the end of the block transfer withthe done bit set.Software can poll the LLI CTLx.DONE bit to see when a blocktransfer is complete. The LLI CTLx.DONE bit should be clearedwhen the linked lists are set up in memory prior to enabling thechannel.LLI accesses are always 32-bit accesses (Hsize = 2) aligned to 32-bitboundaries and cannot be changed or programmed to anything otherthan 32-bit. For more information, refer to ※Multi-Block Transfers§ onpage 247.Reset Value: 0x0*/
        uint32_t                     reserved_0 : 19; /*31:13,                       Reserved*/
    } bit_field;
} T_DMAC_CH4_CTRL_H;

//ch4_cfg_L
typedef union
{
    uint32_t val;
    struct
    {
        uint32_t                     reserved_0 :  5; /* 4: 0,                       Reserved*/
        uint32_t                       ch_prior :  3; /* 7: 5, Channel priority. A priority of 7 is the highest priority, and 0 isthe lowest. This field must be programmed within the followingrange:0: (DMAH_NUM_CHANNELS 每 1)A programmed value outside this range will cause erroneousbehavior.*/
        uint32_t                        ch_susp :  1; /* 8: 8, Channel Suspend. Suspends all DMA data transfers from thesource until this bit is cleared. There is no guarantee that thecurrent transaction will complete. Can also be used in conjunctionwith CFGx.FIFO_EMPTY to cleanly disable a channel withoutlosing any data.0 = Not suspended.1 = Suspend DMA transfer from the source.For more information, refer to ※Disabling a Channel Prior toTransfer Completion§ on page 282.*/
        uint32_t                     fifo_empty :  1; /* 9: 9, Indicates if there is data left in the channel FIFO. Can be used inconjunction with CFGx.CH_SUSP to cleanly disable a channel.For more information, refer to ※Disabling a Channel Prior toTransfer Completion§ on page 282.1 = Channel FIFO empty0 = Channel FIFO not empty*/
        uint32_t                     hs_sel_dst :  1; /*10:10, Destination Software or Hardware Handshaking Select. Thisregister selects which of the handshaking interfaces 每 hardwareor software 每 is active for destination requests on this channel.0 = Hardware handshaking interface. Software-initiatedtransaction requests are ignored.1 = Software handshaking interface. Hardware- initiatedtransaction requests are ignored.If the destination peripheral is memory, then this bit is ignored.*/
        uint32_t                     hs_sel_src :  1; /*11:11, Source Software or Hardware Handshaking Select. Thisregister selects which of the handshaking interfaces 每 hardwareor software 每 is active for source requests on this channel.0 = Hardware handshaking interface. Software-initiatedtransaction requests are ignored.1 = Software handshaking interface. Hardware-initiatedtransaction requests are ignored.If the source peripheral is memory, then this bit is ignored.*/
        uint32_t                      lock_ch_l :  2; /*13:12, Channel Lock Level. Indicates the duration over whichCFGx.LOCK_CH bit applies.00 = Over complete DMA transfer01 = Over complete DMA block transfer1x = Over complete DMA transactionThis field does not exist if the configuration parameterDMAH_CHx_LOCK_EN is set to False (page 128); in this case,the read-back value is always 0.*/
        uint32_t                       lock_b_l :  2; /*15:14, Bus Lock Level. Indicates the duration over whichCFGx.LOCK_B bit applies.00 = Over complete DMA transfer01 = Over complete DMA block transfer1x = Over complete DMA transactionThis field does not exist if the parameter DMAH_CHx_LOCK_ENis set to False (page 128); in this case, the read-back value isalways 0.*/
        uint32_t                        lock_ch :  1; /*16:16, Bus Lock Bit. When active, the AHB bus master signal hlock isasserted for the duration specified in CFGx.LOCK_B_L. For moreinformation, refer to ※Locked DMA Transfers§ on page 103.This field does not exist if the configuration parameterDMAH_CHx_LOCK_EN is set to False (page 128); in this case,the read-back value is always 0.*/
        uint32_t                         lock_b :  1; /*17:17, Bus Lock Bit. When active, the AHB bus master signal hlock isasserted for the duration specified in CFGx.LOCK_B_L. For moreinformation, refer to ※Locked DMA Transfers§ on page 103.This field does not exist if the configuration parameterDMAH_CHx_LOCK_EN is set to False (page 128); in this case,the read-back value is always 0.*/
        uint32_t                     dst_hs_pol :  1; /*18:18, Source Handshaking Interface Polarity.0 = Active high1 = Active lowFor information on this, refer to ※Handshaking Interface§ onpage 42.*/
        uint32_t                     src_hs_pol :  1; /*19:19, Source Handshaking Interface Polarity.0 = Active high1 = Active lowFor information on this, refer to ※Handshaking Interface§ onpage 42.*/
        uint32_t                      max_abrst : 10; /*29:20, Maximum AMBA Burst Length. Maximum AMBA burst lengththat is used for DMA transfers on this channel.A value of 0 indicates that software is not limiting the maximumAMBA burst length for DMA transfers on this channel.This field does not exist if the configuration parameterDMAH_MABRST (page 121) is not selected; in this case, theread-back value is always 0, and the maximum AMBA burstlength cannot be limited by software.*/
        uint32_t                     reload_src :  1; /*30:30, Automatic Source Reload. The SARx register can beautomatically reloaded from its initial value at the end of everyblock for multi-block transfers. A new block transfer is theninitiated. For conditions under which this occurs, refer to Table 8-1on page 249.This field does not exist if the configuration parameterDMAH_CHx_MULTI_BLK_EN (page 130) is not selected; in thiscase, the read-back value is always 0.*/
        uint32_t                     reload_dst :  1; /*31:31, Automatic Destination Reload. The DARx register can beautomatically reloaded from its initial value at the end of everyblock for multi-block transfers. A new block transfer is theninitiated. For conditions under which this occurs, refer to Table 8-1on page 249.This register does not exist if the configuration parameterDMAH_CHx_MULTI_BLK_EN (page 130) is not selected; in thiscase, the read-back value is always 0.*/
    } bit_field;
} T_DMAC_CH4_CFG_L;

//ch4_cfg_H
typedef union
{
    uint32_t val;
    struct
    {
        uint32_t                        fc_mode :  1; /* 0: 0, Flow Control Mode. Determines when source transactionrequests are serviced when the Destination Peripheral is the flowcontroller.0 = Source transaction requests are serviced when they occur.Data pre-fetching is enabled.1 = Source transaction requests are not serviced until adestination transaction request occurs. In this mode, the amountof data transferred from the source is limited so that it isguaranteed to be transferred to the destination prior to blocktermination by the destination. Data pre-fetching is disabled.*/
        uint32_t                      fifo_mode :  1; /* 1: 1, FIFO Mode Select. Determines how much space or data needsto be available in the FIFO before a burst transaction request isserviced.0 = Space/data available for single AHB transfer of the specifiedtransfer width.1 = Data available is greater than or equal to half the FIFO depthfor destination transfers and space available is greater than halfthe fifo depth for source transfers. The exceptions are at the endof a burst transaction request or at the end of a block transfer.*/
        uint32_t                        protctl :  3; /* 4: 2, Protection Control bits used to drive the AHB HPROT[3:1] bus.The AMBA Specification recommends that the default value ofHPROT indicates a non-cached, non-buffered, privileged dataaccess. The reset value is used to indicate such an access.HPROT[0] is tied high because all transfers are data accesses,as there are no opcode fetches.There is a one-to-one mapping of these register bits to theHPROT[3:1] master interface signals. Table 6-5 on page 185shows the mapping of bits in this field to the AHB HPROT[3:1]bus.*/
        uint32_t                      ds_upd_en :  1; /* 5: 5, Destination Status Update Enable. Destination statusinformation is fetched only from the location pointed to by theDSTATARx register, stored in the DSTATx register and written outto the DSTATx location of the LLI (refer to Figure 8-3 on page250) if DS_UPD_EN is high.This field does not exist if the configuration parameterDMAH_CHx_STAT_DST is set to False; in this case, theread-back value is always 0.*/
        uint32_t                      ss_upd_en :  1; /* 6: 6, Source Status Update Enable. Source status information isfetched only from the location pointed to by the SSTATARxregister, stored in the SSTATx register and written out to theSSTATx location of the LLI (refer to Figure 8-3 on page 250) ifSS_UPD_EN is high.NOTE: This enable is applicable only if DMAH_CHx_STAT_SRC(page 132) is set to True.This field does not exist if the configuration parameterDMAH_CHx_STAT_SRC is set to False; in this case, theread-back value is always 0.*/
        uint32_t                        src_per :  4; /*10: 7, Assigns a hardware handshaking interface(0 - DMAH_NUM_HS_INT-1) to the source of channel x if theCFGx.HS_SEL_SRC field is 0; otherwise, this field is ignored.The channel can then communicate with the source peripheralconnected to that interface through the assigned hardwarehandshaking interface.NOTE1: For correct DW_ahb_dmac operation, only oneperipheral (source or destination) should be assigned to thesame handshaking interface.NOTE2: This field does not exist if the configuration parameterDMAH_NUM_HS_INT is set to 0.NOTE3:← b = 39 if DMAH_NUM_HS_INT is 1← b = ceil(log2(DMAH_NUM_HS_INT)) + 38 ifDMAH_NUM_HS_INT is greater than 1Bits 42:(b+1) do not exist and return 0 on a read.*/
        uint32_t                       dest_per :  4; /*14:11, Assigns a hardware handshaking interface(0 - DMAH_NUM_HS_INT-1) to the destination of channel x if theCFGx.HS_SEL_DST field is 0; otherwise, this field is ignored.The channel can then communicate with the destinationperipheral connected to that interface through the assignedhardware handshaking interface.NOTE1: For correct DMA operation, only one peripheral (sourceor destination) should be assigned to the same handshakinginterface.NOTE2: This field does not exist if the configuration parameterDMAH_NUM_HS_INT is set to 0.NOTE3:← b = 43 if DMAH_NUM_HS_INT is 1← b = ceil(log2(DMAH_NUM_HS_INT)) + 42 ifDMAH_NUM_HS_INT is greater than 1Bits 46:(b+1) do not exist and return 0 on a read.*/
        uint32_t                     reserved_0 : 17; /*31:15,                       Reserved*/
    } bit_field;
} T_DMAC_CH4_CFG_H;

//ch5_sar_L
typedef union
{
    uint32_t val;
    struct
    {
        uint32_t                            sar : 32; /*31: 0, Current Source Address of DMA transfer. Updated after each sourcetransfer. The SINC field in the CTLx register determines whether the address increments, decrements, or is left unchanged on every source transfer throughout the block transfer.*/
    } bit_field;
} T_DMAC_CH5_SAR_L;

//ch5_sar_H
typedef union
{
    uint32_t val;
    struct
    {
        uint32_t                     reserved_0 : 32; /*31: 0,                       Reserved*/
    } bit_field;
} T_DMAC_CH5_SAR_H;

//ch5_dar_L
typedef union
{
    uint32_t val;
    struct
    {
        uint32_t                            dar : 32; /*31: 0, Current Destination address of DMA transfer. Updated after eachdestination transfer. The DINC field in the CTLx register determines whetherthe address increments, decrements, or is left unchanged on everydestination transfer throughout the block transfer.*/
    } bit_field;
} T_DMAC_CH5_DAR_L;

//ch5_dar_H
typedef union
{
    uint32_t val;
    struct
    {
        uint32_t                     reserved_0 : 32; /*31: 0,                       Reserved*/
    } bit_field;
} T_DMAC_CH5_DAR_H;

//ch5_ctrl_L
typedef union
{
    uint32_t val;
    struct
    {
        uint32_t                         int_en :  1; /* 0: 0, Interrupt Enable Bit. If set, then all interrupt-generating sources areenabled. Functions as a global mask bit for all interrupts for thechannel; raw* interrupt registers still assert if CTLx.INT_EN = 0.Reset Value: 0x1*/
        uint32_t                   dst_tr_width :  3; /* 3: 1, Destination Transfer Width. Table 6-3 on page 175 lists thedecoding for this field. Mapped to AHB bus ※hsize.§ For anon-memory peripheral, typically rgw peripheral (destination) FIFOwidth.This value must be less than or equal to DMAH_Mk_HDATA_WIDTH,where k is the AHB layer 1 to 4 where the destination resides.Reset Value: Encoded value; refer to Table 6-3 on page 175.Dependencies: This field does not exist if DMAH_CHx_DTW(page 129) is hardcoded. In this case, the read-back value is alwaysthe hardcoded destination transfer width, DMAH_CHx_DTW.*/
        uint32_t                   src_tr_width :  3; /* 6: 4, Source Transfer Width. Table 6-3 on page 175 lists the decoding forthis field.Mapped to AHB bus ※hsize.§ For a non-memory peripheral, typicallythe peripheral (source) FIFO width.This value must be less than or equal to DMAH_Mx_HDATA_WIDTH,where x is the AHB layer 1 to 4 where the source resides.Reset Value: Encoded value; refer to Table 6-3 on page 175.Dependencies: This field does not exist if the parameterDMAH_CHx_STW (page 129) is hardcoded. In this case, theread-back value is always the hardcoded source transfer width,DMAH_CHx_STW.*/
        uint32_t                           dinc :  2; /* 8: 7, Destination Address Increment. Indicates whether to increment ordecrement the destination address on every destination transfer. Ifyour device is writing data to a destination peripheral FIFO with afixed address, then set this field to ※No change.§00 = Increment01 = Decrement1x = No changeNOTE: Incrementing or decrementing is done for alignment to thenext CTLx.DST_TR_WIDTH boundary.Reset Value: 0x0*/
        uint32_t                           sinc :  2; /*10: 9, Source Address Increment. Indicates whether to increment ordecrement the source address on every source transfer. If the deviceis fetching data from a source peripheral FIFO with a fixed address,then set this field to ※No change.§00 = Increment01 = Decrement1x = No changeNOTE: Incrementing or decrementing is done for alignment to thenext CTLx.SRC_TR_WIDTH boundary.Reset Value: 0x0*/
        uint32_t                     dest_msize :  3; /*13:11, Destination Burst Transaction Length. Number of data items, eachof width CTLx.DST_TR_WIDTH, to be written to the destinationevery time a destination burst transaction request is made from eitherthe corresponding hardware or software handshaking interface.NOTE: This value is not related to the AHB bus master HBURSTbus.Reset Value: 0x1Dependencies: DMAH_CHx_MAX_MULT_SIZE determines the bit width of this field. All surplus bits in this field do not exist and read back as 0.*/
        uint32_t                      src_msize :  3; /*16:14, Source Burst Transaction Length. Number of data items, each ofwidth CTLx.SRC_TR_WIDTH, to be read from the source every timea source burst transaction request is made from either thecorresponding hardware or software handshaking interface.Table 6-2 on page 174 lists the decoding for this field; refer to※Choosing the Receive Watermark level§ on page 101.NOTE: This value is not related to the AHB bus master HBURSTbus.Reset Value: 0x1Dependencies: The configuration parameter DMAH_CHx_MAX_MULT_SIZE determines the bit widthof this field. All remaining bits in this field do not exist and read backas 0.*/
        uint32_t                  src_gather_en :  1; /*17:17, Source gather enable bit:0 = Gather disabled1 = Gather enabledGather on the source side is applicable only when the CTLx.SINC bitindicates an incrementing or decrementing address control.Reset Value: 0x0Dependencies: This field does not exist ifDMAH_CHx_SRC_GAT_EN is not selected; in this case, the read-back value is always 0.*/
        uint32_t                 dst_scatter_en :  1; /*18:18, Destination scatter enable bit:0 = Scatter disabled1 = Scatter enabledScatter on the destination side is applicable only when theCTLx.DINC bit indicates an incrementing or decrementing addresscontrol.Reset Value: 0x0Dependencies: This field does not exist if DMAH_CHx_DST_SCA_EN is not selected; in this case, the read-back value is always 0.*/
        uint32_t                     reserved_1 :  1; /*19:19,                       Reserved*/
        uint32_t                          tt_fc :  2; /*21:20, Transfer Type and Flow Control. The following transfer types aresupported.? Memory to Memory? Memory to Peripheral? Peripheral to Memory? Peripheral to PeripheralFlow Control can be assigned to the DW_ahb_dmac, the sourceperipheral, or the destination peripheral. Reset Value: Configuration dependent:TT_FC[0] = 1＊b1TT_FC[1] = DMAH_CHx_FC[1] || (!DMAH_CHx_FC[0])TT_FC[2] = DMAH_CHx_FC[1] ^ DMAH_CHx_FC[0]Dependencies: If the configuration parameter DMAH_CHx_FCis set to DMA_FC_ONLY, then TT_FC[2] does not existand TT_FC[2] always reads back 0.If DMAH_CHx_FC is set to SRC_FC_ONLY, then TT_FC[2:1] doesnot exist and TT_FC[2:1] always reads back 2＊b10.If DMAH_CHx_FC is set to DST_FC_ONLY, then TT_FC[2:1] doesnot exist and TT_FC[2:1] always reads back 2＊b11.For multi-block transfers using linked list operation, TT_FC must beconstant for all blocks of this multi-block transfer.*/
        uint32_t                            dms :  3; /*24:22, Destination Master Select. Identifies the Master Interface layerwhere the destination device (peripheral or memory) resides.00 = AHB master 101 = AHB master 210 = AHB master 311 = AHB master 4The maximum value of this field that can be read back isDMAH_NUM_MASTER_INT 每 1.Reset Value: DMAH_CHx_DMS[1:0]Dependencies: This field does not exist if the configurationparameter DMAH_CHx_DMS (page 129) is hardcoded; in this case,the read-back value is always the hardcoded value.*/
        uint32_t                            sms :  2; /*26:25, Source Master Select. Identifies the Master Interface layer fromwhich the source device (peripheral or memory) is accessed.00 = AHB master 101 = AHB master 210 = AHB master 311 = AHB master 4The maximum value of this field that can be read back isDMAH_NUM_MASTER_INT 每 1.Reset Value: DMAH_CHx_SMS[1:0]Dependencies: This field does not exist if the configurationparameter DMAH_CHx_SMS is hardcoded; in this case,the read-back value is always the hardcoded value.*/
        uint32_t                     llp_dst_en :  1; /*27:27, Block chaining is enabled on the destination side only if theLLP_DST_EN field is high and LLPx.LOC is non-zero.Reset Value: 0x0Dependencies: This field does not exist if the configurationparameter DMAH_CHx_MULTI_BLK_EN is not selected or if DMAH_CHx_HC_LLP is selected; in this case, the read-back value is always 0.*/
        uint32_t                     llp_src_en :  1; /*28:28, Block chaining is enabled on the source side only if theLLP_SRC_EN field is high and LLPx.LOC is non-zeroReset Value: 0x0Dependencies: This field does not exist if the configurationparameter DMAH_CHx_MULTI_BLK_EN is not selected or if DMAH_CHx_HC_LLP is selected; in this case, the read-back value is always 0.*/
        uint32_t                     reserved_0 :  3; /*31:29,                       Reserved*/
    } bit_field;
} T_DMAC_CH5_CTRL_L;

//ch5_ctrl_H
typedef union
{
    uint32_t val;
    struct
    {
        uint32_t                       block_ts : 12; /*11: 0, Block Transfer Size.When the DW_ahb_dmac is the flow controller, the user writes thisfield before the channel is enabled in order to indicate the block size.The number programmed into BLOCK_TS indicates the total numberof single transactions to perform for every block transfer; a singletransaction is mapped to a single AMBA beat.Width: The width of the single transaction is determined byCTLx.SRC_TR_WIDTH. For further information on setting this field,refer to Table 6-3 on page 175 and ※Transfer Operation§ on page 61.Once the transfer starts, the read-back value is the total number ofdata items already read from the source peripheral, regardless ofwhat is the flow controller.When the source or destination peripheral is assigned as the flowcontroller, then the maximum block size that can be read backsaturates at DMAH_CHx_MAX_BLK_SIZE, but the actual block sizecan be greater. Refer to Table 8-1 on page 249.b = log2(DMAH_CHx_MAX_BLK_SIZE + 1) + 31Bits 43:b+1 do not exist and return 0 on a read.Reset Value: 0x2*/
        uint32_t                           done :  1; /*12:12, Done bitIf status write-back is enabled, the upper word of the control register,CTLx[63:32], is written to the control register location of the LinkedList Item (LLI) in system memory at the end of the block transfer withthe done bit set.Software can poll the LLI CTLx.DONE bit to see when a blocktransfer is complete. The LLI CTLx.DONE bit should be clearedwhen the linked lists are set up in memory prior to enabling thechannel.LLI accesses are always 32-bit accesses (Hsize = 2) aligned to 32-bitboundaries and cannot be changed or programmed to anything otherthan 32-bit. For more information, refer to ※Multi-Block Transfers§ onpage 247.Reset Value: 0x0*/
        uint32_t                     reserved_0 : 19; /*31:13,                       Reserved*/
    } bit_field;
} T_DMAC_CH5_CTRL_H;

//ch5_cfg_L
typedef union
{
    uint32_t val;
    struct
    {
        uint32_t                     reserved_0 :  5; /* 4: 0,                       Reserved*/
        uint32_t                       ch_prior :  3; /* 7: 5, Channel priority. A priority of 7 is the highest priority, and 0 isthe lowest. This field must be programmed within the followingrange:0: (DMAH_NUM_CHANNELS 每 1)A programmed value outside this range will cause erroneousbehavior.*/
        uint32_t                        ch_susp :  1; /* 8: 8, Channel Suspend. Suspends all DMA data transfers from thesource until this bit is cleared. There is no guarantee that thecurrent transaction will complete. Can also be used in conjunctionwith CFGx.FIFO_EMPTY to cleanly disable a channel withoutlosing any data.0 = Not suspended.1 = Suspend DMA transfer from the source.For more information, refer to ※Disabling a Channel Prior toTransfer Completion§ on page 282.*/
        uint32_t                     fifo_empty :  1; /* 9: 9, Indicates if there is data left in the channel FIFO. Can be used inconjunction with CFGx.CH_SUSP to cleanly disable a channel.For more information, refer to ※Disabling a Channel Prior toTransfer Completion§ on page 282.1 = Channel FIFO empty0 = Channel FIFO not empty*/
        uint32_t                     hs_sel_dst :  1; /*10:10, Destination Software or Hardware Handshaking Select. Thisregister selects which of the handshaking interfaces 每 hardwareor software 每 is active for destination requests on this channel.0 = Hardware handshaking interface. Software-initiatedtransaction requests are ignored.1 = Software handshaking interface. Hardware- initiatedtransaction requests are ignored.If the destination peripheral is memory, then this bit is ignored.*/
        uint32_t                     hs_sel_src :  1; /*11:11, Source Software or Hardware Handshaking Select. Thisregister selects which of the handshaking interfaces 每 hardwareor software 每 is active for source requests on this channel.0 = Hardware handshaking interface. Software-initiatedtransaction requests are ignored.1 = Software handshaking interface. Hardware-initiatedtransaction requests are ignored.If the source peripheral is memory, then this bit is ignored.*/
        uint32_t                      lock_ch_l :  2; /*13:12, Channel Lock Level. Indicates the duration over whichCFGx.LOCK_CH bit applies.00 = Over complete DMA transfer01 = Over complete DMA block transfer1x = Over complete DMA transactionThis field does not exist if the configuration parameterDMAH_CHx_LOCK_EN is set to False (page 128); in this case,the read-back value is always 0.*/
        uint32_t                       lock_b_l :  2; /*15:14, Bus Lock Level. Indicates the duration over whichCFGx.LOCK_B bit applies.00 = Over complete DMA transfer01 = Over complete DMA block transfer1x = Over complete DMA transactionThis field does not exist if the parameter DMAH_CHx_LOCK_ENis set to False (page 128); in this case, the read-back value isalways 0.*/
        uint32_t                        lock_ch :  1; /*16:16, Bus Lock Bit. When active, the AHB bus master signal hlock isasserted for the duration specified in CFGx.LOCK_B_L. For moreinformation, refer to ※Locked DMA Transfers§ on page 103.This field does not exist if the configuration parameterDMAH_CHx_LOCK_EN is set to False (page 128); in this case,the read-back value is always 0.*/
        uint32_t                         lock_b :  1; /*17:17, Bus Lock Bit. When active, the AHB bus master signal hlock isasserted for the duration specified in CFGx.LOCK_B_L. For moreinformation, refer to ※Locked DMA Transfers§ on page 103.This field does not exist if the configuration parameterDMAH_CHx_LOCK_EN is set to False (page 128); in this case,the read-back value is always 0.*/
        uint32_t                     dst_hs_pol :  1; /*18:18, Source Handshaking Interface Polarity.0 = Active high1 = Active lowFor information on this, refer to ※Handshaking Interface§ onpage 42.*/
        uint32_t                     src_hs_pol :  1; /*19:19, Source Handshaking Interface Polarity.0 = Active high1 = Active lowFor information on this, refer to ※Handshaking Interface§ onpage 42.*/
        uint32_t                      max_abrst : 10; /*29:20, Maximum AMBA Burst Length. Maximum AMBA burst lengththat is used for DMA transfers on this channel.A value of 0 indicates that software is not limiting the maximumAMBA burst length for DMA transfers on this channel.This field does not exist if the configuration parameterDMAH_MABRST (page 121) is not selected; in this case, theread-back value is always 0, and the maximum AMBA burstlength cannot be limited by software.*/
        uint32_t                     reload_src :  1; /*30:30, Automatic Source Reload. The SARx register can beautomatically reloaded from its initial value at the end of everyblock for multi-block transfers. A new block transfer is theninitiated. For conditions under which this occurs, refer to Table 8-1on page 249.This field does not exist if the configuration parameterDMAH_CHx_MULTI_BLK_EN (page 130) is not selected; in thiscase, the read-back value is always 0.*/
        uint32_t                     reload_dst :  1; /*31:31, Automatic Destination Reload. The DARx register can beautomatically reloaded from its initial value at the end of everyblock for multi-block transfers. A new block transfer is theninitiated. For conditions under which this occurs, refer to Table 8-1on page 249.This register does not exist if the configuration parameterDMAH_CHx_MULTI_BLK_EN (page 130) is not selected; in thiscase, the read-back value is always 0.*/
    } bit_field;
} T_DMAC_CH5_CFG_L;

//ch5_cfg_H
typedef union
{
    uint32_t val;
    struct
    {
        uint32_t                        fc_mode :  1; /* 0: 0, Flow Control Mode. Determines when source transactionrequests are serviced when the Destination Peripheral is the flowcontroller.0 = Source transaction requests are serviced when they occur.Data pre-fetching is enabled.1 = Source transaction requests are not serviced until adestination transaction request occurs. In this mode, the amountof data transferred from the source is limited so that it isguaranteed to be transferred to the destination prior to blocktermination by the destination. Data pre-fetching is disabled.*/
        uint32_t                      fifo_mode :  1; /* 1: 1, FIFO Mode Select. Determines how much space or data needsto be available in the FIFO before a burst transaction request isserviced.0 = Space/data available for single AHB transfer of the specifiedtransfer width.1 = Data available is greater than or equal to half the FIFO depthfor destination transfers and space available is greater than halfthe fifo depth for source transfers. The exceptions are at the endof a burst transaction request or at the end of a block transfer.*/
        uint32_t                        protctl :  3; /* 4: 2, Protection Control bits used to drive the AHB HPROT[3:1] bus.The AMBA Specification recommends that the default value ofHPROT indicates a non-cached, non-buffered, privileged dataaccess. The reset value is used to indicate such an access.HPROT[0] is tied high because all transfers are data accesses,as there are no opcode fetches.There is a one-to-one mapping of these register bits to theHPROT[3:1] master interface signals. Table 6-5 on page 185shows the mapping of bits in this field to the AHB HPROT[3:1]bus.*/
        uint32_t                      ds_upd_en :  1; /* 5: 5, Destination Status Update Enable. Destination statusinformation is fetched only from the location pointed to by theDSTATARx register, stored in the DSTATx register and written outto the DSTATx location of the LLI (refer to Figure 8-3 on page250) if DS_UPD_EN is high.This field does not exist if the configuration parameterDMAH_CHx_STAT_DST is set to False; in this case, theread-back value is always 0.*/
        uint32_t                      ss_upd_en :  1; /* 6: 6, Source Status Update Enable. Source status information isfetched only from the location pointed to by the SSTATARxregister, stored in the SSTATx register and written out to theSSTATx location of the LLI (refer to Figure 8-3 on page 250) ifSS_UPD_EN is high.NOTE: This enable is applicable only if DMAH_CHx_STAT_SRC(page 132) is set to True.This field does not exist if the configuration parameterDMAH_CHx_STAT_SRC is set to False; in this case, theread-back value is always 0.*/
        uint32_t                        src_per :  4; /*10: 7, Assigns a hardware handshaking interface(0 - DMAH_NUM_HS_INT-1) to the source of channel x if theCFGx.HS_SEL_SRC field is 0; otherwise, this field is ignored.The channel can then communicate with the source peripheralconnected to that interface through the assigned hardwarehandshaking interface.NOTE1: For correct DW_ahb_dmac operation, only oneperipheral (source or destination) should be assigned to thesame handshaking interface.NOTE2: This field does not exist if the configuration parameterDMAH_NUM_HS_INT is set to 0.NOTE3:← b = 39 if DMAH_NUM_HS_INT is 1← b = ceil(log2(DMAH_NUM_HS_INT)) + 38 ifDMAH_NUM_HS_INT is greater than 1Bits 42:(b+1) do not exist and return 0 on a read.*/
        uint32_t                       dest_per :  4; /*14:11, Assigns a hardware handshaking interface(0 - DMAH_NUM_HS_INT-1) to the destination of channel x if theCFGx.HS_SEL_DST field is 0; otherwise, this field is ignored.The channel can then communicate with the destinationperipheral connected to that interface through the assignedhardware handshaking interface.NOTE1: For correct DMA operation, only one peripheral (sourceor destination) should be assigned to the same handshakinginterface.NOTE2: This field does not exist if the configuration parameterDMAH_NUM_HS_INT is set to 0.NOTE3:← b = 43 if DMAH_NUM_HS_INT is 1← b = ceil(log2(DMAH_NUM_HS_INT)) + 42 ifDMAH_NUM_HS_INT is greater than 1Bits 46:(b+1) do not exist and return 0 on a read.*/
        uint32_t                     reserved_0 : 17; /*31:15,                       Reserved*/
    } bit_field;
} T_DMAC_CH5_CFG_H;

//rawtfr_L
typedef union
{
    uint32_t val;
    struct
    {
        uint32_t                            raw :  6; /* 5: 0,          Raw interrupt status.*/
        uint32_t                     reserved_0 : 26; /*31: 6,                       Reserved*/
    } bit_field;
} T_DMAC_RAWTFR_L;

//rawtfr_H
typedef union
{
    uint32_t val;
    struct
    {
        uint32_t                     reserved_0 : 32; /*31: 0,                       Reserved*/
    } bit_field;
} T_DMAC_RAWTFR_H;

//rawblock_L
typedef union
{
    uint32_t val;
    struct
    {
        uint32_t                            raw :  6; /* 5: 0,          Raw interrupt status.*/
        uint32_t                     reserved_0 : 26; /*31: 6,                       Reserved*/
    } bit_field;
} T_DMAC_RAWBLOCK_L;

//rawblock_H
typedef union
{
    uint32_t val;
    struct
    {
        uint32_t                     reserved_0 : 32; /*31: 0,                       Reserved*/
    } bit_field;
} T_DMAC_RAWBLOCK_H;

//rawsrcTran_L
typedef union
{
    uint32_t val;
    struct
    {
        uint32_t                            raw :  6; /* 5: 0,          Raw interrupt status.*/
        uint32_t                     reserved_0 : 26; /*31: 6,                       Reserved*/
    } bit_field;
} T_DMAC_RAWSRCTRAN_L;

//rawsrcTran_H
typedef union
{
    uint32_t val;
    struct
    {
        uint32_t                     reserved_0 : 32; /*31: 0,                       Reserved*/
    } bit_field;
} T_DMAC_RAWSRCTRAN_H;

//rawdstTran_L
typedef union
{
    uint32_t val;
    struct
    {
        uint32_t                            raw :  6; /* 5: 0,          Raw interrupt status.*/
        uint32_t                     reserved_0 : 26; /*31: 6,                       Reserved*/
    } bit_field;
} T_DMAC_RAWDSTTRAN_L;

//rawdstTran_H
typedef union
{
    uint32_t val;
    struct
    {
        uint32_t                     reserved_0 : 32; /*31: 0,                       Reserved*/
    } bit_field;
} T_DMAC_RAWDSTTRAN_H;

//rawerr_L
typedef union
{
    uint32_t val;
    struct
    {
        uint32_t                            raw :  6; /* 5: 0,          Raw interrupt status.*/
        uint32_t                     reserved_0 : 26; /*31: 6,                       Reserved*/
    } bit_field;
} T_DMAC_RAWERR_L;

//rawerr_H
typedef union
{
    uint32_t val;
    struct
    {
        uint32_t                     reserved_0 : 32; /*31: 0,                       Reserved*/
    } bit_field;
} T_DMAC_RAWERR_H;

//statustfr_L
typedef union
{
    uint32_t val;
    struct
    {
        uint32_t                         status :  6; /* 5: 0,               Interrupt Status*/
        uint32_t                     reserved_0 : 26; /*31: 6,                       Reserved*/
    } bit_field;
} T_DMAC_STATUSTFR_L;

//statustfr_H
typedef union
{
    uint32_t val;
    struct
    {
        uint32_t                     reserved_0 : 32; /*31: 0,                       Reserved*/
    } bit_field;
} T_DMAC_STATUSTFR_H;

//statusblock_L
typedef union
{
    uint32_t val;
    struct
    {
        uint32_t                         status :  6; /* 5: 0,               Interrupt Status*/
        uint32_t                     reserved_0 : 26; /*31: 6,                       Reserved*/
    } bit_field;
} T_DMAC_STATUSBLOCK_L;

//statusblock_H
typedef union
{
    uint32_t val;
    struct
    {
        uint32_t                     reserved_0 : 32; /*31: 0,                       Reserved*/
    } bit_field;
} T_DMAC_STATUSBLOCK_H;

//statussrcTran_L
typedef union
{
    uint32_t val;
    struct
    {
        uint32_t                         status :  6; /* 5: 0,               Interrupt Status*/
        uint32_t                     reserved_0 : 26; /*31: 6,                       Reserved*/
    } bit_field;
} T_DMAC_STATUSSRCTRAN_L;

//statussrcTran_H
typedef union
{
    uint32_t val;
    struct
    {
        uint32_t                     reserved_0 : 32; /*31: 0,                       Reserved*/
    } bit_field;
} T_DMAC_STATUSSRCTRAN_H;

//statusdstTran_L
typedef union
{
    uint32_t val;
    struct
    {
        uint32_t                         status :  6; /* 5: 0,               Interrupt Status*/
        uint32_t                     reserved_0 : 26; /*31: 6,                       Reserved*/
    } bit_field;
} T_DMAC_STATUSDSTTRAN_L;

//statusdstTran_H
typedef union
{
    uint32_t val;
    struct
    {
        uint32_t                     reserved_0 : 32; /*31: 0,                       Reserved*/
    } bit_field;
} T_DMAC_STATUSDSTTRAN_H;

//statuserr_L
typedef union
{
    uint32_t val;
    struct
    {
        uint32_t                         status :  6; /* 5: 0,               Interrupt Status*/
        uint32_t                     reserved_0 : 26; /*31: 6,                       Reserved*/
    } bit_field;
} T_DMAC_STATUSERR_L;

//statuserr_H
typedef union
{
    uint32_t val;
    struct
    {
        uint32_t                     reserved_0 : 32; /*31: 0,                       Reserved*/
    } bit_field;
} T_DMAC_STATUSERR_H;

//masktfr_L
typedef union
{
    uint32_t val;
    struct
    {
        uint32_t                       int_mask :  6; /* 5: 0, Interrupt Mask0 = masked1 = unmaskeddnc = DMAH_NUM_CHANNELSReset Value: 0x0*/
        uint32_t                     reserved_1 :  2; /* 7: 6, Reserveddnc = DMAH_NUM_CHANNELSIf dnc = 8, then this field is not present.Reset Value: 0x0*/
        uint32_t                    int_mask_we :  6; /*13: 8, Interrupt Mask Write Enable0 = write disabled1 = write enableddnc = DMAH_NUM_CHANNELSReset Value: 0x0*/
        uint32_t                     reserved_0 : 18; /*31:14, Reserveddnc = DMAH_NUM_CHANNELSReset Value: 0x0*/
    } bit_field;
} T_DMAC_MASKTFR_L;

//masktfr_H
typedef union
{
    uint32_t val;
    struct
    {
        uint32_t                     reserved_0 : 32; /*31: 0,                       Reserved*/
    } bit_field;
} T_DMAC_MASKTFR_H;

//maskblock_L
typedef union
{
    uint32_t val;
    struct
    {
        uint32_t                       int_mask :  6; /* 5: 0, Interrupt Mask0 = masked1 = unmaskeddnc = DMAH_NUM_CHANNELSReset Value: 0x0*/
        uint32_t                     reserved_1 :  2; /* 7: 6, Reserveddnc = DMAH_NUM_CHANNELSIf dnc = 8, then this field is not present.Reset Value: 0x0*/
        uint32_t                    int_mask_we :  6; /*13: 8, Interrupt Mask Write Enable0 = write disabled1 = write enableddnc = DMAH_NUM_CHANNELSReset Value: 0x0*/
        uint32_t                     reserved_0 : 18; /*31:14, Reserveddnc = DMAH_NUM_CHANNELSReset Value: 0x0*/
    } bit_field;
} T_DMAC_MASKBLOCK_L;

//maskblock_H
typedef union
{
    uint32_t val;
    struct
    {
        uint32_t                     reserved_0 : 32; /*31: 0,                       Reserved*/
    } bit_field;
} T_DMAC_MASKBLOCK_H;

//masksrcTran_L
typedef union
{
    uint32_t val;
    struct
    {
        uint32_t                       int_mask :  6; /* 5: 0, Interrupt Mask0 = masked1 = unmaskeddnc = DMAH_NUM_CHANNELSReset Value: 0x0*/
        uint32_t                     reserved_1 :  2; /* 7: 6, Reserveddnc = DMAH_NUM_CHANNELSIf dnc = 8, then this field is not present.Reset Value: 0x0*/
        uint32_t                    int_mask_we :  6; /*13: 8, Interrupt Mask Write Enable0 = write disabled1 = write enableddnc = DMAH_NUM_CHANNELSReset Value: 0x0*/
        uint32_t                     reserved_0 : 18; /*31:14, Reserveddnc = DMAH_NUM_CHANNELSReset Value: 0x0*/
    } bit_field;
} T_DMAC_MASKSRCTRAN_L;

//masksrcTran_H
typedef union
{
    uint32_t val;
    struct
    {
        uint32_t                     reserved_0 : 32; /*31: 0,                       Reserved*/
    } bit_field;
} T_DMAC_MASKSRCTRAN_H;

//maskdstTran_L
typedef union
{
    uint32_t val;
    struct
    {
        uint32_t                       int_mask :  6; /* 5: 0, Interrupt Mask0 = masked1 = unmaskeddnc = DMAH_NUM_CHANNELSReset Value: 0x0*/
        uint32_t                     reserved_1 :  2; /* 7: 6, Reserveddnc = DMAH_NUM_CHANNELSIf dnc = 8, then this field is not present.Reset Value: 0x0*/
        uint32_t                    int_mask_we :  6; /*13: 8, Interrupt Mask Write Enable0 = write disabled1 = write enableddnc = DMAH_NUM_CHANNELSReset Value: 0x0*/
        uint32_t                     reserved_0 : 18; /*31:14, Reserveddnc = DMAH_NUM_CHANNELSReset Value: 0x0*/
    } bit_field;
} T_DMAC_MASKDSTTRAN_L;

//maskdstTran_H
typedef union
{
    uint32_t val;
    struct
    {
        uint32_t                     reserved_0 : 32; /*31: 0,                       Reserved*/
    } bit_field;
} T_DMAC_MASKDSTTRAN_H;

//maskerr_L
typedef union
{
    uint32_t val;
    struct
    {
        uint32_t                       int_mask :  6; /* 5: 0, Interrupt Mask0 = masked1 = unmaskeddnc = DMAH_NUM_CHANNELSReset Value: 0x0*/
        uint32_t                     reserved_1 :  2; /* 7: 6, Reserveddnc = DMAH_NUM_CHANNELSIf dnc = 8, then this field is not present.Reset Value: 0x0*/
        uint32_t                    int_mask_we :  6; /*13: 8, Interrupt Mask Write Enable0 = write disabled1 = write enableddnc = DMAH_NUM_CHANNELSReset Value: 0x0*/
        uint32_t                     reserved_0 : 18; /*31:14, Reserveddnc = DMAH_NUM_CHANNELSReset Value: 0x0*/
    } bit_field;
} T_DMAC_MASKERR_L;

//maskerr_H
typedef union
{
    uint32_t val;
    struct
    {
        uint32_t                     reserved_0 : 32; /*31: 0,                       Reserved*/
    } bit_field;
} T_DMAC_MASKERR_H;

//cleartfr_L
typedef union
{
    uint32_t val;
    struct
    {
        uint32_t                          clear :  6; /* 5: 0,                Interrupt clear*/
        uint32_t                     reserved_0 : 26; /*31: 6,                       Reserved*/
    } bit_field;
} T_DMAC_CLEARTFR_L;

//cleartfr_H
typedef union
{
    uint32_t val;
    struct
    {
        uint32_t                     reserved_0 : 32; /*31: 0,                       Reserved*/
    } bit_field;
} T_DMAC_CLEARTFR_H;

//clearblock_L
typedef union
{
    uint32_t val;
    struct
    {
        uint32_t                          clear :  6; /* 5: 0,                Interrupt clear*/
        uint32_t                     reserved_0 : 26; /*31: 6,                       Reserved*/
    } bit_field;
} T_DMAC_CLEARBLOCK_L;

//clearblock_H
typedef union
{
    uint32_t val;
    struct
    {
        uint32_t                     reserved_0 : 32; /*31: 0,                       Reserved*/
    } bit_field;
} T_DMAC_CLEARBLOCK_H;

//clearsrcTran_L
typedef union
{
    uint32_t val;
    struct
    {
        uint32_t                          clear :  6; /* 5: 0,                Interrupt clear*/
        uint32_t                     reserved_0 : 26; /*31: 6,                       Reserved*/
    } bit_field;
} T_DMAC_CLEARSRCTRAN_L;

//clearsrcTran_H
typedef union
{
    uint32_t val;
    struct
    {
        uint32_t                     reserved_0 : 32; /*31: 0,                       Reserved*/
    } bit_field;
} T_DMAC_CLEARSRCTRAN_H;

//cleardstTran_L
typedef union
{
    uint32_t val;
    struct
    {
        uint32_t                          clear :  6; /* 5: 0,                Interrupt clear*/
        uint32_t                     reserved_0 : 26; /*31: 6,                       Reserved*/
    } bit_field;
} T_DMAC_CLEARDSTTRAN_L;

//cleardstTran_H
typedef union
{
    uint32_t val;
    struct
    {
        uint32_t                     reserved_0 : 32; /*31: 0,                       Reserved*/
    } bit_field;
} T_DMAC_CLEARDSTTRAN_H;

//clearerr_L
typedef union
{
    uint32_t val;
    struct
    {
        uint32_t                          clear :  6; /* 5: 0,                Interrupt clear*/
        uint32_t                     reserved_0 : 26; /*31: 6,                       Reserved*/
    } bit_field;
} T_DMAC_CLEARERR_L;

//clearerr_H
typedef union
{
    uint32_t val;
    struct
    {
        uint32_t                     reserved_0 : 32; /*31: 0,                       Reserved*/
    } bit_field;
} T_DMAC_CLEARERR_H;

//statusInt_L
typedef union
{
    uint32_t val;
    struct
    {
        uint32_t                     reserved_1 :  1; /* 5: 5,                               */
        uint32_t                            tfr :  1; /* 0: 0, OR of the contents of StatusTfr register.*/
        uint32_t                          block :  1; /* 1: 1, OR of the contents of StatusBlock register.*/
        uint32_t                           srct :  1; /* 2: 2, OR of the contents of StatusSrcTran register.*/
        uint32_t                           dstt :  1; /* 3: 3, OR of the contents of StatusDst register.*/
        uint32_t                            err :  1; /* 4: 4, OR of the contents of StatusErr register.*/
        uint32_t                     reserved_0 : 26; /*31: 6,                       Reserved*/
    } bit_field;
} T_DMAC_STATUSINT_L;

//statusInt_H
typedef union
{
    uint32_t val;
    struct
    {
        uint32_t                     reserved_0 : 32; /*31: 0,                       Reserved*/
    } bit_field;
} T_DMAC_STATUSINT_H;

//reqSrcReg_L
typedef union
{
    uint32_t val;
    struct
    {
        uint32_t                        src_req :  6; /* 5: 0,                 Source request*/
        uint32_t                     reserved_1 :  2; /* 7: 6,                       Reserved*/
        uint32_t                     src_req_we :  6; /*13: 8, Source request write enable0 = write disabled1 = write enabled*/
        uint32_t                     reserved_0 : 18; /*31:14,                       Reserved*/
    } bit_field;
} T_DMAC_REQSRCREG_L;

//reqSrcReg_H
typedef union
{
    uint32_t val;
    struct
    {
        uint32_t                     reserved_0 : 32; /*31: 0,                       Reserved*/
    } bit_field;
} T_DMAC_REQSRCREG_H;

//reqDstReg_L
typedef union
{
    uint32_t val;
    struct
    {
        uint32_t                        dst_req :  6; /* 5: 0,            Destination request*/
        uint32_t                     reserved_1 :  2; /* 7: 6,                       Reserved*/
        uint32_t                     dst_req_we :  6; /*13: 8, Destination request write enable0 = write disabled1 = write enabled*/
        uint32_t                     reserved_0 : 18; /*31:14,                       Reserved*/
    } bit_field;
} T_DMAC_REQDSTREG_L;

//reqDstReg_H
typedef union
{
    uint32_t val;
    struct
    {
        uint32_t                     reserved_0 : 32; /*31: 0,                       Reserved*/
    } bit_field;
} T_DMAC_REQDSTREG_H;

//sglReqSrcReg_L
typedef union
{
    uint32_t val;
    struct
    {
        uint32_t                     src_sglreq :  6; /* 5: 0,          Source single request*/
        uint32_t                     reserved_1 :  2; /* 7: 6,                       Reserved*/
        uint32_t                  src_sglreq_we :  6; /*13: 8, Single write enable0 = write disabled1 = write enabled*/
        uint32_t                     reserved_0 : 18; /*31:14,                       Reserved*/
    } bit_field;
} T_DMAC_SGLREQSRCREG_L;

//sglReqSrcReg_H
typedef union
{
    uint32_t val;
    struct
    {
        uint32_t                     reserved_0 : 32; /*31: 0,                       Reserved*/
    } bit_field;
} T_DMAC_SGLREQSRCREG_H;

//sglReqDstReg_L
typedef union
{
    uint32_t val;
    struct
    {
        uint32_t                     dst_sglreq :  6; /* 5: 0, Destination single or burst request*/
        uint32_t                     reserved_1 :  2; /* 7: 6,                       Reserved*/
        uint32_t                  dst_sglreq_we :  6; /*13: 8, Destination write enable0 = write disabled1 = write enabled*/
        uint32_t                     reserved_0 : 18; /*31:14,                       Reserved*/
    } bit_field;
} T_DMAC_SGLREQDSTREG_L;

//sglReqDstReg_H
typedef union
{
    uint32_t val;
    struct
    {
        uint32_t                     reserved_0 : 32; /*31: 0,                       Reserved*/
    } bit_field;
} T_DMAC_SGLREQDSTREG_H;

//lstSrcReg_L
typedef union
{
    uint32_t val;
    struct
    {
        uint32_t                         lstsrc :  6; /* 5: 0, Source last transaction request0 = Not last transaction in current block1 = Last transaction in current block*/
        uint32_t                     reserved_1 :  2; /* 7: 6,                       Reserved*/
        uint32_t                      lstsrc_we :  6; /*13: 8, Source last transaction request write enable0 = write disabled1 = write enabled*/
        uint32_t                     reserved_0 : 18; /*31:14,                       Reserved*/
    } bit_field;
} T_DMAC_LSTSRCREG_L;

//lstSrcReg_H
typedef union
{
    uint32_t val;
    struct
    {
        uint32_t                     reserved_0 : 32; /*31: 0,                       Reserved*/
    } bit_field;
} T_DMAC_LSTSRCREG_H;

//lstDstReg_L
typedef union
{
    uint32_t val;
    struct
    {
        uint32_t                         lstdst :  6; /* 5: 0, Destination last transaction request0 = Not last transaction in current block1 = Last transaction in current block*/
        uint32_t                     reserved_1 :  2; /* 7: 6,                       Reserved*/
        uint32_t                      lstdst_we :  6; /*13: 8, Destination last transaction request write enable0 = write disabled1 = write enabled*/
        uint32_t                     reserved_0 : 18; /*31:14,                       Reserved*/
    } bit_field;
} T_DMAC_LSTDSTREG_L;

//lstDstReg_H
typedef union
{
    uint32_t val;
    struct
    {
        uint32_t                     reserved_0 : 32; /*31: 0,                       Reserved*/
    } bit_field;
} T_DMAC_LSTDSTREG_H;

//dmaCfgReg_L
typedef union
{
    uint32_t val;
    struct
    {
        uint32_t                         dma_en :  1; /* 0: 0, DW_ahb_dmac Enable bit.0 = DW_ahb_dmac Disabled1 = DW_ahb_dmac Enabled.*/
        uint32_t                     reserved_0 : 31; /*31: 1,                       Reserved*/
    } bit_field;
} T_DMAC_DMACFGREG_L;

//dmaCfgReg_H
typedef union
{
    uint32_t val;
    struct
    {
        uint32_t                     reserved_0 : 32; /*31: 0,                       Reserved*/
    } bit_field;
} T_DMAC_DMACFGREG_H;

//chEnReg_L
typedef union
{
    uint32_t val;
    struct
    {
        uint32_t                          ch_en :  6; /* 5: 0, Enables/Disables the channel. Setting this bit enables a channel;clearing this bit disables the channel.0 = Disable the Channel1 = Enable the ChannelThe ChEnReg.CH_EN bit is automatically cleared by hardware todisable the channel after the last AMBA transfer of the DMA transferto the destination has completed. Software can therefore poll this bitto determine when this channel is free for a new DMA transfer.*/
        uint32_t                     reserved_1 :  2; /* 7: 6,                       Reserved*/
        uint32_t                       ch_en_we :  6; /*13: 8,   Channel enable write enable.*/
        uint32_t                     reserved_0 : 18; /*31:14,                       Reserved*/
    } bit_field;
} T_DMAC_CHENREG_L;

//chEnReg_H
typedef union
{
    uint32_t val;
    struct
    {
        uint32_t                     reserved_0 : 32; /*31: 0,                       Reserved*/
    } bit_field;
} T_DMAC_CHENREG_H;

//dmaIdReg_L
typedef union
{
    uint32_t val;
    struct
    {
        uint32_t                    dmah_id_num : 32; /*31: 0, Hardcoded DW_ahb_dmac Peripheral ID*/
    } bit_field;
} T_DMAC_DMAIDREG_L;

//dmaIdReg_H
typedef union
{
    uint32_t val;
    struct
    {
        uint32_t                     reserved_0 : 32; /*31: 0,                       Reserved*/
    } bit_field;
} T_DMAC_DMAIDREG_H;

//dmaTestReg_L
typedef union
{
    uint32_t val;
    struct
    {
        uint32_t                    test_slv_if :  1; /* 0: 0, Puts the AHB slave interface into test mode. In this mode, thereadback value of the writable registers always matches the valuewritten. This bit does not allow writing to read-only registers.0 = Normal mode1 = Test mode*/
        uint32_t                     reserved_0 : 31; /*31: 1,                       Reserved*/
    } bit_field;
} T_DMAC_DMATESTREG_L;

//dmaTestReg_H
typedef union
{
    uint32_t val;
    struct
    {
        uint32_t                     reserved_0 : 32; /*31: 0,                       Reserved*/
    } bit_field;
} T_DMAC_DMATESTREG_H;

//dma_Comp_Params_6_L
typedef union
{
    uint32_t val;
    struct
    {
        uint32_t                     reserved_0 : 32; /*31: 0,                       Reserved*/
    } bit_field;
} T_DMAC_DMA_COMP_PARAMS_6_L;

//dma_Comp_Params_6_H
typedef union
{
    uint32_t val;
    struct
    {
        uint32_t                     reserved_0 : 32; /*31: 0,                       Reserved*/
    } bit_field;
} T_DMAC_DMA_COMP_PARAMS_6_H;

//dma_Comp_Params_5_L
typedef union
{
    uint32_t val;
    struct
    {
        uint32_t                     reserved_0 : 32; /*31: 0,                       Reserved*/
    } bit_field;
} T_DMAC_DMA_COMP_PARAMS_5_L;

//dma_Comp_Params_5_H
typedef union
{
    uint32_t val;
    struct
    {
        uint32_t                        ch5_dtw :  3; /* 2: 0, The value of this register is derived from the DMAH_CH5_DTWcoreConsultant parameter. For a description of this parameter,refer to page 129.0x0 = NO_HARDCODE(﹟)0x1 = 80x2 = 160x3 = 320x4 = 640x5 = 1280x6 = 2560x7 = reserved*/
        uint32_t                        ch5_stw :  3; /* 5: 3, The value of this register is derived from the DMAH_CH5_STWcoreConsultant parameter. For a description of this parameter,refer to page 129.0x0 = NO_HARDCODE(﹟)0x1 = 80x2 = 160x3 = 320x4 = 640x5 = 1280x6 = 2560x7 = reserved*/
        uint32_t                   ch5_stat_dst :  1; /* 6: 6, The value of this register is derived from theDMAH_CH5_STAT_DST coreConsultant parameter. For adescription of this parameter, refer to page 132.0x0 = FALSE(﹟)0x1 = TRUE*/
        uint32_t                   ch5_stat_src :  1; /* 7: 7, The value of this register is derived from theDMAH_CH5_STAT_SRC coreConsultant parameter. For adescription of this parameter, refer to page 132.0x0 = FALSE(﹟)0x1 = TRUE*/
        uint32_t                 ch5_dst_sca_en :  1; /* 8: 8, The value of this register is derived from theDMAH_CH5_DST_SCA_EN coreConsultant parameter. For adescription of this parameter, refer to page 133.0x0 = FALSE(﹟)0x1 = TRUE*/
        uint32_t                 ch5_src_gat_en :  1; /* 9: 9, The value of this register is derived from theDMAH_CH5_SRC_GAT_EN coreConsultant parameter. For adescription of this parameter, refer to page 133.0x0 = FALSE(﹟)0x1 = TRUE*/
        uint32_t                    ch5_lock_en :  1; /*10:10, The value of this register is derived from theDMAH_CH5_LOCK_EN coreConsultant parameter. For adescription of this parameter, refer to page 128.0x0 = FALSE(﹟)0x1 = TRUE*/
        uint32_t               ch5_multi_blk_en :  1; /*11:11, The value of this register is derived from theDMAH_CH5_MULTI_BLK_EN coreConsultant parameter. For adescription of this parameter, refer to page 130.0x0 = FALSE(﹟)0x1 = TRUE*/
        uint32_t                  ch5_ctl_wb_en :  1; /*12:12, The value of this register is derived from theDMAH_CH5_CTL_WB_EN coreConsultant parameter. For adescription of this parameter, refer to page 133.0x0 = FALSE(﹟)0x1 = TRUE*/
        uint32_t                     ch5_hc_llp :  1; /*13:13, The value of this register is derived from theDMAH_CH5_HC_LLP coreConsultant parameter. For adescription of this parameter, refer to page 131.0x0 = FALSE0x1 = TRUE(﹟)*/
        uint32_t                         ch5_fc :  2; /*15:14, The value of this register is derived from the DMAH_CH5_FCcoreConsultant parameter. For a description of this parameter,refer to page 128.0x0 = DMA(﹟)0x1 = SRC0x2 = DST0x3 = ANY*/
        uint32_t              ch5_max_mult_size :  3; /*18:16, The value of this register is derived from theDMAH_CH5_MULT_SIZE coreConsultant parameter. For adescription of this parameter, refer to page 127.0x0 = 4(﹟)0x1 = 80x2 = 160x3 = 320x4 = 640x5 = 1280x6 = 2560x7 = reserved*/
        uint32_t                        ch5_dms :  3; /*21:19, The value of this register is derived from the DMAH_CH5_DMScoreConsultant parameter. For a description of this parameter,refer to page 129.0x0 = MASTER_1(﹟)0x1 = MASTER_20x2 = MASTER_30x3 = MASTER_40x4 = NO_HARDCODE*/
        uint32_t                        ch5_lms :  3; /*24:22, The value of this register is derived from the DMAH_CH5_LMScoreConsultant parameter. For a description of this parameter,refer to page 132.0x0 = MASTER_1(﹟)0x1 = MASTER_20x2 = MASTER_30x3 = MASTER_40x4 = NO_HARDCODE*/
        uint32_t                        ch5_sms :  3; /*27:25, The value of this register is derived from the DMAH_CH5_SMScoreConsultant parameter. For a description of this parameter,refer to page 129.0x0 = MASTER_1(﹟)0x1 = MASTER_20x2 = MASTER_30x3 = MASTER_40x4 = NO_HARDCODE*/
        uint32_t                 ch5_fifo_depth :  3; /*30:28, The value of this register is derived from theDMAH_CH5_FIFO_DEPTH coreConsultant parameter. For adescription of this parameter, refer to page 127.0x0 = 80x1 = 16(﹟)0x2 = 320x3 = 640x4 = 128*/
        uint32_t                     reserved_0 :  1; /*31:31,                       Reserved*/
    } bit_field;
} T_DMAC_DMA_COMP_PARAMS_5_H;

//dma_Comp_Params_4_L
typedef union
{
    uint32_t val;
    struct
    {
        uint32_t                        ch4_dtw :  3; /* 2: 0, The value of this register is derived from the DMAH_CH4_DTWcoreConsultant parameter. For a description of this parameter,refer to page 129.0x0 = NO_HARDCODE(﹟)0x1 = 80x2 = 160x3 = 320x4 = 640x5 = 1280x6 = 2560x7 = reserved*/
        uint32_t                        ch4_stw :  3; /* 5: 3, The value of this register is derived from the DMAH_CH4_STWcoreConsultant parameter. For a description of this parameter,refer to page 129.0x0 = NO_HARDCODE(﹟)0x1 = 80x2 = 160x3 = 320x4 = 640x5 = 1280x6 = 2560x7 = reserved*/
        uint32_t                   ch4_stat_dst :  1; /* 6: 6, The value of this register is derived from theDMAH_CH4_STAT_DST coreConsultant parameter. For adescription of this parameter, refer to page 132.0x0 = FALSE(﹟)0x1 = TRUE*/
        uint32_t                   ch4_stat_src :  1; /* 7: 7, The value of this register is derived from theDMAH_CH4_STAT_SRC coreConsultant parameter. For adescription of this parameter, refer to page 132.0x0 = FALSE(﹟)0x1 = TRUE*/
        uint32_t                 ch4_dst_sca_en :  1; /* 8: 8, The value of this register is derived from theDMAH_CH4_DST_SCA_EN coreConsultant parameter. For adescription of this parameter, refer to page 133.0x0 = FALSE(﹟)0x1 = TRUE*/
        uint32_t                 ch4_src_gat_en :  1; /* 9: 9, The value of this register is derived from theDMAH_CH4_SRC_GAT_EN coreConsultant parameter. For adescription of this parameter, refer to page 133.0x0 = FALSE(﹟)0x1 = TRUE*/
        uint32_t                    ch4_lock_en :  1; /*10:10, The value of this register is derived from theDMAH_CH4_LOCK_EN coreConsultant parameter. For adescription of this parameter, refer to page 128.0x0 = FALSE(﹟)0x1 = TRUE*/
        uint32_t               ch4_multi_blk_en :  1; /*11:11, The value of this register is derived from theDMAH_CH4_MULTI_BLK_EN coreConsultant parameter. For adescription of this parameter, refer to page 130.0x0 = FALSE(﹟)0x1 = TRUE*/
        uint32_t                  ch4_ctl_wb_en :  1; /*12:12, The value of this register is derived from theDMAH_CH4_CTL_WB_EN coreConsultant parameter. For adescription of this parameter, refer to page 133.0x0 = FALSE(﹟)0x1 = TRUE*/
        uint32_t                     ch4_hc_llp :  1; /*13:13, The value of this register is derived from theDMAH_CH4_HC_LLP coreConsultant parameter. For adescription of this parameter, refer to page 131.0x0 = FALSE0x1 = TRUE(﹟)*/
        uint32_t                         ch4_fc :  2; /*15:14, The value of this register is derived from the DMAH_CH4_FCcoreConsultant parameter. For a description of this parameter,refer to page 128.0x0 = DMA(﹟)0x1 = SRC0x2 = DST0x3 = ANY*/
        uint32_t              ch4_max_mult_size :  3; /*18:16, The value of this register is derived from theDMAH_CH4_MULT_SIZE coreConsultant parameter. For adescription of this parameter, refer to page 127.0x0 = 4(﹟)0x1 = 80x2 = 160x3 = 320x4 = 640x5 = 1280x6 = 2560x7 = reserved*/
        uint32_t                        ch4_dms :  3; /*21:19, The value of this register is derived from the DMAH_CH4_DMScoreConsultant parameter. For a description of this parameter,refer to page 129.0x0 = MASTER_1(﹟)0x1 = MASTER_20x2 = MASTER_30x3 = MASTER_40x4 = NO_HARDCODE*/
        uint32_t                        ch4_lms :  3; /*24:22, The value of this register is derived from the DMAH_CH4_LMScoreConsultant parameter. For a description of this parameter,refer to page 132.0x0 = MASTER_1(﹟)0x1 = MASTER_20x2 = MASTER_30x3 = MASTER_40x4 = NO_HARDCODE*/
        uint32_t                        ch4_sms :  3; /*27:25, The value of this register is derived from the DMAH_CH4_SMScoreConsultant parameter. For a description of this parameter,refer to page 129.0x0 = MASTER_1(﹟)0x1 = MASTER_20x2 = MASTER_30x3 = MASTER_40x4 = NO_HARDCODE*/
        uint32_t                 ch4_fifo_depth :  3; /*30:28, The value of this register is derived from theDMAH_CH4_FIFO_DEPTH coreConsultant parameter. For adescription of this parameter, refer to page 127.0x0 = 80x1 = 16(﹟)0x2 = 320x3 = 640x4 = 128*/
        uint32_t                     reserved_0 :  1; /*31:31,                       Reserved*/
    } bit_field;
} T_DMAC_DMA_COMP_PARAMS_4_L;

//dma_Comp_Params_4_H
typedef union
{
    uint32_t val;
    struct
    {
        uint32_t                        ch3_dtw :  3; /* 2: 0, The value of this register is derived from the DMAH_CH3_DTWcoreConsultant parameter. For a description of this parameter,refer to page 129.0x0 = NO_HARDCODE(﹟)0x1 = 80x2 = 160x3 = 320x4 = 640x5 = 1280x6 = 2560x7 = reserved*/
        uint32_t                        ch3_stw :  3; /* 5: 3, The value of this register is derived from the DMAH_CH3_STWcoreConsultant parameter. For a description of this parameter,refer to page 129.0x0 = NO_HARDCODE(﹟)0x1 = 80x2 = 160x3 = 320x4 = 640x5 = 1280x6 = 2560x7 = reserved*/
        uint32_t                   ch3_stat_dst :  1; /* 6: 6, The value of this register is derived from theDMAH_CH3_STAT_DST coreConsultant parameter. For adescription of this parameter, refer to page 132.0x0 = FALSE(﹟)0x1 = TRUE*/
        uint32_t                   ch3_stat_src :  1; /* 7: 7, The value of this register is derived from theDMAH_CH3_STAT_SRC coreConsultant parameter. For adescription of this parameter, refer to page 132.0x0 = FALSE(﹟)0x1 = TRUE*/
        uint32_t                 ch3_dst_sca_en :  1; /* 8: 8, The value of this register is derived from theDMAH_CH3_DST_SCA_EN coreConsultant parameter. For adescription of this parameter, refer to page 133.0x0 = FALSE(﹟)0x1 = TRUE*/
        uint32_t                 ch3_src_gat_en :  1; /* 9: 9, The value of this register is derived from theDMAH_CH3_SRC_GAT_EN coreConsultant parameter. For adescription of this parameter, refer to page 133.0x0 = FALSE(﹟)0x1 = TRUE*/
        uint32_t                    ch3_lock_en :  1; /*10:10, The value of this register is derived from theDMAH_CH3_LOCK_EN coreConsultant parameter. For adescription of this parameter, refer to page 128.0x0 = FALSE(﹟)0x1 = TRUE*/
        uint32_t               ch3_multi_blk_en :  1; /*11:11, The value of this register is derived from theDMAH_CH3_MULTI_BLK_EN coreConsultant parameter. For adescription of this parameter, refer to page 130.0x0 = FALSE(﹟)0x1 = TRUE*/
        uint32_t                  ch3_ctl_wb_en :  1; /*12:12, The value of this register is derived from theDMAH_CH3_CTL_WB_EN coreConsultant parameter. For adescription of this parameter, refer to page 133.0x0 = FALSE(﹟)0x1 = TRUE*/
        uint32_t                     ch3_hc_llp :  1; /*13:13, The value of this register is derived from theDMAH_CH3_HC_LLP coreConsultant parameter. For adescription of this parameter, refer to page 131.0x0 = FALSE0x1 = TRUE(﹟)*/
        uint32_t                         ch3_fc :  2; /*15:14, The value of this register is derived from the DMAH_CH3_FCcoreConsultant parameter. For a description of this parameter,refer to page 128.0x0 = DMA(﹟)0x1 = SRC0x2 = DST0x3 = ANY*/
        uint32_t              ch3_max_mult_size :  3; /*18:16, The value of this register is derived from theDMAH_CH3_MULT_SIZE coreConsultant parameter. For adescription of this parameter, refer to page 127.0x0 = 4(﹟)0x1 = 80x2 = 160x3 = 320x4 = 640x5 = 1280x6 = 2560x7 = reserved*/
        uint32_t                        ch3_dms :  3; /*21:19, The value of this register is derived from the DMAH_CH3_DMScoreConsultant parameter. For a description of this parameter,refer to page 129.0x0 = MASTER_1(﹟)0x1 = MASTER_20x2 = MASTER_30x3 = MASTER_40x4 = NO_HARDCODE*/
        uint32_t                        ch3_lms :  3; /*24:22, The value of this register is derived from the DMAH_CH3_LMScoreConsultant parameter. For a description of this parameter,refer to page 132.0x0 = MASTER_1(﹟)0x1 = MASTER_20x2 = MASTER_30x3 = MASTER_40x4 = NO_HARDCODE*/
        uint32_t                        ch3_sms :  3; /*27:25, The value of this register is derived from the DMAH_CH3_SMScoreConsultant parameter. For a description of this parameter,refer to page 129.0x0 = MASTER_1(﹟)0x1 = MASTER_20x2 = MASTER_30x3 = MASTER_40x4 = NO_HARDCODE*/
        uint32_t                 ch3_fifo_depth :  3; /*30:28, The value of this register is derived from theDMAH_CH3_FIFO_DEPTH coreConsultant parameter. For adescription of this parameter, refer to page 127.0x0 = 80x1 = 16(﹟)0x2 = 320x3 = 640x4 = 128*/
        uint32_t                     reserved_0 :  1; /*31:31,                       Reserved*/
    } bit_field;
} T_DMAC_DMA_COMP_PARAMS_4_H;

//dma_Comp_Params_3_L
typedef union
{
    uint32_t val;
    struct
    {
        uint32_t                        ch2_dtw :  3; /* 2: 0, The value of this register is derived from the DMAH_CH2_DTWcoreConsultant parameter. For a description of this parameter,refer to page 129.0x0 = NO_HARDCODE(﹟)0x1 = 80x2 = 160x3 = 320x4 = 640x5 = 1280x6 = 2560x7 = reserved*/
        uint32_t                        ch2_stw :  3; /* 5: 3, The value of this register is derived from the DMAH_CH2_STWcoreConsultant parameter. For a description of this parameter,refer to page 129.0x0 = NO_HARDCODE(﹟)0x1 = 80x2 = 160x3 = 320x4 = 640x5 = 1280x6 = 2560x7 = reserved*/
        uint32_t                   ch2_stat_dst :  1; /* 6: 6, The value of this register is derived from theDMAH_CH2_STAT_DST coreConsultant parameter. For adescription of this parameter, refer to page 132.0x0 = FALSE(﹟)0x1 = TRUE*/
        uint32_t                   ch2_stat_src :  1; /* 7: 7, The value of this register is derived from theDMAH_CH2_STAT_SRC coreConsultant parameter. For adescription of this parameter, refer to page 132.0x0 = FALSE(﹟)0x1 = TRUE*/
        uint32_t                 ch2_dst_sca_en :  1; /* 8: 8, The value of this register is derived from theDMAH_CH2_DST_SCA_EN coreConsultant parameter. For adescription of this parameter, refer to page 133.0x0 = FALSE(﹟)0x1 = TRUE*/
        uint32_t                 ch2_src_gat_en :  1; /* 9: 9, The value of this register is derived from theDMAH_CH2_SRC_GAT_EN coreConsultant parameter. For adescription of this parameter, refer to page 133.0x0 = FALSE(﹟)0x1 = TRUE*/
        uint32_t                    ch2_lock_en :  1; /*10:10, The value of this register is derived from theDMAH_CH2_LOCK_EN coreConsultant parameter. For adescription of this parameter, refer to page 128.0x0 = FALSE(﹟)0x1 = TRUE*/
        uint32_t               ch2_multi_blk_en :  1; /*11:11, The value of this register is derived from theDMAH_CH2_MULTI_BLK_EN coreConsultant parameter. For adescription of this parameter, refer to page 130.0x0 = FALSE(﹟)0x1 = TRUE*/
        uint32_t                  ch2_ctl_wb_en :  1; /*12:12, The value of this register is derived from theDMAH_CH2_CTL_WB_EN coreConsultant parameter. For adescription of this parameter, refer to page 133.0x0 = FALSE(﹟)0x1 = TRUE*/
        uint32_t                     ch2_hc_llp :  1; /*13:13, The value of this register is derived from theDMAH_CH2_HC_LLP coreConsultant parameter. For adescription of this parameter, refer to page 131.0x0 = FALSE0x1 = TRUE(﹟)*/
        uint32_t                         ch2_fc :  2; /*15:14, The value of this register is derived from the DMAH_CH2_FCcoreConsultant parameter. For a description of this parameter,refer to page 128.0x0 = DMA(﹟)0x1 = SRC0x2 = DST0x3 = ANY*/
        uint32_t              ch2_max_mult_size :  3; /*18:16, The value of this register is derived from theDMAH_CH2_MULT_SIZE coreConsultant parameter. For adescription of this parameter, refer to page 127.0x0 = 4(﹟)0x1 = 80x2 = 160x3 = 320x4 = 640x5 = 1280x6 = 2560x7 = reserved*/
        uint32_t                        ch2_dms :  3; /*21:19, The value of this register is derived from the DMAH_CH2_DMScoreConsultant parameter. For a description of this parameter,refer to page 129.0x0 = MASTER_1(﹟)0x1 = MASTER_20x2 = MASTER_30x3 = MASTER_40x4 = NO_HARDCODE*/
        uint32_t                        ch2_lms :  3; /*24:22, The value of this register is derived from the DMAH_CH2_LMScoreConsultant parameter. For a description of this parameter,refer to page 132.0x0 = MASTER_1(﹟)0x1 = MASTER_20x2 = MASTER_30x3 = MASTER_40x4 = NO_HARDCODE*/
        uint32_t                        ch2_sms :  3; /*27:25, The value of this register is derived from the DMAH_CH2_SMScoreConsultant parameter. For a description of this parameter,refer to page 129.0x0 = MASTER_1(﹟)0x1 = MASTER_20x2 = MASTER_30x3 = MASTER_40x4 = NO_HARDCODE*/
        uint32_t                 ch2_fifo_depth :  3; /*30:28, The value of this register is derived from theDMAH_CH2_FIFO_DEPTH coreConsultant parameter. For adescription of this parameter, refer to page 127.0x0 = 80x1 = 16(﹟)0x2 = 320x3 = 640x4 = 128*/
        uint32_t                     reserved_0 :  1; /*31:31,                       Reserved*/
    } bit_field;
} T_DMAC_DMA_COMP_PARAMS_3_L;

//dma_Comp_Params_3_H
typedef union
{
    uint32_t val;
    struct
    {
        uint32_t                        ch1_dtw :  3; /* 2: 0, The value of this register is derived from the DMAH_CH1_DTWcoreConsultant parameter. For a description of this parameter,refer to page 129.0x0 = NO_HARDCODE(﹟)0x1 = 80x2 = 160x3 = 320x4 = 640x5 = 1280x6 = 2560x7 = reserved*/
        uint32_t                        ch1_stw :  3; /* 5: 3, The value of this register is derived from the DMAH_CH1_STWcoreConsultant parameter. For a description of this parameter,refer to page 129.0x0 = NO_HARDCODE(﹟)0x1 = 80x2 = 160x3 = 320x4 = 640x5 = 1280x6 = 2560x7 = reserved*/
        uint32_t                   ch1_stat_dst :  1; /* 6: 6, The value of this register is derived from theDMAH_CH1_STAT_DST coreConsultant parameter. For adescription of this parameter, refer to page 132.0x0 = FALSE(﹟)0x1 = TRUE*/
        uint32_t                   ch1_stat_src :  1; /* 7: 7, The value of this register is derived from theDMAH_CH1_STAT_SRC coreConsultant parameter. For adescription of this parameter, refer to page 132.0x0 = FALSE(﹟)0x1 = TRUE*/
        uint32_t                 ch1_dst_sca_en :  1; /* 8: 8, The value of this register is derived from theDMAH_CH1_DST_SCA_EN coreConsultant parameter. For adescription of this parameter, refer to page 133.0x0 = FALSE(﹟)0x1 = TRUE*/
        uint32_t                 ch1_src_gat_en :  1; /* 9: 9, The value of this register is derived from theDMAH_CH1_SRC_GAT_EN coreConsultant parameter. For adescription of this parameter, refer to page 133.0x0 = FALSE(﹟)0x1 = TRUE*/
        uint32_t                    ch1_lock_en :  1; /*10:10, The value of this register is derived from theDMAH_CH1_LOCK_EN coreConsultant parameter. For adescription of this parameter, refer to page 128.0x0 = FALSE(﹟)0x1 = TRUE*/
        uint32_t               ch1_multi_blk_en :  1; /*11:11, The value of this register is derived from theDMAH_CH1_MULTI_BLK_EN coreConsultant parameter. For adescription of this parameter, refer to page 130.0x0 = FALSE(﹟)0x1 = TRUE*/
        uint32_t                  ch1_ctl_wb_en :  1; /*12:12, The value of this register is derived from theDMAH_CH1_CTL_WB_EN coreConsultant parameter. For adescription of this parameter, refer to page 133.0x0 = FALSE(﹟)0x1 = TRUE*/
        uint32_t                     ch1_hc_llp :  1; /*13:13, The value of this register is derived from theDMAH_CH1_HC_LLP coreConsultant parameter. For adescription of this parameter, refer to page 131.0x0 = FALSE0x1 = TRUE(﹟)*/
        uint32_t                         ch1_fc :  2; /*15:14, The value of this register is derived from the DMAH_CH1_FCcoreConsultant parameter. For a description of this parameter,refer to page 128.0x0 = DMA(﹟)0x1 = SRC0x2 = DST0x3 = ANY*/
        uint32_t              ch1_max_mult_size :  3; /*18:16, The value of this register is derived from theDMAH_CH1_MULT_SIZE coreConsultant parameter. For adescription of this parameter, refer to page 127.0x0 = 4(﹟)0x1 = 80x2 = 160x3 = 320x4 = 640x5 = 1280x6 = 2560x7 = reserved*/
        uint32_t                        ch1_dms :  3; /*21:19, The value of this register is derived from the DMAH_CH1_DMScoreConsultant parameter. For a description of this parameter,refer to page 129.0x0 = MASTER_1(﹟)0x1 = MASTER_20x2 = MASTER_30x3 = MASTER_40x4 = NO_HARDCODE*/
        uint32_t                        ch1_lms :  3; /*24:22, The value of this register is derived from the DMAH_CH1_LMScoreConsultant parameter. For a description of this parameter,refer to page 132.0x0 = MASTER_1(﹟)0x1 = MASTER_20x2 = MASTER_30x3 = MASTER_40x4 = NO_HARDCODE*/
        uint32_t                        ch1_sms :  3; /*27:25, The value of this register is derived from the DMAH_CH1_SMScoreConsultant parameter. For a description of this parameter,refer to page 129.0x0 = MASTER_1(﹟)0x1 = MASTER_20x2 = MASTER_30x3 = MASTER_40x4 = NO_HARDCODE*/
        uint32_t                 ch1_fifo_depth :  3; /*30:28, The value of this register is derived from theDMAH_CH1_FIFO_DEPTH coreConsultant parameter. For adescription of this parameter, refer to page 127.0x0 = 80x1 = 16(﹟)0x2 = 320x3 = 640x4 = 128*/
        uint32_t                     reserved_0 :  1; /*31:31,                       Reserved*/
    } bit_field;
} T_DMAC_DMA_COMP_PARAMS_3_H;

//dma_Comp_Params_2_L
typedef union
{
    uint32_t val;
    struct
    {
        uint32_t                        ch0_dtw :  3; /* 2: 0, The value of this register is derived from the DMAH_CH0_DTWcoreConsultant parameter. For a description of this parameter,refer to page 129.0x0 = NO_HARDCODE(﹟)0x1 = 80x2 = 160x3 = 320x4 = 640x5 = 1280x6 = 2560x7 = reserved*/
        uint32_t                        ch0_stw :  3; /* 5: 3, The value of this register is derived from the DMAH_CH0_STWcoreConsultant parameter. For a description of this parameter,refer to page 129.0x0 = NO_HARDCODE(﹟)0x1 = 80x2 = 160x3 = 320x4 = 640x5 = 1280x6 = 2560x7 = reserved*/
        uint32_t                   ch0_stat_dst :  1; /* 6: 6, The value of this register is derived from theDMAH_CH0_STAT_DST coreConsultant parameter. For adescription of this parameter, refer to page 132.0x0 = FALSE(﹟)0x1 = TRUE*/
        uint32_t                   ch0_stat_src :  1; /* 7: 7, The value of this register is derived from theDMAH_CH0_STAT_SRC coreConsultant parameter. For adescription of this parameter, refer to page 132.0x0 = FALSE(﹟)0x1 = TRUE*/
        uint32_t                 ch0_dst_sca_en :  1; /* 8: 8, The value of this register is derived from theDMAH_CH0_DST_SCA_EN coreConsultant parameter. For adescription of this parameter, refer to page 133.0x0 = FALSE(﹟)0x1 = TRUE*/
        uint32_t                 ch0_src_gat_en :  1; /* 9: 9, The value of this register is derived from theDMAH_CH0_SRC_GAT_EN coreConsultant parameter. For adescription of this parameter, refer to page 133.0x0 = FALSE(﹟)0x1 = TRUE*/
        uint32_t                    ch0_lock_en :  1; /*10:10, The value of this register is derived from theDMAH_CH0_LOCK_EN coreConsultant parameter. For adescription of this parameter, refer to page 128.0x0 = FALSE(﹟)0x1 = TRUE*/
        uint32_t               ch0_multi_blk_en :  1; /*11:11, The value of this register is derived from theDMAH_CH0_MULTI_BLK_EN coreConsultant parameter. For adescription of this parameter, refer to page 130.0x0 = FALSE(﹟)0x1 = TRUE*/
        uint32_t                  ch0_ctl_wb_en :  1; /*12:12, The value of this register is derived from theDMAH_CH0_CTL_WB_EN coreConsultant parameter. For adescription of this parameter, refer to page 133.0x0 = FALSE(﹟)0x1 = TRUE*/
        uint32_t                     ch0_hc_llp :  1; /*13:13, The value of this register is derived from theDMAH_CH0_HC_LLP coreConsultant parameter. For adescription of this parameter, refer to page 131.0x0 = FALSE0x1 = TRUE(﹟)*/
        uint32_t                         ch0_fc :  2; /*15:14, The value of this register is derived from the DMAH_CH0_FCcoreConsultant parameter. For a description of this parameter,refer to page 128.0x0 = DMA(﹟)0x1 = SRC0x2 = DST0x3 = ANY*/
        uint32_t              ch0_max_mult_size :  3; /*18:16, The value of this register is derived from theDMAH_CH0_MULT_SIZE coreConsultant parameter. For adescription of this parameter, refer to page 127.0x0 = 4(﹟)0x1 = 80x2 = 160x3 = 320x4 = 640x5 = 1280x6 = 2560x7 = reserved*/
        uint32_t                        ch0_dms :  3; /*21:19, The value of this register is derived from the DMAH_CH0_DMScoreConsultant parameter. For a description of this parameter,refer to page 129.0x0 = MASTER_1(﹟)0x1 = MASTER_20x2 = MASTER_30x3 = MASTER_40x4 = NO_HARDCODE*/
        uint32_t                        ch0_lms :  3; /*24:22, The value of this register is derived from the DMAH_CH0_LMScoreConsultant parameter. For a description of this parameter,refer to page 132.0x0 = MASTER_1(﹟)0x1 = MASTER_20x2 = MASTER_30x3 = MASTER_40x4 = NO_HARDCODE*/
        uint32_t                        ch0_sms :  3; /*27:25, The value of this register is derived from the DMAH_CH0_SMScoreConsultant parameter. For a description of this parameter,refer to page 129.0x0 = MASTER_1(﹟)0x1 = MASTER_20x2 = MASTER_30x3 = MASTER_40x4 = NO_HARDCODE*/
        uint32_t                 ch0_fifo_depth :  3; /*30:28, The value of this register is derived from theDMAH_CH0_FIFO_DEPTH coreConsultant parameter. For adescription of this parameter, refer to page 127.0x0 = 80x1 = 16(﹟)0x2 = 320x3 = 640x4 = 128*/
        uint32_t                     reserved_0 :  1; /*31:31,                       Reserved*/
    } bit_field;
} T_DMAC_DMA_COMP_PARAMS_2_L;

//dma_Comp_Params_2_H
typedef union
{
    uint32_t val;
    struct
    {
        uint32_t             ch0_multi_blk_type :  4; /* 3: 0,                  Same as above*/
        uint32_t             ch1_multi_blk_type :  4; /* 7: 4,                  Same as above*/
        uint32_t             ch2_multi_blk_type :  4; /*11: 8,                  Same as above*/
        uint32_t             ch3_multi_blk_type :  4; /*15:12,                  Same as above*/
        uint32_t             ch4_multi_blk_type :  4; /*19:16,                  Same as above*/
        uint32_t             ch5_multi_blk_type :  4; /*23:20,                  Same as above*/
        uint32_t             ch6_multi_blk_type :  4; /*27:24,                  Same as above*/
        uint32_t             ch7_multi_blk_type :  4; /*31:28, The values of these bit fields are derived from theDMAH_CHx_MULTI_BLK_TYPE coreConsultant parameter. For adescription of these parameters, refer to page 131.0x0 = NO_HARDCODE(﹟)0x1 = CONT_RELOAD0x2 = RELOAD_CONT0x3 = RELOAD_RELOAD0x4 = CONT_LLP0x5 = RELOAD_LLP0x6 = LLP_CONT0x7 = LLP_RELOAD0x8 = LLP_LLP*/
    } bit_field;
} T_DMAC_DMA_COMP_PARAMS_2_H;

//dma_Comp_Params_1_L
typedef union
{
    uint32_t val;
    struct
    {
        uint32_t               ch0_max_blk_size :  4; /* 3: 0,                  Same as above*/
        uint32_t               ch1_max_blk_size :  4; /* 7: 4,                  Same as above*/
        uint32_t               ch2_max_blk_size :  4; /*11: 8,                  Same as above*/
        uint32_t               ch3_max_blk_size :  4; /*15:12,                  Same as above*/
        uint32_t               ch4_max_blk_size :  4; /*19:16,                  Same as above*/
        uint32_t               ch5_max_blk_size :  4; /*23:20, The values of these bit fields are derived from theDMAH_CHx_MAX_BLK_SIZE coreConsultant parameter. Fora description of these parameters, refer to page 128.0x0 = 30x1 = 70x2 =150x3 = 310x4 = 630x5 = 1270x6 = 255(﹟)0x7 = 5110x8 = 10230x9 = 20470xa = 4095*/
        uint32_t               ch6_max_blk_size :  4; /*27:24,                  Same as above*/
        uint32_t               ch7_max_blk_size :  4; /*31:28, The values of these bit fields are derived from theDMAH_CHx_MAX_BLK_SIZE coreConsultant parameter. Fora description of these parameters, refer to page 128.0x0 = 30x1 = 70x2 =150x3 = 31(﹟)0x4 = 630x5 = 1270x6 = 2550x7 = 5110x8 = 10230x9 = 20470xa = 4095*/
    } bit_field;
} T_DMAC_DMA_COMP_PARAMS_1_L;

//dma_Comp_Params_1_H
typedef union
{
    uint32_t val;
    struct
    {
        uint32_t                     big_endian :  1; /* 0: 0, The value of this register is derived from theDMAH_BIG_ENDIAN coreConsultant parameter. For adescription of this parameter, refer to page 124.0 = FALSE(﹟)1 = TRUE*/
        uint32_t                        intr_io :  2; /* 2: 1, The value of this register is derived from the DMAH_INTR_IOcoreConsultant parameter. For a description of this parameter,refer to page 121.0x0 = ALL0x1 = TYPE0x2 = COMBINED(﹟)0x3 = reserved*/
        uint32_t                         mabrst :  1; /* 3: 3, The value of this register is derived from the DMAH_MABRSTcoreConsultant parameter. For a description of this parameter,refer to page 121.0 = FALSE1 = TRUE(﹟)*/
        uint32_t                     reserved_1 :  4; /* 7: 4,                       Reserved*/
        uint32_t                   num_channels :  3; /*10: 8, The value of this register is derived from theDMAH_NUM_CHANNELS coreConsultant parameter. For adescription of this parameter, refer to page 120.0x0 = 1to0x7 = 8*/
        uint32_t              number_master_int :  2; /*12:11, The value of this register is derived from theDMAH_NUM_MASTER_INT coreConsultant parameter. For adescription of this parameter, refer to page 120.0x0 = 1(﹟)to0x3 = 4*/
        uint32_t                  s_hdata_width :  2; /*14:13, The value of this register is derived from theDMAH_S_HDATA_WIDTH coreConsultant parameter. For adescription of this parameter, refer to page 126.0x0 = 32 bits(﹟)0x1 = 64 bits0x2 = 128 bits0x3 = 256 bits*/
        uint32_t                 m4_hdata_width :  2; /*16:15, The value of this register is derived from theDMAH_M4_HDATA_WIDTH coreConsultant parameter. For adescription of this parameter, refer to page 126.0x0 = 32 bits(﹟)0x1 = 64 bits0x2 = 128 bits0x3 = 256 bits*/
        uint32_t                 m3_hdata_width :  2; /*18:17, The value of this register is derived from theDMAH_M3_HDATA_WIDTH coreConsultant parameter. For adescription of this parameter, refer to page 126.0x0 = 32 bits(﹟)0x1 = 64 bits0x2 = 128 bits0x3 = 256 bits*/
        uint32_t                 m2_hdata_width :  2; /*20:19, The value of this register is derived from theDMAH_M2_HDATA_WIDTH coreConsultant parameter. For adescription of this parameter, refer to page 126.0x0 = 32 bits(﹟)0x1 = 64 bits0x2 = 128 bits0x3 = 256 bits*/
        uint32_t                 m1_hdata_width :  2; /*22:21, The value of this register is derived from theDMAH_M1_HDATA_WIDTH coreConsultant parameter. For adescription of this parameter, refer to page 126.0x0 = 32 bits(﹟)0x1 = 64 bits0x2 = 128 bits0x3 = 256 bits*/
        uint32_t                     num_hs_int :  5; /*27:23, The value of this register is derived from theDMAH_NUM_HS_INT coreConsultant parameter. For adescription of this parameter, refer to page 120.0x00 = 0to0x10 = 16(﹟)*/
        uint32_t             add_encoded_params :  1; /*28:28, The value of this register is derived from theDMAH_ADD_ENCODED_PARAMS coreConsultant parameter.For a description of this parameter, refer to page 122.0 = FALSE1 = TRUE(﹟)*/
        uint32_t           static_endian_select :  1; /*29:29, The value of this register is derived from theDMAH_STATIC_ENDIAN_SELECT coreConsultant parameter.For a description of this parameter, refer to page 123.0 = FALSE1 = TRUE(﹟)*/
        uint32_t                     reserved_0 :  2; /*31:30,                       Reserved*/
    } bit_field;
} T_DMAC_DMA_COMP_PARAMS_1_H;

//dmaCompIdReg_L
typedef union
{
    uint32_t val;
    struct
    {
        uint32_t                  dma_comp_type : 32; /*31: 0, Designware Component Type number= 0x44_57_11_10. This assignedunique hex value is constant and isderived from the two ASCII letters※DW§ followed by a 32-bit unsignednumber.*/
    } bit_field;
} T_DMAC_DMACOMPIDREG_L;

//dmaCompIdReg_H
typedef union
{
    uint32_t val;
    struct
    {
        uint32_t               dma_comp_version : 32; /*31: 0,      Version of the component.*/
    } bit_field;
} T_DMAC_DMACOMPIDREG_H;

//Registers Mapping to the same address

typedef struct
{
    volatile                T_DMAC_CH0_SAR_L                      ch0_sar_l; /*  0x0,    RW, 0x00000000, channel 0 register source address Register*/
    volatile                T_DMAC_CH0_SAR_H                      ch0_sar_h; /*  0x4,    RO, 0x00000000, channel 0 register source address Register*/
    volatile                T_DMAC_CH0_DAR_L                      ch0_dar_l; /*  0x8,    RW, 0x00000000, channel 0 register destination address Register*/
    volatile                T_DMAC_CH0_DAR_H                      ch0_dar_h; /*  0xc,    RO, 0x00000000, channel 0 register destinatin address Register*/
    volatile                        uint32_t                  reserved_1[2];
    volatile               T_DMAC_CH0_CTRL_L                     ch0_ctrl_l; /* 0x18,    RW, 0x00304801,     Channel 0 Control Register*/
    volatile               T_DMAC_CH0_CTRL_H                     ch0_ctrl_h; /* 0x1c,    RW, 0x00000002,     Channel 0 Control Register*/
    volatile                        uint32_t                  reserved_2[8];
    volatile                T_DMAC_CH0_CFG_L                      ch0_cfg_l; /* 0x40,    RW, 0x00000E00, channel 0 configuration register*/
    volatile                T_DMAC_CH0_CFG_H                      ch0_cfg_h; /* 0x44,    RW, 0x00000004, channel 0 configuration register*/
    volatile                        uint32_t                  reserved_3[4];
    volatile                T_DMAC_CH1_SAR_L                      ch1_sar_l; /* 0x58,    RW, 0x00000000, Channel 1 register source address Register*/
    volatile                T_DMAC_CH1_SAR_H                      ch1_sar_h; /* 0x5c,    RO, 0x00000000, Channel 1 register source address Register*/
    volatile                T_DMAC_CH1_DAR_L                      ch1_dar_l; /* 0x60,    RW, 0x00000000, Channel 1 register destination address Register*/
    volatile                T_DMAC_CH1_DAR_H                      ch1_dar_h; /* 0x64,    RO, 0x00000000, Channel 1 register destinatin address Register*/
    volatile                        uint32_t                  reserved_4[2];
    volatile               T_DMAC_CH1_CTRL_L                     ch1_ctrl_l; /* 0x70,    RW, 0x00304801,     Channel 1 Control Register*/
    volatile               T_DMAC_CH1_CTRL_H                     ch1_ctrl_h; /* 0x74,    RW, 0x00000002,     Channel 1 Control Register*/
    volatile                        uint32_t                  reserved_5[8];
    volatile                T_DMAC_CH1_CFG_L                      ch1_cfg_l; /* 0x98,    RW, 0x00000E20, Channel 1 configuration register*/
    volatile                T_DMAC_CH1_CFG_H                      ch1_cfg_h; /* 0x9c,    RW, 0x00000004, Channel 1 configuration register*/
    volatile                        uint32_t                  reserved_6[4];
    volatile                T_DMAC_CH2_SAR_L                      ch2_sar_l; /* 0xb0,    RW, 0x00000000, Channel 2 register source address Register*/
    volatile                T_DMAC_CH2_SAR_H                      ch2_sar_h; /* 0xb4,    RO, 0x00000000, Channel 2 register source address Register*/
    volatile                T_DMAC_CH2_DAR_L                      ch2_dar_l; /* 0xb8,    RW, 0x00000000, Channel 2 register destination address Register*/
    volatile                T_DMAC_CH2_DAR_H                      ch2_dar_h; /* 0xbc,    RO, 0x00000000, Channel 2 register destinatin address Register*/
    volatile                        uint32_t                  reserved_7[2];
    volatile               T_DMAC_CH2_CTRL_L                     ch2_ctrl_l; /* 0xc8,    RW, 0x00304801,     Channel 2 Control Register*/
    volatile               T_DMAC_CH2_CTRL_H                     ch2_ctrl_h; /* 0xcc,    RW, 0x00000002,     Channel 2 Control Register*/
    volatile                        uint32_t                  reserved_8[8];
    volatile                T_DMAC_CH2_CFG_L                      ch2_cfg_l; /* 0xf0,    RW, 0x00000E40, Channel 2 configuration register*/
    volatile                T_DMAC_CH2_CFG_H                      ch2_cfg_h; /* 0xf4,    RW, 0x00000004, Channel 2 configuration register*/
    volatile                        uint32_t                  reserved_9[4];
    volatile                T_DMAC_CH3_SAR_L                      ch3_sar_l; /*0x108,    RW, 0x00000000, Channel 3 register source address Register*/
    volatile                T_DMAC_CH3_SAR_H                      ch3_sar_h; /*0x10c,    RO, 0x00000000, Channel 3 register source address Register*/
    volatile                T_DMAC_CH3_DAR_L                      ch3_dar_l; /*0x110,    RW, 0x00000000, Channel 3 register destination address Register*/
    volatile                T_DMAC_CH3_DAR_H                      ch3_dar_h; /*0x114,    RO, 0x00000000, Channel 3 register destinatin address Register*/
    volatile                        uint32_t                 reserved_10[2];
    volatile               T_DMAC_CH3_CTRL_L                     ch3_ctrl_l; /*0x120,    RW, 0x00304801,     Channel 3 Control Register*/
    volatile               T_DMAC_CH3_CTRL_H                     ch3_ctrl_h; /*0x124,    RW, 0x00000002,     Channel 3 Control Register*/
    volatile                        uint32_t                 reserved_11[8];
    volatile                T_DMAC_CH3_CFG_L                      ch3_cfg_l; /*0x148,    RW, 0x00000E60, Channel 3 configuration register*/
    volatile                T_DMAC_CH3_CFG_H                      ch3_cfg_h; /*0x14c,    RW, 0x00000004, Channel 3 configuration register*/
    volatile                        uint32_t                 reserved_12[4];
    volatile                T_DMAC_CH4_SAR_L                      ch4_sar_l; /*0x160,    RW, 0x00000000, Channel 4 register source address Register*/
    volatile                T_DMAC_CH4_SAR_H                      ch4_sar_h; /*0x164,    RO, 0x00000000, Channel 4 register source address Register*/
    volatile                T_DMAC_CH4_DAR_L                      ch4_dar_l; /*0x168,    RW, 0x00000000, Channel 4 register destination address Register*/
    volatile                T_DMAC_CH4_DAR_H                      ch4_dar_h; /*0x16c,    RO, 0x00000000, Channel 4 register destinatin address Register*/
    volatile                        uint32_t                 reserved_13[2];
    volatile               T_DMAC_CH4_CTRL_L                     ch4_ctrl_l; /*0x178,    RW, 0x00304801,     Channel 4 Control Register*/
    volatile               T_DMAC_CH4_CTRL_H                     ch4_ctrl_h; /*0x17c,    RW, 0x00000002,     Channel 4 Control Register*/
    volatile                        uint32_t                 reserved_14[8];
    volatile                T_DMAC_CH4_CFG_L                      ch4_cfg_l; /*0x1a0,    RW, 0x00000E80, Channel 4 configuration register*/
    volatile                T_DMAC_CH4_CFG_H                      ch4_cfg_h; /*0x1a4,    RW, 0x00000004, Channel 4 configuration register*/
    volatile                        uint32_t                 reserved_15[4];
    volatile                T_DMAC_CH5_SAR_L                      ch5_sar_l; /*0x1b8,    RW, 0x00000000, Channel 5 register source address Register*/
    volatile                T_DMAC_CH5_SAR_H                      ch5_sar_h; /*0x1bc,    RO, 0x00000000, Channel 5 register source address Register*/
    volatile                T_DMAC_CH5_DAR_L                      ch5_dar_l; /*0x1c0,    RW, 0x00000000, Channel 5 register destination address Register*/
    volatile                T_DMAC_CH5_DAR_H                      ch5_dar_h; /*0x1c4,    RO, 0x00000000, Channel 5 register destinatin address Register*/
    volatile                        uint32_t                 reserved_16[2];
    volatile               T_DMAC_CH5_CTRL_L                     ch5_ctrl_l; /*0x1d0,    RW, 0x00304801,     Channel 5 Control Register*/
    volatile               T_DMAC_CH5_CTRL_H                     ch5_ctrl_h; /*0x1d4,    RW, 0x00000002,     Channel 5 Control Register*/
    volatile                        uint32_t                 reserved_17[8];
    volatile                T_DMAC_CH5_CFG_L                      ch5_cfg_l; /*0x1f8,    RW, 0x00000EA0, Channel 5 configuration register*/
    volatile                T_DMAC_CH5_CFG_H                      ch5_cfg_h; /*0x1fc,    RW, 0x00000004, Channel 5 configuration register*/
    volatile                        uint32_t                reserved_18[48];
    volatile                 T_DMAC_RAWTFR_L                       rawtfr_l; /*0x2c0,    RW, 0x00000000, Interrupt Raw Status Registers*/
    volatile                 T_DMAC_RAWTFR_H                       rawtfr_h; /*0x2c4,    RO, 0x00000000, Interrupt Raw Status Registers*/
    volatile               T_DMAC_RAWBLOCK_L                     rawblock_l; /*0x2c8,    RW, 0x00000000, Interrupt Raw Status Registers*/
    volatile               T_DMAC_RAWBLOCK_H                     rawblock_h; /*0x2cc,    RO, 0x00000000, Interrupt Raw Status Registers*/
    volatile             T_DMAC_RAWSRCTRAN_L                   rawsrctran_l; /*0x2d0,    RW, 0x00000000, Interrupt Raw Status Registers*/
    volatile             T_DMAC_RAWSRCTRAN_H                   rawsrctran_h; /*0x2d4,    RO, 0x00000000, Interrupt Raw Status Registers*/
    volatile             T_DMAC_RAWDSTTRAN_L                   rawdsttran_l; /*0x2d8,    RW, 0x00000000, Interrupt Raw Status Registers*/
    volatile             T_DMAC_RAWDSTTRAN_H                   rawdsttran_h; /*0x2dc,    RO, 0x00000000, Interrupt Raw Status Registers*/
    volatile                 T_DMAC_RAWERR_L                       rawerr_l; /*0x2e0,    RW, 0x00000000, Interrupt Raw Status Registers*/
    volatile                 T_DMAC_RAWERR_H                       rawerr_h; /*0x2e4,    RO, 0x00000000, Interrupt Raw Status Registers*/
    volatile              T_DMAC_STATUSTFR_L                    statustfr_l; /*0x2e8,    RO, 0x00000000,     Interrupt Status Registers*/
    volatile              T_DMAC_STATUSTFR_H                    statustfr_h; /*0x2ec,    RO, 0x00000000,     Interrupt Status Registers*/
    volatile            T_DMAC_STATUSBLOCK_L                  statusblock_l; /*0x2f0,    RO, 0x00000000,     Interrupt Status Registers*/
    volatile            T_DMAC_STATUSBLOCK_H                  statusblock_h; /*0x2f4,    RO, 0x00000000,     Interrupt Status Registers*/
    volatile          T_DMAC_STATUSSRCTRAN_L                statussrctran_l; /*0x2f8,    RO, 0x00000000,     Interrupt Status Registers*/
    volatile          T_DMAC_STATUSSRCTRAN_H                statussrctran_h; /*0x2fc,    RO, 0x00000000,     Interrupt Status Registers*/
    volatile          T_DMAC_STATUSDSTTRAN_L                statusdsttran_l; /*0x300,    RO, 0x00000000,     Interrupt Status Registers*/
    volatile          T_DMAC_STATUSDSTTRAN_H                statusdsttran_h; /*0x304,    RO, 0x00000000,     Interrupt Status Registers*/
    volatile              T_DMAC_STATUSERR_L                    statuserr_l; /*0x308,    RO, 0x00000000,     Interrupt Status Registers*/
    volatile              T_DMAC_STATUSERR_H                    statuserr_h; /*0x30c,    RO, 0x00000000,     Interrupt Status Registers*/
    volatile                T_DMAC_MASKTFR_L                      masktfr_l; /*0x310,    RW, 0x00000000,       Interrupt Mask Registers*/
    volatile                T_DMAC_MASKTFR_H                      masktfr_h; /*0x314,    RO, 0x00000000,       Interrupt Mask Registers*/
    volatile              T_DMAC_MASKBLOCK_L                    maskblock_l; /*0x318,    RW, 0x00000000,       Interrupt Mask Registers*/
    volatile              T_DMAC_MASKBLOCK_H                    maskblock_h; /*0x31c,    RO, 0x00000000,       Interrupt Mask Registers*/
    volatile            T_DMAC_MASKSRCTRAN_L                  masksrctran_l; /*0x320,    RW, 0x00000000,       Interrupt Mask Registers*/
    volatile            T_DMAC_MASKSRCTRAN_H                  masksrctran_h; /*0x324,    RO, 0x00000000,       Interrupt Mask Registers*/
    volatile            T_DMAC_MASKDSTTRAN_L                  maskdsttran_l; /*0x328,    RW, 0x00000000,       Interrupt Mask Registers*/
    volatile            T_DMAC_MASKDSTTRAN_H                  maskdsttran_h; /*0x32c,    RO, 0x00000000,       Interrupt Mask Registers*/
    volatile                T_DMAC_MASKERR_L                      maskerr_l; /*0x330,    RW, 0x00000000,       Interrupt Mask Registers*/
    volatile                T_DMAC_MASKERR_H                      maskerr_h; /*0x334,    RO, 0x00000000,       Interrupt Mask Registers*/
    volatile               T_DMAC_CLEARTFR_L                     cleartfr_l; /*0x338,    WO, 0x00000000,      Interrupt clear Registers*/
    volatile               T_DMAC_CLEARTFR_H                     cleartfr_h; /*0x33c,    RO, 0x00000000,      Interrupt clear Registers*/
    volatile             T_DMAC_CLEARBLOCK_L                   clearblock_l; /*0x340,    WO, 0x00000000,      Interrupt clear Registers*/
    volatile             T_DMAC_CLEARBLOCK_H                   clearblock_h; /*0x344,    RO, 0x00000000,      Interrupt clear Registers*/
    volatile           T_DMAC_CLEARSRCTRAN_L                 clearsrctran_l; /*0x348,    WO, 0x00000000,      Interrupt clear Registers*/
    volatile           T_DMAC_CLEARSRCTRAN_H                 clearsrctran_h; /*0x34c,    RO, 0x00000000,      Interrupt clear Registers*/
    volatile           T_DMAC_CLEARDSTTRAN_L                 cleardsttran_l; /*0x350,    WO, 0x00000000,      Interrupt clear Registers*/
    volatile           T_DMAC_CLEARDSTTRAN_H                 cleardsttran_h; /*0x354,    RO, 0x00000000,      Interrupt clear Registers*/
    volatile               T_DMAC_CLEARERR_L                     clearerr_l; /*0x358,    WO, 0x00000000,      Interrupt clear Registers*/
    volatile               T_DMAC_CLEARERR_H                     clearerr_h; /*0x35c,    RO, 0x00000000,      Interrupt clear Registers*/
    volatile              T_DMAC_STATUSINT_L                    statusint_l; /*0x360,    RO, 0x00000000, Combined Interrupt Status Register*/
    volatile              T_DMAC_STATUSINT_H                    statusint_h; /*0x364,    RO, 0x00000000, Combined Interrupt Status Register*/
    volatile              T_DMAC_REQSRCREG_L                    reqsrcreg_l; /*0x368,    RW, 0x00000000, Source Software Transaction Request Register*/
    volatile              T_DMAC_REQSRCREG_H                    reqsrcreg_h; /*0x36c,    RO, 0x00000000, Source Software Transaction Request Register*/
    volatile              T_DMAC_REQDSTREG_L                    reqdstreg_l; /*0x370,    RW, 0x00000000, Destination Software Transaction Request Register*/
    volatile              T_DMAC_REQDSTREG_H                    reqdstreg_h; /*0x374,    RO, 0x00000000, Destination Software Transaction Request Register*/
    volatile           T_DMAC_SGLREQSRCREG_L                 sglreqsrcreg_l; /*0x378,    RW, 0x00000000, Single Source Transaction Request Register*/
    volatile           T_DMAC_SGLREQSRCREG_H                 sglreqsrcreg_h; /*0x37c,    RO, 0x00000000, Single Source Transaction Request Register*/
    volatile           T_DMAC_SGLREQDSTREG_L                 sglreqdstreg_l; /*0x380,    RW, 0x00000000, Single Destination Transaction Request Register*/
    volatile           T_DMAC_SGLREQDSTREG_H                 sglreqdstreg_h; /*0x384,    RO, 0x00000000, Single Destination Transaction Request Register*/
    volatile              T_DMAC_LSTSRCREG_L                    lstsrcreg_l; /*0x388,    RW, 0x00000000, Last Source Transaction Request Register*/
    volatile              T_DMAC_LSTSRCREG_H                    lstsrcreg_h; /*0x38c,    RO, 0x00000000, Last Source Transaction Request Register*/
    volatile              T_DMAC_LSTDSTREG_L                    lstdstreg_l; /*0x390,    RW, 0x00000000, Last Destination Transaction Request Register*/
    volatile              T_DMAC_LSTDSTREG_H                    lstdstreg_h; /*0x394,    RO, 0x00000000, Last Destination Transaction Request Register*/
    volatile              T_DMAC_DMACFGREG_L                    dmacfgreg_l; /*0x398,    RW, 0x00000000, DW_ahb_dmac Configuration Register*/
    volatile              T_DMAC_DMACFGREG_H                    dmacfgreg_h; /*0x39c,    RO, 0x00000000, DW_ahb_dmac Configuration Register*/
    volatile                T_DMAC_CHENREG_L                      chenreg_l; /*0x3a0,    RW, 0x00000000, DW_ahb_dmac Channel Enable Register*/
    volatile                T_DMAC_CHENREG_H                      chenreg_h; /*0x3a4,    RO, 0x00000000, DW_ahb_dmac Channel Enable Register*/
    volatile               T_DMAC_DMAIDREG_L                     dmaidreg_l; /*0x3a8,    RO, 0x00000000,        DW_ahb_dmac ID Register*/
    volatile               T_DMAC_DMAIDREG_H                     dmaidreg_h; /*0x3ac,    RO, 0x00000000,        DW_ahb_dmac ID Register*/
    volatile             T_DMAC_DMATESTREG_L                   dmatestreg_l; /*0x3b0,    RW, 0x00000000,      DW_ahb_dmac Test Register*/
    volatile             T_DMAC_DMATESTREG_H                   dmatestreg_h; /*0x3b4,    RO, 0x00000000,      DW_ahb_dmac Test Register*/
    volatile                        uint32_t                 reserved_19[4];
    volatile      T_DMAC_DMA_COMP_PARAMS_6_L            dma_comp_params_6_l; /*0x3c8,    RO, 0x00000000, DW_ahb_dmac Component Parameters Register 6*/
    volatile      T_DMAC_DMA_COMP_PARAMS_6_H            dma_comp_params_6_h; /*0x3cc,    RO, 0x00000000, DW_ahb_dmac Component Parameters Register 6*/
    volatile      T_DMAC_DMA_COMP_PARAMS_5_L            dma_comp_params_5_l; /*0x3d0,    RO, 0x00000000, DW_ahb_dmac Component Parameters Register 5*/
    volatile      T_DMAC_DMA_COMP_PARAMS_5_H            dma_comp_params_5_h; /*0x3d4,    RO, 0x10032000, DW_ahb_dmac Component Parameters Register 5*/
    volatile      T_DMAC_DMA_COMP_PARAMS_4_L            dma_comp_params_4_l; /*0x3d8,    RO, 0x10032000, DW_ahb_dmac Component Parameters Register 4*/
    volatile      T_DMAC_DMA_COMP_PARAMS_4_H            dma_comp_params_4_h; /*0x3dc,    RO, 0x10032000, DW_ahb_dmac Component Parameters Register 4*/
    volatile      T_DMAC_DMA_COMP_PARAMS_3_L            dma_comp_params_3_l; /*0x3e0,    RO, 0x10032000, DW_ahb_dmac Component Parameters Register 3*/
    volatile      T_DMAC_DMA_COMP_PARAMS_3_H            dma_comp_params_3_h; /*0x3e4,    RO, 0x10032000, DW_ahb_dmac Component Parameters Register 3*/
    volatile      T_DMAC_DMA_COMP_PARAMS_2_L            dma_comp_params_2_l; /*0x3e8,    RO, 0x10032000, DW_ahb_dmac Component Parameters Register 2*/
    volatile      T_DMAC_DMA_COMP_PARAMS_2_H            dma_comp_params_2_h; /*0x3ec,    RO, 0x00000000, DW_ahb_dmac Component Parameters Register 2*/
    volatile      T_DMAC_DMA_COMP_PARAMS_1_L            dma_comp_params_1_l; /*0x3f0,    RO, 0x33AAAAAA, DW_ahb_dmac Component Parameters Register 1*/
    volatile      T_DMAC_DMA_COMP_PARAMS_1_H            dma_comp_params_1_h; /*0x3f4,    RO, 0x3800050C, DW_ahb_dmac Component Parameters Register 1*/
    volatile           T_DMAC_DMACOMPIDREG_L                 dmacompidreg_l; /*0x3f8,    RO, 0x44571110,        DW_ahb_dmac ID Register*/
    volatile           T_DMAC_DMACOMPIDREG_H                 dmacompidreg_h; /*0x3fc,    RO, 0x3232302A,        DW_ahb_dmac ID Register*/
} T_HWP_DMAC_T;

#define hwp_dmac ((T_HWP_DMAC_T*)REG_DMAC_BASE)


__STATIC_INLINE uint32_t dmac_ch0_sar_l_get(void)
{
    return hwp_dmac->ch0_sar_l.val;
}

__STATIC_INLINE void dmac_ch0_sar_l_set(uint32_t value)
{
    hwp_dmac->ch0_sar_l.val = value;
}

__STATIC_INLINE void dmac_ch0_sar_l_pack(uint32_t sar)
{
    hwp_dmac->ch0_sar_l.val = (((uint32_t)sar << 0));
}

__STATIC_INLINE void dmac_ch0_sar_l_unpack(uint32_t* sar)
{
    T_DMAC_CH0_SAR_L localVal = hwp_dmac->ch0_sar_l;

    *sar = localVal.bit_field.sar;
}

__STATIC_INLINE uint32_t dmac_ch0_sar_h_get(void)
{
    return hwp_dmac->ch0_sar_h.val;
}

__STATIC_INLINE uint32_t dmac_ch0_dar_l_get(void)
{
    return hwp_dmac->ch0_dar_l.val;
}

__STATIC_INLINE void dmac_ch0_dar_l_set(uint32_t value)
{
    hwp_dmac->ch0_dar_l.val = value;
}

__STATIC_INLINE void dmac_ch0_dar_l_pack(uint32_t dar)
{
    hwp_dmac->ch0_dar_l.val = (((uint32_t)dar << 0));
}

__STATIC_INLINE void dmac_ch0_dar_l_unpack(uint32_t* dar)
{
    T_DMAC_CH0_DAR_L localVal = hwp_dmac->ch0_dar_l;

    *dar = localVal.bit_field.dar;
}

__STATIC_INLINE uint32_t dmac_ch0_dar_h_get(void)
{
    return hwp_dmac->ch0_dar_h.val;
}

__STATIC_INLINE uint32_t dmac_ch0_ctrl_l_get(void)
{
    return hwp_dmac->ch0_ctrl_l.val;
}

__STATIC_INLINE void dmac_ch0_ctrl_l_set(uint32_t value)
{
    hwp_dmac->ch0_ctrl_l.val = value;
}

__STATIC_INLINE void dmac_ch0_ctrl_l_pack(uint8_t tt_fc, uint8_t src_msize, uint8_t dest_msize, uint8_t sinc, uint8_t dinc, uint8_t src_tr_width, uint8_t dst_tr_width, uint8_t int_en)
{
    hwp_dmac->ch0_ctrl_l.val = (((uint32_t)tt_fc << 20) | ((uint32_t)src_msize << 14) | ((uint32_t)dest_msize << 11) | ((uint32_t)sinc << 9) | ((uint32_t)dinc << 7) | ((uint32_t)src_tr_width << 4) | ((uint32_t)dst_tr_width << 1) | ((uint32_t)int_en << 0));
}

__STATIC_INLINE void dmac_ch0_ctrl_l_unpack(uint8_t* llp_src_en, uint8_t* llp_dst_en, uint8_t* sms, uint8_t* dms, uint8_t* tt_fc, uint8_t* dst_scatter_en, uint8_t* src_gather_en, uint8_t* src_msize, uint8_t* dest_msize, uint8_t* sinc, uint8_t* dinc, uint8_t* src_tr_width, uint8_t* dst_tr_width, uint8_t* int_en)
{
    T_DMAC_CH0_CTRL_L localVal = hwp_dmac->ch0_ctrl_l;

    *llp_src_en = localVal.bit_field.llp_src_en;
    *llp_dst_en = localVal.bit_field.llp_dst_en;
    *sms = localVal.bit_field.sms;
    *dms = localVal.bit_field.dms;
    *tt_fc = localVal.bit_field.tt_fc;
    *dst_scatter_en = localVal.bit_field.dst_scatter_en;
    *src_gather_en = localVal.bit_field.src_gather_en;
    *src_msize = localVal.bit_field.src_msize;
    *dest_msize = localVal.bit_field.dest_msize;
    *sinc = localVal.bit_field.sinc;
    *dinc = localVal.bit_field.dinc;
    *src_tr_width = localVal.bit_field.src_tr_width;
    *dst_tr_width = localVal.bit_field.dst_tr_width;
    *int_en = localVal.bit_field.int_en;
}

__STATIC_INLINE uint32_t dmac_ch0_ctrl_h_get(void)
{
    return hwp_dmac->ch0_ctrl_h.val;
}

__STATIC_INLINE void dmac_ch0_ctrl_h_set(uint32_t value)
{
    hwp_dmac->ch0_ctrl_h.val = value;
}

__STATIC_INLINE void dmac_ch0_ctrl_h_pack(uint8_t done, uint16_t block_ts)
{
    hwp_dmac->ch0_ctrl_h.val = (((uint32_t)done << 12) | ((uint32_t)block_ts << 0));
}

__STATIC_INLINE void dmac_ch0_ctrl_h_unpack(uint8_t* done, uint16_t* block_ts)
{
    T_DMAC_CH0_CTRL_H localVal = hwp_dmac->ch0_ctrl_h;

    *done = localVal.bit_field.done;
    *block_ts = localVal.bit_field.block_ts;
}

__STATIC_INLINE uint32_t dmac_ch0_cfg_l_get(void)
{
    return hwp_dmac->ch0_cfg_l.val;
}

__STATIC_INLINE void dmac_ch0_cfg_l_set(uint32_t value)
{
    hwp_dmac->ch0_cfg_l.val = value;
}

__STATIC_INLINE void dmac_ch0_cfg_l_pack(uint16_t max_abrst, uint8_t src_hs_pol, uint8_t dst_hs_pol, uint8_t hs_sel_src, uint8_t hs_sel_dst, uint8_t ch_susp, uint8_t ch_prior)
{
    hwp_dmac->ch0_cfg_l.val = (((uint32_t)max_abrst << 20) | ((uint32_t)src_hs_pol << 19) | ((uint32_t)dst_hs_pol << 18) | ((uint32_t)hs_sel_src << 11) | ((uint32_t)hs_sel_dst << 10) | ((uint32_t)ch_susp << 8) | ((uint32_t)ch_prior << 5));
}

__STATIC_INLINE void dmac_ch0_cfg_l_unpack(uint8_t* reload_dst, uint8_t* reload_src, uint16_t* max_abrst, uint8_t* src_hs_pol, uint8_t* dst_hs_pol, uint8_t* lock_b, uint8_t* lock_ch, uint8_t* lock_b_l, uint8_t* lock_ch_l, uint8_t* hs_sel_src, uint8_t* hs_sel_dst, uint8_t* fifo_empty, uint8_t* ch_susp, uint8_t* ch_prior)
{
    T_DMAC_CH0_CFG_L localVal = hwp_dmac->ch0_cfg_l;

    *reload_dst = localVal.bit_field.reload_dst;
    *reload_src = localVal.bit_field.reload_src;
    *max_abrst = localVal.bit_field.max_abrst;
    *src_hs_pol = localVal.bit_field.src_hs_pol;
    *dst_hs_pol = localVal.bit_field.dst_hs_pol;
    *lock_b = localVal.bit_field.lock_b;
    *lock_ch = localVal.bit_field.lock_ch;
    *lock_b_l = localVal.bit_field.lock_b_l;
    *lock_ch_l = localVal.bit_field.lock_ch_l;
    *hs_sel_src = localVal.bit_field.hs_sel_src;
    *hs_sel_dst = localVal.bit_field.hs_sel_dst;
    *fifo_empty = localVal.bit_field.fifo_empty;
    *ch_susp = localVal.bit_field.ch_susp;
    *ch_prior = localVal.bit_field.ch_prior;
}

__STATIC_INLINE uint32_t dmac_ch0_cfg_h_get(void)
{
    return hwp_dmac->ch0_cfg_h.val;
}

__STATIC_INLINE void dmac_ch0_cfg_h_set(uint32_t value)
{
    hwp_dmac->ch0_cfg_h.val = value;
}

__STATIC_INLINE void dmac_ch0_cfg_h_pack(uint8_t dest_per, uint8_t src_per, uint8_t protctl, uint8_t fifo_mode, uint8_t fc_mode)
{
    hwp_dmac->ch0_cfg_h.val = (((uint32_t)dest_per << 11) | ((uint32_t)src_per << 7) | ((uint32_t)protctl << 2) | ((uint32_t)fifo_mode << 1) | ((uint32_t)fc_mode << 0));
}

__STATIC_INLINE void dmac_ch0_cfg_h_unpack(uint8_t* dest_per, uint8_t* src_per, uint8_t* ss_upd_en, uint8_t* ds_upd_en, uint8_t* protctl, uint8_t* fifo_mode, uint8_t* fc_mode)
{
    T_DMAC_CH0_CFG_H localVal = hwp_dmac->ch0_cfg_h;

    *dest_per = localVal.bit_field.dest_per;
    *src_per = localVal.bit_field.src_per;
    *ss_upd_en = localVal.bit_field.ss_upd_en;
    *ds_upd_en = localVal.bit_field.ds_upd_en;
    *protctl = localVal.bit_field.protctl;
    *fifo_mode = localVal.bit_field.fifo_mode;
    *fc_mode = localVal.bit_field.fc_mode;
}

__STATIC_INLINE uint32_t dmac_ch1_sar_l_get(void)
{
    return hwp_dmac->ch1_sar_l.val;
}

__STATIC_INLINE void dmac_ch1_sar_l_set(uint32_t value)
{
    hwp_dmac->ch1_sar_l.val = value;
}

__STATIC_INLINE void dmac_ch1_sar_l_pack(uint32_t sar)
{
    hwp_dmac->ch1_sar_l.val = (((uint32_t)sar << 0));
}

__STATIC_INLINE void dmac_ch1_sar_l_unpack(uint32_t* sar)
{
    T_DMAC_CH1_SAR_L localVal = hwp_dmac->ch1_sar_l;

    *sar = localVal.bit_field.sar;
}

__STATIC_INLINE uint32_t dmac_ch1_sar_h_get(void)
{
    return hwp_dmac->ch1_sar_h.val;
}

__STATIC_INLINE uint32_t dmac_ch1_dar_l_get(void)
{
    return hwp_dmac->ch1_dar_l.val;
}

__STATIC_INLINE void dmac_ch1_dar_l_set(uint32_t value)
{
    hwp_dmac->ch1_dar_l.val = value;
}

__STATIC_INLINE void dmac_ch1_dar_l_pack(uint32_t dar)
{
    hwp_dmac->ch1_dar_l.val = (((uint32_t)dar << 0));
}

__STATIC_INLINE void dmac_ch1_dar_l_unpack(uint32_t* dar)
{
    T_DMAC_CH1_DAR_L localVal = hwp_dmac->ch1_dar_l;

    *dar = localVal.bit_field.dar;
}

__STATIC_INLINE uint32_t dmac_ch1_dar_h_get(void)
{
    return hwp_dmac->ch1_dar_h.val;
}

__STATIC_INLINE uint32_t dmac_ch1_ctrl_l_get(void)
{
    return hwp_dmac->ch1_ctrl_l.val;
}

__STATIC_INLINE void dmac_ch1_ctrl_l_set(uint32_t value)
{
    hwp_dmac->ch1_ctrl_l.val = value;
}

__STATIC_INLINE void dmac_ch1_ctrl_l_pack(uint8_t tt_fc, uint8_t src_msize, uint8_t dest_msize, uint8_t sinc, uint8_t dinc, uint8_t src_tr_width, uint8_t dst_tr_width, uint8_t int_en)
{
    hwp_dmac->ch1_ctrl_l.val = (((uint32_t)tt_fc << 20) | ((uint32_t)src_msize << 14) | ((uint32_t)dest_msize << 11) | ((uint32_t)sinc << 9) | ((uint32_t)dinc << 7) | ((uint32_t)src_tr_width << 4) | ((uint32_t)dst_tr_width << 1) | ((uint32_t)int_en << 0));
}

__STATIC_INLINE void dmac_ch1_ctrl_l_unpack(uint8_t* llp_src_en, uint8_t* llp_dst_en, uint8_t* sms, uint8_t* dms, uint8_t* tt_fc, uint8_t* dst_scatter_en, uint8_t* src_gather_en, uint8_t* src_msize, uint8_t* dest_msize, uint8_t* sinc, uint8_t* dinc, uint8_t* src_tr_width, uint8_t* dst_tr_width, uint8_t* int_en)
{
    T_DMAC_CH1_CTRL_L localVal = hwp_dmac->ch1_ctrl_l;

    *llp_src_en = localVal.bit_field.llp_src_en;
    *llp_dst_en = localVal.bit_field.llp_dst_en;
    *sms = localVal.bit_field.sms;
    *dms = localVal.bit_field.dms;
    *tt_fc = localVal.bit_field.tt_fc;
    *dst_scatter_en = localVal.bit_field.dst_scatter_en;
    *src_gather_en = localVal.bit_field.src_gather_en;
    *src_msize = localVal.bit_field.src_msize;
    *dest_msize = localVal.bit_field.dest_msize;
    *sinc = localVal.bit_field.sinc;
    *dinc = localVal.bit_field.dinc;
    *src_tr_width = localVal.bit_field.src_tr_width;
    *dst_tr_width = localVal.bit_field.dst_tr_width;
    *int_en = localVal.bit_field.int_en;
}

__STATIC_INLINE uint32_t dmac_ch1_ctrl_h_get(void)
{
    return hwp_dmac->ch1_ctrl_h.val;
}

__STATIC_INLINE void dmac_ch1_ctrl_h_set(uint32_t value)
{
    hwp_dmac->ch1_ctrl_h.val = value;
}

__STATIC_INLINE void dmac_ch1_ctrl_h_pack(uint8_t done, uint16_t block_ts)
{
    hwp_dmac->ch1_ctrl_h.val = (((uint32_t)done << 12) | ((uint32_t)block_ts << 0));
}

__STATIC_INLINE void dmac_ch1_ctrl_h_unpack(uint8_t* done, uint16_t* block_ts)
{
    T_DMAC_CH1_CTRL_H localVal = hwp_dmac->ch1_ctrl_h;

    *done = localVal.bit_field.done;
    *block_ts = localVal.bit_field.block_ts;
}

__STATIC_INLINE uint32_t dmac_ch1_cfg_l_get(void)
{
    return hwp_dmac->ch1_cfg_l.val;
}

__STATIC_INLINE void dmac_ch1_cfg_l_set(uint32_t value)
{
    hwp_dmac->ch1_cfg_l.val = value;
}

__STATIC_INLINE void dmac_ch1_cfg_l_pack(uint16_t max_abrst, uint8_t src_hs_pol, uint8_t dst_hs_pol, uint8_t hs_sel_src, uint8_t hs_sel_dst, uint8_t ch_susp, uint8_t ch_prior)
{
    hwp_dmac->ch1_cfg_l.val = (((uint32_t)max_abrst << 20) | ((uint32_t)src_hs_pol << 19) | ((uint32_t)dst_hs_pol << 18) | ((uint32_t)hs_sel_src << 11) | ((uint32_t)hs_sel_dst << 10) | ((uint32_t)ch_susp << 8) | ((uint32_t)ch_prior << 5));
}

__STATIC_INLINE void dmac_ch1_cfg_l_unpack(uint8_t* reload_dst, uint8_t* reload_src, uint16_t* max_abrst, uint8_t* src_hs_pol, uint8_t* dst_hs_pol, uint8_t* lock_b, uint8_t* lock_ch, uint8_t* lock_b_l, uint8_t* lock_ch_l, uint8_t* hs_sel_src, uint8_t* hs_sel_dst, uint8_t* fifo_empty, uint8_t* ch_susp, uint8_t* ch_prior)
{
    T_DMAC_CH1_CFG_L localVal = hwp_dmac->ch1_cfg_l;

    *reload_dst = localVal.bit_field.reload_dst;
    *reload_src = localVal.bit_field.reload_src;
    *max_abrst = localVal.bit_field.max_abrst;
    *src_hs_pol = localVal.bit_field.src_hs_pol;
    *dst_hs_pol = localVal.bit_field.dst_hs_pol;
    *lock_b = localVal.bit_field.lock_b;
    *lock_ch = localVal.bit_field.lock_ch;
    *lock_b_l = localVal.bit_field.lock_b_l;
    *lock_ch_l = localVal.bit_field.lock_ch_l;
    *hs_sel_src = localVal.bit_field.hs_sel_src;
    *hs_sel_dst = localVal.bit_field.hs_sel_dst;
    *fifo_empty = localVal.bit_field.fifo_empty;
    *ch_susp = localVal.bit_field.ch_susp;
    *ch_prior = localVal.bit_field.ch_prior;
}

__STATIC_INLINE uint32_t dmac_ch1_cfg_h_get(void)
{
    return hwp_dmac->ch1_cfg_h.val;
}

__STATIC_INLINE void dmac_ch1_cfg_h_set(uint32_t value)
{
    hwp_dmac->ch1_cfg_h.val = value;
}

__STATIC_INLINE void dmac_ch1_cfg_h_pack(uint8_t dest_per, uint8_t src_per, uint8_t protctl, uint8_t fifo_mode, uint8_t fc_mode)
{
    hwp_dmac->ch1_cfg_h.val = (((uint32_t)dest_per << 11) | ((uint32_t)src_per << 7) | ((uint32_t)protctl << 2) | ((uint32_t)fifo_mode << 1) | ((uint32_t)fc_mode << 0));
}

__STATIC_INLINE void dmac_ch1_cfg_h_unpack(uint8_t* dest_per, uint8_t* src_per, uint8_t* ss_upd_en, uint8_t* ds_upd_en, uint8_t* protctl, uint8_t* fifo_mode, uint8_t* fc_mode)
{
    T_DMAC_CH1_CFG_H localVal = hwp_dmac->ch1_cfg_h;

    *dest_per = localVal.bit_field.dest_per;
    *src_per = localVal.bit_field.src_per;
    *ss_upd_en = localVal.bit_field.ss_upd_en;
    *ds_upd_en = localVal.bit_field.ds_upd_en;
    *protctl = localVal.bit_field.protctl;
    *fifo_mode = localVal.bit_field.fifo_mode;
    *fc_mode = localVal.bit_field.fc_mode;
}

__STATIC_INLINE uint32_t dmac_ch2_sar_l_get(void)
{
    return hwp_dmac->ch2_sar_l.val;
}

__STATIC_INLINE void dmac_ch2_sar_l_set(uint32_t value)
{
    hwp_dmac->ch2_sar_l.val = value;
}

__STATIC_INLINE void dmac_ch2_sar_l_pack(uint32_t sar)
{
    hwp_dmac->ch2_sar_l.val = (((uint32_t)sar << 0));
}

__STATIC_INLINE void dmac_ch2_sar_l_unpack(uint32_t* sar)
{
    T_DMAC_CH2_SAR_L localVal = hwp_dmac->ch2_sar_l;

    *sar = localVal.bit_field.sar;
}

__STATIC_INLINE uint32_t dmac_ch2_sar_h_get(void)
{
    return hwp_dmac->ch2_sar_h.val;
}

__STATIC_INLINE uint32_t dmac_ch2_dar_l_get(void)
{
    return hwp_dmac->ch2_dar_l.val;
}

__STATIC_INLINE void dmac_ch2_dar_l_set(uint32_t value)
{
    hwp_dmac->ch2_dar_l.val = value;
}

__STATIC_INLINE void dmac_ch2_dar_l_pack(uint32_t dar)
{
    hwp_dmac->ch2_dar_l.val = (((uint32_t)dar << 0));
}

__STATIC_INLINE void dmac_ch2_dar_l_unpack(uint32_t* dar)
{
    T_DMAC_CH2_DAR_L localVal = hwp_dmac->ch2_dar_l;

    *dar = localVal.bit_field.dar;
}

__STATIC_INLINE uint32_t dmac_ch2_dar_h_get(void)
{
    return hwp_dmac->ch2_dar_h.val;
}

__STATIC_INLINE uint32_t dmac_ch2_ctrl_l_get(void)
{
    return hwp_dmac->ch2_ctrl_l.val;
}

__STATIC_INLINE void dmac_ch2_ctrl_l_set(uint32_t value)
{
    hwp_dmac->ch2_ctrl_l.val = value;
}

__STATIC_INLINE void dmac_ch2_ctrl_l_pack(uint8_t tt_fc, uint8_t src_msize, uint8_t dest_msize, uint8_t sinc, uint8_t dinc, uint8_t src_tr_width, uint8_t dst_tr_width, uint8_t int_en)
{
    hwp_dmac->ch2_ctrl_l.val = (((uint32_t)tt_fc << 20) | ((uint32_t)src_msize << 14) | ((uint32_t)dest_msize << 11) | ((uint32_t)sinc << 9) | ((uint32_t)dinc << 7) | ((uint32_t)src_tr_width << 4) | ((uint32_t)dst_tr_width << 1) | ((uint32_t)int_en << 0));
}

__STATIC_INLINE void dmac_ch2_ctrl_l_unpack(uint8_t* llp_src_en, uint8_t* llp_dst_en, uint8_t* sms, uint8_t* dms, uint8_t* tt_fc, uint8_t* dst_scatter_en, uint8_t* src_gather_en, uint8_t* src_msize, uint8_t* dest_msize, uint8_t* sinc, uint8_t* dinc, uint8_t* src_tr_width, uint8_t* dst_tr_width, uint8_t* int_en)
{
    T_DMAC_CH2_CTRL_L localVal = hwp_dmac->ch2_ctrl_l;

    *llp_src_en = localVal.bit_field.llp_src_en;
    *llp_dst_en = localVal.bit_field.llp_dst_en;
    *sms = localVal.bit_field.sms;
    *dms = localVal.bit_field.dms;
    *tt_fc = localVal.bit_field.tt_fc;
    *dst_scatter_en = localVal.bit_field.dst_scatter_en;
    *src_gather_en = localVal.bit_field.src_gather_en;
    *src_msize = localVal.bit_field.src_msize;
    *dest_msize = localVal.bit_field.dest_msize;
    *sinc = localVal.bit_field.sinc;
    *dinc = localVal.bit_field.dinc;
    *src_tr_width = localVal.bit_field.src_tr_width;
    *dst_tr_width = localVal.bit_field.dst_tr_width;
    *int_en = localVal.bit_field.int_en;
}

__STATIC_INLINE uint32_t dmac_ch2_ctrl_h_get(void)
{
    return hwp_dmac->ch2_ctrl_h.val;
}

__STATIC_INLINE void dmac_ch2_ctrl_h_set(uint32_t value)
{
    hwp_dmac->ch2_ctrl_h.val = value;
}

__STATIC_INLINE void dmac_ch2_ctrl_h_pack(uint8_t done, uint16_t block_ts)
{
    hwp_dmac->ch2_ctrl_h.val = (((uint32_t)done << 12) | ((uint32_t)block_ts << 0));
}

__STATIC_INLINE void dmac_ch2_ctrl_h_unpack(uint8_t* done, uint16_t* block_ts)
{
    T_DMAC_CH2_CTRL_H localVal = hwp_dmac->ch2_ctrl_h;

    *done = localVal.bit_field.done;
    *block_ts = localVal.bit_field.block_ts;
}

__STATIC_INLINE uint32_t dmac_ch2_cfg_l_get(void)
{
    return hwp_dmac->ch2_cfg_l.val;
}

__STATIC_INLINE void dmac_ch2_cfg_l_set(uint32_t value)
{
    hwp_dmac->ch2_cfg_l.val = value;
}

__STATIC_INLINE void dmac_ch2_cfg_l_pack(uint16_t max_abrst, uint8_t src_hs_pol, uint8_t dst_hs_pol, uint8_t hs_sel_src, uint8_t hs_sel_dst, uint8_t ch_susp, uint8_t ch_prior)
{
    hwp_dmac->ch2_cfg_l.val = (((uint32_t)max_abrst << 20) | ((uint32_t)src_hs_pol << 19) | ((uint32_t)dst_hs_pol << 18) | ((uint32_t)hs_sel_src << 11) | ((uint32_t)hs_sel_dst << 10) | ((uint32_t)ch_susp << 8) | ((uint32_t)ch_prior << 5));
}

__STATIC_INLINE void dmac_ch2_cfg_l_unpack(uint8_t* reload_dst, uint8_t* reload_src, uint16_t* max_abrst, uint8_t* src_hs_pol, uint8_t* dst_hs_pol, uint8_t* lock_b, uint8_t* lock_ch, uint8_t* lock_b_l, uint8_t* lock_ch_l, uint8_t* hs_sel_src, uint8_t* hs_sel_dst, uint8_t* fifo_empty, uint8_t* ch_susp, uint8_t* ch_prior)
{
    T_DMAC_CH2_CFG_L localVal = hwp_dmac->ch2_cfg_l;

    *reload_dst = localVal.bit_field.reload_dst;
    *reload_src = localVal.bit_field.reload_src;
    *max_abrst = localVal.bit_field.max_abrst;
    *src_hs_pol = localVal.bit_field.src_hs_pol;
    *dst_hs_pol = localVal.bit_field.dst_hs_pol;
    *lock_b = localVal.bit_field.lock_b;
    *lock_ch = localVal.bit_field.lock_ch;
    *lock_b_l = localVal.bit_field.lock_b_l;
    *lock_ch_l = localVal.bit_field.lock_ch_l;
    *hs_sel_src = localVal.bit_field.hs_sel_src;
    *hs_sel_dst = localVal.bit_field.hs_sel_dst;
    *fifo_empty = localVal.bit_field.fifo_empty;
    *ch_susp = localVal.bit_field.ch_susp;
    *ch_prior = localVal.bit_field.ch_prior;
}

__STATIC_INLINE uint32_t dmac_ch2_cfg_h_get(void)
{
    return hwp_dmac->ch2_cfg_h.val;
}

__STATIC_INLINE void dmac_ch2_cfg_h_set(uint32_t value)
{
    hwp_dmac->ch2_cfg_h.val = value;
}

__STATIC_INLINE void dmac_ch2_cfg_h_pack(uint8_t dest_per, uint8_t src_per, uint8_t protctl, uint8_t fifo_mode, uint8_t fc_mode)
{
    hwp_dmac->ch2_cfg_h.val = (((uint32_t)dest_per << 11) | ((uint32_t)src_per << 7) | ((uint32_t)protctl << 2) | ((uint32_t)fifo_mode << 1) | ((uint32_t)fc_mode << 0));
}

__STATIC_INLINE void dmac_ch2_cfg_h_unpack(uint8_t* dest_per, uint8_t* src_per, uint8_t* ss_upd_en, uint8_t* ds_upd_en, uint8_t* protctl, uint8_t* fifo_mode, uint8_t* fc_mode)
{
    T_DMAC_CH2_CFG_H localVal = hwp_dmac->ch2_cfg_h;

    *dest_per = localVal.bit_field.dest_per;
    *src_per = localVal.bit_field.src_per;
    *ss_upd_en = localVal.bit_field.ss_upd_en;
    *ds_upd_en = localVal.bit_field.ds_upd_en;
    *protctl = localVal.bit_field.protctl;
    *fifo_mode = localVal.bit_field.fifo_mode;
    *fc_mode = localVal.bit_field.fc_mode;
}

__STATIC_INLINE uint32_t dmac_ch3_sar_l_get(void)
{
    return hwp_dmac->ch3_sar_l.val;
}

__STATIC_INLINE void dmac_ch3_sar_l_set(uint32_t value)
{
    hwp_dmac->ch3_sar_l.val = value;
}

__STATIC_INLINE void dmac_ch3_sar_l_pack(uint32_t sar)
{
    hwp_dmac->ch3_sar_l.val = (((uint32_t)sar << 0));
}

__STATIC_INLINE void dmac_ch3_sar_l_unpack(uint32_t* sar)
{
    T_DMAC_CH3_SAR_L localVal = hwp_dmac->ch3_sar_l;

    *sar = localVal.bit_field.sar;
}

__STATIC_INLINE uint32_t dmac_ch3_sar_h_get(void)
{
    return hwp_dmac->ch3_sar_h.val;
}

__STATIC_INLINE uint32_t dmac_ch3_dar_l_get(void)
{
    return hwp_dmac->ch3_dar_l.val;
}

__STATIC_INLINE void dmac_ch3_dar_l_set(uint32_t value)
{
    hwp_dmac->ch3_dar_l.val = value;
}

__STATIC_INLINE void dmac_ch3_dar_l_pack(uint32_t dar)
{
    hwp_dmac->ch3_dar_l.val = (((uint32_t)dar << 0));
}

__STATIC_INLINE void dmac_ch3_dar_l_unpack(uint32_t* dar)
{
    T_DMAC_CH3_DAR_L localVal = hwp_dmac->ch3_dar_l;

    *dar = localVal.bit_field.dar;
}

__STATIC_INLINE uint32_t dmac_ch3_dar_h_get(void)
{
    return hwp_dmac->ch3_dar_h.val;
}

__STATIC_INLINE uint32_t dmac_ch3_ctrl_l_get(void)
{
    return hwp_dmac->ch3_ctrl_l.val;
}

__STATIC_INLINE void dmac_ch3_ctrl_l_set(uint32_t value)
{
    hwp_dmac->ch3_ctrl_l.val = value;
}

__STATIC_INLINE void dmac_ch3_ctrl_l_pack(uint8_t tt_fc, uint8_t src_msize, uint8_t dest_msize, uint8_t sinc, uint8_t dinc, uint8_t src_tr_width, uint8_t dst_tr_width, uint8_t int_en)
{
    hwp_dmac->ch3_ctrl_l.val = (((uint32_t)tt_fc << 20) | ((uint32_t)src_msize << 14) | ((uint32_t)dest_msize << 11) | ((uint32_t)sinc << 9) | ((uint32_t)dinc << 7) | ((uint32_t)src_tr_width << 4) | ((uint32_t)dst_tr_width << 1) | ((uint32_t)int_en << 0));
}

__STATIC_INLINE void dmac_ch3_ctrl_l_unpack(uint8_t* llp_src_en, uint8_t* llp_dst_en, uint8_t* sms, uint8_t* dms, uint8_t* tt_fc, uint8_t* dst_scatter_en, uint8_t* src_gather_en, uint8_t* src_msize, uint8_t* dest_msize, uint8_t* sinc, uint8_t* dinc, uint8_t* src_tr_width, uint8_t* dst_tr_width, uint8_t* int_en)
{
    T_DMAC_CH3_CTRL_L localVal = hwp_dmac->ch3_ctrl_l;

    *llp_src_en = localVal.bit_field.llp_src_en;
    *llp_dst_en = localVal.bit_field.llp_dst_en;
    *sms = localVal.bit_field.sms;
    *dms = localVal.bit_field.dms;
    *tt_fc = localVal.bit_field.tt_fc;
    *dst_scatter_en = localVal.bit_field.dst_scatter_en;
    *src_gather_en = localVal.bit_field.src_gather_en;
    *src_msize = localVal.bit_field.src_msize;
    *dest_msize = localVal.bit_field.dest_msize;
    *sinc = localVal.bit_field.sinc;
    *dinc = localVal.bit_field.dinc;
    *src_tr_width = localVal.bit_field.src_tr_width;
    *dst_tr_width = localVal.bit_field.dst_tr_width;
    *int_en = localVal.bit_field.int_en;
}

__STATIC_INLINE uint32_t dmac_ch3_ctrl_h_get(void)
{
    return hwp_dmac->ch3_ctrl_h.val;
}

__STATIC_INLINE void dmac_ch3_ctrl_h_set(uint32_t value)
{
    hwp_dmac->ch3_ctrl_h.val = value;
}

__STATIC_INLINE void dmac_ch3_ctrl_h_pack(uint8_t done, uint16_t block_ts)
{
    hwp_dmac->ch3_ctrl_h.val = (((uint32_t)done << 12) | ((uint32_t)block_ts << 0));
}

__STATIC_INLINE void dmac_ch3_ctrl_h_unpack(uint8_t* done, uint16_t* block_ts)
{
    T_DMAC_CH3_CTRL_H localVal = hwp_dmac->ch3_ctrl_h;

    *done = localVal.bit_field.done;
    *block_ts = localVal.bit_field.block_ts;
}

__STATIC_INLINE uint32_t dmac_ch3_cfg_l_get(void)
{
    return hwp_dmac->ch3_cfg_l.val;
}

__STATIC_INLINE void dmac_ch3_cfg_l_set(uint32_t value)
{
    hwp_dmac->ch3_cfg_l.val = value;
}

__STATIC_INLINE void dmac_ch3_cfg_l_pack(uint16_t max_abrst, uint8_t src_hs_pol, uint8_t dst_hs_pol, uint8_t hs_sel_src, uint8_t hs_sel_dst, uint8_t ch_susp, uint8_t ch_prior)
{
    hwp_dmac->ch3_cfg_l.val = (((uint32_t)max_abrst << 20) | ((uint32_t)src_hs_pol << 19) | ((uint32_t)dst_hs_pol << 18) | ((uint32_t)hs_sel_src << 11) | ((uint32_t)hs_sel_dst << 10) | ((uint32_t)ch_susp << 8) | ((uint32_t)ch_prior << 5));
}

__STATIC_INLINE void dmac_ch3_cfg_l_unpack(uint8_t* reload_dst, uint8_t* reload_src, uint16_t* max_abrst, uint8_t* src_hs_pol, uint8_t* dst_hs_pol, uint8_t* lock_b, uint8_t* lock_ch, uint8_t* lock_b_l, uint8_t* lock_ch_l, uint8_t* hs_sel_src, uint8_t* hs_sel_dst, uint8_t* fifo_empty, uint8_t* ch_susp, uint8_t* ch_prior)
{
    T_DMAC_CH3_CFG_L localVal = hwp_dmac->ch3_cfg_l;

    *reload_dst = localVal.bit_field.reload_dst;
    *reload_src = localVal.bit_field.reload_src;
    *max_abrst = localVal.bit_field.max_abrst;
    *src_hs_pol = localVal.bit_field.src_hs_pol;
    *dst_hs_pol = localVal.bit_field.dst_hs_pol;
    *lock_b = localVal.bit_field.lock_b;
    *lock_ch = localVal.bit_field.lock_ch;
    *lock_b_l = localVal.bit_field.lock_b_l;
    *lock_ch_l = localVal.bit_field.lock_ch_l;
    *hs_sel_src = localVal.bit_field.hs_sel_src;
    *hs_sel_dst = localVal.bit_field.hs_sel_dst;
    *fifo_empty = localVal.bit_field.fifo_empty;
    *ch_susp = localVal.bit_field.ch_susp;
    *ch_prior = localVal.bit_field.ch_prior;
}

__STATIC_INLINE uint32_t dmac_ch3_cfg_h_get(void)
{
    return hwp_dmac->ch3_cfg_h.val;
}

__STATIC_INLINE void dmac_ch3_cfg_h_set(uint32_t value)
{
    hwp_dmac->ch3_cfg_h.val = value;
}

__STATIC_INLINE void dmac_ch3_cfg_h_pack(uint8_t dest_per, uint8_t src_per, uint8_t protctl, uint8_t fifo_mode, uint8_t fc_mode)
{
    hwp_dmac->ch3_cfg_h.val = (((uint32_t)dest_per << 11) | ((uint32_t)src_per << 7) | ((uint32_t)protctl << 2) | ((uint32_t)fifo_mode << 1) | ((uint32_t)fc_mode << 0));
}

__STATIC_INLINE void dmac_ch3_cfg_h_unpack(uint8_t* dest_per, uint8_t* src_per, uint8_t* ss_upd_en, uint8_t* ds_upd_en, uint8_t* protctl, uint8_t* fifo_mode, uint8_t* fc_mode)
{
    T_DMAC_CH3_CFG_H localVal = hwp_dmac->ch3_cfg_h;

    *dest_per = localVal.bit_field.dest_per;
    *src_per = localVal.bit_field.src_per;
    *ss_upd_en = localVal.bit_field.ss_upd_en;
    *ds_upd_en = localVal.bit_field.ds_upd_en;
    *protctl = localVal.bit_field.protctl;
    *fifo_mode = localVal.bit_field.fifo_mode;
    *fc_mode = localVal.bit_field.fc_mode;
}

__STATIC_INLINE uint32_t dmac_ch4_sar_l_get(void)
{
    return hwp_dmac->ch4_sar_l.val;
}

__STATIC_INLINE void dmac_ch4_sar_l_set(uint32_t value)
{
    hwp_dmac->ch4_sar_l.val = value;
}

__STATIC_INLINE void dmac_ch4_sar_l_pack(uint32_t sar)
{
    hwp_dmac->ch4_sar_l.val = (((uint32_t)sar << 0));
}

__STATIC_INLINE void dmac_ch4_sar_l_unpack(uint32_t* sar)
{
    T_DMAC_CH4_SAR_L localVal = hwp_dmac->ch4_sar_l;

    *sar = localVal.bit_field.sar;
}

__STATIC_INLINE uint32_t dmac_ch4_sar_h_get(void)
{
    return hwp_dmac->ch4_sar_h.val;
}

__STATIC_INLINE uint32_t dmac_ch4_dar_l_get(void)
{
    return hwp_dmac->ch4_dar_l.val;
}

__STATIC_INLINE void dmac_ch4_dar_l_set(uint32_t value)
{
    hwp_dmac->ch4_dar_l.val = value;
}

__STATIC_INLINE void dmac_ch4_dar_l_pack(uint32_t dar)
{
    hwp_dmac->ch4_dar_l.val = (((uint32_t)dar << 0));
}

__STATIC_INLINE void dmac_ch4_dar_l_unpack(uint32_t* dar)
{
    T_DMAC_CH4_DAR_L localVal = hwp_dmac->ch4_dar_l;

    *dar = localVal.bit_field.dar;
}

__STATIC_INLINE uint32_t dmac_ch4_dar_h_get(void)
{
    return hwp_dmac->ch4_dar_h.val;
}

__STATIC_INLINE uint32_t dmac_ch4_ctrl_l_get(void)
{
    return hwp_dmac->ch4_ctrl_l.val;
}

__STATIC_INLINE void dmac_ch4_ctrl_l_set(uint32_t value)
{
    hwp_dmac->ch4_ctrl_l.val = value;
}

__STATIC_INLINE void dmac_ch4_ctrl_l_pack(uint8_t tt_fc, uint8_t src_msize, uint8_t dest_msize, uint8_t sinc, uint8_t dinc, uint8_t src_tr_width, uint8_t dst_tr_width, uint8_t int_en)
{
    hwp_dmac->ch4_ctrl_l.val = (((uint32_t)tt_fc << 20) | ((uint32_t)src_msize << 14) | ((uint32_t)dest_msize << 11) | ((uint32_t)sinc << 9) | ((uint32_t)dinc << 7) | ((uint32_t)src_tr_width << 4) | ((uint32_t)dst_tr_width << 1) | ((uint32_t)int_en << 0));
}

__STATIC_INLINE void dmac_ch4_ctrl_l_unpack(uint8_t* llp_src_en, uint8_t* llp_dst_en, uint8_t* sms, uint8_t* dms, uint8_t* tt_fc, uint8_t* dst_scatter_en, uint8_t* src_gather_en, uint8_t* src_msize, uint8_t* dest_msize, uint8_t* sinc, uint8_t* dinc, uint8_t* src_tr_width, uint8_t* dst_tr_width, uint8_t* int_en)
{
    T_DMAC_CH4_CTRL_L localVal = hwp_dmac->ch4_ctrl_l;

    *llp_src_en = localVal.bit_field.llp_src_en;
    *llp_dst_en = localVal.bit_field.llp_dst_en;
    *sms = localVal.bit_field.sms;
    *dms = localVal.bit_field.dms;
    *tt_fc = localVal.bit_field.tt_fc;
    *dst_scatter_en = localVal.bit_field.dst_scatter_en;
    *src_gather_en = localVal.bit_field.src_gather_en;
    *src_msize = localVal.bit_field.src_msize;
    *dest_msize = localVal.bit_field.dest_msize;
    *sinc = localVal.bit_field.sinc;
    *dinc = localVal.bit_field.dinc;
    *src_tr_width = localVal.bit_field.src_tr_width;
    *dst_tr_width = localVal.bit_field.dst_tr_width;
    *int_en = localVal.bit_field.int_en;
}

__STATIC_INLINE uint32_t dmac_ch4_ctrl_h_get(void)
{
    return hwp_dmac->ch4_ctrl_h.val;
}

__STATIC_INLINE void dmac_ch4_ctrl_h_set(uint32_t value)
{
    hwp_dmac->ch4_ctrl_h.val = value;
}

__STATIC_INLINE void dmac_ch4_ctrl_h_pack(uint8_t done, uint16_t block_ts)
{
    hwp_dmac->ch4_ctrl_h.val = (((uint32_t)done << 12) | ((uint32_t)block_ts << 0));
}

__STATIC_INLINE void dmac_ch4_ctrl_h_unpack(uint8_t* done, uint16_t* block_ts)
{
    T_DMAC_CH4_CTRL_H localVal = hwp_dmac->ch4_ctrl_h;

    *done = localVal.bit_field.done;
    *block_ts = localVal.bit_field.block_ts;
}

__STATIC_INLINE uint32_t dmac_ch4_cfg_l_get(void)
{
    return hwp_dmac->ch4_cfg_l.val;
}

__STATIC_INLINE void dmac_ch4_cfg_l_set(uint32_t value)
{
    hwp_dmac->ch4_cfg_l.val = value;
}

__STATIC_INLINE void dmac_ch4_cfg_l_pack(uint16_t max_abrst, uint8_t src_hs_pol, uint8_t dst_hs_pol, uint8_t hs_sel_src, uint8_t hs_sel_dst, uint8_t ch_susp, uint8_t ch_prior)
{
    hwp_dmac->ch4_cfg_l.val = (((uint32_t)max_abrst << 20) | ((uint32_t)src_hs_pol << 19) | ((uint32_t)dst_hs_pol << 18) | ((uint32_t)hs_sel_src << 11) | ((uint32_t)hs_sel_dst << 10) | ((uint32_t)ch_susp << 8) | ((uint32_t)ch_prior << 5));
}

__STATIC_INLINE void dmac_ch4_cfg_l_unpack(uint8_t* reload_dst, uint8_t* reload_src, uint16_t* max_abrst, uint8_t* src_hs_pol, uint8_t* dst_hs_pol, uint8_t* lock_b, uint8_t* lock_ch, uint8_t* lock_b_l, uint8_t* lock_ch_l, uint8_t* hs_sel_src, uint8_t* hs_sel_dst, uint8_t* fifo_empty, uint8_t* ch_susp, uint8_t* ch_prior)
{
    T_DMAC_CH4_CFG_L localVal = hwp_dmac->ch4_cfg_l;

    *reload_dst = localVal.bit_field.reload_dst;
    *reload_src = localVal.bit_field.reload_src;
    *max_abrst = localVal.bit_field.max_abrst;
    *src_hs_pol = localVal.bit_field.src_hs_pol;
    *dst_hs_pol = localVal.bit_field.dst_hs_pol;
    *lock_b = localVal.bit_field.lock_b;
    *lock_ch = localVal.bit_field.lock_ch;
    *lock_b_l = localVal.bit_field.lock_b_l;
    *lock_ch_l = localVal.bit_field.lock_ch_l;
    *hs_sel_src = localVal.bit_field.hs_sel_src;
    *hs_sel_dst = localVal.bit_field.hs_sel_dst;
    *fifo_empty = localVal.bit_field.fifo_empty;
    *ch_susp = localVal.bit_field.ch_susp;
    *ch_prior = localVal.bit_field.ch_prior;
}

__STATIC_INLINE uint32_t dmac_ch4_cfg_h_get(void)
{
    return hwp_dmac->ch4_cfg_h.val;
}

__STATIC_INLINE void dmac_ch4_cfg_h_set(uint32_t value)
{
    hwp_dmac->ch4_cfg_h.val = value;
}

__STATIC_INLINE void dmac_ch4_cfg_h_pack(uint8_t dest_per, uint8_t src_per, uint8_t protctl, uint8_t fifo_mode, uint8_t fc_mode)
{
    hwp_dmac->ch4_cfg_h.val = (((uint32_t)dest_per << 11) | ((uint32_t)src_per << 7) | ((uint32_t)protctl << 2) | ((uint32_t)fifo_mode << 1) | ((uint32_t)fc_mode << 0));
}

__STATIC_INLINE void dmac_ch4_cfg_h_unpack(uint8_t* dest_per, uint8_t* src_per, uint8_t* ss_upd_en, uint8_t* ds_upd_en, uint8_t* protctl, uint8_t* fifo_mode, uint8_t* fc_mode)
{
    T_DMAC_CH4_CFG_H localVal = hwp_dmac->ch4_cfg_h;

    *dest_per = localVal.bit_field.dest_per;
    *src_per = localVal.bit_field.src_per;
    *ss_upd_en = localVal.bit_field.ss_upd_en;
    *ds_upd_en = localVal.bit_field.ds_upd_en;
    *protctl = localVal.bit_field.protctl;
    *fifo_mode = localVal.bit_field.fifo_mode;
    *fc_mode = localVal.bit_field.fc_mode;
}

__STATIC_INLINE uint32_t dmac_ch5_sar_l_get(void)
{
    return hwp_dmac->ch5_sar_l.val;
}

__STATIC_INLINE void dmac_ch5_sar_l_set(uint32_t value)
{
    hwp_dmac->ch5_sar_l.val = value;
}

__STATIC_INLINE void dmac_ch5_sar_l_pack(uint32_t sar)
{
    hwp_dmac->ch5_sar_l.val = (((uint32_t)sar << 0));
}

__STATIC_INLINE void dmac_ch5_sar_l_unpack(uint32_t* sar)
{
    T_DMAC_CH5_SAR_L localVal = hwp_dmac->ch5_sar_l;

    *sar = localVal.bit_field.sar;
}

__STATIC_INLINE uint32_t dmac_ch5_sar_h_get(void)
{
    return hwp_dmac->ch5_sar_h.val;
}

__STATIC_INLINE uint32_t dmac_ch5_dar_l_get(void)
{
    return hwp_dmac->ch5_dar_l.val;
}

__STATIC_INLINE void dmac_ch5_dar_l_set(uint32_t value)
{
    hwp_dmac->ch5_dar_l.val = value;
}

__STATIC_INLINE void dmac_ch5_dar_l_pack(uint32_t dar)
{
    hwp_dmac->ch5_dar_l.val = (((uint32_t)dar << 0));
}

__STATIC_INLINE void dmac_ch5_dar_l_unpack(uint32_t* dar)
{
    T_DMAC_CH5_DAR_L localVal = hwp_dmac->ch5_dar_l;

    *dar = localVal.bit_field.dar;
}

__STATIC_INLINE uint32_t dmac_ch5_dar_h_get(void)
{
    return hwp_dmac->ch5_dar_h.val;
}

__STATIC_INLINE uint32_t dmac_ch5_ctrl_l_get(void)
{
    return hwp_dmac->ch5_ctrl_l.val;
}

__STATIC_INLINE void dmac_ch5_ctrl_l_set(uint32_t value)
{
    hwp_dmac->ch5_ctrl_l.val = value;
}

__STATIC_INLINE void dmac_ch5_ctrl_l_pack(uint8_t tt_fc, uint8_t src_msize, uint8_t dest_msize, uint8_t sinc, uint8_t dinc, uint8_t src_tr_width, uint8_t dst_tr_width, uint8_t int_en)
{
    hwp_dmac->ch5_ctrl_l.val = (((uint32_t)tt_fc << 20) | ((uint32_t)src_msize << 14) | ((uint32_t)dest_msize << 11) | ((uint32_t)sinc << 9) | ((uint32_t)dinc << 7) | ((uint32_t)src_tr_width << 4) | ((uint32_t)dst_tr_width << 1) | ((uint32_t)int_en << 0));
}

__STATIC_INLINE void dmac_ch5_ctrl_l_unpack(uint8_t* llp_src_en, uint8_t* llp_dst_en, uint8_t* sms, uint8_t* dms, uint8_t* tt_fc, uint8_t* dst_scatter_en, uint8_t* src_gather_en, uint8_t* src_msize, uint8_t* dest_msize, uint8_t* sinc, uint8_t* dinc, uint8_t* src_tr_width, uint8_t* dst_tr_width, uint8_t* int_en)
{
    T_DMAC_CH5_CTRL_L localVal = hwp_dmac->ch5_ctrl_l;

    *llp_src_en = localVal.bit_field.llp_src_en;
    *llp_dst_en = localVal.bit_field.llp_dst_en;
    *sms = localVal.bit_field.sms;
    *dms = localVal.bit_field.dms;
    *tt_fc = localVal.bit_field.tt_fc;
    *dst_scatter_en = localVal.bit_field.dst_scatter_en;
    *src_gather_en = localVal.bit_field.src_gather_en;
    *src_msize = localVal.bit_field.src_msize;
    *dest_msize = localVal.bit_field.dest_msize;
    *sinc = localVal.bit_field.sinc;
    *dinc = localVal.bit_field.dinc;
    *src_tr_width = localVal.bit_field.src_tr_width;
    *dst_tr_width = localVal.bit_field.dst_tr_width;
    *int_en = localVal.bit_field.int_en;
}

__STATIC_INLINE uint32_t dmac_ch5_ctrl_h_get(void)
{
    return hwp_dmac->ch5_ctrl_h.val;
}

__STATIC_INLINE void dmac_ch5_ctrl_h_set(uint32_t value)
{
    hwp_dmac->ch5_ctrl_h.val = value;
}

__STATIC_INLINE void dmac_ch5_ctrl_h_pack(uint8_t done, uint16_t block_ts)
{
    hwp_dmac->ch5_ctrl_h.val = (((uint32_t)done << 12) | ((uint32_t)block_ts << 0));
}

__STATIC_INLINE void dmac_ch5_ctrl_h_unpack(uint8_t* done, uint16_t* block_ts)
{
    T_DMAC_CH5_CTRL_H localVal = hwp_dmac->ch5_ctrl_h;

    *done = localVal.bit_field.done;
    *block_ts = localVal.bit_field.block_ts;
}

__STATIC_INLINE uint32_t dmac_ch5_cfg_l_get(void)
{
    return hwp_dmac->ch5_cfg_l.val;
}

__STATIC_INLINE void dmac_ch5_cfg_l_set(uint32_t value)
{
    hwp_dmac->ch5_cfg_l.val = value;
}

__STATIC_INLINE void dmac_ch5_cfg_l_pack(uint16_t max_abrst, uint8_t src_hs_pol, uint8_t dst_hs_pol, uint8_t hs_sel_src, uint8_t hs_sel_dst, uint8_t ch_susp, uint8_t ch_prior)
{
    hwp_dmac->ch5_cfg_l.val = (((uint32_t)max_abrst << 20) | ((uint32_t)src_hs_pol << 19) | ((uint32_t)dst_hs_pol << 18) | ((uint32_t)hs_sel_src << 11) | ((uint32_t)hs_sel_dst << 10) | ((uint32_t)ch_susp << 8) | ((uint32_t)ch_prior << 5));
}

__STATIC_INLINE void dmac_ch5_cfg_l_unpack(uint8_t* reload_dst, uint8_t* reload_src, uint16_t* max_abrst, uint8_t* src_hs_pol, uint8_t* dst_hs_pol, uint8_t* lock_b, uint8_t* lock_ch, uint8_t* lock_b_l, uint8_t* lock_ch_l, uint8_t* hs_sel_src, uint8_t* hs_sel_dst, uint8_t* fifo_empty, uint8_t* ch_susp, uint8_t* ch_prior)
{
    T_DMAC_CH5_CFG_L localVal = hwp_dmac->ch5_cfg_l;

    *reload_dst = localVal.bit_field.reload_dst;
    *reload_src = localVal.bit_field.reload_src;
    *max_abrst = localVal.bit_field.max_abrst;
    *src_hs_pol = localVal.bit_field.src_hs_pol;
    *dst_hs_pol = localVal.bit_field.dst_hs_pol;
    *lock_b = localVal.bit_field.lock_b;
    *lock_ch = localVal.bit_field.lock_ch;
    *lock_b_l = localVal.bit_field.lock_b_l;
    *lock_ch_l = localVal.bit_field.lock_ch_l;
    *hs_sel_src = localVal.bit_field.hs_sel_src;
    *hs_sel_dst = localVal.bit_field.hs_sel_dst;
    *fifo_empty = localVal.bit_field.fifo_empty;
    *ch_susp = localVal.bit_field.ch_susp;
    *ch_prior = localVal.bit_field.ch_prior;
}

__STATIC_INLINE uint32_t dmac_ch5_cfg_h_get(void)
{
    return hwp_dmac->ch5_cfg_h.val;
}

__STATIC_INLINE void dmac_ch5_cfg_h_set(uint32_t value)
{
    hwp_dmac->ch5_cfg_h.val = value;
}

__STATIC_INLINE void dmac_ch5_cfg_h_pack(uint8_t dest_per, uint8_t src_per, uint8_t protctl, uint8_t fifo_mode, uint8_t fc_mode)
{
    hwp_dmac->ch5_cfg_h.val = (((uint32_t)dest_per << 11) | ((uint32_t)src_per << 7) | ((uint32_t)protctl << 2) | ((uint32_t)fifo_mode << 1) | ((uint32_t)fc_mode << 0));
}

__STATIC_INLINE void dmac_ch5_cfg_h_unpack(uint8_t* dest_per, uint8_t* src_per, uint8_t* ss_upd_en, uint8_t* ds_upd_en, uint8_t* protctl, uint8_t* fifo_mode, uint8_t* fc_mode)
{
    T_DMAC_CH5_CFG_H localVal = hwp_dmac->ch5_cfg_h;

    *dest_per = localVal.bit_field.dest_per;
    *src_per = localVal.bit_field.src_per;
    *ss_upd_en = localVal.bit_field.ss_upd_en;
    *ds_upd_en = localVal.bit_field.ds_upd_en;
    *protctl = localVal.bit_field.protctl;
    *fifo_mode = localVal.bit_field.fifo_mode;
    *fc_mode = localVal.bit_field.fc_mode;
}

__STATIC_INLINE uint32_t dmac_rawtfr_l_get(void)
{
    return hwp_dmac->rawtfr_l.val;
}

__STATIC_INLINE void dmac_rawtfr_l_set(uint32_t value)
{
    hwp_dmac->rawtfr_l.val = value;
}

__STATIC_INLINE void dmac_rawtfr_l_pack(uint8_t raw)
{
    hwp_dmac->rawtfr_l.val = (((uint32_t)raw << 0));
}

__STATIC_INLINE void dmac_rawtfr_l_unpack(uint8_t* raw)
{
    T_DMAC_RAWTFR_L localVal = hwp_dmac->rawtfr_l;

    *raw = localVal.bit_field.raw;
}

__STATIC_INLINE uint32_t dmac_rawtfr_h_get(void)
{
    return hwp_dmac->rawtfr_h.val;
}

__STATIC_INLINE uint32_t dmac_rawblock_l_get(void)
{
    return hwp_dmac->rawblock_l.val;
}

__STATIC_INLINE void dmac_rawblock_l_set(uint32_t value)
{
    hwp_dmac->rawblock_l.val = value;
}

__STATIC_INLINE void dmac_rawblock_l_pack(uint8_t raw)
{
    hwp_dmac->rawblock_l.val = (((uint32_t)raw << 0));
}

__STATIC_INLINE void dmac_rawblock_l_unpack(uint8_t* raw)
{
    T_DMAC_RAWBLOCK_L localVal = hwp_dmac->rawblock_l;

    *raw = localVal.bit_field.raw;
}

__STATIC_INLINE uint32_t dmac_rawblock_h_get(void)
{
    return hwp_dmac->rawblock_h.val;
}

__STATIC_INLINE uint32_t dmac_rawsrctran_l_get(void)
{
    return hwp_dmac->rawsrctran_l.val;
}

__STATIC_INLINE void dmac_rawsrctran_l_set(uint32_t value)
{
    hwp_dmac->rawsrctran_l.val = value;
}

__STATIC_INLINE void dmac_rawsrctran_l_pack(uint8_t raw)
{
    hwp_dmac->rawsrctran_l.val = (((uint32_t)raw << 0));
}

__STATIC_INLINE void dmac_rawsrctran_l_unpack(uint8_t* raw)
{
    T_DMAC_RAWSRCTRAN_L localVal = hwp_dmac->rawsrctran_l;

    *raw = localVal.bit_field.raw;
}

__STATIC_INLINE uint32_t dmac_rawsrctran_h_get(void)
{
    return hwp_dmac->rawsrctran_h.val;
}

__STATIC_INLINE uint32_t dmac_rawdsttran_l_get(void)
{
    return hwp_dmac->rawdsttran_l.val;
}

__STATIC_INLINE void dmac_rawdsttran_l_set(uint32_t value)
{
    hwp_dmac->rawdsttran_l.val = value;
}

__STATIC_INLINE void dmac_rawdsttran_l_pack(uint8_t raw)
{
    hwp_dmac->rawdsttran_l.val = (((uint32_t)raw << 0));
}

__STATIC_INLINE void dmac_rawdsttran_l_unpack(uint8_t* raw)
{
    T_DMAC_RAWDSTTRAN_L localVal = hwp_dmac->rawdsttran_l;

    *raw = localVal.bit_field.raw;
}

__STATIC_INLINE uint32_t dmac_rawdsttran_h_get(void)
{
    return hwp_dmac->rawdsttran_h.val;
}

__STATIC_INLINE uint32_t dmac_rawerr_l_get(void)
{
    return hwp_dmac->rawerr_l.val;
}

__STATIC_INLINE void dmac_rawerr_l_set(uint32_t value)
{
    hwp_dmac->rawerr_l.val = value;
}

__STATIC_INLINE void dmac_rawerr_l_pack(uint8_t raw)
{
    hwp_dmac->rawerr_l.val = (((uint32_t)raw << 0));
}

__STATIC_INLINE void dmac_rawerr_l_unpack(uint8_t* raw)
{
    T_DMAC_RAWERR_L localVal = hwp_dmac->rawerr_l;

    *raw = localVal.bit_field.raw;
}

__STATIC_INLINE uint32_t dmac_rawerr_h_get(void)
{
    return hwp_dmac->rawerr_h.val;
}

__STATIC_INLINE uint32_t dmac_statustfr_l_get(void)
{
    return hwp_dmac->statustfr_l.val;
}

__STATIC_INLINE void dmac_statustfr_l_unpack(uint8_t* status)
{
    T_DMAC_STATUSTFR_L localVal = hwp_dmac->statustfr_l;

    *status = localVal.bit_field.status;
}

__STATIC_INLINE uint32_t dmac_statustfr_h_get(void)
{
    return hwp_dmac->statustfr_h.val;
}

__STATIC_INLINE uint32_t dmac_statusblock_l_get(void)
{
    return hwp_dmac->statusblock_l.val;
}

__STATIC_INLINE void dmac_statusblock_l_unpack(uint8_t* status)
{
    T_DMAC_STATUSBLOCK_L localVal = hwp_dmac->statusblock_l;

    *status = localVal.bit_field.status;
}

__STATIC_INLINE uint32_t dmac_statusblock_h_get(void)
{
    return hwp_dmac->statusblock_h.val;
}

__STATIC_INLINE uint32_t dmac_statussrctran_l_get(void)
{
    return hwp_dmac->statussrctran_l.val;
}

__STATIC_INLINE void dmac_statussrctran_l_unpack(uint8_t* status)
{
    T_DMAC_STATUSSRCTRAN_L localVal = hwp_dmac->statussrctran_l;

    *status = localVal.bit_field.status;
}

__STATIC_INLINE uint32_t dmac_statussrctran_h_get(void)
{
    return hwp_dmac->statussrctran_h.val;
}

__STATIC_INLINE uint32_t dmac_statusdsttran_l_get(void)
{
    return hwp_dmac->statusdsttran_l.val;
}

__STATIC_INLINE void dmac_statusdsttran_l_unpack(uint8_t* status)
{
    T_DMAC_STATUSDSTTRAN_L localVal = hwp_dmac->statusdsttran_l;

    *status = localVal.bit_field.status;
}

__STATIC_INLINE uint32_t dmac_statusdsttran_h_get(void)
{
    return hwp_dmac->statusdsttran_h.val;
}

__STATIC_INLINE uint32_t dmac_statuserr_l_get(void)
{
    return hwp_dmac->statuserr_l.val;
}

__STATIC_INLINE void dmac_statuserr_l_unpack(uint8_t* status)
{
    T_DMAC_STATUSERR_L localVal = hwp_dmac->statuserr_l;

    *status = localVal.bit_field.status;
}

__STATIC_INLINE uint32_t dmac_statuserr_h_get(void)
{
    return hwp_dmac->statuserr_h.val;
}

__STATIC_INLINE uint32_t dmac_masktfr_l_get(void)
{
    return hwp_dmac->masktfr_l.val;
}

__STATIC_INLINE void dmac_masktfr_l_set(uint32_t value)
{
    hwp_dmac->masktfr_l.val = value;
}

__STATIC_INLINE void dmac_masktfr_l_pack(uint8_t int_mask_we, uint8_t int_mask)
{
    hwp_dmac->masktfr_l.val = (((uint32_t)int_mask_we << 8) | ((uint32_t)int_mask << 0));
}

__STATIC_INLINE void dmac_masktfr_l_unpack(uint8_t* int_mask)
{
    T_DMAC_MASKTFR_L localVal = hwp_dmac->masktfr_l;

    *int_mask = localVal.bit_field.int_mask;
}

__STATIC_INLINE uint32_t dmac_masktfr_h_get(void)
{
    return hwp_dmac->masktfr_h.val;
}

__STATIC_INLINE uint32_t dmac_maskblock_l_get(void)
{
    return hwp_dmac->maskblock_l.val;
}

__STATIC_INLINE void dmac_maskblock_l_set(uint32_t value)
{
    hwp_dmac->maskblock_l.val = value;
}

__STATIC_INLINE void dmac_maskblock_l_pack(uint8_t int_mask_we, uint8_t int_mask)
{
    hwp_dmac->maskblock_l.val = (((uint32_t)int_mask_we << 8) | ((uint32_t)int_mask << 0));
}

__STATIC_INLINE void dmac_maskblock_l_unpack(uint8_t* int_mask)
{
    T_DMAC_MASKBLOCK_L localVal = hwp_dmac->maskblock_l;

    *int_mask = localVal.bit_field.int_mask;
}

__STATIC_INLINE uint32_t dmac_maskblock_h_get(void)
{
    return hwp_dmac->maskblock_h.val;
}

__STATIC_INLINE uint32_t dmac_masksrctran_l_get(void)
{
    return hwp_dmac->masksrctran_l.val;
}

__STATIC_INLINE void dmac_masksrctran_l_set(uint32_t value)
{
    hwp_dmac->masksrctran_l.val = value;
}

__STATIC_INLINE void dmac_masksrctran_l_pack(uint8_t int_mask_we, uint8_t int_mask)
{
    hwp_dmac->masksrctran_l.val = (((uint32_t)int_mask_we << 8) | ((uint32_t)int_mask << 0));
}

__STATIC_INLINE void dmac_masksrctran_l_unpack(uint8_t* int_mask)
{
    T_DMAC_MASKSRCTRAN_L localVal = hwp_dmac->masksrctran_l;

    *int_mask = localVal.bit_field.int_mask;
}

__STATIC_INLINE uint32_t dmac_masksrctran_h_get(void)
{
    return hwp_dmac->masksrctran_h.val;
}

__STATIC_INLINE uint32_t dmac_maskdsttran_l_get(void)
{
    return hwp_dmac->maskdsttran_l.val;
}

__STATIC_INLINE void dmac_maskdsttran_l_set(uint32_t value)
{
    hwp_dmac->maskdsttran_l.val = value;
}

__STATIC_INLINE void dmac_maskdsttran_l_pack(uint8_t int_mask_we, uint8_t int_mask)
{
    hwp_dmac->maskdsttran_l.val = (((uint32_t)int_mask_we << 8) | ((uint32_t)int_mask << 0));
}

__STATIC_INLINE void dmac_maskdsttran_l_unpack(uint8_t* int_mask)
{
    T_DMAC_MASKDSTTRAN_L localVal = hwp_dmac->maskdsttran_l;

    *int_mask = localVal.bit_field.int_mask;
}

__STATIC_INLINE uint32_t dmac_maskdsttran_h_get(void)
{
    return hwp_dmac->maskdsttran_h.val;
}

__STATIC_INLINE uint32_t dmac_maskerr_l_get(void)
{
    return hwp_dmac->maskerr_l.val;
}

__STATIC_INLINE void dmac_maskerr_l_set(uint32_t value)
{
    hwp_dmac->maskerr_l.val = value;
}

__STATIC_INLINE void dmac_maskerr_l_pack(uint8_t int_mask_we, uint8_t int_mask)
{
    hwp_dmac->maskerr_l.val = (((uint32_t)int_mask_we << 8) | ((uint32_t)int_mask << 0));
}

__STATIC_INLINE void dmac_maskerr_l_unpack(uint8_t* int_mask)
{
    T_DMAC_MASKERR_L localVal = hwp_dmac->maskerr_l;

    *int_mask = localVal.bit_field.int_mask;
}

__STATIC_INLINE uint32_t dmac_maskerr_h_get(void)
{
    return hwp_dmac->maskerr_h.val;
}

__STATIC_INLINE void dmac_cleartfr_l_set(uint32_t value)
{
    hwp_dmac->cleartfr_l.val = value;
}

__STATIC_INLINE void dmac_cleartfr_l_pack(uint8_t clear)
{
    hwp_dmac->cleartfr_l.val = (((uint32_t)clear << 0));
}

__STATIC_INLINE uint32_t dmac_cleartfr_h_get(void)
{
    return hwp_dmac->cleartfr_h.val;
}

__STATIC_INLINE void dmac_clearblock_l_set(uint32_t value)
{
    hwp_dmac->clearblock_l.val = value;
}

__STATIC_INLINE void dmac_clearblock_l_pack(uint8_t clear)
{
    hwp_dmac->clearblock_l.val = (((uint32_t)clear << 0));
}

__STATIC_INLINE uint32_t dmac_clearblock_h_get(void)
{
    return hwp_dmac->clearblock_h.val;
}

__STATIC_INLINE void dmac_clearsrctran_l_set(uint32_t value)
{
    hwp_dmac->clearsrctran_l.val = value;
}

__STATIC_INLINE void dmac_clearsrctran_l_pack(uint8_t clear)
{
    hwp_dmac->clearsrctran_l.val = (((uint32_t)clear << 0));
}

__STATIC_INLINE uint32_t dmac_clearsrctran_h_get(void)
{
    return hwp_dmac->clearsrctran_h.val;
}

__STATIC_INLINE void dmac_cleardsttran_l_set(uint32_t value)
{
    hwp_dmac->cleardsttran_l.val = value;
}

__STATIC_INLINE void dmac_cleardsttran_l_pack(uint8_t clear)
{
    hwp_dmac->cleardsttran_l.val = (((uint32_t)clear << 0));
}

__STATIC_INLINE uint32_t dmac_cleardsttran_h_get(void)
{
    return hwp_dmac->cleardsttran_h.val;
}

__STATIC_INLINE void dmac_clearerr_l_set(uint32_t value)
{
    hwp_dmac->clearerr_l.val = value;
}

__STATIC_INLINE void dmac_clearerr_l_pack(uint8_t clear)
{
    hwp_dmac->clearerr_l.val = (((uint32_t)clear << 0));
}

__STATIC_INLINE uint32_t dmac_clearerr_h_get(void)
{
    return hwp_dmac->clearerr_h.val;
}

__STATIC_INLINE uint32_t dmac_statusint_l_get(void)
{
    return hwp_dmac->statusint_l.val;
}

__STATIC_INLINE void dmac_statusint_l_unpack(uint8_t* err, uint8_t* dstt, uint8_t* srct, uint8_t* block, uint8_t* tfr)
{
    T_DMAC_STATUSINT_L localVal = hwp_dmac->statusint_l;

    *err = localVal.bit_field.err;
    *dstt = localVal.bit_field.dstt;
    *srct = localVal.bit_field.srct;
    *block = localVal.bit_field.block;
    *tfr = localVal.bit_field.tfr;
}

__STATIC_INLINE uint32_t dmac_statusint_h_get(void)
{
    return hwp_dmac->statusint_h.val;
}

__STATIC_INLINE uint32_t dmac_reqsrcreg_l_get(void)
{
    return hwp_dmac->reqsrcreg_l.val;
}

__STATIC_INLINE void dmac_reqsrcreg_l_set(uint32_t value)
{
    hwp_dmac->reqsrcreg_l.val = value;
}

__STATIC_INLINE void dmac_reqsrcreg_l_pack(uint8_t src_req_we, uint8_t src_req)
{
    hwp_dmac->reqsrcreg_l.val = (((uint32_t)src_req_we << 8) | ((uint32_t)src_req << 0));
}

__STATIC_INLINE void dmac_reqsrcreg_l_unpack(uint8_t* src_req)
{
    T_DMAC_REQSRCREG_L localVal = hwp_dmac->reqsrcreg_l;

    *src_req = localVal.bit_field.src_req;
}

__STATIC_INLINE uint32_t dmac_reqsrcreg_h_get(void)
{
    return hwp_dmac->reqsrcreg_h.val;
}

__STATIC_INLINE uint32_t dmac_reqdstreg_l_get(void)
{
    return hwp_dmac->reqdstreg_l.val;
}

__STATIC_INLINE void dmac_reqdstreg_l_set(uint32_t value)
{
    hwp_dmac->reqdstreg_l.val = value;
}

__STATIC_INLINE void dmac_reqdstreg_l_pack(uint8_t dst_req_we, uint8_t dst_req)
{
    hwp_dmac->reqdstreg_l.val = (((uint32_t)dst_req_we << 8) | ((uint32_t)dst_req << 0));
}

__STATIC_INLINE void dmac_reqdstreg_l_unpack(uint8_t* dst_req)
{
    T_DMAC_REQDSTREG_L localVal = hwp_dmac->reqdstreg_l;

    *dst_req = localVal.bit_field.dst_req;
}

__STATIC_INLINE uint32_t dmac_reqdstreg_h_get(void)
{
    return hwp_dmac->reqdstreg_h.val;
}

__STATIC_INLINE uint32_t dmac_sglreqsrcreg_l_get(void)
{
    return hwp_dmac->sglreqsrcreg_l.val;
}

__STATIC_INLINE void dmac_sglreqsrcreg_l_set(uint32_t value)
{
    hwp_dmac->sglreqsrcreg_l.val = value;
}

__STATIC_INLINE void dmac_sglreqsrcreg_l_pack(uint8_t src_sglreq_we, uint8_t src_sglreq)
{
    hwp_dmac->sglreqsrcreg_l.val = (((uint32_t)src_sglreq_we << 8) | ((uint32_t)src_sglreq << 0));
}

__STATIC_INLINE void dmac_sglreqsrcreg_l_unpack(uint8_t* src_sglreq)
{
    T_DMAC_SGLREQSRCREG_L localVal = hwp_dmac->sglreqsrcreg_l;

    *src_sglreq = localVal.bit_field.src_sglreq;
}

__STATIC_INLINE uint32_t dmac_sglreqsrcreg_h_get(void)
{
    return hwp_dmac->sglreqsrcreg_h.val;
}

__STATIC_INLINE uint32_t dmac_sglreqdstreg_l_get(void)
{
    return hwp_dmac->sglreqdstreg_l.val;
}

__STATIC_INLINE void dmac_sglreqdstreg_l_set(uint32_t value)
{
    hwp_dmac->sglreqdstreg_l.val = value;
}

__STATIC_INLINE void dmac_sglreqdstreg_l_pack(uint8_t dst_sglreq_we, uint8_t dst_sglreq)
{
    hwp_dmac->sglreqdstreg_l.val = (((uint32_t)dst_sglreq_we << 8) | ((uint32_t)dst_sglreq << 0));
}

__STATIC_INLINE void dmac_sglreqdstreg_l_unpack(uint8_t* dst_sglreq)
{
    T_DMAC_SGLREQDSTREG_L localVal = hwp_dmac->sglreqdstreg_l;

    *dst_sglreq = localVal.bit_field.dst_sglreq;
}

__STATIC_INLINE uint32_t dmac_sglreqdstreg_h_get(void)
{
    return hwp_dmac->sglreqdstreg_h.val;
}

__STATIC_INLINE uint32_t dmac_lstsrcreg_l_get(void)
{
    return hwp_dmac->lstsrcreg_l.val;
}

__STATIC_INLINE void dmac_lstsrcreg_l_set(uint32_t value)
{
    hwp_dmac->lstsrcreg_l.val = value;
}

__STATIC_INLINE void dmac_lstsrcreg_l_pack(uint8_t lstsrc_we, uint8_t lstsrc)
{
    hwp_dmac->lstsrcreg_l.val = (((uint32_t)lstsrc_we << 8) | ((uint32_t)lstsrc << 0));
}

__STATIC_INLINE void dmac_lstsrcreg_l_unpack(uint8_t* lstsrc)
{
    T_DMAC_LSTSRCREG_L localVal = hwp_dmac->lstsrcreg_l;

    *lstsrc = localVal.bit_field.lstsrc;
}

__STATIC_INLINE uint32_t dmac_lstsrcreg_h_get(void)
{
    return hwp_dmac->lstsrcreg_h.val;
}

__STATIC_INLINE uint32_t dmac_lstdstreg_l_get(void)
{
    return hwp_dmac->lstdstreg_l.val;
}

__STATIC_INLINE void dmac_lstdstreg_l_set(uint32_t value)
{
    hwp_dmac->lstdstreg_l.val = value;
}

__STATIC_INLINE void dmac_lstdstreg_l_pack(uint8_t lstdst_we, uint8_t lstdst)
{
    hwp_dmac->lstdstreg_l.val = (((uint32_t)lstdst_we << 8) | ((uint32_t)lstdst << 0));
}

__STATIC_INLINE void dmac_lstdstreg_l_unpack(uint8_t* lstdst)
{
    T_DMAC_LSTDSTREG_L localVal = hwp_dmac->lstdstreg_l;

    *lstdst = localVal.bit_field.lstdst;
}

__STATIC_INLINE uint32_t dmac_lstdstreg_h_get(void)
{
    return hwp_dmac->lstdstreg_h.val;
}

__STATIC_INLINE uint32_t dmac_dmacfgreg_l_get(void)
{
    return hwp_dmac->dmacfgreg_l.val;
}

__STATIC_INLINE void dmac_dmacfgreg_l_set(uint32_t value)
{
    hwp_dmac->dmacfgreg_l.val = value;
}

__STATIC_INLINE void dmac_dmacfgreg_l_pack(uint8_t dma_en)
{
    hwp_dmac->dmacfgreg_l.val = (((uint32_t)dma_en << 0));
}

__STATIC_INLINE void dmac_dmacfgreg_l_unpack(uint8_t* dma_en)
{
    T_DMAC_DMACFGREG_L localVal = hwp_dmac->dmacfgreg_l;

    *dma_en = localVal.bit_field.dma_en;
}

__STATIC_INLINE uint32_t dmac_dmacfgreg_h_get(void)
{
    return hwp_dmac->dmacfgreg_h.val;
}

__STATIC_INLINE uint32_t dmac_chenreg_l_get(void)
{
    return hwp_dmac->chenreg_l.val;
}

__STATIC_INLINE void dmac_chenreg_l_set(uint32_t value)
{
    hwp_dmac->chenreg_l.val = value;
}

__STATIC_INLINE void dmac_chenreg_l_pack(uint8_t ch_en_we, uint8_t ch_en)
{
    hwp_dmac->chenreg_l.val = (((uint32_t)ch_en_we << 8) | ((uint32_t)ch_en << 0));
}

__STATIC_INLINE void dmac_chenreg_l_unpack(uint8_t* ch_en)
{
    T_DMAC_CHENREG_L localVal = hwp_dmac->chenreg_l;

    *ch_en = localVal.bit_field.ch_en;
}

__STATIC_INLINE uint32_t dmac_chenreg_h_get(void)
{
    return hwp_dmac->chenreg_h.val;
}

__STATIC_INLINE uint32_t dmac_dmaidreg_l_get(void)
{
    return hwp_dmac->dmaidreg_l.val;
}

__STATIC_INLINE void dmac_dmaidreg_l_unpack(uint32_t* dmah_id_num)
{
    T_DMAC_DMAIDREG_L localVal = hwp_dmac->dmaidreg_l;

    *dmah_id_num = localVal.bit_field.dmah_id_num;
}

__STATIC_INLINE uint32_t dmac_dmaidreg_h_get(void)
{
    return hwp_dmac->dmaidreg_h.val;
}

__STATIC_INLINE uint32_t dmac_dmatestreg_l_get(void)
{
    return hwp_dmac->dmatestreg_l.val;
}

__STATIC_INLINE void dmac_dmatestreg_l_set(uint32_t value)
{
    hwp_dmac->dmatestreg_l.val = value;
}

__STATIC_INLINE void dmac_dmatestreg_l_pack(uint8_t test_slv_if)
{
    hwp_dmac->dmatestreg_l.val = (((uint32_t)test_slv_if << 0));
}

__STATIC_INLINE void dmac_dmatestreg_l_unpack(uint8_t* test_slv_if)
{
    T_DMAC_DMATESTREG_L localVal = hwp_dmac->dmatestreg_l;

    *test_slv_if = localVal.bit_field.test_slv_if;
}

__STATIC_INLINE uint32_t dmac_dmatestreg_h_get(void)
{
    return hwp_dmac->dmatestreg_h.val;
}

__STATIC_INLINE uint32_t dmac_dma_comp_params_6_l_get(void)
{
    return hwp_dmac->dma_comp_params_6_l.val;
}

__STATIC_INLINE uint32_t dmac_dma_comp_params_6_h_get(void)
{
    return hwp_dmac->dma_comp_params_6_h.val;
}

__STATIC_INLINE uint32_t dmac_dma_comp_params_5_l_get(void)
{
    return hwp_dmac->dma_comp_params_5_l.val;
}

__STATIC_INLINE uint32_t dmac_dma_comp_params_5_h_get(void)
{
    return hwp_dmac->dma_comp_params_5_h.val;
}

__STATIC_INLINE void dmac_dma_comp_params_5_h_unpack(uint8_t* ch5_fifo_depth, uint8_t* ch5_sms, uint8_t* ch5_lms, uint8_t* ch5_dms, uint8_t* ch5_max_mult_size, uint8_t* ch5_fc, uint8_t* ch5_hc_llp, uint8_t* ch5_ctl_wb_en, uint8_t* ch5_multi_blk_en, uint8_t* ch5_lock_en, uint8_t* ch5_src_gat_en, uint8_t* ch5_dst_sca_en, uint8_t* ch5_stat_src, uint8_t* ch5_stat_dst, uint8_t* ch5_stw, uint8_t* ch5_dtw)
{
    T_DMAC_DMA_COMP_PARAMS_5_H localVal = hwp_dmac->dma_comp_params_5_h;

    *ch5_fifo_depth = localVal.bit_field.ch5_fifo_depth;
    *ch5_sms = localVal.bit_field.ch5_sms;
    *ch5_lms = localVal.bit_field.ch5_lms;
    *ch5_dms = localVal.bit_field.ch5_dms;
    *ch5_max_mult_size = localVal.bit_field.ch5_max_mult_size;
    *ch5_fc = localVal.bit_field.ch5_fc;
    *ch5_hc_llp = localVal.bit_field.ch5_hc_llp;
    *ch5_ctl_wb_en = localVal.bit_field.ch5_ctl_wb_en;
    *ch5_multi_blk_en = localVal.bit_field.ch5_multi_blk_en;
    *ch5_lock_en = localVal.bit_field.ch5_lock_en;
    *ch5_src_gat_en = localVal.bit_field.ch5_src_gat_en;
    *ch5_dst_sca_en = localVal.bit_field.ch5_dst_sca_en;
    *ch5_stat_src = localVal.bit_field.ch5_stat_src;
    *ch5_stat_dst = localVal.bit_field.ch5_stat_dst;
    *ch5_stw = localVal.bit_field.ch5_stw;
    *ch5_dtw = localVal.bit_field.ch5_dtw;
}

__STATIC_INLINE uint32_t dmac_dma_comp_params_4_l_get(void)
{
    return hwp_dmac->dma_comp_params_4_l.val;
}

__STATIC_INLINE void dmac_dma_comp_params_4_l_unpack(uint8_t* ch4_fifo_depth, uint8_t* ch4_sms, uint8_t* ch4_lms, uint8_t* ch4_dms, uint8_t* ch4_max_mult_size, uint8_t* ch4_fc, uint8_t* ch4_hc_llp, uint8_t* ch4_ctl_wb_en, uint8_t* ch4_multi_blk_en, uint8_t* ch4_lock_en, uint8_t* ch4_src_gat_en, uint8_t* ch4_dst_sca_en, uint8_t* ch4_stat_src, uint8_t* ch4_stat_dst, uint8_t* ch4_stw, uint8_t* ch4_dtw)
{
    T_DMAC_DMA_COMP_PARAMS_4_L localVal = hwp_dmac->dma_comp_params_4_l;

    *ch4_fifo_depth = localVal.bit_field.ch4_fifo_depth;
    *ch4_sms = localVal.bit_field.ch4_sms;
    *ch4_lms = localVal.bit_field.ch4_lms;
    *ch4_dms = localVal.bit_field.ch4_dms;
    *ch4_max_mult_size = localVal.bit_field.ch4_max_mult_size;
    *ch4_fc = localVal.bit_field.ch4_fc;
    *ch4_hc_llp = localVal.bit_field.ch4_hc_llp;
    *ch4_ctl_wb_en = localVal.bit_field.ch4_ctl_wb_en;
    *ch4_multi_blk_en = localVal.bit_field.ch4_multi_blk_en;
    *ch4_lock_en = localVal.bit_field.ch4_lock_en;
    *ch4_src_gat_en = localVal.bit_field.ch4_src_gat_en;
    *ch4_dst_sca_en = localVal.bit_field.ch4_dst_sca_en;
    *ch4_stat_src = localVal.bit_field.ch4_stat_src;
    *ch4_stat_dst = localVal.bit_field.ch4_stat_dst;
    *ch4_stw = localVal.bit_field.ch4_stw;
    *ch4_dtw = localVal.bit_field.ch4_dtw;
}

__STATIC_INLINE uint32_t dmac_dma_comp_params_4_h_get(void)
{
    return hwp_dmac->dma_comp_params_4_h.val;
}

__STATIC_INLINE void dmac_dma_comp_params_4_h_unpack(uint8_t* ch3_fifo_depth, uint8_t* ch3_sms, uint8_t* ch3_lms, uint8_t* ch3_dms, uint8_t* ch3_max_mult_size, uint8_t* ch3_fc, uint8_t* ch3_hc_llp, uint8_t* ch3_ctl_wb_en, uint8_t* ch3_multi_blk_en, uint8_t* ch3_lock_en, uint8_t* ch3_src_gat_en, uint8_t* ch3_dst_sca_en, uint8_t* ch3_stat_src, uint8_t* ch3_stat_dst, uint8_t* ch3_stw, uint8_t* ch3_dtw)
{
    T_DMAC_DMA_COMP_PARAMS_4_H localVal = hwp_dmac->dma_comp_params_4_h;

    *ch3_fifo_depth = localVal.bit_field.ch3_fifo_depth;
    *ch3_sms = localVal.bit_field.ch3_sms;
    *ch3_lms = localVal.bit_field.ch3_lms;
    *ch3_dms = localVal.bit_field.ch3_dms;
    *ch3_max_mult_size = localVal.bit_field.ch3_max_mult_size;
    *ch3_fc = localVal.bit_field.ch3_fc;
    *ch3_hc_llp = localVal.bit_field.ch3_hc_llp;
    *ch3_ctl_wb_en = localVal.bit_field.ch3_ctl_wb_en;
    *ch3_multi_blk_en = localVal.bit_field.ch3_multi_blk_en;
    *ch3_lock_en = localVal.bit_field.ch3_lock_en;
    *ch3_src_gat_en = localVal.bit_field.ch3_src_gat_en;
    *ch3_dst_sca_en = localVal.bit_field.ch3_dst_sca_en;
    *ch3_stat_src = localVal.bit_field.ch3_stat_src;
    *ch3_stat_dst = localVal.bit_field.ch3_stat_dst;
    *ch3_stw = localVal.bit_field.ch3_stw;
    *ch3_dtw = localVal.bit_field.ch3_dtw;
}

__STATIC_INLINE uint32_t dmac_dma_comp_params_3_l_get(void)
{
    return hwp_dmac->dma_comp_params_3_l.val;
}

__STATIC_INLINE void dmac_dma_comp_params_3_l_unpack(uint8_t* ch2_fifo_depth, uint8_t* ch2_sms, uint8_t* ch2_lms, uint8_t* ch2_dms, uint8_t* ch2_max_mult_size, uint8_t* ch2_fc, uint8_t* ch2_hc_llp, uint8_t* ch2_ctl_wb_en, uint8_t* ch2_multi_blk_en, uint8_t* ch2_lock_en, uint8_t* ch2_src_gat_en, uint8_t* ch2_dst_sca_en, uint8_t* ch2_stat_src, uint8_t* ch2_stat_dst, uint8_t* ch2_stw, uint8_t* ch2_dtw)
{
    T_DMAC_DMA_COMP_PARAMS_3_L localVal = hwp_dmac->dma_comp_params_3_l;

    *ch2_fifo_depth = localVal.bit_field.ch2_fifo_depth;
    *ch2_sms = localVal.bit_field.ch2_sms;
    *ch2_lms = localVal.bit_field.ch2_lms;
    *ch2_dms = localVal.bit_field.ch2_dms;
    *ch2_max_mult_size = localVal.bit_field.ch2_max_mult_size;
    *ch2_fc = localVal.bit_field.ch2_fc;
    *ch2_hc_llp = localVal.bit_field.ch2_hc_llp;
    *ch2_ctl_wb_en = localVal.bit_field.ch2_ctl_wb_en;
    *ch2_multi_blk_en = localVal.bit_field.ch2_multi_blk_en;
    *ch2_lock_en = localVal.bit_field.ch2_lock_en;
    *ch2_src_gat_en = localVal.bit_field.ch2_src_gat_en;
    *ch2_dst_sca_en = localVal.bit_field.ch2_dst_sca_en;
    *ch2_stat_src = localVal.bit_field.ch2_stat_src;
    *ch2_stat_dst = localVal.bit_field.ch2_stat_dst;
    *ch2_stw = localVal.bit_field.ch2_stw;
    *ch2_dtw = localVal.bit_field.ch2_dtw;
}

__STATIC_INLINE uint32_t dmac_dma_comp_params_3_h_get(void)
{
    return hwp_dmac->dma_comp_params_3_h.val;
}

__STATIC_INLINE void dmac_dma_comp_params_3_h_unpack(uint8_t* ch1_fifo_depth, uint8_t* ch1_sms, uint8_t* ch1_lms, uint8_t* ch1_dms, uint8_t* ch1_max_mult_size, uint8_t* ch1_fc, uint8_t* ch1_hc_llp, uint8_t* ch1_ctl_wb_en, uint8_t* ch1_multi_blk_en, uint8_t* ch1_lock_en, uint8_t* ch1_src_gat_en, uint8_t* ch1_dst_sca_en, uint8_t* ch1_stat_src, uint8_t* ch1_stat_dst, uint8_t* ch1_stw, uint8_t* ch1_dtw)
{
    T_DMAC_DMA_COMP_PARAMS_3_H localVal = hwp_dmac->dma_comp_params_3_h;

    *ch1_fifo_depth = localVal.bit_field.ch1_fifo_depth;
    *ch1_sms = localVal.bit_field.ch1_sms;
    *ch1_lms = localVal.bit_field.ch1_lms;
    *ch1_dms = localVal.bit_field.ch1_dms;
    *ch1_max_mult_size = localVal.bit_field.ch1_max_mult_size;
    *ch1_fc = localVal.bit_field.ch1_fc;
    *ch1_hc_llp = localVal.bit_field.ch1_hc_llp;
    *ch1_ctl_wb_en = localVal.bit_field.ch1_ctl_wb_en;
    *ch1_multi_blk_en = localVal.bit_field.ch1_multi_blk_en;
    *ch1_lock_en = localVal.bit_field.ch1_lock_en;
    *ch1_src_gat_en = localVal.bit_field.ch1_src_gat_en;
    *ch1_dst_sca_en = localVal.bit_field.ch1_dst_sca_en;
    *ch1_stat_src = localVal.bit_field.ch1_stat_src;
    *ch1_stat_dst = localVal.bit_field.ch1_stat_dst;
    *ch1_stw = localVal.bit_field.ch1_stw;
    *ch1_dtw = localVal.bit_field.ch1_dtw;
}

__STATIC_INLINE uint32_t dmac_dma_comp_params_2_l_get(void)
{
    return hwp_dmac->dma_comp_params_2_l.val;
}

__STATIC_INLINE void dmac_dma_comp_params_2_l_unpack(uint8_t* ch0_fifo_depth, uint8_t* ch0_sms, uint8_t* ch0_lms, uint8_t* ch0_dms, uint8_t* ch0_max_mult_size, uint8_t* ch0_fc, uint8_t* ch0_hc_llp, uint8_t* ch0_ctl_wb_en, uint8_t* ch0_multi_blk_en, uint8_t* ch0_lock_en, uint8_t* ch0_src_gat_en, uint8_t* ch0_dst_sca_en, uint8_t* ch0_stat_src, uint8_t* ch0_stat_dst, uint8_t* ch0_stw, uint8_t* ch0_dtw)
{
    T_DMAC_DMA_COMP_PARAMS_2_L localVal = hwp_dmac->dma_comp_params_2_l;

    *ch0_fifo_depth = localVal.bit_field.ch0_fifo_depth;
    *ch0_sms = localVal.bit_field.ch0_sms;
    *ch0_lms = localVal.bit_field.ch0_lms;
    *ch0_dms = localVal.bit_field.ch0_dms;
    *ch0_max_mult_size = localVal.bit_field.ch0_max_mult_size;
    *ch0_fc = localVal.bit_field.ch0_fc;
    *ch0_hc_llp = localVal.bit_field.ch0_hc_llp;
    *ch0_ctl_wb_en = localVal.bit_field.ch0_ctl_wb_en;
    *ch0_multi_blk_en = localVal.bit_field.ch0_multi_blk_en;
    *ch0_lock_en = localVal.bit_field.ch0_lock_en;
    *ch0_src_gat_en = localVal.bit_field.ch0_src_gat_en;
    *ch0_dst_sca_en = localVal.bit_field.ch0_dst_sca_en;
    *ch0_stat_src = localVal.bit_field.ch0_stat_src;
    *ch0_stat_dst = localVal.bit_field.ch0_stat_dst;
    *ch0_stw = localVal.bit_field.ch0_stw;
    *ch0_dtw = localVal.bit_field.ch0_dtw;
}

__STATIC_INLINE uint32_t dmac_dma_comp_params_2_h_get(void)
{
    return hwp_dmac->dma_comp_params_2_h.val;
}

__STATIC_INLINE void dmac_dma_comp_params_2_h_unpack(uint8_t* ch7_multi_blk_type, uint8_t* ch6_multi_blk_type, uint8_t* ch5_multi_blk_type, uint8_t* ch4_multi_blk_type, uint8_t* ch3_multi_blk_type, uint8_t* ch2_multi_blk_type, uint8_t* ch1_multi_blk_type, uint8_t* ch0_multi_blk_type)
{
    T_DMAC_DMA_COMP_PARAMS_2_H localVal = hwp_dmac->dma_comp_params_2_h;

    *ch7_multi_blk_type = localVal.bit_field.ch7_multi_blk_type;
    *ch6_multi_blk_type = localVal.bit_field.ch6_multi_blk_type;
    *ch5_multi_blk_type = localVal.bit_field.ch5_multi_blk_type;
    *ch4_multi_blk_type = localVal.bit_field.ch4_multi_blk_type;
    *ch3_multi_blk_type = localVal.bit_field.ch3_multi_blk_type;
    *ch2_multi_blk_type = localVal.bit_field.ch2_multi_blk_type;
    *ch1_multi_blk_type = localVal.bit_field.ch1_multi_blk_type;
    *ch0_multi_blk_type = localVal.bit_field.ch0_multi_blk_type;
}

__STATIC_INLINE uint32_t dmac_dma_comp_params_1_l_get(void)
{
    return hwp_dmac->dma_comp_params_1_l.val;
}

__STATIC_INLINE void dmac_dma_comp_params_1_l_unpack(uint8_t* ch7_max_blk_size, uint8_t* ch6_max_blk_size, uint8_t* ch5_max_blk_size, uint8_t* ch4_max_blk_size, uint8_t* ch3_max_blk_size, uint8_t* ch2_max_blk_size, uint8_t* ch1_max_blk_size, uint8_t* ch0_max_blk_size)
{
    T_DMAC_DMA_COMP_PARAMS_1_L localVal = hwp_dmac->dma_comp_params_1_l;

    *ch7_max_blk_size = localVal.bit_field.ch7_max_blk_size;
    *ch6_max_blk_size = localVal.bit_field.ch6_max_blk_size;
    *ch5_max_blk_size = localVal.bit_field.ch5_max_blk_size;
    *ch4_max_blk_size = localVal.bit_field.ch4_max_blk_size;
    *ch3_max_blk_size = localVal.bit_field.ch3_max_blk_size;
    *ch2_max_blk_size = localVal.bit_field.ch2_max_blk_size;
    *ch1_max_blk_size = localVal.bit_field.ch1_max_blk_size;
    *ch0_max_blk_size = localVal.bit_field.ch0_max_blk_size;
}

__STATIC_INLINE uint32_t dmac_dma_comp_params_1_h_get(void)
{
    return hwp_dmac->dma_comp_params_1_h.val;
}

__STATIC_INLINE void dmac_dma_comp_params_1_h_unpack(uint8_t* static_endian_select, uint8_t* add_encoded_params, uint8_t* num_hs_int, uint8_t* m1_hdata_width, uint8_t* m2_hdata_width, uint8_t* m3_hdata_width, uint8_t* m4_hdata_width, uint8_t* s_hdata_width, uint8_t* number_master_int, uint8_t* num_channels, uint8_t* mabrst, uint8_t* intr_io, uint8_t* big_endian)
{
    T_DMAC_DMA_COMP_PARAMS_1_H localVal = hwp_dmac->dma_comp_params_1_h;

    *static_endian_select = localVal.bit_field.static_endian_select;
    *add_encoded_params = localVal.bit_field.add_encoded_params;
    *num_hs_int = localVal.bit_field.num_hs_int;
    *m1_hdata_width = localVal.bit_field.m1_hdata_width;
    *m2_hdata_width = localVal.bit_field.m2_hdata_width;
    *m3_hdata_width = localVal.bit_field.m3_hdata_width;
    *m4_hdata_width = localVal.bit_field.m4_hdata_width;
    *s_hdata_width = localVal.bit_field.s_hdata_width;
    *number_master_int = localVal.bit_field.number_master_int;
    *num_channels = localVal.bit_field.num_channels;
    *mabrst = localVal.bit_field.mabrst;
    *intr_io = localVal.bit_field.intr_io;
    *big_endian = localVal.bit_field.big_endian;
}

__STATIC_INLINE uint32_t dmac_dmacompidreg_l_get(void)
{
    return hwp_dmac->dmacompidreg_l.val;
}

__STATIC_INLINE void dmac_dmacompidreg_l_unpack(uint32_t* dma_comp_type)
{
    T_DMAC_DMACOMPIDREG_L localVal = hwp_dmac->dmacompidreg_l;

    *dma_comp_type = localVal.bit_field.dma_comp_type;
}

__STATIC_INLINE uint32_t dmac_dmacompidreg_h_get(void)
{
    return hwp_dmac->dmacompidreg_h.val;
}

__STATIC_INLINE void dmac_dmacompidreg_h_unpack(uint32_t* dma_comp_version)
{
    T_DMAC_DMACOMPIDREG_H localVal = hwp_dmac->dmacompidreg_h;

    *dma_comp_version = localVal.bit_field.dma_comp_version;
}
#endif


