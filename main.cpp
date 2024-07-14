#include "DS3231.h"
#include <chrono>
#include <thread>
#include <iostream>
#include <iomanip>

using namespace std;

void displayCurrentTime(DS3231& rtc){
	while (true){
	rtc.printTime();
	this_thread::sleep_for(chrono::seconds(1));
	cout << "\033[2J\033[1;1H";	
	}
}

void setCurrentTime(DS3231& rtc) {
    uint8_t seconds, minutes, hours, dayOfWeek, dayOfMonth, month, year;

    cout << "Enter seconds: ";
    cin >> seconds;
    cout << "Enter minutes: ";
    cin >> minutes;
    cout << "Enter hours: ";
    cin >> hours;
    cout << "Enter day of the week (1=Sun, 2=Mon, ... 7=Sat): ";
    cin >> dayOfWeek;
    cout << "Enter day of the month: ";
    cin >> dayOfMonth;
    cout << "Enter month: ";
    cin >> month;
    cout << "Enter year (last two digits): ";
    cin >> year;

    rtc.setTime(seconds, minutes, hours, dayOfWeek, dayOfMonth, month, year);
}

void displaySetTime(DS3231& rtc) {
    rtc.printTime();
}

int main(){
	DS3231 rtc(1, 0x68);
	int choice;
	while (true) {
        cout << "1. Display current time\n";
	cout << "2. Set current time\n";
	cout << "3. Display set time\n";
	cout << "4. Exit\n";
	cout << "Enter your choice";
	cin >> choice;
	
	switch (choice){
	case 1:
	displayCurrentTime(rtc);
	break;
	case 2:
	setCurrentTime(rtc);
	break;
	case 3:
	displaySetTime(rtc);
	break;
	case 4:
	return 0;
	default:
	cout << "Invalid choice. Please try again.\n";
	}
    }	
return 0;
}

