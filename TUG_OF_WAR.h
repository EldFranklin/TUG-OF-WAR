#ifndef TUF_OF_WAR_H
#define TUF_OF_WAR_H


//LEDS EMBARCADOS
void usr_leds(int ledn, int value);

// Funções para manipulação de GPIOs
void export_gpio(int gpio_number);
void usr_leds(int ledn, int value);
void configure_gpio_output(int gpio_number);
void configure_gpio_input(int gpio_number);
void set_gpio_value(int gpio_number, int value);


#endif //TUF_OF_WAR_H
