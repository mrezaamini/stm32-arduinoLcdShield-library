/*---------------------------------------------------------*/
/*                    HAL AddOn codes                      */
/*              for using Arduino LCD shield               */
/*        ----------------------------------------         */
/*  first configure your project GPIO as described before  */
/*   then, generate your code & add these parts to your    */
/*   generated code. Finally use LCD functions freely in   */
/*                   your code & Run it!                   */
/*---------------------------------------------------------*/

/* Includes >> Add these in your includes------------------*/
#include <stdio.h>
#include <stdlib.h>

/* port defines >> Add these in your defines---------------*/

#define D7 GPIO_PIN_8
#define D7_GPIO GPIOA

#define D6 GPIO_PIN_10
#define D6_GPIO GPIOB

#define D5 GPIO_PIN_4
#define D5_GPIO GPIOB

#define D4 GPIO_PIN_5
#define D4_GPIO GPIOB

#define EN GPIO_PIN_7
#define EN_GPIO GPIOC

#define RS GPIO_PIN_9
#define RS_GPIO GPIOA

/* functions  >> Add these in your functions----------------*/

void delayMs(int n);
void LCD_nibble_write(char data, unsigned char control);
void LCD_command(unsigned char command);
void LCD_data(char data);
void LCD_init(void);

// int main(void)
//{

/* in Main  >> Add these 2 lines into your main before starting your code-----------------*/
/*------------ for initializing and clearing LCD -----------------*/
LCD_init();
LCD_command(0x01);

// while (1)
//{
/* USER CODE END WHILE */
//}
//}

/* functions implementations >> Add this part to your function implementation part-----------------*/
void LCD_init(void) // for initializing LCD
{
    delayMs(20); /* LCD controller reset sequence */
    LCD_nibble_write(0x30, 0);
    delayMs(5);
    LCD_nibble_write(0x30, 0);
    delayMs(1);
    LCD_nibble_write(0x30, 0);
    delayMs(1);

    LCD_nibble_write(0x20, 0); /* use 4-bit data mode */
    delayMs(1);
    LCD_command(0x28); /* set 4-bit data, 2-line, 5x7 font */
    LCD_command(0x06); /* move cursor right */
    LCD_command(0x01); /* clear screen, move cursor to home */
    LCD_command(0x0F); /* turn on display, cursor blinking */
}

void LCD_nibble_write(char data, unsigned char control)
{

    if (control == 0) // setting rs
        HAL_GPIO_WritePin(RS_GPIO, RS, GPIO_PIN_RESET);
    else
        HAL_GPIO_WritePin(RS_GPIO, RS, GPIO_PIN_SET);

    HAL_GPIO_WritePin(D4_GPIO, D4, data % 2);
    data /= 2;
    HAL_GPIO_WritePin(D5_GPIO, D5, data % 2);
    data /= 2;
    HAL_GPIO_WritePin(D6_GPIO, D6, data % 2);
    data /= 2;
    HAL_GPIO_WritePin(D7_GPIO, D7, data % 2);

    // pulse E
    HAL_GPIO_WritePin(EN_GPIO, EN, GPIO_PIN_SET);
    delayMs(0);
    HAL_GPIO_WritePin(EN_GPIO, EN, GPIO_PIN_RESET);
}

void LCD_command(unsigned char command)
{
    LCD_nibble_write(((command & 0xF0) >> 4), 0); /* upper nibble first */
    LCD_nibble_write(command & 0x0F, 0);          /* then lower nibble */

    if (command < 4)
        delayMs(5); /* command 1 and 2 needs up to 1.64ms */
    else
        delayMs(2); /* all others 40 us */
}

void LCD_data(char data)
{
    LCD_nibble_write(((data & 0xF0) >> 4), 1); /* upper nibble first */
    LCD_nibble_write(data & 0x0F, 1);          /* then lower nibble */

    delayMs(2);
}

/* delay n milliseconds (16 MHz CPU clock) */
void delayMs(int n)
{
    int i;
    for (; n > 0; n--)
        for (i = 0; i < 3195; i++)
            __NOP();
}
/********END OF FILE********/
