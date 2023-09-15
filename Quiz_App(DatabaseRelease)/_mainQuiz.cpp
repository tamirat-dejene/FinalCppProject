#include <iostream>
#include <string>
#include <fstream>
#include <vector>
#include <chrono>


#include <mysql_driver.h>
#include <mysql_connection.h>
#include <cppconn/exception.h>
#include <cppconn/prepared_statement.h>
#include <cppconn/resultset.h>

using namespace std;
using namespace sql;
using namespace mysql;

const char* server = "tcp://127.0.0.1:3306";
const char* username = "root";
const char* password = "tamirat.mySQL";
const char* database = "quiz_application_db";  // Name of the database to be created

MySQL_Driver* driver;
Connection* con;
Statement* stmt;
ResultSet* result;
PreparedStatement* pstmt;

class User {
private:
    string id_number,
        first_name,
        last_name,
        email_address,
        user_name,
        password = "";
public:
    void create_new_user();
    void log_in();
    bool check_password(string);
    void encrypt_password(string&);
    void reset_password();
    void delete_account();
    string getId();
};
class Question : public User {
private:
    string category, level;
    vector<string> quest, choice_a, choice_b, choice_c, choice_d;
    vector<char> correct_answer;
    vector<int> question_id, question_number;
public:
    char show_main_menu();
    void store_question();
    void write_to_the_database();
    void prprdstmt_insertion(ifstream&, string, string);
    void fetch_question();
    void display_question(int);
    /*void create_scoreb();
    void save_sb();
    void reset_sb();
    void display_sb();*/
};



int home_page();
char home_p2();
void about_us();
void exit_from_app();
char manage_account();

void connect_tomySQL() {
    try
    {
        driver = get_mysql_driver_instance();
        con = driver->connect(server, username, password);
        con->setSchema(database); // setting the schema to the new database
    }
    catch (sql::SQLException& e)
    {
        cout << "Could not connect to server. Error message: " << e.what() << endl;
        system("pause");
        exit(1);
    }
}
string User::getId() {
    return id_number;
}
void User::create_new_user() {
Home:   system("CLS");
    cout << "          Fill out the following form\n";
    cout << "+---------------------------------------------+\n";
    cout << "    First name: "; cin.ignore(); getline(cin, first_name);
    cout << "    Last name: "; getline(cin, last_name);
    cout << "    Email address: "; getline(cin, email_address);
    cout << "    ID number: "; getline(cin, id_number);
    cout << "    --- User name: "; getline(cin, user_name);
    cout << "    ---  Password: "; getline(cin, password);
    cout << "+---------------------------------------------+\n";
    encrypt_password(password);

    if (con->isValid()) {
        stmt = con->createStatement();
        stmt->execute("CREATE DATABASE IF NOT EXISTS " + string(database)); // Creating if didn't exist
        con->setSchema(database); // setting the schema to the new database

        vector<string> line;
        line.push_back("CREATE TABLE IF NOT EXISTS user ");
        line.push_back("(id_num VARCHAR(20) PRIMARY KEY, ");
        line.push_back("f_name VARCHAR(15) NOT NULL, ");
        line.push_back("l_name VARCHAR(15) NOT NULL, ");
        line.push_back("email VARCHAR(50) NOT NULL, ");
        line.push_back("user_name VARCHAR(25) NOT NULL UNIQUE, ");
        line.push_back("password VARCHAR(15) NOT NULL);");

        // Concatenate the SQL statements into a single string.
        std::string sqlCommand = "";
        for (string& sqlLine : line) {
            sqlCommand += sqlLine;
        }
        stmt->execute(sqlCommand); // Creating table user 
        line.clear();


        pstmt = con->prepareStatement("INSERT INTO user(id_num, f_name, l_name, email, user_name, password) VALUES(?,?,?,?,?,?)");
        pstmt->setString(1, id_number);
        pstmt->setString(2, first_name);
        pstmt->setString(3, last_name);
        pstmt->setString(4, email_address);
        pstmt->setString(5, user_name);
        pstmt->setString(6, password);

        try { 
            pstmt->execute(); 
            system("CLS");
            cout << "You have successfully created created your account.\n";
        }
        catch (sql::SQLException& e) {
            if (e.getErrorCode() == 1062) { // Duplicate username
                std::cout << "User/ID name already exists! Try again." << endl;
                goto Home;
            }
            else if (e.getErrorCode() == 1048 || e.getErrorCode() == 1138) { // Null Id or username
                cout << "You can't leave id/user name null! Try again" << endl;
                goto Home;
            }
            else {
                cout << "Database error: " << e.what() << "Try again!" << endl;
                goto Home;
            }
        }
        delete con;
    }
    else {
        cout << "System error!\n Press any key to retry!";
        system("pause>0");
        exit(1);
    }
}
void User::log_in() {
    system("CLS");
    cout << "Log in page\n";
    string runtime_pass, runtime_uname;
    cout << " ----- User name: "; cin.ignore(); getline(cin, runtime_uname);
    cout << " ----- Password : "; getline(cin, runtime_pass);
    encrypt_password(runtime_pass);

    stmt = con->createStatement();
    string queryUser = "SELECT * FROM user WHERE user_name = '" + runtime_uname + "';";
    try {
        result = stmt->executeQuery(queryUser);
        if (result->next()) {
            string fname, lname, idn, emaail, pass, usern;
            User::password = result->getString("password");
            if (check_password(runtime_pass)) {
                first_name = result->getString("f_name");
                last_name= result->getString("l_name");
                email_address = result->getString("email");
                User::id_number = result->getString("id_num");
                user_name = result->getString("user_name");
                User::password = runtime_pass;
                system("CLS");
                cout << "Logged in as ------ " << id_number << " -----\n";
            }
            else {
                system("CLS");
                cout << "Incorrect password!\n";
                User::password = "";
            }
        }
        else {
            system("CLS");
            std::cout << "Incorrect user name!" << endl;
            User::password = "";
        }
        delete stmt;
        delete result;
    }
    catch (sql::SQLException& e) {
        // Handle database error
        std::cerr << "Database error: " << e.what() << std::endl;
    }
}
void User::reset_password() {
    string passw;
    cout << "Enter current password: "; cin.ignore(); getline(cin, passw); encrypt_password(passw);
    if (check_password(passw)) {
        cout << "Enter new password: "; getline(cin, User::password);

        string queryUser = "UPDATE user SET password = '" + passw + "' WHERE id_num = '" + id_number + "';";

        stmt = con->createStatement();
        try {
            stmt->execute(queryUser);
            system("CLS");
            cout << "Password reset successful!";
        }
        catch (sql::SQLException& e) {
            std::cerr << "Database error: " << e.what() << endl;
            system("pause");
            exit(1);
        }
    }

}
void User::delete_account() {
    string confirmation;
    cout << "This action can't be undone. Write 'confirm' to confirm: "; cin.ignore(); getline(cin, confirmation);
    int x = 0;
    string key = "confirm";
    bool confirmed = true;
    while (x < 7 && confirmation.length() == 7) {
        if (key[x] != confirmation[x]) {
            confirmed = false;
        }
        ++x;
    }
    if (confirmed) {
        string queryUser = "DELETE FROM user WHERE id_num = '" + id_number + "';";

        stmt = con->createStatement();
        try {
            stmt->execute(queryUser);
            system("CLS");
            cout << "Account deleted!";
        }
        catch (sql::SQLException& e) {
            std::cerr << "Database error: " << e.what() << endl;
            system("pause");
            exit(1);
        }
    }
}
bool User::check_password(string pass) {
    bool length_check = false;
    bool char_check = true;
    if (pass.length() == password.length()) {
        length_check = true;
        for (int i = 0; i < pass.length(); ++i) {
            if (pass[i] != password[i]) {
                char_check = false;
                break;
            }
        } 
    }
    return (char_check && length_check);
}
void User::encrypt_password(string& p) {
    int i = 0;
    while (p[i] != '\0') {
        if (int(p[i]) <= 91)
            p[i] = char(int(p[i]) + 10);
        else
            p[i] = char(int(p[i]) - 10);
        ++i;
    }
}
void Question::store_question() {
    std::vector<std::string> line;
    line.push_back("CREATE TABLE IF NOT EXISTS question( ");

    line.push_back("quest_id INT PRIMARY KEY AUTO_INCREMENT, ");
    line.push_back("quest_num INT NOT NULL, ");
    line.push_back("category VARCHAR(25) NOT NULL check(category in('Science', 'Technology', 'General', 'Sport')), ");
    line.push_back("level VARCHAR(10) NOT NULL check(level in('Easy', 'Medium', 'Hard')), ");
    line.push_back("quest VARCHAR(200) NOT NULL, ");
    line.push_back("choice_a VARCHAR(400) NOT NULL, ");
    line.push_back("choice_b VARCHAR(400) NOT NULL, ");
    line.push_back("choice_c VARCHAR(400) NOT NULL, ");
    line.push_back("choice_d VARCHAR(400) NOT NULL, ");
    line.push_back("correct_ans CHAR(5) NOT NULL);");
    // Concatenate the SQL statements
    string sqlCommand = "";
    for (std::string& sqlLine : line) {
        sqlCommand += sqlLine;
    }

    stmt = con->createStatement();
    try {
        stmt->execute(sqlCommand);
        write_to_the_database();
    }
    catch (sql::SQLException& e) {
        std::cerr << "Database error: " << e.what() << endl;
        system("pause");
        exit(1);
    }
}
void Question::write_to_the_database() {
    ifstream qFile;
    category = "Science";
    level = "Easy"; qFile.open("questionBank/sciEasy.txt", ios::_Nocreate);
    prprdstmt_insertion(qFile, category, level);
    level = "Medium"; qFile.open("questionBank/sciMedium.txt", ios::_Nocreate);
    prprdstmt_insertion(qFile, category, level);
    level = "Hard"; qFile.open("questionBank/sciHard.txt", ios::_Nocreate);
    prprdstmt_insertion(qFile, category, level);

    category = "Technology";
    level = "Easy"; qFile.open("questionBank/techEasy.txt", ios::_Nocreate);
    prprdstmt_insertion(qFile, category, level);
    level = "Medium"; qFile.open("questionBank/techMedium.txt", ios::_Nocreate);
    prprdstmt_insertion(qFile, category, level);
    level = "Hard"; qFile.open("questionBank/techHard.txt", ios::_Nocreate);
    prprdstmt_insertion(qFile, category, level);

    category = "General";
    level = "Easy"; qFile.open("questionBank/genEasy.txt", ios::_Nocreate);
    prprdstmt_insertion(qFile, category, level);
    level = "Medium"; qFile.open("questionBank/genMedium.txt", ios::_Nocreate);
    prprdstmt_insertion(qFile, category, level);
    level = "Hard"; qFile.open("questionBank/genHard.txt", ios::_Nocreate);
    prprdstmt_insertion(qFile, category, level);

    category = "Sport";
    level = "Easy"; qFile.open("questionBank/spoEasy.txt", ios::_Nocreate);
    prprdstmt_insertion(qFile, category, level);
    level = "Medium"; qFile.open("questionBank/spoMedium.txt", ios::_Nocreate);
    prprdstmt_insertion(qFile, category, level);
    level = "Hard"; qFile.open("questionBank/spoHard.txt", ios::_Nocreate);
    prprdstmt_insertion(qFile, category, level);

}
void Question::prprdstmt_insertion(ifstream& qFile, string categ, string level) {
    // Inserting the questions into the database
    string sqlCommand1 = "INSERT INTO question(quest_num, category, level, quest, choice_a, choice_b, choice_c, choice_d, correct_ans)";
    string sqlCommand2 = " VALUES(?,?,?,?,?,?,?,?,?)";
    string command = sqlCommand1 + sqlCommand2;
    string strInput; char charInput;

    if (qFile.is_open()) {
        pstmt = con->prepareStatement(command);
        int quest_n = 1;
        while (!qFile.eof()) {
            pstmt->setInt(1, quest_n);
            pstmt->setString(2, category);
            pstmt->setString(3, level);
            getline(qFile, strInput); pstmt->setString(4, strInput);
            getline(qFile, strInput); pstmt->setString(5, strInput);
            getline(qFile, strInput); pstmt->setString(6, strInput);
            getline(qFile, strInput); pstmt->setString(7, strInput);
            getline(qFile, strInput); pstmt->setString(8, strInput);
            qFile.seekg(8 + qFile.tellg());
            qFile.get(charInput); pstmt->setString(9, to_string(charInput));
            getline(qFile, strInput);
            getline(qFile, strInput);
            ++quest_n;
            try {
                pstmt->execute();
            }
            catch (sql::SQLException& e) {
                cout << "Database error: " << e.what() << "Try again!" << endl;
            }
        }
        qFile.close();
    }
    else {
        cout << "Unsuccessful file opening!\n";
        system("pause");
        exit(1);
    }
}
void Question::fetch_question() {
home:   int num_of_questions;
    system("CLS");
    char cat, lev;
    cout << "+------ CATEGORY -------+\n";
    cout << "|    A. Science         |\n";
    cout << "|    B. Technology      |\n";
    cout << "|    C. General         |\n";
    cout << "|    D. Sport           |\n";
    cout << "+-----------------------+\n";
    cout << "+-- >> "; cin >> cat;
    system("CLS");
h2: cout << "+------ LEVEL -----+\n";
    cout << "|    A. Easy       |\n";
    cout << "|    B. Medium     |\n";
    cout << "|    C. Hard       |\n";
    cout << "+------------------+\n";
    cout << "+-- >> "; cin >> lev; lev = toupper(lev); cat = toupper(cat);
    string category, level;
    switch (cat) {
    case 'A': category = "Science"; break;
    case 'B': category = "Technology"; break;
    case 'C': category = "General"; break;
    case 'D': category = "Sport"; break;
    default: cout << "Invalid input! "; goto home;
    }
    switch (lev) {
    case 'A': level = "Easy"; break;
    case 'B': level = "Medium"; break;
    case 'C': level = "Hard"; break;
    default: cout << "Invalid input! "; goto h2;
    }

    cout << "+-- Number of question (<= 10) --+\n";
    cout << "+-- >> "; cin >> num_of_questions;
    stmt = con->createStatement();
    string query1 = "SELECT * FROM question WHERE quest_num <= " + to_string(num_of_questions) + " AND category = '" + category + "' AND level = '" + level + "';";

    string line; int n;

    try {
        result = stmt->executeQuery(query1);
        while (result->next()) {
            n = result->getInt("quest_num"); question_number.push_back(n);
            line = result->getString("quest"); quest.push_back(line);
            line = result->getString("choice_a"); choice_a.push_back(line);
            line = result->getString("choice_b"); choice_b.push_back(line);
            line = result->getString("choice_c"); choice_c.push_back(line);
            line = result->getString("choice_d"); choice_d.push_back(line);
            n = result->getInt("correct_ans"); correct_answer.push_back(char(n));
        }
        display_question(num_of_questions);
        delete stmt;
        delete result;
    }
    catch (sql::SQLException& e) {
        // Handle database errors
        std::cerr << "Database error: " << e.what() << std::endl;
    }
}
void Question::display_question(int noq) {
    system("pause");
    int j = 0;
    char userInput;
    int correct = 0;
    cout << "Let's go! The timer has started :)\n\n";
    auto start = std::chrono::high_resolution_clock::now();
    for (int i = 0; i < noq; ++i) {
        cout << question_number[i] << ". " << quest[i] << endl;
        cout << "  " << choice_a[j] << endl;
        cout << "  " << choice_b[j] << endl;
        cout << "  " << choice_c[j] << endl;
        cout << "  " << choice_d[j] << endl;
        ++j;
        cout << "Answer: "; cin >> userInput;
        if (toupper(userInput) == correct_answer[i]) {
            cout << "Correct!\n";
            ++correct;
        }
        else {
            cout << "Incorrect! Answer: " << correct_answer[i] << endl;
        }
        cout << "Press any key to continue \n";
        system("pause>0");
        system("CLS");
    }
    auto end = std::chrono::high_resolution_clock::now();
    chrono::duration<double> duration = end - start;
    double timeElapsed = duration.count();

    cout << "Score: " << correct << "/" << noq << endl;
    cout << "Time: " << timeElapsed << "secs" << endl;

    question_number.clear();
    quest.clear();
    choice_a.clear();
    choice_b.clear();
    choice_c.clear();
    choice_d.clear();
}
char Question::show_main_menu() {
    system("CLS");
    char ch;
    cout << "--------------------------------\n";
    cout << "       a. Take Quiz\n";
    cout << "       b. Show My Scoreboard\n";
    cout << "       c. Reset Scoreboard\n";
    cout << "       d. Back to home page  -- "; back: cin >> ch; ch = tolower(ch);
    if (ch != 'a' && ch != 'b' && ch != 'c' && ch != 'd') goto back;
    system("CLS");
    return ch;
}

int main()
{  
    connect_tomySQL();
    User U; Question Q;
Home: 
    switch (home_page()) {
    case 1:
        U.log_in();
        break;
    case 2:
        U.create_new_user();
        break;
    case 3:
        exit_from_app();
    default:
        break;
    }
    switch (home_p2()) {
    case 'a':
        goto Home;
        break;
    case 'b':
        break;
    case 'c':
        switch (manage_account()) {
        case 'a':
            U.reset_password();
            break;
        case 'b':
            U.delete_account();
            break;
        }
        break;
    case 'd':
        about_us();
        break;
    case 'e':
        exit_from_app();
    }
    switch (Q.show_main_menu()) {
    case 'a':
        Q.store_question();
        Q.fetch_question();
        break;
    case 'b':
        break;
    case 'c':
        break;
    case 'd':
        goto Home;

    }
    system("pause");
    return 0;
}
int home_page() {
    int choice;
    system("CLS");
    cout << "+--------------------------------------+\n";
    cout << "             Quiz Application           \n";
    cout << "+--------------------------------------+\n";
    cout << "               1. Log In   \n";
    cout << "               2. Sign Up  \n";
    cout << "               3. Exit     ";
a:  cin >> choice; 
    if (choice != 1 && choice != 2 && choice != 3) {
        cout << "INVALID INPUT RETRY: "; goto a;
    }
    return choice;
    system("pause>0");
}
char home_p2() {
    char ch;
    cout << "  a. Back to homepage\n";
    cout << "  b. Proceed to main menu\n";
    cout << "  c. Manage account\n";
    cout << "  d. About us\n";
    cout << "  e. Exit ----------- "; K: cin >> ch; ch = tolower(ch);
    if (ch != 'a' && ch != 'b' && ch != 'c' && ch != 'd' && ch != 'e') goto K;
    return ch;
}
void about_us() {
    system("CLS");
    cout << "+-------------------------------------------------+\n";
    cout << "           Developed by team Innov8\n";
    cout << "               Class of 2018/26\n";
    cout << "                  Section E\n";
    cout << "   Name                                 ID Number\n";
    cout << " Tamirat  Dejenie                      ETS1518/14\n";
    cout << " Tadios    Dejene                      ETS1522/14\n";
    cout << " Tebarek   Shemsu                      ETS1526/14\n";
    cout << " Yohannes Tigistu                      ETS1703/14\n";
    cout << "+-------------------------------------------------+";
}
void exit_from_app() {
    system("CLS");
    cout << "Thanks for using QuizApp\n";
    cout << "Press and key to proceed!";
    system("pause>0");
    exit(1);
}
char manage_account() {
    char choose;
    system("CLS");
    cout << "+----------------------+\n";
    cout << "   a. Reset password\n";
    cout << "   b. Delete account\n";
    cout << "+----------------------+> "; k: cin >> choose; choose = tolower(choose);
    if (choose != 'a' && choose != 'b') goto k;
    return choose;
}