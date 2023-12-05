#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "TUG_OF_WAR"

#define GPIO_PATH "/sys/class/gpio/gpio"

//Macros Leds Embarcados
#define Led_Usr0 "/sys/class/leds/beaglebone:green:usr0/brightness"
#define Led_Usr1 "/sys/class/leds/beaglebone:green:usr1/brightness"
#define Led_Usr2 "/sys/class/leds/beaglebone:green:usr2/brightness"
#define Led_Usr3 "/sys/class/leds/beaglebone:green:usr0/brightness"
//---------------------------------------------------------------
//---------------------------------------------------------------

//Macro Botoes
#define BOTAO_ESQUERDO_PIN "/sys/class/gpio/gpio60/value"
#define BOTAO_DIREITO_PIN "/sys/class/gpio/gpio30/value"
//------------------------------------------------------
//------------------------------------------------------



int gpio_fd, ret;
struct pollfd fdset[1];
char buf[10];


int main() {
    int gpio_number = 60;  // Substitua pelo número do pino GPIO que você está usando

    usr_leds(0,0);
    usr_leds(1,0);
    usr_leds(2,0);
    usr_leds(3,0);

    usr_leds(0,1);
    usr_leds(1,0);
    usr_leds(2,1);
    usr_leds(3,0);

    // Exporta o pino GPIO
    export_gpio(gpio_number);


    // Configura o pino GPIO como saída
    configure_gpio_output(gpio_number);


    // Acende o LED (define o valor como 1)
    set_gpio_value(gpio_number, 1);


    // Aguarda por um tempo (você pode substituir isso por sua lógica)
    //sleep(2);

    // Apaga o LED (define o valor como 0)
    //set_gpio_value(gpio_number, 0);
    printf("teste adicional\r\n");
    return 0;
}
