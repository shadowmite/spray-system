#include "lpc17xx_timer.h"
#include "../app/cfg/cfg_handler.h"
extern uint32_t idx_7,idx_6;
extern uint16_t idx_7_freq,idx_6_freq;
static volatile uint8_t timer0_1ms_timeout_flag = 0;
extern volatile uint32_t g_current_tick ;
uint32_t idx_7_tick,idx_6_tick;
void TIMER0_IRQHandler( void )
{
	uint32_t ir_reg = LPC_TIM0->IR;
	LPC_TIM0->IR = ir_reg;
	if(g_current_tick % 1000 == 0)
	{
		idx_7_freq = idx_7;
		idx_6_freq = idx_6;
		idx_6 = 0;
		idx_7 = 0;
	}
	
	if(FOC_1_STATUS.enable_flag == STOP_SPARY&&
  	 FOC_2_STATUS.enable_flag == STOP_SPARY)
	{

  }
	else if(FOC_1_STATUS.flow_speed == 0
		   && FOC_2_STATUS.flow_speed == 0)
	{
	
	}	
	else
	{
		if(idx_7_freq > 5) // 有流量时在计数 
			idx_7_tick++;
		
		if(idx_6_freq > 5) // 有流量时在计数 
			idx_6_tick++;
	}
	timer0_1ms_timeout_flag = 1;
	g_current_tick ++ ;
}

void TIMER0_Init( void )
{
	LPC_SC->PCONP |= ( 1 << 1 );
	
	LPC_TIM0->TCR = 0x02;
	LPC_TIM0->IR   = 0xF;
	LPC_TIM0->CTCR = 0;
	LPC_TIM0->PR   = 24;
	LPC_TIM0->MR0	 = 1000; 	//1ms
	LPC_TIM0->MCR  = 0x3;
	NVIC_EnableIRQ( TIMER0_IRQn );
}

void TIMER0_Start( void )
{
	LPC_TIM0->TCR = 0x01;
}

void TIMER0_Stop( void )
{
	LPC_TIM0->TCR &= ~0x01;
}

void TIMER0_Reset( void )
{
	LPC_TIM0->TCR |= 0x02;
}

uint8_t TIMER0_1Ms_Timeout( void )
{
	if( timer0_1ms_timeout_flag ) {
		timer0_1ms_timeout_flag = 0;
		return 1;
	} else {
		return 0;
	}
}

void TIMER1_Init( void )
{
	LPC_SC->PCONP |= ( 1 << 2 );
	
	LPC_TIM1->TCR = 0x02;
	LPC_TIM1->IR   = 0xF;
	LPC_TIM1->CTCR = 0;
	LPC_TIM1->PR   = 24;
	LPC_TIM1->MR0	 = 80; 	// 0.08ms
	LPC_TIM1->MCR  = 0x3;
	NVIC_EnableIRQ( TIMER1_IRQn );
}

void TIMER1_Start( void )
{
	LPC_TIM1->TCR = 0x01;
}

void TIMER1_Stop( void )
{
	LPC_TIM1->TCR &= ~0x01;
}

#include "lpc17xx_uart.h"

void TIMER1_IRQHandler( void )
{
	uint32_t ir_reg = LPC_TIM1->IR;
	LPC_TIM1->IR = ir_reg;
	TIMER1_Stop();
//	RS485_RD();
}

void TIMER2_Init( void )
{
	LPC_SC->PCONP |= ( 1 << 22 );
	LPC_TIM2->TCR = 0x02;
	LPC_TIM2->IR   = 0xF;
	LPC_TIM2->CTCR = 0;
	LPC_TIM2->PR   = 24;
	LPC_TIM2->MR0	 = 300; 	// 0.1ms
	LPC_TIM2->MCR  = 0x3;
	NVIC_EnableIRQ( TIMER2_IRQn );
}


void TIMER2_Start( void )
{
	LPC_TIM2->TCR = 0x01;
}

void TIMER2_Stop( void )
{
	LPC_TIM2->TCR &= ~0x01;
}


void TIMER2_IRQHandler( void )
{
	uint32_t ir_reg = LPC_TIM2->IR;
	LPC_TIM2->IR = ir_reg;
	TIMER2_Stop();
	Radar_RS485_RD();
}
