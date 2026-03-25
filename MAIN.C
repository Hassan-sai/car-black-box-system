/*
 * File:   MAIN.C
 * Author: shaik
 *
 * Created on 28 March, 2024, 2:00 PM
 */


#include <xc.h>
#include "main.h"



void init_config()
{
    init_adc();
    init_clcd();
    init_matrix_keypad();
    init_timer0();
    init_i2c();
	init_ds1307();
    PEIE = 1;
    GIE = 1;
    TRISB0 = 0;
}

void main() {
    init_config();
    dashboard();
    event_capture();
    for(int i = 0; i < 4; i++)
    {
        write_24c02(pass_addr++,'0');
    }
    pass_addr = 0xC8;
    while(1)
    {
        key = read_switches(STATE_CHANGE);
        while(key == MK_SW11 && main_flag > 0)
        {
            key = read_switches(LEVEL_CHANGE);
            if(delay++ >= 50000)
            {
                delay = 0;
                key = MK_SW13;
                CLEAR_DISP_SCREEN;
                break;
            }
            
        }
        if(delay > 30 && delay < 50000)
        {
            key = MK_SW11;
            delay = 0;
        }
        
        while(key == MK_SW12 && main_flag > 0)
        {
            key = read_switches(LEVEL_CHANGE);
            if(delay2++ >= 50000)
            {
                delay2 = 0;
                key = MK_SW14;
                CLEAR_DISP_SCREEN;
                break;
            }
        }
        if(delay2 > 30 && delay2 < 50000)
        {
            delay2 = 0;
            key = MK_SW12;
        }
        
        if(key == MK_SW11 && key_flag == 0 && main_flag == 0)
        {
            key_flag = 1;
            key = 0xFF;
            CLEAR_DISP_SCREEN;
        }
        if(key_flag == 1)
        {
            password();
        }
        else if(main_flag > 0)
        {
            if(menu_flag == 0)
            {
                clcd_putch('*',LINE1(0));
            }
            if(main_flag == 1)
            {
                main_menu();
            }
            else if(main_flag == 2)
            {
                view_log();
            }
            else if(main_flag == 3)
            {
                CLEAR_DISP_SCREEN;
                download_log();
                main_flag = 1;
            }
            else if(main_flag == 4)
            {
                clear_log();
                main_flag = 1;
            }
            else if(main_flag == 5)
            {
                set_time();
            }
            else if(main_flag == 6)
            {
                change_pass();
            }
        }
        else
        {
            dashboard();
        }
    }
}

