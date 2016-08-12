#ifndef USART_H_
#define USART_H_

#include "common.h"

//#define XLIB_USE_CORE_USART_INIT_VAR uint8_t

class Usart //! Object for communication using usart modules
{
private:
public: 

//	volatile uint8_t * rx_buffer;
//	volatile uint8_t * tx_buffer;
//	volatile uint8_t rx_index, read_index, rx_len, rx_buffer_size;
//	volatile uint8_t tx_index, write_index, tx_len, tx_buffer_size;
	volatile uint8_t rx_len;

public:
	volatile uint8_t rx_ovf;
	void Init(uint8_t  usart,uint32_t baud);
	void InitBuffers(uint8_t rx_size, uint8_t tx_size);
	bool isRxBufferEmpty();
	uint8_t Read();
	void Write(uint8_t c);
	void ClearRxBuffer();

	void WriteBinary(uint8_t c);

};

//typedef void (*usart_event_cb_t)(Usart *);


#endif /* USART_H_ */
