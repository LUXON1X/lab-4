#define F_CPU 8000000UL

#include <avr/io.h>
#include <util/delay.h>
#include <stdint.h>

#define BUZZER_PORT   PORTD
#define BUZZER_DDR    DDRD
#define BUZZER_PIN    PD7

#define BTN_PORT      PORTA
#define BTN_PIN_REG   PINA
#define BTN_DDR       DDRA
#define BTN_BIT       PA0

#define US_IN_S 1000000UL

void delay_us_custom(uint16_t us) {
    uint32_t loops = (uint32_t)us * 2;
    while (loops > 65535) {
        _delay_loop_2(65535);
        loops -= 65535;
    }
    if (loops > 0) {
        _delay_loop_2((uint16_t)loops);
    }
}

void delay_ms_custom(uint16_t ms) {
    while (ms > 0) {
        _delay_loop_2(2000);
        ms--;
    }
}

uint16_t melody[][2] = {
    {US_IN_S / 587.32, 250},
    {US_IN_S / 587.32, 250},
    {US_IN_S / 493.88, 250},
    {US_IN_S / 523.25, 500},
    {US_IN_S / 440.00, 750},
    {US_IN_S / 440.00, 150},
    {US_IN_S / 587.32, 250},
    {US_IN_S / 587.32, 250},
    {US_IN_S / 523.25, 250},
    {US_IN_S / 493.88, 250},
    {US_IN_S / 587.32, 250},
    {US_IN_S / 587.32, 250},
    {US_IN_S / 493.88, 250},
    {US_IN_S / 523.25, 500},
    {US_IN_S / 440.00, 500},
    {US_IN_S / 493.88, 250},
    {US_IN_S / 392.00, 250},
	{US_IN_S / 440.00, 500},
    {0, 1000}
};

void Sound_Play(uint16_t freqPeriodUs, uint16_t timeMs) {
    if (freqPeriodUs == 0) {
        delay_ms_custom(timeMs);
        return;
    }
    
    int32_t noteTime = (int32_t)timeMs * 1000;
    uint16_t halfPeriod = freqPeriodUs / 2;
    
    while (noteTime > 0) {
        BUZZER_PORT ^= (1 << BUZZER_PIN);
        delay_us_custom(halfPeriod);
        noteTime -= halfPeriod;
    }
    
    BUZZER_PORT &= ~(1 << BUZZER_PIN);
}

void Wait_Button() {
    while (BTN_PIN_REG & (1 << BTN_BIT));
    delay_ms_custom(50);
    while (!(BTN_PIN_REG & (1 << BTN_BIT)));
    delay_ms_custom(50);
}

int main() {
    BUZZER_DDR |= (1 << BUZZER_PIN);
    BUZZER_PORT &= ~(1 << BUZZER_PIN);
    
    BTN_DDR &= ~(1 << BTN_BIT);
    BTN_PORT |= (1 << BTN_BIT);
    
    uint8_t notes_count = sizeof(melody) / sizeof(melody[0]);
    
    while (1) {
        Wait_Button();
        for (uint8_t i = 0; i < notes_count; i++) {
            Sound_Play(melody[i][0], melody[i][1]);
        }
    }
    
    return 0;
}
