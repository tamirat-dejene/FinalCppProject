#include <iostream>
#include <fstream>
#include <string>
#include <cstdlib>
#include <iomanip>
#include "user.h"
#include "question.h"
using namespace std;

int main() {
H:	User U; Question Q;
	switch (U.showMenu()) {
	case 1:
		U.signIn();
		break;
	case 2:
		U.signUp();
		U.saveSignUpInfo();
		U.signIn();
		break;
	}

	char category = Q.chooseCategory();
	if (category != 'F') {
		int difficultyL = Q.difficultyLevel();
		Q.readQuestion(category, difficultyL);
		Q.displayQuestion();

		string userFile = U.getUsername();
		Q.saveScore(userFile, category, difficultyL);
		Q.evaluateScore();
		Q.freeMemory();
	}
	switch (U.moreOption()) {
	case 'A':
		Q.addQuestion();
		cout << "Press any key to exit.";
		system("pause>0"); 
		exit(1);
	case 'B':
		U.scoreBoard();
		break;
	case 'C':
		system("CLS");
		goto H;
		break;
	case 'D':
		U.resetScoreboard();
		break;
	case 'E':
		U.deleteAccount();
		break;
	case 'F':
		system("CLS");
		cout << "Thanks for using our app!\n Press any key to exit.";
		system("pause>0");
		exit(1);
	}
	system("pause");
	return 0;
}