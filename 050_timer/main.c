/* Timer/Counter0(TC0,8bit)をオーバーフローするまで繰り返しカウント */

#include <avr/io.h>
#include <avr/interrupt.h>

uint8_t b = 0;
ISR(TIMER0_OVF_vect)
{
	PORTB = b;	/* write value of led on PORTB */
	b = !b;
}

int main(void)
{
	PORTB = 0x00;	/* 初期状態：LEDは全消灯 */
	DDRB  = 0xFF;	/* PORTBの全ピンを出力に */
	TIMSK0 = _BV(TOIE0);	/* タイマ1オーバーフロー割り込みの許可 */
	TCCR0A= 0;	/* TCCR1A タイマモード */
	TCCR0B= 1;	/* プリスケールは ck/1 */
	TCNT0 = 0x00;	/* タイマ1の初期値設定 */
	sei();	/* 割り込みの許可
		 * 個別の割り込み許可と全体の割り込み許可双方が必要 */
	while (1);

	return 0;
}
