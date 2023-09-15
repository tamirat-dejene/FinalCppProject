//user.h
#ifndef USER_H
#define USER_H
#include <string>
class User {
private:
	std::string full_name, email, user_name, typeof_User;
	char password[5];
public:
	int showMenu();
	void signIn();
	void signUp();
	void saveSignUpInfo();
	bool checkLoginInfo(std::string, char[]);
	void scoreBoard();
	void resetScoreboard();
	void deleteAccount();

	std::string fullName();
	std::string getEmail();
	std::string getUsername();
	char* getPassword();
	char moreOption();

	bool checkPasswordValidity(char[]);
	void encryptPassword(char[]);
};
#endif