#include "user.h"
#include <string>
#include <cstdlib>
#include <iostream>
#include <fstream>
#include <vector>
using namespace std;
int User::showMenu() {
	int choice;
	cout << "---------------------------------------" << endl;
	cout << "----- Welcome to Quiz Application -----" << endl;
	cout << "---------------------------------------" << endl;
	cout << "              1. Log in" << endl;
	cout << "              2. Sign up  "; cin >> choice;
	a:
	if (!(choice == 1 || choice == 2)) {
		cout << "Please enter a valid input: "; cin >> choice; goto a;
	}
	return choice;
}
void User::signIn() {
	string userName; char securityKey[5];
re:	cout << "   User name: "; cin.ignore(); getline(cin, userName);
	cout << "   Password : "; cin.get(securityKey, 5);
	encryptPassword(securityKey);

	bool check = checkLoginInfo(userName, securityKey);
	if (!check) {
		cout << "Incorrect username or password. Retry\n";
		goto re;
	}
	else {
		ifstream inf;
		string fileN = "userInfo/" + userName + ".txt";
		inf.open(fileN);
		if (inf.is_open()) {
			inf.seekg(6);
			getline(inf, full_name);
			inf.seekg(7 + inf.tellg());
			getline(inf, email);
			inf.seekg(10 + inf.tellg());
			inf.get(password, 5);
			inf.seekg(8 + inf.tellg());
			getline(inf, typeof_User);
			inf.close();
		}
		user_name = userName;
		system("CLS");
		cout << "Logged in as ----  " << user_name << "  ----\n";
	}
}
bool User::checkLoginInfo(string userN, char pass[]) {
	string fileName = "userInfo/" + userN + ".txt";
	ifstream readData;
	string name, Email, type;
	char password[5] = "";
	//User U2;
	readData.open(fileName);

	if (readData.is_open()) {
		readData.seekg(6);
		getline(readData, name);
		readData.seekg(7 + readData.tellg());
		getline(readData, Email);
		readData.seekg(10 + readData.tellg());
		readData.get(password, 5);
		readData.seekg(8 + readData.tellg());
		getline(readData, type);
		readData.close();
	}
	bool passwordIsCorrect = true;
	for (int i = 0; i < 4; ++i) {
		if (password[i] != pass[i]) {
			passwordIsCorrect = false;
			break;
		}
	}
	return passwordIsCorrect;
}
void User::signUp() {
	string fn, ln;
	cout << "Well :):\n";
	cout << "  First name         : "; cin.ignore(); getline(cin, fn);
	cout << "  Last name          : "; getline(cin, ln);
	full_name = fn + " " + ln;
	cout << "  E-mail address     : "; getline(cin, email);
	cout << "  Create user name   : "; getline(cin, user_name);
	cout << "  Password (4 char)  : ";

p: cin.get(password, 5);

	if (checkPasswordValidity(password))
		encryptPassword(password);
	else {
		cout << "Please enter a valid password: "; goto p;
	}
}
void User::saveSignUpInfo() {
	char choice;
	system("CLS");
	cout << "   ----- Sign-up as -----" << endl;
	cout << "         a. Student" << endl;
	cout << "         b. Teacher" << endl;
	cout << "         c. Other" << endl;
	cout << "*-*-*--> "; cin >> choice;

	system("CLS");
	string fileName = "userInfo/" + user_name + ".txt";

	ofstream newUser;
	newUser.open(fileName);

	if (newUser.is_open()) {
		newUser << "Name: " << full_name << endl;
		newUser << "Email: " << email << endl;
		newUser << "Password: " << password << endl;
		switch (choice) {
		case 'a':
			newUser << "Type: Student\n"; break;
		case 'b':
			newUser << "Type: Teacher\n"; break;
		case 'c':
			newUser << "Type: Other\n" << endl;
		}
		newUser.close();
		system("CLS");
		cout << "--------You have succesfully created your account!--------" << endl;
		cout << "      a. Back to log in page\n"
			<< "      b. Exit -------------> "; cin >> choice;
		switch (choice) {
		case 'a':
			break;
		case 'b': {
			cout << "Thanks for registering.\n";
			system("pause");
			exit(1);
		}
		}
	}
	else {
		cout << "Operation Unsuccessful! Retry later.\n";
		system("pause");
		exit(1);
	}
}
bool User::checkPasswordValidity(char pass[]) {
	bool isCorrect = true;
	int i = 0;
	while (pass[i] != '\0')
		++i;
	if (i != 4)
		isCorrect = false;
	return isCorrect;
}
void User::encryptPassword(char pass[]) {
	int i = 0;
	while (pass[i] != '\0') {
		if (int(pass[i]) <= 91)
			pass[i] = char(int(pass[i]) + 10);
		else
			pass[i] = char(int(pass[i]) - 10);
		++i;
	}
}
string User::fullName() {
	return full_name;
}
string User::getEmail() {
	return email;
}
string User::getUsername() {
	return user_name;
}
char* User::getPassword() {
	return password;
}
void User::resetScoreboard() {
	string filePath = "userInfo/" + user_name + ".txt";
	vector<string> lines;
	string line;

	ifstream infile;
	infile.open(filePath);
	if (infile.is_open()) {
		while (getline(infile, line)) {
			lines.push_back(line);
		}
		infile.close();
	}

	const int start_l = 5;
	const int end_l = lines.size();

	ofstream outFile(filePath, ios::out | ios::trunc);
	if (outFile.is_open()) {
		for (int i = 0; i < start_l - 1; ++i)
		{
			outFile << lines[i] << endl;
		}
		outFile.close();
	}
	system("CLS");
	cout << "Erasure successful!\n";
	cout << " Press any key to exit.";
	system("pause>0");
	exit(1);
}
char User::moreOption() {
	char moreChoice;
	cout << "Press any key to proceed."; system("pause>0");
	system("CLS");
	cout << "More options :)\n";
	cout << "  a. Add own question\n"
		<< "  b. Show my scoreboard\n"
		<< "  c. Back to homepage\n"
		<< "  d. Reset score board\n"
		<< "  e. Delete Account\n"
		<< "  f. Exit --- >> ";  k: cin >> moreChoice;
	if (moreChoice == 'a' || moreChoice == 'b' || moreChoice == 'c' || moreChoice == 'd' || moreChoice == 'e' || moreChoice == 'f') {
		moreChoice = toupper(moreChoice);
		system("CLS");
		return moreChoice;
	}
	else {
		system("CLS");
		cout << "Please enter valid input: "; goto k;
	}

}
void User::scoreBoard() {
	string username = getUsername();
	ifstream scoreOut;
	string score;
	string fileName = "userInfo/" + username + ".txt";
	scoreOut.open(fileName);
	int line = 1;
	if (scoreOut.is_open()) {
		while (!scoreOut.eof() && getline(scoreOut, score)) {
			if (line > 5)
				cout << score << endl;
			++line;
		}
		scoreOut.close();
	}
}
void User::deleteAccount() {
	char decision;
	cout << "Are you sure? You can't undo this. (y/n) "; cin >> decision;
	if (decision == 'y' || decision == 'Y') {
		string userFile = "userInfo/" + user_name + ".txt";
		if (remove(userFile.c_str()) != 0) {
			perror("Error deleting the account! Retry Later.");
			exit(1);
		}
		else {
			cout << "Succesfully deleted!\n";
			cout << "Press any key to exit :) "; system("pause>0");
			exit(1);
		}
	}
}