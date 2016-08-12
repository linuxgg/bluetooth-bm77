/*
 * pan1026.h
 *
 *  Created on: 18.5.2015
 *      Author: horinek
 */

#ifndef PAN1026_H_
#define PAN1026_H_

#include "common.h"
#include "usart.h"

#define PARSER_BUFFER_SIZE 64

class pan1026
{
public:
	bool connected;
	uint8_t next_cmd;
	uint8_t last_cmd;
	uint8_t state;
	uint8_t parser_status;
	uint16_t parser_packet_length;
	uint8_t parser_buffer_index;
	uint8_t parser_buffer[PARSER_BUFFER_SIZE];
	uint8_t cmd_iter;
	uint16_t btle_service_handles[3];
	uint16_t btle_characteristic_handles[6];
	uint16_t btle_element_handles[7];
	uint16_t btle_connection_handles[5];
	bool repat_last_cmd;
	uint8_t pan_mac_address[6];
	uint8_t random_address[6];
	uint8_t client_mac_address[6];
	char label[32];
	char client_name[32];
  uint32_t timer;
	Usart * usart;
	
	void Init(Usart * uart);
	void Restart();
	void Step();
	void SetNextStep(uint8_t cmd);
	void Parse(uint8_t c);
	void ParseHCI();
	void ParseMNG();
	void ParseSPP();
	void ParseMNG_LE();
	void ParseGAT_cli();
	void ParseGAT_ser();
	void WaitForAnswer();
	void StreamWrite(uint8_t data);
	void StreamWriteBinary(uint8_t data);
	void RawSendStatic(const uint8_t * data, uint8_t len);
	void RawSendStaticBinary(const uint8_t * data, uint8_t len);
	void SendString(char * str);
	void SendBinary(char * str,uint16_t len);
	
	void DoSomething(uint8_t *data, uint16_t len);
};

#endif /* PAN1026_H_ */
