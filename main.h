/* Microchip Technology Inc. and its subsidiaries.  You may use this software 
 * and any derivatives exclusively with Microchip products. 
 * 
 * THIS SOFTWARE IS SUPPLIED BY MICROCHIP "AS IS".  NO WARRANTIES, WHETHER 
 * EXPRESS, IMPLIED OR STATUTORY, APPLY TO THIS SOFTWARE, INCLUDING ANY IMPLIED 
 * WARRANTIES OF NON-INFRINGEMENT, MERCHANTABILITY, AND FITNESS FOR A 
 * PARTICULAR PURPOSE, OR ITS INTERACTION WITH MICROCHIP PRODUCTS, COMBINATION 
 * WITH ANY OTHER PRODUCTS, OR USE IN ANY APPLICATION. 
 *
 * IN NO EVENT WILL MICROCHIP BE LIABLE FOR ANY INDIRECT, SPECIAL, PUNITIVE, 
 * INCIDENTAL OR CONSEQUENTIAL LOSS, DAMAGE, COST OR EXPENSE OF ANY KIND 
 * WHATSOEVER RELATED TO THE SOFTWARE, HOWEVER CAUSED, EVEN IF MICROCHIP HAS 
 * BEEN ADVISED OF THE POSSIBILITY OR THE DAMAGES ARE FORESEEABLE.  TO THE 
 * FULLEST EXTENT ALLOWED BY LAW, MICROCHIP'S TOTAL LIABILITY ON ALL CLAIMS 
 * IN ANY WAY RELATED TO THIS SOFTWARE WILL NOT EXCEED THE AMOUNT OF FEES, IF 
 * ANY, THAT YOU HAVE PAID DIRECTLY TO MICROCHIP FOR THIS SOFTWARE.
 *
 * MICROCHIP PROVIDES THIS SOFTWARE CONDITIONALLY UPON YOUR ACCEPTANCE OF THESE 
 * TERMS. 
 */

/* 
 * File:   
 * Author: 
 * Comments:
 * Revision history: 
 */

// This is a guard condition so that contents of this file are not included
// more than once.  
#ifndef MAIN_H
#define	MAIN_H

#include "adc.h"
#include "clcd.h"
#include "eeprom.h"
#include "matrix_keypad.h"
#include "timer0.h"
#include "ds1307.h"
#include "i2c.h"
#include "uart.h"

unsigned char key;
unsigned int dash_count = 0;
char gear[8][3] = {"ON","GN","GR","G1","G2","G3","G4","CO"};
unsigned int speed = 0;
char time[9];
unsigned char address = 0x00;
unsigned char data;
char org_pass[5];
char re_pass[5];
char usr_pass[5];
unsigned short flag = 3;
unsigned int count = 0;
unsigned int sec = 0;
unsigned short key_flag = 0;
unsigned int a;
unsigned short pass = 0;
unsigned long int delay = 0;
char menu[5][16] = {"View Log","Download Log","Clear Log","Set Time","Change Password"};
unsigned short main_flag = 0;
unsigned short star_flag = 0;
unsigned short menu_flag = 0;
unsigned short main_count = 0;
unsigned char clock_reg[3];
unsigned short view_count = 0;
unsigned short main_display = 0;
unsigned long int delay1 = 0;
short view = 1;
short log_count = 1;
unsigned long int delay2 = 0;
short set = 0;
char time1[9];
unsigned long int set_delay = 0;
int hour,min,sec1;
unsigned char pass_addr = 0xC8;
int change = 0;
unsigned long secs = 0;




void dashboard();
void event_capture();
void password();
int mystrcmp(char str1[], char str2[]);
void main_menu();
void get_time();
void option(int num);
void view_log();
void download_log();
void clear_log();
void set_time();
void change_pass();
void write_24c02(unsigned char address, unsigned char data);
unsigned char read_24c02(unsigned char address);


#endif

