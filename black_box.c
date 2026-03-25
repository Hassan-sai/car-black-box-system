/*
 * File:   dashboard.c
 * Author: shaik
 *
 * Created on 28 March, 2024, 2:06 PM
 */


#include <xc.h>
#include "main.h"

int mystrcmp(char str1[], char str2[])
{
    for(int i = 0; i < 4; i++)
    {
        if(str1[i] != str2[i])
        {
            return 0;
        }
    }
    return 1;
}

void write_24c02(unsigned char address, unsigned char data)
{
	i2c_start();
	i2c_write(0xA0);
	i2c_write(address);
	i2c_write(data);
	i2c_stop();
    for(unsigned long int i = 3000; i > 0; i--);

}

unsigned char read_24c02(unsigned char address)
{
	unsigned char data;

	i2c_start();
	i2c_write(0xA0);
	i2c_write(address);
	i2c_rep_start();
	i2c_write(0xA1);
	data = i2c_read();
	i2c_stop();

	return data;
}


void get_time(void)
{
	clock_reg[0] = read_ds1307(HOUR_ADDR);
	clock_reg[1] = read_ds1307(MIN_ADDR);
	clock_reg[2] = read_ds1307(SEC_ADDR);

	if (clock_reg[0] & 0x00)
	{
		time[0] = '0' + ((clock_reg[0] >> 4) & 0x01);
        time[1] = '0' + (clock_reg[0] & 0x0F);
    }
	else
	{
		time[0] = '0' + ((clock_reg[0] >> 4) & 0x03);
			time[1] = '0' + (clock_reg[0] & 0x0F);
	}
	
	time[2] = ':';
	time[3] = '0' + ((clock_reg[1] >> 4) & 0x0F);
	time[4] = '0' + (clock_reg[1] & 0x0F);
	time[5] = ':';
	time[6] = '0' + ((clock_reg[2] >> 4) & 0x0F);
	time[7] = '0' + (clock_reg[2] & 0x0F);
	time[8] = '\0';
}

void dashboard() {
    
    if(key == MK_SW1)
    {
        if(dash_count == 7)
        {
            dash_count = 1;
             event_capture();
           
        }else if(dash_count !=6)
        {
            dash_count++;
             event_capture();
            
        }
       
    }else if(key == MK_SW2)
    {
        if(dash_count == 7)
        {
            dash_count = 1;
            event_capture();
        }
        else if(dash_count != 1 && dash_count > 0)
        {
            dash_count--;
            event_capture();
        }
    }else if(key == MK_SW3 && dash_count != 7)
    {
        dash_count = 7;
        event_capture();
    }
    get_time();
    speed = ((read_adc(CHANNEL4))/10.25);
    clcd_print("  TIME    EV  SP",LINE1(0));
    clcd_print(time,LINE2(0));
    clcd_print(gear[dash_count],LINE2(10));
    clcd_putch(speed/10+'0',LINE2(14));
    clcd_putch(speed%10+'0',LINE2(15));
    secs = 0;
    
}


void event_capture()
{
    if(address >= 99)
    {
        address = 0;        
    }
    for(int i = 0; i < 8; i++)
    {
        if(i == 2 || i == 5)
        {
            continue;
        }
        write_24c02(address++,time[i]);
    }
    write_24c02(address++,gear[dash_count][0]);
    write_24c02(address++,gear[dash_count][1]);
    write_24c02(address++,speed/10+'0');
    write_24c02(address++,speed%10+'0');
    view_count++;
//    CLEAR_DISP_SCREEN;
//    while(1)
//    {
//        address = 0x00;
//        for(int i = 0; i < 8; i++)
//        {
//            if(i == 2 || i == 5)
//            {
//                clcd_putch(':',LINE2(i));
//            }
//            else
//            {
//            data = read_24c02(address++);
//            clcd_putch(data,LINE2(i));
//            }
//        }
//        clcd_print("  ",LINE2(8));
//        data = read_24c02(address++);
//        clcd_putch(data,LINE2(10));
//        data = read_24c02(address++);
//        clcd_putch(data,LINE2(11));
//        clcd_print("  ",LINE2(12));
//        data = read_24c02(address++);
//        clcd_putch(data,LINE2(14));
//        data = read_24c02(address++);
//        clcd_putch(data,LINE2(15));
//        
//        
//    }
    
} 

void password()
{
    if (flag > 0 && pass == 0) {
        clcd_print("Enter Password", LINE1(0));
        if (count < 4) {
            if(secs == 3)
            {
                key_flag = 0;
                count = 0;
                flag = 3;
                CLEAR_DISP_SCREEN;
            }
            if (key == MK_SW11) {
                usr_pass[count] = '1';
                clcd_putch('*', LINE2(count));
                count++;
                secs = 0;
            } else if (key == MK_SW12) {
                usr_pass[count] = '0';
                clcd_putch('*', LINE2(count));
                count++;
                secs = 0;
            }
            if (delay1++ == 400) {
                clcd_putch('_', LINE2(count));
            } else if (delay1++ == 800) {
                delay1 = 0;
                clcd_putch(' ', LINE2(count));
            }
        }
        if (count > 3) {
            usr_pass[count] = '\0';
            for(int i = 0; i < 4; i++)
            {
                org_pass[i] = read_24c02(pass_addr++);
            }
            org_pass[4] = '\0';
            pass_addr = 0xC8;
            if (mystrcmp(org_pass, usr_pass)) {
                for (long int i = 0; i < 100000; i++);
                CLEAR_DISP_SCREEN;
                pass = 1;
            } else {
                flag--;
                for (long int i = 0; i < 100000; i++);
                CLEAR_DISP_SCREEN;
                clcd_print("FAILED....", LINE1(0));
                clcd_print("Attempt left - ", LINE2(0));
                clcd_putch(flag + '0', LINE2(15));
                for (long int i = 0; i < 300000; i++);
                CLEAR_DISP_SCREEN;
                count = 0;
                sec = 0;
                secs = 0;
                
            }
        }
    }
    else if (pass == 1) {
        clcd_print("Well Done", LINE1(0));
        clcd_print("My BOY!!!!", LINE2(1));
        for (long int i = 0; i < 100000; i++);
        CLEAR_DISP_SCREEN;
        flag = 3;
        count = 0;
        key_flag = 0;
        main_flag = 1;
    }
    
    if(flag == 0)
    {
        if(sec == 120)
        {
            key_flag = 0;
            flag = 3;
            CLEAR_DISP_SCREEN;
        }
        else
        {
            a = 120 - sec;
            clcd_print("Its OK!", LINE1(0));
            clcd_putch((a%10)+'0', LINE2(3));
            clcd_putch(((a/10)%10)+'0', LINE2(2));
            clcd_putch((a/100)+'0', LINE2(1));
        }
        
    }
}


void main_menu()
{
    
    if(key == MK_SW12)
    {
        menu_flag = 1;
        if(star_flag == 0)
        {
            star_flag = 1;
        }
        else if(main_count < 3)
        {
            main_count++;
            CLEAR_DISP_SCREEN;
        }
    }
    if(key == MK_SW11)
    {
        menu_flag = 1;
        if(star_flag == 1)
        {
            star_flag = 0;
        }
        else if(main_count > 0)
        {
            main_count--;
            CLEAR_DISP_SCREEN;
            
        }
    }
    if(star_flag == 0)
        {
            clcd_putch('*',LINE1(0));
            clcd_putch(' ',LINE2(0));
        }
        else
        {
            clcd_putch(' ',LINE1(0));
            clcd_putch('*',LINE2(0));
        }

        clcd_print(menu[main_count], LINE1(1));
        clcd_print(menu[main_count + 1], LINE2(1));

        if(key == MK_SW13)
        {
            if(star_flag == 0)
            {
                CLEAR_DISP_SCREEN;
                option(main_count);
            }
            else
            {
            CLEAR_DISP_SCREEN;
            option(main_count+1);
            }
        }
        
        if (key == MK_SW14 && main_flag == 1) {
            star_flag = 0;
            main_count = 0;
            menu_flag = 0;
            main_flag = 0;
            flag = 3;
            count = 0;
            pass = 0;
            CLEAR_DISP_SCREEN;
        }
}

void option(int num)
{
    switch(num)
    {
        case 0:
            main_flag = 2;
            break;
        case 1:
            main_flag = 3;
            break;
        case 2:   
            main_flag = 4;
            break;
        case 3:
            main_flag = 5;
            break;
        case 4:
            main_flag = 6;
            break;
    }
}

void view_log()
{
    
//    if(view > 9 && log_count <= 10)
//    {
//        view = 0;
//    }
    if(view_count == 0)
    {
        clcd_print("View Log",LINE1(0));
        clcd_print("EMPTY!!!",LINE2(0));
    }
    else
    {
        clcd_print("View Log - ",LINE1(0));
    clcd_putch((view/10)+'0',LINE1(12));
    clcd_putch((view%10)+'0',LINE1(13));
//    if (view > 9) {
//            view = 0;
//        }
//        if (view < 0) {
//            view = 9;
//        }
        //    if(log_count > 10)
        //    {
        //        log_count = 1;
        //    }
    if ((key == MK_SW11) && (view > 1)) {
            view--;
           // log_count--;
        }
        if ((key == MK_SW12) && (view < view_count) && (view < 10)) {
            view++;
            //log_count++;
        }
        char view_address = (view - 1)* 10;
        for (int i = 0; i < 8; i++) {
            if (i == 2 || i == 5) {
                clcd_putch(':', LINE2(i));
            } else {
                data = read_24c02(view_address++);
                clcd_putch(data, LINE2(i));
            }
        }
        clcd_print("  ", LINE2(8));
        data = read_24c02(view_address++);
        clcd_putch(data, LINE2(10));
        data = read_24c02(view_address++);
        clcd_putch(data, LINE2(11));
        clcd_print("  ", LINE2(12));
        data = read_24c02(view_address++);
        clcd_putch(data, LINE2(14));
        data = read_24c02(view_address++);
        clcd_putch(data, LINE2(15));
         
    }
    if (key == MK_SW14) {
            view = 1;
            main_flag = 1;
            CLEAR_DISP_SCREEN;
        }
}

void download_log()
{
    init_uart();
    char arr[15];
    char add = 0;
    unsigned short count = 0;
    int a;
    if(view_count == 0)
    {
        clcd_print("NO LOGS",LINE1(0));
        clcd_print("EMPTY!!!",LINE2(0));
        for(unsigned long wait = 500000; wait > 0; wait--);
    }
    else
    {
    clcd_print("Downloading Logs...",LINE1(0));
    puts("Downloading Logs...\n\r");
    for(unsigned long wait = 500000; wait > 0; wait--);
    if(view_count > 10)
    {
        a = 10;
    }
    else
    {
        a = view_count - 1;
    }
    for(int i = 0; i < a; i++)
    {
        count = 0;
        for(int i = 0; i < 8; i++)
        {
            if(i == 2 || i == 5)
            {
                arr[i] = ':';
            }
            else
            {
            data = read_24c02(add++);
            arr[i] = data;
            }
            count++;
        }
        arr[count] = ' ';
        count++;
        data = read_24c02(add++);
        arr[count] = data;
        count++;
        data = read_24c02(add++);
        arr[count] = data;
        count++;
        arr[count] = ' ';
        count++;
        data = read_24c02(add++);
        arr[count] = data;
        count++;
        data = read_24c02(add++);
        arr[count] = data;
        count++;
        arr[count] = '\0';
        puts(arr);
        puts("\n\r");
    }
}
    CLEAR_DISP_SCREEN;
}

void clear_log()
{
    clcd_print("Clearing Logs...",LINE1(0));
    for(unsigned long i = 300000; i > 0; i--);
    view_count = 0;
    address = 0;
    CLEAR_DISP_SCREEN;
}

void set_time()
{
    if(set == 0)
    {
    clock_reg[0] = read_ds1307(HOUR_ADDR);
	clock_reg[1] = read_ds1307(MIN_ADDR);
	clock_reg[2] = read_ds1307(SEC_ADDR);
	if (clock_reg[0] & 0x00)
	{
		time1[0] = '0' + ((clock_reg[0] >> 4) & 0x01);
        time1[1] = '0' + (clock_reg[0] & 0x0F);
    }
	else
	{
		time1[0] = '0' + ((clock_reg[0] >> 4) & 0x03);
        time1[1] = '0' + (clock_reg[0] & 0x0F);
	}
	
	time1[2] = ' ';
	time1[3] = '0' + ((clock_reg[1] >> 4) & 0x0F);
	time1[4] = '0' + (clock_reg[1] & 0x0F);
	time1[5] = ' ';
	time1[6] = '0' + ((clock_reg[2] >> 4) & 0x0F);
	time1[7] = '0' + (clock_reg[2] & 0x0F);
	time1[8] = '\0';
    set = 1;
                hour = ((time1[0] - '0') * 10)+ (time1[1] - '0');
            min = ((time1[3] - '0') * 10)+ (time1[4] - '0');
            sec1 = ((time1[6] - '0') * 10)+ (time1[7] - '0');
    }
    else
    {
        clcd_print("HH  MM  SS",LINE1(2));
        if(set == 1)
        {
            if(set_delay++ == 600)
            {
                clcd_putch(time1[0],LINE2(2));
                clcd_putch(time1[1],LINE2(3)); 
            }
            else if(set_delay++ == 1200)
            {
                set_delay = 0;
                clcd_putch(' ',LINE2(2));
                clcd_putch(' ',LINE2(3));
            }
            clcd_putch(time1[2],LINE2(4));
            clcd_putch(time1[2],LINE2(5));
            clcd_putch(time1[3],LINE2(6));
            clcd_putch(time1[4],LINE2(7));
            clcd_putch(time1[5],LINE2(8));
            clcd_putch(time1[5],LINE2(9));
            clcd_putch(time1[6],LINE2(10));
            clcd_putch(time1[7],LINE2(11));
        }
        else if(set == 2)
        {
            clcd_putch(time1[0],LINE2(2));
            clcd_putch(time1[1],LINE2(3));
            clcd_putch(time1[2],LINE2(4));
            clcd_putch(time1[2],LINE2(5));
            if(set_delay++ == 600)
            {
                clcd_putch(time1[3],LINE2(6));
                clcd_putch(time1[4],LINE2(7)); 
            }
            else if(set_delay++ == 1200)
            {
                set_delay = 0;
                clcd_putch(' ',LINE2(6));
                clcd_putch(' ',LINE2(7));
            }
            clcd_putch(time1[5],LINE2(8));
            clcd_putch(time1[5],LINE2(9));
            clcd_putch(time1[6],LINE2(10));
            clcd_putch(time1[7],LINE2(11));
        }
        else if(set == 3)
        {
            clcd_putch(time1[0],LINE2(2));
            clcd_putch(time1[1],LINE2(3));
            clcd_putch(time1[2],LINE2(4));
            clcd_putch(time1[2],LINE2(5));
            clcd_putch(time1[3],LINE2(6));
            clcd_putch(time1[4],LINE2(7)); 
            clcd_putch(time1[5],LINE2(8));
            clcd_putch(time1[5],LINE2(9));
            if(set_delay++ == 600)
            {
                clcd_putch(time1[6],LINE2(10));
                clcd_putch(time1[7],LINE2(11));
            }
            else if(set_delay++ == 1200)
            {
                set_delay = 0;
                clcd_putch(' ',LINE2(10));
                clcd_putch(' ',LINE2(11));
            }   
        }
//        clcd_print(time1,LINE2(0));
        if(key == MK_SW11)
        {
//            hour = (((time1[0] - '0') << 4) | ((time1[1] - '0')));
//            min = (((time1[3] - '0') << 4) | ((time1[4] - '0')));
//            sec1 = (((time1[6] - '0') << 4) | ((time1[7] - '0')));
            if(set == 1)
            {
                if(hour < 23)
                {
                    hour++;
                }
                else
                {
                    hour = 0;
                }
                time1[0] = '0' + hour/10;
                time1[1] = '0' + hour%10;
            }
            else if(set == 2)
            {
                if(min < 59)
                {
                    min++;    
                }
                else
                {
                    min = 0;
                }
                time1[3] = '0' + min/10;
                time1[4] = '0' + min%10;
            }
            else if(set == 3)
            {
                if(sec1 < 59)
                {
                    sec1++;
                }
                else
                {
                    sec1 = 0;
                }
                time1[6] = '0' + sec1/10;
                time1[7] = '0' + sec1%10;
            }
        }
        if(key == MK_SW12)
        {
            if(set == 3)
            {
                set = 1;
                set_delay = 0;
            }
            else
            {
                set++;
                set_delay = 0;
            }
        }
        if(key == MK_SW13)
        {
            clock_reg[0] = (((time1[0] - '0') << 4) | (time1[1] - '0'));
            clock_reg[1] = (((time1[3] - '0')<< 4) | (time1[4] - '0'));
            clock_reg[2] = (((time1[6] - '0')<< 4) | (time1[7] - '0'));
            write_ds1307(HOUR_ADDR,clock_reg[0]);
            write_ds1307(MIN_ADDR,clock_reg[1]);
            write_ds1307(SEC_ADDR,clock_reg[2]);
            main_flag = 1;
            CLEAR_DISP_SCREEN;
        }
        if(key == MK_SW14)
        {
            main_flag = 1;
            CLEAR_DISP_SCREEN;
        }
    }
}

void change_pass()
{
    
    if (count < 4 && change == 0) {
        clcd_print("Enter Password",LINE1(0));
            if (key == MK_SW11) {
                org_pass[count] = '1';
                clcd_putch('*', LINE2(count));
                count++;
            } else if (key == MK_SW12) {
                org_pass[count] = '0';
                clcd_putch('*', LINE2(count));
                count++;
            }
            if (delay1++ == 400) {
                clcd_putch('_', LINE2(count));
            } else if (delay1++ == 800) {
                delay1 = 0;
                clcd_putch(' ', LINE2(count));
            }
        if(count == 4)
        {
            for (long int i = 0; i < 100000; i++);
            CLEAR_DISP_SCREEN;
            count = 0;
            change = 1;
        }
    }
    else if(count < 4 && change == 1)
    {
        clcd_print("Confirm Password",LINE1(0));
            if (key == MK_SW11) {
                re_pass[count] = '1';
                clcd_putch('*', LINE2(count));
                count++;
            } else if (key == MK_SW12) {
                re_pass[count] = '0';
                clcd_putch('*', LINE2(count));
                count++;
            }
            if (delay1++ == 400) {
                clcd_putch('_', LINE2(count));
            } else if (delay1++ == 800) {
                delay1 = 0;
                clcd_putch(' ', LINE2(count));
            }
        if(count == 4)
        {
            for (long int i = 0; i < 100000; i++);
            CLEAR_DISP_SCREEN;
            count = 0;
            change = 2;
        }
    }
    else
    {
        org_pass[4] = '\0';
        re_pass[4] = '\0';
        if(mystrcmp(org_pass,re_pass))
        {
            clcd_print("New Password",LINE1(0));
            clcd_print("Updated!!",LINE2(0));
            for(unsigned long wait = 300000; wait > 0; wait--);
            for(int i = 0; i < 4; i++)
            {
                write_24c02(pass_addr++,org_pass[i]);
            }
            pass_addr = 0xC8;
            CLEAR_DISP_SCREEN;
            main_flag = 1;
            change = 0;
            delay1 = 0;
        }
        else
        {
            clcd_print("Wrong Password",LINE1(0));
            clcd_print("Try Later!!",LINE2(0));
            for(unsigned long wait = 300000; wait > 0; wait--);
            CLEAR_DISP_SCREEN;
            main_flag = 1;
            change = 0;
            delay1 = 0;
        }
    }
    
    
            
}
