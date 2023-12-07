#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <fcntl.h>
#include <unistd.h>
#include <poll.h>
#include <time.h>

#define GPIO_PATH "/sys/class/gpio/gpio"
#define GPIO_1 60
#define GPIO_2 61
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
#define debounce 100

//Macro path dos gpio


//--------------------
//--------------------

char lokc1 = 0; 

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

//Função para exportar PWM
void export_gpio_PWM(int gpio_number) {
    FILE *file = fopen("/sys/class/gpio/export", "w");
    if (file == NULL) {
        perror("Erro ao exportar o pino GPIO");
        exit(EXIT_FAILURE);
    }

    fprintf(file, "%d", gpio_number);
    fclose(file);
    printf("GPIO %d exportado...\n", gpio_number);
}
//Função para setar PWM como saída
void configurar_pino_saida_PWM(int gpio_number) {
    char path[50];
    sprintf(path, "%s%d/direction", GPIO_PATH, gpio_number);
    FILE *file = fopen(path, "w");
    if (file == NULL) {
        perror("Erro ao configurar o pino GPIO como saída");
        exit(EXIT_FAILURE);
    }

    fprintf(file, "out");
    fclose(file);
    printf("GPIO %d definido como saída...\n", gpio_number);
}

//Função gerar PWM
void gerar_pwm(int gpio_number, int frequencia, float dutycicle) {
    char value_file_path[50];
    sprintf(value_file_path, "%s%d/value", GPIO_PATH, gpio_number);

    export_gpio_PWM(gpio_number);
    configurar_pino_saida_PWM(gpio_number);

    while (1) {
        // Lógica do PWM
        FILE *value_file = fopen(value_file_path, "w");
        if (value_file == NULL) {
            perror("Erro ao abrir o arquivo de valor para PWM");
            exit(EXIT_FAILURE);
        }

        fprintf(value_file, "1");
        fclose(value_file);
        usleep(500000 / frequencia);  // Metade do período (50% de duty cycle)

        value_file = fopen(value_file_path, "w");
        if (value_file == NULL) {
            perror("Erro ao abrir o arquivo de valor para PWM");
            exit(EXIT_FAILURE);
        }

        fprintf(value_file, "0");
        fclose(value_file);
        usleep(500000 / frequencia);  // Metade do período (50% de duty cycle)
    }
}

char read_gpio(int gpio_number) {
    char path[50];
    sprintf(path, "/sys/class/gpio/gpio%d/value", gpio_number);

    FILE *file = fopen(path, "r");
    if (file == NULL) {
        perror("Erro ao abrir o arquivo GPIO para leitura");
        exit(EXIT_FAILURE);
    }

    char value;
    fscanf(file, " %c", &value);

    fclose(file);

    return value;
}

char buffer[50];

void read_rtc0_time() {

    time_t tempo;
    struct tm *time_info;
    char buffer[80];

    time(&tempo);
    time_info = localtime(&tempo);

    strftime(buffer, sizeof(buffer), "%M-%S", time_info);
    printf("Data e Hora: %s\n", buffer);

    /*FILE *file = fopen(rtc_timer, "w");
    if (file == NULL) {
        perror("Erro ao ler o timer");
        exit(EXIT_FAILURE);
    }

    fclose(file);
    printf("Valor %d definido no led %d...\r\n",value,ledn);
    */

}
//all leds da placa
void all_leds(int x){
    usr_leds(0,0);
    usr_leds(1,0);
    usr_leds(2,0);
    usr_leds(3,0);

    if(x > 15){
        printf("val invalido");
        return;
    }else{
        if(x & 1){usr_leds(0,1);};
        if(x & 2){usr_leds(1,1);};
        if(x & 4){usr_leds(2,1);};
        if(x & 8){usr_leds(3,1);};
    }

}


void PWM0(int pwm_number,int period,int duty_cycle) {
    FILE *file = fopen("/sys/class/pwm/pwmchip0/export", "w");
    if (file == NULL) {
        perror("Erro ao exportar o pwm");
        exit(EXIT_FAILURE);
    }

    fprintf(file,  "%d", pwm_number);
    fclose(file);

    if(lokc1 == 0){printf("\n\rPWM %d exportado...", pwm_number);}

    
    FILE *file1 = fopen("/sys/class/pwm/pwmchip0/pwm0/period", "w");
    fprintf(file1, "%d", period);
    fclose(file1);
    if(lokc1 == 0){printf("\n\rperiodo %d definido...", period);}

    FILE *file2 = fopen("/sys/class/pwm/pwmchip0/pwm0/duty_cycle", "w");
    fprintf(file2, "%d", duty_cycle);
    fclose(file2);
    if(lokc1 == 0){printf("\n\rdutycycle %d definido...\n\r", duty_cycle);lokc1++;}
    


    /*
    FILE *file3 = fopen("/sys/class/pwm/pwmchip0/pwm0/status", "w");
    fprintf(file3, "%d", status);
    fclose(file3);
    */
    return;
}

void on_pwm0(int status){
    
    FILE *file = fopen("/sys/class/pwm/pwmchip0/pwm0/enable", "w");
    if (file == NULL) {
        perror("Erro ao ativar o pwm");
        exit(EXIT_FAILURE);
    }
    fprintf(file, "%d", status);
    fclose(file);

}

void debounce_delay(){
    usleep(debounce);
}
