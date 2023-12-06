#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <fcntl.h>
#include <unistd.h>
#include <poll.h>

#include <time.h>
#include <sys/ioctl.h>
#include <linux/i2c-dev.h>

#define GPIO_PATH "/sys/class/gpio/gpio"
#define GPIO_1 60
#define GPIO_2 61

#define GPIO_3 20

//leds
#define GPIO_LED_0 66
#define GPIO_LED_1 67
#define GPIO_LED_2 68
#define GPIO_LED_3 69
#define GPIO_LED_4 65
#define GPIO_LED_5 45
#define GPIO_LED_6 26

//team red
#define GPIO_LED_RED 70

//team blue
#define GPIO_LED_BLUE 27

//Macros Leds Embarcados
#define Led_Usr0 "/sys/class/leds/beaglebone:green:usr0/brightness"
#define Led_Usr1 "/sys/class/leds/beaglebone:green:usr1/brightness"
#define Led_Usr2 "/sys/class/leds/beaglebone:green:usr2/brightness"
#define Led_Usr3 "/sys/class/leds/beaglebone:green:usr0/brightness"

#define rtc_timer "/sys/class/rtc/rtc0/time"
//---------------------------------------------------------------
//---------------------------------------------------------------

//Macro Botoes
#define BOTAO_ESQUERDO_PIN "/sys/class/gpio/gpio60/value"
#define BOTAO_DIREITO_PIN "/sys/class/gpio/gpio30/value"
//------------------------------------------------------
//------------------------------------------------------
#define debounce 100

void debounce_delay(){
    usleep(debounce);
}

//Macro path dos gpio


//--------------------
//--------------------


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

// Função para acessar timer da placa
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

char lock0 = 0; 
void PWM0(int pwm_number,int period,int duty_cycle) {
    FILE *file = fopen("/sys/class/pwm/pwmchip0/export", "w");
    if (file == NULL) {
        perror("Erro ao exportar o pwm");
        exit(EXIT_FAILURE);
    }

    fprintf(file,  "%d", pwm_number);
    fclose(file);

    if(lock0 == 0){printf("\n\rPWM %d exportado...", pwm_number);}

    
    FILE *file1 = fopen("/sys/class/pwm/pwmchip0/pwm0/period", "w");
    fprintf(file1, "%d", period);
    fclose(file1);
    if(lock0 == 0){printf("\n\rperiodo %d definido...", period);}

    FILE *file2 = fopen("/sys/class/pwm/pwmchip0/pwm0/duty_cycle", "w");
    fprintf(file2, "%d", duty_cycle);
    fclose(file2);
    if(lock0 == 0){printf("\n\rdutycycle %d definido...\n\r", duty_cycle);lock0++;}
    


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
/////

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

int gpio_fd, ret;
struct pollfd fdset[1];
char buf[10];

enum EstadoDoBotao {
    RELEASED,
    PRESSED
};




int main() {
    //int gpio_number = 60;  // Substitua pelo número do pino GPIO que você está usando
    //teste PWM
    //int frequencia = 1000;    // 1 kHz
    //float dutycicle = 0.5;    // 50%

    //teste pwm
   // gerar_pwm(50, 1000000, 500000);
   
    read_rtc0_time();

    //botoes
    export_gpio(GPIO_1);
    export_gpio(GPIO_2);
    export_gpio(GPIO_3);
    

    configure_gpio_input(GPIO_1);
    configure_gpio_input(GPIO_2);
    configure_gpio_input(GPIO_3);

    //leds
    export_gpio(GPIO_LED_0);
    export_gpio(GPIO_LED_1);
    export_gpio(GPIO_LED_2);
    export_gpio(GPIO_LED_3);
    export_gpio(GPIO_LED_4);

    export_gpio(GPIO_LED_RED);
    export_gpio(GPIO_LED_BLUE);


    configure_gpio_output(GPIO_LED_0);
    configure_gpio_output(GPIO_LED_1);
    configure_gpio_output(GPIO_LED_2);
    configure_gpio_output(GPIO_LED_3);
    configure_gpio_output(GPIO_LED_4);

    configure_gpio_output(GPIO_LED_RED);
    configure_gpio_output(GPIO_LED_BLUE);

    //variaveis controle
    int qtd_pontos_player_1 = 0, qtd_pontos_player_2 = 0;
    enum EstadoDoBotao estado_player1 = RELEASED;
    enum EstadoDoBotao estado_player2 = RELEASED;
    char beep = 0;

    start:
    set_gpio_value(GPIO_LED_0,1);
    sleep(1);
    set_gpio_value(GPIO_LED_1,1);
    sleep(1);
    set_gpio_value(GPIO_LED_2,1);
    sleep(1);
    set_gpio_value(GPIO_LED_3,1);
    sleep(1);
    set_gpio_value(GPIO_LED_4,1);
    sleep(1);

    set_gpio_value(GPIO_LED_0,0);
    set_gpio_value(GPIO_LED_1,0);
    set_gpio_value(GPIO_LED_2,0);
    set_gpio_value(GPIO_LED_3,0);
    set_gpio_value(GPIO_LED_4,0);

    printf("Começar!\n\r");
    // Loop principal
    while (1) {
        
        char r = read_gpio(GPIO_3);
        if (r == '0')
        {   
            printf("\n\rreiniciado!\n\r");
            set_gpio_value(GPIO_LED_0,0);
            set_gpio_value(GPIO_LED_1,0);
            set_gpio_value(GPIO_LED_2,0);
            set_gpio_value(GPIO_LED_3,0);
            set_gpio_value(GPIO_LED_4,0);
            qtd_pontos_player_1 = 0;
            qtd_pontos_player_2 = 0;
            lock0 = 0;
            goto start;
        }
        
        
        
        char x = read_gpio(GPIO_1);

        // Verifica a borda de descida no pino GPIO_1 com debounce
        if (x == '0' && estado_player1 == RELEASED) {
            debounce_delay();  // Aguarda um período de debounce
            x = read_gpio(GPIO_1);  // Lê novamente após o debounce
            if (x == '0') {
                qtd_pontos_player_1++;
                set_gpio_value(GPIO_LED_RED,1);
                estado_player1 = PRESSED;
            }
        } else if (x == '1') {
            estado_player1 = RELEASED;
            set_gpio_value(GPIO_LED_RED,0);
        }

        char x_player2 = read_gpio(GPIO_2);

        // Verifica a borda de descida no pino GPIO_PLAYER2 com debounce
        if (x_player2 == '0' && estado_player2 == RELEASED) {
            debounce_delay();  // Aguarda um período de debounce
            x_player2 = read_gpio(GPIO_2);  // Lê novamente após o debounce
            if (x_player2 == '0') {
                qtd_pontos_player_2++;
                set_gpio_value(GPIO_LED_BLUE,1);
                estado_player2 = PRESSED;
            }
        } else if (x_player2 == '1') {
            estado_player2 = RELEASED;
            set_gpio_value(GPIO_LED_BLUE,0);
        }

        // Limite de taxa de leitura para evitar uso excessivo da CPU
        usleep(10000);  // Aguarde 10 ms (ajuste conforme necessário)

        // Limite para evitar loop infinito sem saída
        // Aqui você pode adicionar uma condição para encerrar o loop se um jogador atingir uma pontuação específica

        // Limpeza do buffer de saída para evitar buffering excessivo no terminal
        fflush(stdout);

        printf("\rPlayer 01: %d | Player 02: %d", qtd_pontos_player_1, qtd_pontos_player_2);
        if (lock0 == 0)
        {
            // Imprime os resultados do jogo
        
            switch (qtd_pontos_player_1)
            {
            case 10:
                set_gpio_value(GPIO_LED_3,1);
                break;
            case 20:
                set_gpio_value(GPIO_LED_2,1);
                break;
            case 30:
                set_gpio_value(GPIO_LED_4,1);
                set_gpio_value(GPIO_LED_0,0);
                set_gpio_value(GPIO_LED_1,0);

                set_gpio_value(GPIO_LED_RED,1);

                printf("\n\rJogador 1 vence!\n\r");
                break;
            
            default:
                break;
            }
            
            switch (qtd_pontos_player_2)
            {
            case 10:
                set_gpio_value(GPIO_LED_0,1);
                break;
            case 20:
                set_gpio_value(GPIO_LED_1,1);
                break;
            case 30:
                set_gpio_value(GPIO_LED_4,1);

                set_gpio_value(GPIO_LED_3,0);
                set_gpio_value(GPIO_LED_2,0);
                set_gpio_value(GPIO_LED_BLUE,1);

                printf("\n\rJogador 2 vence!\n\r");
                break;
            
            default:
                break;
            }
        
        }

        if((qtd_pontos_player_1 >= 30 || qtd_pontos_player_2 >= 30) && lock0 == 0){
                    //PWM0(0,3333334,1666667);
                      PWM0(0,1000000,500000);
                    while(beep < 5){
                        on_pwm0(1);
                        all_leds(2);
                        usleep(250000);
                        on_pwm0(0);
                        all_leds(5);
                        usleep(250000);
                        beep++;
                    }
                    all_leds(15);

                    printf("Para jogar novamente aperte R \n\r");
                    sleep(2);
        }
        
    }

    return 0;
}

