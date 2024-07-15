#include <avr/io.h>
#include <avr/interrupt.h>

#define PWM_PIN     PB0   // PWM output pin
#define ADC_PIN     PB2   // ADC input pin

void setup_PWM() {
    // Configure PB0 (PWM pin) as output
    DDRB |= (1 << PWM_PIN);

    // Set up Timer/Counter0 for PWM generation
    // WGM02:0 = 011 -> Fast PWM, 8-bit
    TCCR0A = (1 << WGM00) | (1 << WGM01) | (1 << COM0A1);  
    TCCR0B = (1 << CS00);  // No prescaler

    // Initial PWM duty cycle
    OCR0A = 0;  // Start with 0% duty cycle
}

void setup_ADC() {
    // Configure PB2 (ADC pin) as input
    DDRB &= ~(1 << ADC_PIN);

    // ADC configuration
    ADMUX = (1 << ADLAR) | (1 << MUX0);  // Left adjust result, select ADC2 (PB2)
    ADCSRA = (1 << ADEN) | (1 << ADPS1) | (1 << ADPS0);  // Enable ADC, prescaler = 8
}

uint8_t read_ADC() {
    // Start ADC conversion
    ADCSRA |= (1 << ADSC);

    // Wait for conversion to complete
    while (ADCSRA & (1 << ADSC));

    // Read ADC result
    return ADCH;
}

int main(void) {
    // Initialize PWM and ADC
    setup_PWM();
    setup_ADC();

    // Enable global interrupts
    sei();

    while (1) {
        // Read ADC value (0-255) and set PWM duty cycle
        uint8_t adc_value = read_ADC();
        OCR0A = adc_value;
        
        // Optional: You can add a delay here to control update rate
        // _delay_ms(10); // Adjust delay as needed
    }
}
