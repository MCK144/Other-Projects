#include <iostream>
#include <fstream>
#include <string>
#include <random>
#include <ctime>
#include <memory>

using namespace std;

// pricing constants
const int TROOP_PRICE = 1000;
const int SPECIAL_UNIT_PRICE = 1000;
const int ALTILLERY_UNIT_PRICE = 1000;
const int BOUNTY_HUNTER_PRICE = 1800;
const int SMUGGLER_PRICE = 500;

const int DAILY_INCOME = 1000;
const int PLANET_INCOME = 300;
const int PLANET_CONQUER_INCOME = 500;

//battle constants
const int TROOP_HEALTH = 100;
const int TROOP_MIN_ATTACK = 10;//20;
const int TROOP_MAX_ATTACK = 50;//50;

const int SPECIAL_UNIT_HEALTH = 300;
const int SPECIAL_UNIT_MIN_ATTACK = 40;
const int SPECIAL_UNIT_MAX_ATTACK = 70;

struct Soldier {
	int health;				
	int minAttack;
	int maxAttack;
	bool special;
};

void getCredits(fstream &file, int& rCredits, int& cCredits) {
	string line1, line2;

	getline(file, line1);
	getline(file, line2);

	int startPos = line1.find(':',0) + 2; // find the colon, skip the next space
	rCredits = atoi(line1.substr(startPos,line1.length()-1).c_str());
	
	startPos = line2.find(':', 0) + 2;
	cCredits = atoi(line2.substr(startPos, line2.length() - 1).c_str());
}
void setCredits(fstream& file, int& rCredits, int& cCredits) {	
	file << "Republic Credits: " << rCredits << "\n";
	file << "Confederacy Credits: " << cCredits << "\n";
		
}

void battle(mt19937& generator) {
	// battle details
	int numRTroops, numRSpecials, numCTroops, numCSpecials; 

	cout << "********** Battle Simulation *********\n\n";
	
	cout << "Enter the number of republic troops: ";
	cin >> numRTroops;

	cout << "\nEnter the number of republic special units: ";
	cin >> numRSpecials;

	cout << "\nEnter the number of confederate troops: ";
	cin >> numCTroops;

	cout << "\nEnter the number of confederate special units: ";
	cin >> numCSpecials;

	// create the republic troops
	Soldier *rTroops = new Soldier[numRTroops + numRSpecials];

	for (int i = 0; i < numRTroops; i++) {
		rTroops[i].health = TROOP_HEALTH;
		rTroops[i].minAttack = TROOP_MIN_ATTACK;
		rTroops[i].maxAttack = TROOP_MAX_ATTACK;
		rTroops[i].special = false;
	}
	for (int i = numRTroops; i < numRTroops + numRSpecials; i++) {
		rTroops[i].health = SPECIAL_UNIT_HEALTH;
		rTroops[i].minAttack = SPECIAL_UNIT_MIN_ATTACK;
		rTroops[i].maxAttack = SPECIAL_UNIT_MAX_ATTACK;
		rTroops[i].special = true;
	}

	// create the confederate troops
	Soldier* cTroops = new Soldier[numCTroops + numCSpecials];
	for (int i = 0; i < numCTroops; i++) {
		cTroops[i].health = TROOP_HEALTH;
		cTroops[i].minAttack = TROOP_MIN_ATTACK;
		cTroops[i].maxAttack = TROOP_MAX_ATTACK;
		cTroops[i].special = false;
	}
	for (int i = numCTroops; i < numCTroops + numCSpecials; i++) {
		cTroops[i].health = SPECIAL_UNIT_HEALTH;
		cTroops[i].minAttack = SPECIAL_UNIT_MIN_ATTACK;
		cTroops[i].maxAttack = SPECIAL_UNIT_MAX_ATTACK;
		cTroops[i].special = true;
	}

	numRTroops = numRTroops + numRSpecials;
	numCTroops = numCTroops + numCSpecials;

	int currRTroop = 0;
	int currCTroop = 0;
	int rTroopDeaths = 0;
	int cTroopDeaths = 0;
	int rSpecialDeaths = 0;
	int cSpecialDeaths = 0;


	while (numRTroops > 0 && numCTroops > 0) {
		Soldier& rTroop = rTroops[currRTroop];
		Soldier& cTroop = cTroops[currCTroop];

		uniform_int_distribution<int> rRandAttack(rTroop.minAttack, rTroop.maxAttack);
		uniform_int_distribution<int> cRandAttack(cTroop.minAttack, cTroop.maxAttack);

		int rAttack = rRandAttack(generator);
		int cAttack = cRandAttack(generator);

		rTroop.health -= cAttack;
		cTroop.health -= rAttack;

		if (rTroop.health <= 0) {
			currRTroop++;
			numRTroops--;
			if (rTroop.special) {
				rSpecialDeaths++;
			}
			else {
				rTroopDeaths++;
			}
		}
		if (cTroop.health <= 0) {
			currCTroop++;
			numCTroops--;
			if (cTroop.special) {
				cSpecialDeaths++;
			}
			else {
				cTroopDeaths++;
			}
		}
	}

	cout << "\n\n*********** Battle Outcome\n\n";
	if (numRTroops > numCTroops) {
		cout << "The Republic wins!" << endl;
	}
	else if (numCTroops > numRTroops) {
		cout << "The Confederacy wins!\n";
	}
	else {
		cout << "Tie!\n";
	}
	cout << "Republic troops remaining: " << numRTroops;
	cout << "\nRepublic troops killed: " << rTroopDeaths;
	cout << "\nRepublic special units destroyed: " << rSpecialDeaths;
	cout << "\n\nConfederate Troops remaining: " << numCTroops;
	cout << "\nConfederate troops killed: " << cTroopDeaths;
	cout << "\nConfederate special units destroyed: " << cSpecialDeaths;
	cout << "\n\n\n";
	
	delete[] rTroops;
	delete[] cTroops;
}

void republicTurn(mt19937 &generator, int& rCredits) {
	static uniform_int_distribution<int> eventSpawn(1, 20);
	static uniform_int_distribution<int> eventType(1, 10);

	bool quit = false;
	int planets;
	int income;

	cout << "Enter the number of Republic controlled systems (-1 to skip income): ";
	cin >> planets;
	if (cin.fail()) { cin.clear(); cin.ignore(100, 'n'); }
	
	if (planets != -1) {
		income = DAILY_INCOME + PLANET_INCOME * planets;
		cout << "\nCredits earned = " << DAILY_INCOME << " + "
			<< PLANET_INCOME << " * " << planets << " planets = " << income;
		rCredits += income;
	}

	do {
		cout << "\n\n\tTotal Republic Credits: " << rCredits << endl;

		cout << "Republic's turn\n";
		cout << "1) Buy troops (1000 credits =  10 troops)\n";
		cout << "2) Buy special units (2000 credits = 1 special unit)\n";
		cout << "3) Buy artillery units (1500 credits = 1 artillery unit)\n";
		cout << "4) Hire bounty hunter (1500 credits)\n";
		cout << "5) Hire smuggler (500 credits)\n";
		cout << "6) Fight battle\n";
		cout << "7) Conquer planet\n";
		cout << "8) End turn...\n\n\n";

		int choice = 0;
		cin >> choice;
		if (cin.fail()) {
			cin.clear();
			cin.ignore(100, '\n');
		}

		switch (choice) {
		case 1:
			cout << "Troops Enlisted\n";
			rCredits -= TROOP_PRICE;;
			break;
		case 2:
			cout << "Special Unit enlisted\n";
			rCredits -= SPECIAL_UNIT_PRICE;
			break;
		case 3:
			cout << "Altillery unit purchased\n";
			rCredits -= ALTILLERY_UNIT_PRICE;
			break;
		case 4:
			cout << "Bounty hunter hired\n";
			rCredits -= BOUNTY_HUNTER_PRICE;
			break;
		case 5:
			cout << "Smuggler hired\n";
			rCredits -= SMUGGLER_PRICE;
			break;
		case 6:
			battle(generator);
			break;
		case 7:
			cout << "Planet conquered. Credits earned: " << PLANET_CONQUER_INCOME << endl;
			rCredits += PLANET_CONQUER_INCOME;
			break;
		default:
			quit = true;
			break;
		}

		//random event
		int num = eventSpawn(generator);
		if (num == 10) {
			num = eventType(generator);
			if (num < 6) {
				cout << "\n!!!! Event - Pirate Attack !!!!\n";
			}
			else {
				cout << "\n!!!! Event - People revolt !!!!\n";
			}
		}

	} while (quit == false);

	
}

void confederacyTurn(mt19937 &generator,int& cCredits) {
	static uniform_int_distribution<int> eventSpawn(1, 20);
	static uniform_int_distribution<int> eventType(1, 10);

	bool quit = false;
	int planets;
	int income;

	cout << "Enter the number of Confederate controlled systems (-1 to skip income): ";
	cin >> planets;
	if (cin.fail()) { cin.clear(); cin.ignore(100, 'n'); }

	if (planets != -1) {
		income = DAILY_INCOME + PLANET_INCOME * planets;
		cout << "\nCredits earned = " << DAILY_INCOME << " + "
			<< PLANET_INCOME << " * " << planets << " planets = " << income;
		cCredits += income;
	}

	do {
		cout << "\n\n\tTotal Confederate Credits: " << cCredits << endl;

		cout << "Republic's turn\n";
		cout << "1) Buy troops (1000 credits =  10 troops)\n";
		cout << "2) Buy special units (2000 credits = 1 special unit)\n";
		cout << "3) Buy artillery units (1500 credits = 1 artillery unit)\n";
		cout << "4) Hire bounty hunter (1500 credits)\n";
		cout << "5) Hire smuggler (500 credits)\n";
		cout << "6) Fight battle\n";
		cout << "7) Conquer planet\n";
		cout << "8) End turn...\n\n\n";

		int choice = 0;
		cin >> choice;
		if (cin.fail()) {
			cin.clear();
			cin.ignore(100, '\n');
		}

		switch (choice) {
		case 1:
			cout << "Troops Enlisted\n";
			cCredits -= TROOP_PRICE;;
			break;
		case 2:
			cout << "Special Unit enlisted\n";
			cCredits -= SPECIAL_UNIT_PRICE;
			break;
		case 3:
			cout << "Altillery unit purchased\n";
			cCredits -= ALTILLERY_UNIT_PRICE;
			break;
		case 4:
			cout << "Bounty hunter hired\n";
			cCredits -= BOUNTY_HUNTER_PRICE;
			break;
		case 5:
			cout << "Smuggler hired\n";
			cCredits -= SMUGGLER_PRICE;
			break;
		case 6:
			battle(generator);
			break;
		case 7:
			cout << "Planet conquered. Credits earned: " << PLANET_CONQUER_INCOME << endl;
			cCredits += PLANET_CONQUER_INCOME;
			break;
		default:
			quit = true;
			break;
		}

		//random event
		int num = eventSpawn(generator);
		if (num == 10) {
			num = eventType(generator);
			if (num < 6) {
				cout << "\n!!!! Event - Pirate Attack !!!!\n";
			}
			else {
				cout << "\n!!!! Event - People revolt !!!!\n";
			}
		}

	} while (quit == false);


}

int main() {

	// random generator
	static random_device::result_type seed = random_device()();
	static mt19937 generator(seed);

	int rCredits = 0;
	int cCredits = 0;

	fstream file;
	file.open("save data.txt");

	if (file.fail()) {
		cout << "File could not open..." << endl;
	}

	getCredits(file, rCredits, cCredits);
	file.close();

	
	// Game start
	cout << "***************************************************\n";
	cout << "                     Matt's Empire at War\n";
	cout << "***************************************************\n\n";
	
	bool quit = false;
	do {
		republicTurn(generator,rCredits);
		confederacyTurn(generator, cCredits);

		//check for the end of the game
		char tmp;
		cout << "Continue? (y / n )";
		cin >> tmp;
		// standard cin error checking
		if (cin.fail()) { cin.clear(); cin.ignore(100, '\n'); }

		if (toupper(tmp) == 'N') {
			quit = true;
		}

	} while (quit == false);


	cout << "...Saving credits...\n\n";
	file.open("save data.txt");
	setCredits(file, rCredits, cCredits);
	file.close();

	return 0;
}