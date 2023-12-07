#ifndef TUG_OF_WAR_H
#define TUG_OF_WAR_H


//LEDS EMBARCADOS
void usr_leds(int ledn, int value);

// Funções para manipulação de GPIOs
void export_gpio(int gpio_number);
void usr_leds(int ledn, int value);
void configure_gpio_output(int gpio_number);
void configure_gpio_input(int gpio_number);
void set_gpio_value(int gpio_number, int value);
char read_gpio(int gpio_number);


void export_gpio_PWM(int gpio_number);
void configurar_pino_saida_PWM(int gpio_number);
void gerar_pwm(int gpio_number, int frequencia, float dutycicle);
void read_rtc0_time();
void all_leds(int x);
void PWM0(int pwm_number,int period,int duty_cycle);
void on_pwm0(int status);
void debounce_delay();

#endif //TUG_OF_WAR_H
