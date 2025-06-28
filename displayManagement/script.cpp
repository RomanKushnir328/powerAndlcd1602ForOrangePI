#include <wiringPi.h>
#include <lcd.h>
#include <thread>
#include <chrono>
#include <string.h>
#include <math.h>
#include "linux-system-usage.hpp"

#define LCD_RS  25
#define LCD_E   23
#define LCD_D0  22
#define LCD_D1  19
#define LCD_D2  17
#define LCD_D3  8
#define LCD_D4  7
#define LCD_D5  5
#define LCD_D6  10
#define LCD_D7  6

#define DELAY_TIME 200
#define BUFFER_SIZE 16

using namespace get_system_usage_linux;

int main(){
		wiringPiSetup();
        static int lcd = lcdInit (2, 16, 8, LCD_RS, LCD_E, LCD_D0, LCD_D1, LCD_D2, LCD_D3, LCD_D4, LCD_D5, LCD_D6, LCD_D7);
		static char buffer[BUFFER_SIZE];
        
        for(;;){
			CPU_stats t1 = read_cpu_data();
			std::this_thread::sleep_for(std::chrono::milliseconds(DELAY_TIME));
			CPU_stats t2 = read_cpu_data();

			float cpuUsage = round((100.0f * get_cpu_usage(t1, t2)) * 100) / 100.0;
			int cpuTemp = get_thermalzone_temperature(2)/1000;
			auto memoryData = read_memory_data();
			float ramUsage = 100.0f * memoryData.get_memory_usage();
			float memoryUsage = 100.0f * get_disk_usage("/");

			int written = snprintf(buffer, BUFFER_SIZE, "CPU %.1f%% %dC", cpuUsage, cpuTemp);
			if(written < BUFFER_SIZE){
				memset(buffer+written, ' ', BUFFER_SIZE-written);
			}
			lcdPosition(lcd, 0, 0);
        	lcdPuts(lcd, buffer);
     
			written = snprintf(buffer, BUFFER_SIZE, "MEM %.1f%% %.1f%%", ramUsage, memoryUsage);
			if(written < BUFFER_SIZE){
				memset(buffer+written, ' ', BUFFER_SIZE-written);
			}
			lcdPosition(lcd, 0, 1);
			lcdPuts(lcd, buffer);
		
		}
        return 0;
}
