#include <iostream>
#include <fstream>
#include <ctime>
#include <chrono>
#include <string>

using namespace std;
using namespace chrono;

//Fri Nov 11 16:30:22 2022
int main() {
	time_t seconds = system_clock::to_time_t(system_clock::now());
	char dateTxt[30];
	
	ctime_s(dateTxt,sizeof(dateTxt),&seconds);
	
	string dayTxt(dateTxt, 8, 2);
	int day = atoi(dayTxt.c_str());
	
	string yearTxt(dateTxt, 20, 4);
	int year = atoi(yearTxt.c_str());

	string monthTxt(dateTxt, 4, 3);
	int month;
	if (monthTxt == "Jan") month = 1;
	else if (monthTxt == "Feb") month = 2;
	else if (monthTxt == "Mar") month = 3;
	else if (monthTxt == "Apr") month = 4;
	else if (monthTxt == "May") month = 5;
	else if (monthTxt == "Jun") month = 6;
	else if (monthTxt == "Jul") month = 7;
	else if (monthTxt == "Aug") month = 8;
	else if (monthTxt == "Sep") month = 9;
	else if (monthTxt == "Oct") month = 10;
	else if (monthTxt == "Nov") month = 11;
	else month = 12;


	fstream file;
	file.open("data.txt");
	if (file.fail()) {
		cout << "Error: could not open the file!" << endl;
		cout << "Enter any key to continue...";
		char tmp;
		cin >> tmp;

		return -1;
	}

	string line = "";

	while (getline(file, line)) {
		int commaPos = line.find(',', 0);
		string name = line.substr(0, commaPos);
		string bDayTxt = line.substr(commaPos + 1, 10);

		int bMonth = atoi(bDayTxt.substr(0, 2).c_str());
		int bDay = atoi(bDayTxt.substr(3, 2).c_str());
		int bYear = atoi(bDayTxt.substr(6, 4).c_str());

		if (bMonth == month && bDay == day) {
			cout << "It is " << name << "'s birthday!" << endl;
			cout << name << " is turning " << year - bYear << endl;
			cout << "Enter a key to continue: ";
			char tmp;
			cin >> tmp;
		}
	}


	return 0;
}