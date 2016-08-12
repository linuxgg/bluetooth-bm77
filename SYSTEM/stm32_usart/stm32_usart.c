#include "stdio.h"
#include "sys.h"
  
//�������´���,֧��printf����,������Ҫѡ��use MicroLIB
#if 1
#pragma import(__use_no_semihosting)             
//��׼����Ҫ��֧�ֺ���                 
struct __FILE 
{ 
	int handle; 
}; 

FILE __stdout;       
//����_sys_exit()�Ա���ʹ�ð�����ģʽ    

void _ttywrch(int ch)
{
    while((USART1->SR&0X40)==0);//ѭ������,ֱ���������    
    USART1->DR = (u8) ch;  
}

int _sys_exit(int x) 
{ 
	return 0;
} 
//�ض���fputc���� 
int fputc(int ch, FILE *f)
{      
	while((USART1->SR&0X40)==0){};//ѭ������,ֱ���������    
  USART1->DR = (u8) ch;      
	return 0;
}
#endif 


/*ʹ��microLib�ķ���*/
 /* 
int fputc(int ch, FILE *f)
{
	USART_SendData(USART1, (uint8_t) ch);

	while (USART_GetFlagStatus(USART1, USART_FLAG_TC) == RESET) {}	
   
    return ch;
}
int GetKey (void)  { 

    while (!(USART1->SR & USART_FLAG_RXNE));

    return ((int)(USART1->DR & 0x1FF));
}
*/


u8 Uart1_Buff[MAXBUF] = {0};
volatile u16 Uart1_RcvCnt = 0;

u8 Uart3_Buff[MAXBUF] = {0};
volatile u16 Uart3_RcvCnt = 0;

void Uart1Init(u32 bound){
    //GPIO�˿�����
    GPIO_InitTypeDef GPIO_InitStructure;
		USART_InitTypeDef USART_InitStructure;
		NVIC_InitTypeDef NVIC_InitStructure;
	 
		RCC_APB2PeriphClockCmd(RCC_APB2Periph_USART1|RCC_APB2Periph_GPIOA|RCC_APB2Periph_AFIO, ENABLE);
    //USART1_TX   PA.9
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_9;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
    GPIO_Init(GPIOA, &GPIO_InitStructure);
    //USART1_RX	  PA.10
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_10;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;
    GPIO_Init(GPIOA, &GPIO_InitStructure);  

    //Usart1 NVIC ����
    NVIC_InitStructure.NVIC_IRQChannel = USART1_IRQn;
		NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority=3;
		NVIC_InitStructure.NVIC_IRQChannelSubPriority = 3;		//

		NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;			//IRQͨ��ʹ��
		NVIC_Init(&NVIC_InitStructure);	//����NVIC_InitStruct��ָ���Ĳ�����ʼ������NVIC�Ĵ���USART1
  
   //USART ��ʼ������
		USART_InitStructure.USART_BaudRate = bound;
		USART_InitStructure.USART_WordLength = USART_WordLength_8b;
		USART_InitStructure.USART_StopBits = USART_StopBits_1;
		USART_InitStructure.USART_Parity = USART_Parity_No;
		USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
		USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;

    USART_Init(USART1, &USART_InitStructure);
    USART_ITConfig(USART1, USART_IT_RXNE, ENABLE);//�����ж�
    USART_Cmd(USART1, ENABLE);                    //ʹ�ܴ��� 

}

void USART1_IRQHandler(void)                	//����1�жϷ������
{
	u8 tmp;
	if(USART_GetITStatus(USART1, USART_IT_RXNE) != RESET)  //�����ж�
	{
		tmp =USART_ReceiveData(USART1);//(USART1->DR);	//��ȡ���յ�������
		if(Uart1_RcvCnt > MAXBUF)Uart1_RcvCnt = 0;	
		Uart1_Buff[Uart1_RcvCnt++] = tmp;
		USART_ClearITPendingBit(USART1,USART_IT_RXNE);
	}
}

void Uart1SendHex(u8 ch)
{
	while(USART_GetFlagStatus(USART1,USART_FLAG_TC) == RESET);
	USART_SendData(USART1,ch);
}

void Uart1SendStr(u8* str)
{
	while(*str)	//�ж�һ�������Ƿ����
		Uart1SendHex(*(str++));
}


void Uart3Init(u32 bound)
{
    //GPIO�˿�����
    GPIO_InitTypeDef GPIO_InitStructure;
		USART_InitTypeDef USART_InitStructure;
		NVIC_InitTypeDef NVIC_InitStructure;
		RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB | RCC_APB2Periph_AFIO, ENABLE);  
		RCC_APB1PeriphClockCmd(RCC_APB1Periph_USART3 , ENABLE);
    //USART3_TX   PB.10
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_10;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
    GPIO_Init(GPIOB, &GPIO_InitStructure);
    //USART3_RX	  PB.11
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_11;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;
    GPIO_Init(GPIOB, &GPIO_InitStructure);  

   //Usart3 NVIC ����
    NVIC_InitStructure.NVIC_IRQChannel = USART3_IRQn;
		NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority=3;
		NVIC_InitStructure.NVIC_IRQChannelSubPriority = 3;		//
		NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;			//IRQͨ��ʹ��
		NVIC_Init(&NVIC_InitStructure);	//����NVIC_InitStruct��ָ���Ĳ�����ʼ������NVIC�Ĵ���USART3
  
   //USART ��ʼ������
		RCC_APB1PeriphClockCmd(RCC_APB1Periph_USART3, ENABLE);  //RCC_APB1_USART3ʹ��
		USART_InitStructure.USART_BaudRate = bound;  //������
		USART_InitStructure.USART_WordLength = USART_WordLength_8b;  //�����ֳ�8
		USART_InitStructure.USART_StopBits = USART_StopBits_1;  //ֹͣλΪ1
		USART_InitStructure.USART_Parity = USART_Parity_No;  //��żʧ��
		USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
		USART_InitStructure.USART_Mode = USART_Mode_Tx | USART_Mode_Rx;  //����ʹ��  ����ʹ��
		USART_Init(USART3, &USART_InitStructure);
    USART_ITConfig(USART3, USART_IT_RXNE, ENABLE);//�����ж�
    USART_Cmd(USART3, ENABLE);                    //ʹ�ܴ���
}

void USART3_IRQHandler(void)                	//����3�жϷ������
{
	u8 tmp;
	if(USART_GetITStatus(USART3, USART_IT_RXNE) != RESET)  //�����ж�
	{
		tmp = USART_ReceiveData(USART3);//(USART3->DR);	//��ȡ���յ�������
		if(Uart3_RcvCnt > MAXBUF)Uart3_RcvCnt = 0;	
		Uart3_Buff[Uart3_RcvCnt++] = tmp;
		USART_ClearITPendingBit(USART3,USART_IT_RXNE);
	}
}

void Uart3SendHex(u8 ch)
{
	while(USART_GetFlagStatus(USART3,USART_FLAG_TC) == RESET);
	USART_SendData(USART3,ch);
}

void Uart3SendStr(u8* str)
{
	while(*str)	//�ж�һ�������Ƿ����
		Uart3SendHex(*(str++));
}
