#include "DS3231.h"
#include <chrono>
#include <thread>
#include <iostream>

int main(){
	DS3231 rtc(1, 0x68);
	rtc.setTime(0,45,20,4,3,7,24);
	while (true) {
        rtc.printTime();
        std::this_thread::sleep_for(std::chrono::seconds(1)); // Update every second
        std::cout << "\033[2J\033[1;1H"; // Clear the console
    }	
	return 0;
}
