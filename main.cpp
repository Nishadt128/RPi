#include "DS3231.h"
#include <chrono>
#include <thread>
#include <iostream>
#include <iomanip>
#include <atomic>

using namespace std;

atomic<bool> running(true);

void displayCurrentTime(DS3231& rtc){
	while (true){
	rtc.printTime();
	this_thread::sleep_for(chrono::seconds(1));
	// cout << "\033[2J\033[1;1H";	
	}
}

void setCurrentTime(DS3231& rtc) {
   uint8_t seconds, minutes, hours, dayOfWeek, dayOfMonth, month, year;

    seconds = 0;
    minutes = 15 ;hours= 20;
    dayOfWeek = 7; 
    dayOfMonth = 20;
    month = 7;
    year = 24;

    rtc.setTime(seconds, minutes, hours, dayOfWeek, dayOfMonth, month, year);
}

void setAlarm1(DS3231& rtc){
    cout << "Setting alarm 1 for 1 minute" << endl;
    rtc.setAlarm1(0,1,0);
    cout << "Alarm 1 set" <<endl;
}

void setAlarm2(DS3231& rtc){
    cout<< "Setting Alarm 2 for 2 mintues"<<endl;
    rtc.setAlarm2(2,0);
    cout << "Alarm 2 set" << endl;
}

void displayTemperature(DS3231& rtc){
	rtc.printTemperature();
}

void displaySetTime(DS3231& rtc) {
    rtc.printTime();
}

int main(){
	DS3231 rtc(1, 0x68);
	char option;
	cout << "Select an option: \n1. Display current time\n2. Set current time\n3. Set Alarm 1\n4. Set Alarm 2\n5. Display Temperature\nq. Quit"<<endl;
	while (true) {
        cin>>option;
	if (option == 'q') break;
	
	switch (option){
		case '1':
			displayCurrentTime(rtc);
			break;
		case '2':
			setCurrentTime(rtc);
			break;
		case '3':
			setAlarm1(rtc);
			break;
		case '4':
			setAlarm2(rtc);
			break;
		case '5':
			displayTemperature(rtc);
			break;
		default:
			cout << "Invalid choice. Please try again.\n";
		}
    }
	running = false;
return 0;
}

