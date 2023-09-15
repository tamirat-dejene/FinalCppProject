#include "question.h"
#include "user.h"
#include <iostream>
#include <fstream>
#include <chrono>
using namespace std;
char Question::chooseCategory() {
	char ch;
	cout << "--- Choose type of the questions ---" << endl;
	cout << "      a - Science related\n"
		<< "      b - General knowledge related\n"
		<< "      c - Technology related\n"
		<< "      d - Sport related\n"
		<< "      e - Do some random questions\n"
		<< "      f - More options   ---- >> "; cin >> ch;
k:	ch = toupper(ch);
    if (!(ch == 'A' || ch == 'B' || ch == 'C' || ch == 'D' || ch == 'E' || ch == 'F')) {
	system("CLS");
	cout << "Enter valid input: "; cin >> ch;
	goto k;
}
system("CLS");
return ch;
}
int Question::difficultyLevel() {
	int d;
	cout << " --- Choose difficulty level ---\n";
	cout << "   1 - Easy\n"
		<< "   2 - Medium\n"
		<< "   3 - Difficult --- > "; cin >> d;
l:   if (!(d == 1 || d == 2 || d == 3)) {
	system("CLS");
	cout << "Enter valid input: "; cin >> d;
	goto l;
}
system("CLS");
return d;
}
void Question::readQuestion(char c, int d) {
	ifstream qFile;
	c = tolower(c);
	switch (c) {
	case 'a':
		if (d == 1) qFile.open("questionBank/sciEasy.txt");
		else if (d == 2) qFile.open("questionBank/sciMedium.txt");
		else qFile.open("questionBank/sciHard.txt");
		break;
	case 'b':
		if (d == 1) qFile.open("questionBank/genEasy.txt");
		else if (d == 2) qFile.open("questionBank/genMedium.txt");
		else qFile.open("questionBank/genHard.txt");
		break;
	case 'c':
		if (d == 1) qFile.open("questionBank/techEasy.txt");
		else if (d == 2) qFile.open("questionBank/techMedium.txt");
		else qFile.open("questionBank/techHard.txt");
		break;
	case 'd':
		if (d == 1) qFile.open("questionBank/spoEasy.txt");
		else if (d == 2) qFile.open("questionBank/spoMedium.txt");
		else qFile.open("questionBank/spoHard.txt"); break;
	case 'e': qFile.open("questionBank/randomQuest.txt");
		break;
	}
	string strInput; char charInput;
	int i = 0;
	if (qFile.is_open()) {
		while (!qFile.eof()) {
			getline(qFile, strInput); question.push_back(strInput);
			while (i < 4) {
				getline(qFile, strInput); choice.push_back(strInput);
				++i;
			}
			i = 0;
			qFile.seekg(8 + qFile.tellg());
			qFile.get(charInput); correctAns.push_back(charInput); getline(qFile, strInput);
			getline(qFile, strInput);
		}
		qFile.close();
	}
	else
		exit(1);
}
void Question::displayQuestion() {
	int numofchoice = 0, totalChoice = 0;
	char userInput;
	cout << "  How many questions do you want to enjoy( < 10): "; cin >> numofQ;
	system("CLS");
	cout << "Let's go! The timer has started :)\n\n";
	auto start = std::chrono::high_resolution_clock::now();
	for (int i = 0; i < numofQ; ++i) {
		cout << i + 1 << ". " << question[i] << endl;
		while (numofchoice < 4) {
			cout << choice[totalChoice] << endl;
			++totalChoice, ++numofchoice;
		}
		numofchoice = 0;

		cout << "--- "; cin >> userInput; userAns.push_back(toupper(userInput));
		if (userAns[i] == correctAns[i]) {
			cout << "Correct! \n";
			++score;
		}
		else
			cout << "Incorrect! Correct ans: " << correctAns[i] << endl;
		system("pause");
		system("CLS");
	}
	auto end = std::chrono::high_resolution_clock::now();
	chrono::duration<double> duration = end - start;
	timeElapsed = duration.count();
}
int Question::getScore() {
	return score;
}
void Question::saveScore(string user, char questionCategory, int difficultyLevel) {
	ofstream userf;
	user = "userInfo/" + user + ".txt";
	userf.open(user, ios::app);
	if (userf.is_open()) {
		userf << "\nCat - " << questionCategory
			<< " : D-level - " << difficultyLevel
			<< " : Score - " << score << "/" << numofQ
			<< " : Time - " << timeElapsed << " sec";
		userf.close();
	}
}
void Question::freeMemory() {
	question.clear();
	choice.clear();
	userAns.clear();
	correctAns.clear();
}
void Question::evaluateScore() {
	cout << "Time elapsed: " << timeElapsed << " seconds\n";
	double percentageScore = ((double)score / numofQ) * 100;
	if (percentageScore >= 75)
		cout << "Excellent! You scored " << percentageScore << "% ";
	else if (percentageScore >= 50)
		cout << "Good! You scored " << percentageScore << "% ";
	else if (percentageScore >= 25)
		cout << "Try hard! You scored " << percentageScore << "% ";
	else
		cout << "Poor! You scored: " << percentageScore << "% ";
	cout << " or " << score << "/" << numofQ << endl;
}
void Question::addQuestion() {
	system("CLS");
	cout << "Subscribe for this service: only $20.00/year.\n";
}