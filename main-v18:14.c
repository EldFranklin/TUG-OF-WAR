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
#include "TUG_OF_WAR.h"
//Macro Botoes
#define BOTAO_ESQUERDO_PIN "/sys/class/gpio/gpio60/value"
#define BOTAO_DIREITO_PIN "/sys/class/gpio/gpio30/value"
//------------------------------------------------------
//------------------------------------------------------
#define debounce 100

//char buffer[50];


char lock0 = 0;





char buf[10];

enum EstadoDoBotao {
    RELEASED,
    PRESSED
};

int main() {

   
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
