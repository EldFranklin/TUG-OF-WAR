#include <stdio.h>
#include <stdlib.h>
#include <string.h>

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
#define BOTAO_DIREITO_PIN "/sys/class/gpio/gpio30/value
//------------------------------------------------------
//------------------------------------------------------

// Função para exportar um pino GPIO
void export_gpio(int gpio_number) {
    FILE *file = fopen("/sys/class/gpio/export", "w");
    if (file == NULL) {
        perror("Erro ao exportar o pino GPIO");
        exit(EXIT_FAILURE);
    }

    fprintf(file, "%d", gpio_number);
    fclose(file);
    printf("GPIO %d exportado...\r\n",gpio_number);

}

// Função para configurar um pino GPIO como entrada
void configure_gpio_input(int gpio_number) {
    char path[50];
    sprintf(path, "%s%d/direction", GPIO_PATH, gpio_number);
    FILE *file = fopen(path, "w");
    if (file == NULL) {
        perror("Erro ao configurar o pino GPIO como entrada");
        exit(EXIT_FAILURE);
    }

    fprintf(file, "in");
    fclose(file);
    printf("GPIO %d definido como entrada...\r\n",gpio_number);
}

//Função para configurar um pino GPIO como saída
void configure_gpio_output(int gpio_number) {

    char path[50];
    sprintf(path, "%s%d/direction", GPIO_PATH, gpio_number);
    FILE *file = fopen(path, "w");
    if (file == NULL) {
        perror("Erro ao configurar o pino GPIO como saída");
        exit(EXIT_FAILURE);
    }

    fprintf(file, "out");
    fclose(file);
    printf("GPIO %d definido como saida...\r\n",gpio_number);
}

//Função para setar GPIO, 0 ou 1 
void set_gpio_value(int gpio_number, int value) {
    char path[50];
    sprintf(path, "%s%d/value", GPIO_PATH, gpio_number);
    FILE *file = fopen(path, "w");
    if (file == NULL) {
        perror("Erro ao acender ou apagar o LED");
        exit(EXIT_FAILURE);
    }

    fprintf(file, "%d", value);
    fclose(file);
}

// Função para exportar um pino GPIO
void export_gpio(int gpio_number) {
    FILE *file = fopen("/sys/class/gpio/export", "w");
    if (file == NULL) {
        perror("Erro ao exportar o pino GPIO");
        exit(EXIT_FAILURE);
    }

    fprintf(file, "%d", gpio_number);
    fclose(file);
    printf("GPIO %d exportado...\r\n",gpio_number);

}

// Função para acessar leds da placa
void usr_leds(int ledn, int value) {
    
    FILE *file;
    switch (ledn) {
        case 0:
            file = fopen(Led_Usr0, "w");
            break;
        case 1:
            file = fopen(Led_Usr1, "w");
            break;
        case 2:
            file = fopen(Led_Usr2, "w");
            break;
        default:
            file = fopen(Led_Usr3, "w");
            break;
    }    


    if (file == NULL) {
        perror("Erro ao exportar os leds");
        exit(EXIT_FAILURE);
    }

    fprintf(file, "%d", value);
    fclose(file);
    //printf("Valor %d definido no led %d...\r\n",value,ledn);

}
