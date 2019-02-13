#ifndef DW_APB_UART_TYPEDEF_H_
#define DW_APB_UART_TYPEDEF_H_
typedef enum
{
	Modem_Status_Interrupt_Disabled = 0,
	Modem_Status_Interrupt_Enabled
}IER_EDSSI_FIELD;
typedef enum
{
	Receiver_Line_Status_Interrupt_Disabled = 0,
	Receiver_Line_Status_Interrupt_Enabled,
}IER_ELSI_FIELD;
typedef enum
{
	Transmit_Holding_Register_Empty_Interrupt_Disabled = 0,
	Transmit_Holding_Register_Empty_Interrupt_Enabled
}IER_ETBEI_FIELD;
typedef enum
{
	Received_Data_Available_Interrupt_Disabled =0,
	Received_Data_Available_Interrupt_Enabled
}IER_ERBFI_FIELD;
typedef enum
{
	Modem_Status_Interrupt = 0,
	No_Interrupt_Pending = 1,
	THR_Empty_Interrupt = 2,
	Received_Data_Available_Interrupt = 4,
	Receiver_Line_Status_Interrupt = 6,
	Busy_Detect_Interrupt = 7,
	Character_Timeout_Interrupt = 12
}IIR_INTERRUPT_ID_FIELD;
typedef enum
{
	RCVR_Trigger_One_Character = 0,
	RCVR_Trigger_FIFO_One_Quarter_Full,
	RCVR_Trigger_FIFO_Half_Full,
	RCVR_Trigger_FIFO_Two_Less_Than_Full
}FCR_RCVR_TRIGGER_FIELD;
typedef enum
{
	TX_Empty_Trigger_FIFO_Empty = 0,
	TX_Empty_Trigger_Two_Characters_In_FIFO,
	TX_Empty_Trigger_One_Quarter_Full,
	Tx_Empty_Trigger_Half_Full
}FCR_TX_EMPTY_TRIGGER_FIELD;
typedef enum
{
	FIFO_Disabled = 0,
	FIFO_Enabled
}FCR_FIFO_ENABLE_FIELD;
typedef enum
{
	Divisor_Latch_Register_Disabled = 0,
	Divisor_Latch_Register_Enabled
}LCR_DLAB_FIELD;
typedef enum
{
	Parity_Disabled = 0,
	Parity_Enabled
}LCR_PEN_FIELD;
typedef enum
{
	One_Stop_bit = 0,
	Two_or_One_Half_Stop_bits
}LCR_STOP_FIELD;
typedef enum
{
	Data_Length_5_bits = 0,
	Data_Length_6_bits,
	Data_Length_7_bits,
	Data_Length_8_bits
}LCR_DLS_FIELD;
typedef enum
{
	RTS_Signal_High = 0,
	RTS_Signal_Low
}MCR_RTS_FIELD;
typedef enum
{
	No_Framing_Error = 0,
	Framing_Error
}LSR_FE_FIELD;
typedef enum
{
	No_Parity_Error = 0,
	Parity_Error
}LSR_PE_FIELD;
typedef enum
{
	No_Overrun_Error = 0,
	Overrun_Error
}LSR_OE_FIELD;
typedef enum
{
	No_Data_Ready = 0,
	Data_Ready
}LSR_DR_FIELD;
typedef enum
{
	Receive_FIFO_Not_Full = 0,
	Receive_FIFO_Full
}USR_RFF_FIELD;
typedef enum
{
	Receive_FIFO_Empty = 0,
	Receive_FIFO_Not_Empty
}USR_RFNE_FIELD;
typedef enum
{
	Transmit_FIFO_Not_Empty = 0,
	Transmit_FIFO_Empty
}USR_TFE_FIELD;
typedef enum
{
	Transmit_FIFO_Full = 0,
	Transmit_FIFO_Not_Full
}USR_TFNF_FIELD;

#endif
