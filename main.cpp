#include "DS3231.h"
#include <chrono>
#include <thread>
#include <iostream>
#include <iomanip>
#include <atomic>

using namespace std;

// Atomic boolean to control the displayCurrentTime loop
atomic<bool> running(true);

// Function to continuously display the current time
void displayCurrentTime(DS3231& rtc){
	while (true){
	rtc.printTime();	// Print the current time
	this_thread::sleep_for(chrono::seconds(1));	// Wait for 1 second
	// cout << "\033[2J\033[1;1H";	// Clear screen
	}
}

// Function to set the current time on the RTC
void setCurrentTime(DS3231& rtc) {
   uint8_t seconds, minutes, hours, dayOfWeek, dayOfMonth, month, year;

    seconds = 0;
    minutes = 00 ;hours= 14;
    dayOfWeek = 1; 
    dayOfMonth = 21;
    month = 7;
    year = 24;

    rtc.setTime(seconds, minutes, hours, dayOfWeek, dayOfMonth, month, year);
}


// Function to set Alarm 1 for 1 minute and check if it's triggered
void setAlarm1(DS3231& rtc){
    cout << "Setting alarm 1 for 1 minute" << endl;
    rtc.setAlarm1(0,1,0);	//Set the alarm for 1 minute
    cout << "Alarm 1 set" <<endl;
    this_thread::sleep_for(chrono::minutes(1));
    rtc.checkAlarms();	//check the status of alarm
}


// Function to set Alarm 2 for 2 minute and check if it's triggered
void setAlarm2(DS3231& rtc){
    cout<< "Setting Alarm 2 for 2 mintues"<<endl;
    rtc.setAlarm2(2,0);		//Set alarm for 2 minutes
    cout << "Alarm 2 set" << endl;
    this_thread::sleep_for(chrono::minutes(2));
    rtc.checkAlarms();	//check the status of alarm
}

//Function to display the current temperature from the RTC
void displayTemperature(DS3231& rtc){
	rtc.printTemperature();	//Print the temperature
}

//Function to display the currently set time on the RTC 
void displaySetTime(DS3231& rtc) {
    rtc.printTime();	//Print current time
}

//Function to set the square wave output frequency of the RTC
void setSquareWave(DS3231& rtc, uint8_t frequency){
    rtc.setSquareWave(frequency);	//Set the frequency
    cout<<"Square wave set to"<< static_cast<int>(frequency)<<"Hz"<<endl;
}

//Fucntion to display the eleapsed time since the RTC was last set (Novel function)
void displayElapsedTime(DS3231& rtc){
	rtc.printElapsedTime();	//Print the elapsed time
}

int main(){
	DS3231 rtc(1, 0x68);	// Create an instance of DS3231 with I2C bus 1 and address 0x68
	char option;
	cout << "Select an option: \n
	     	<<"1. Display current time\n"
		<<"2. Set current time\n"
		<<"3. Set Alarm 1\n"
		<<"4. Set Alarm 2\n"
		<<"5. Display Temperature\n"
		<<"q. Quit"<<endl;
	while (true) {
        cin>>option;	//Get the user Input
	if (option == 'q') break;	//Exit the loop if 'q' is entered
	
	switch (option){
		case '1':
			displayCurrentTime(rtc);	//Display current time
			break;
		case '2':
			setCurrentTime(rtc);	//Set the RTC time
			break;
		case '3':
			setAlarm1(rtc);	//Set alarm 2 and check its status
			break;
		case '4':
			setAlarm2(rtc);	//Set alarm 2 and check its status
			break;
		case '5':
			displayTemperature(rtc);	//Display temperature
			break;
		case '6':
			uint8_t frequency;
			cout<<"Enter frequency (1,2,4,8):";
			cin>>frequency;	//Get frequency input
			setSquareWave(rtc,frequency);	//Set the square wave frequency
			break;
		case '7':
			displayElapsedTime(rtc);	//Display elaapsed time
			break;
		default:
			cout << "Invalid choice. Please try again.\n";
		}
    }
	running = false;
return 0;
}

