#include <wiringPi.h>
#include <thread>
#include <chrono>
#include <sstream>

#define HDD "/dev/sda/"

#define LED_PIN 26
#define BUT_PIN 27
#define POWER_OFF_BUT_PIN 24

#define DELAY_TIME 100

bool isConnectedToInternet() {
    int response = system("ping -c 1 -W 1 8.8.8.8 > /dev/null 2>&1");
    return response == 0;
}

int main(){
        wiringPiSetup();
        pinMode(LED_PIN, OUTPUT);
        pinMode(BUT_PIN, INPUT);
        pinMode(POWER_OFF_BUT_PIN, INPUT);
        
        for(;;){
                isConnectedToInternet() ? digitalWrite(LED_PIN, HIGH) : digitalWrite(LED_PIN, LOW);
                if(!digitalRead(BUT_PIN)){
                        digitalWrite(LED_PIN, LOW);
                        system("sudo reboot");
                        return 0;
                }
		if(digitalRead(POWER_OFF_BUT_PIN)){
                        digitalWrite(LED_PIN, LOW);
#ifdef HDD
			const char command[] = "sudo hdparm -Y ";
			char result[sizeof(HDD) + sizeof(command)];
			snprintf(result, sizeof(result), "%s%s", command, HDD);
                       	system(result);
#endif
                        system("sudo shutdown -h now");
                        return 0;
                }
                std::this_thread::sleep_for(std::chrono::milliseconds(DELAY_TIME));
        }
        return 0;
}
