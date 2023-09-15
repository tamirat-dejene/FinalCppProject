//question.h
#ifndef QUESTION_H
#define QUESTION_H
#include "user.h"
#include <string>
#include <vector>
using namespace std;

class Question : public User {
private:
	vector <string> question;
	vector <string> choice;
	vector <char> correctAns;
	vector <char> userAns;
	int score = 0, numofQ;
	double timeElapsed;
public:
	char chooseCategory();
	int difficultyLevel();
	void readQuestion(char, int);
	void displayQuestion();
	void saveScore(string, char, int);
	int getScore();
	void freeMemory();
	void evaluateScore();
	void addQuestion();
};
#endif
