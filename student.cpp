
#include "user.cpp"
#include <iostream>
#include <string>
#include <vector>
#include <regex>
#include <cctype>
#include <limits>
using namespace std;

class Student : public User {
private:
    string studentID;
    static vector<Student> students;

    
static bool validID(const string& id) {
    if (id.empty()) return false;

    for (unsigned char c : id) {
        if (!isdigit(c)) {
            return false;
        }
    }

    return true;
}


    static bool validName(const string& name) {
        if (name.empty()) return false;

        bool hasLetter = false;

        for (unsigned char c : name) {
            if (isalpha(c)) {
                hasLetter = true;
            } else if (!isspace(c) && c != '-' && c != '\'') {
                return false;
            }
        }
        return hasLetter;
    }

    static bool validEmail(const string& email) {
        regex pattern(
            R"(^[A-Za-z0-9_%+\-]+(\.[A-Za-z0-9_%+\-]+)*@[A-Za-z0-9-]+(\.[A-Za-z0-9-]+)+$)"
        );
        return regex_match(email, pattern);
    }

    static bool validPassword(const string& password) {
        if (password.length() < 8) return false;

        bool upper = false;
        bool lower = false;
        bool digit = false;

        for (unsigned char c : password) {
            if (isupper(c)) upper = true;
            if (islower(c)) lower = true;
            if (isdigit(c)) digit = true;
        }

        return upper && lower && digit;
    }

public:
    Student() = default;

    Student(string id, string n, string e, string p)
        : User(n, e, p), studentID(id) {}

    string getStudentID() const {
        return studentID;
    }

    static const Student* findStudent(const string& id) {
        for (const Student& s : students) {
            if (s.getStudentID() == id) {
                return &s;
            }
        }
        return nullptr;
    }

static void displayAllStudents() {
    if (students.empty()) {
        cout << "No students registered!\n";
        return;
    }

    cout << "\n--- All Registered Students ---\n";

    for (const Student& s : students) {
        cout << "Student ID: " << s.getStudentID() << endl;
        cout << "Name: " << s.getName() << endl;
        cout << "Email: " << s.getEmail() << endl;
        cout << "--------------------------\n";
    }
}


    void createAccount() {
        string id, name, email, password;

        cout << "\n--- Student Registration ---\n";

        while (true) {
            cout << "Enter Student ID: ";
            if (!getline(cin, id)) return;

            
        if (!validID(id)) {
            cout << "Invalid Student ID! Numbers only (0-9).\n";
        }
        else if (findStudent(id)) {
                cout << "Student ID already exists!\n";
            } else {
                break;
            }
        }

        while (true) {
            cout << "Enter Name: ";
            if (!getline(cin, name)) return;

            if (validName(name)) break;

            cout << "Invalid name! Use letters, not numbers.\n";
        }

        while (true) {
            cout << "Enter Email: ";
            if (!getline(cin, email)) return;

            if (!validEmail(email)) {
                cout << "Invalid email! Example: nisha@gmail.com\n";
                continue;
            }

            bool duplicate = false;

            for (const Student& s : students) {
                if (s.getEmail() == email) {
                    duplicate = true;
                    break;
                }
            }

            if (duplicate) {
                cout << "Email already registered!\n";
            } else {
                break;
            }
        }

        while (true) {
            cout << "Enter Password: ";
            if (!getline(cin, password)) return;

            if (validPassword(password)) break;

            cout << "Password needs at least 8 characters, "
                    "uppercase, lowercase and a number.\n";
        }

        students.emplace_back(id, name, email, password);

        cout << "\nAccount created successfully!\n";
    }

    bool login(string& loggedInID) {
        string id, password;
        loggedInID.clear();

        cout << "\n--- Student Login ---\n";

        cout << "Enter Student ID: ";
        if (!getline(cin, id)) return false;

        cout << "Enter Password: ";
        if (!getline(cin, password)) return false;

        const Student* s = findStudent(id);

        if (s && s->checkPassword(password)) {
            loggedInID = id;
            cout << "Login successful! Welcome, "
                 << s->getName() << "!\n";
            return true;
        }

        cout << "Invalid Student ID or Password!\n";
        return false;
    }

    bool login() {
        string id;
        return login(id);
    }

    void displayProfile() const override {
        cout << "\n--- Student Profile ---\n";
        cout << "Student ID: " << studentID << endl;
        User::displayProfile();
    }
};

vector<Student> Student::students;

int main() {
    Student student;
    string loggedInID;
    int choice;

    while (true) {
        cout << "\n--- Student Management ---\n";
        cout << "1. Create Account\n";
        cout << "2. Login\n";
        cout << "3. View My Profile\n";
        cout << "4. Logout\n";
        cout << "5. Exit\n";
        cout << "Enter choice: ";

        if (!(cin >> choice)) {
            if (cin.eof()) break;

            cin.clear();
            cin.ignore(numeric_limits<streamsize>::max(), '\n');

            cout << "Invalid choice! Enter a number.\n";
            continue;
        }

        cin.ignore(numeric_limits<streamsize>::max(), '\n');

        switch (choice) {
            case 1:
                student.createAccount();
                break;

            case 2:
                student.login(loggedInID);
                break;

            case 3: {
                if (loggedInID.empty()) {
                    cout << "Please login first!\n";
                    break;
                }

                const Student* s = Student::findStudent(loggedInID);

                if (s) {
                    s->displayProfile();
                } else {
                    cout << "Student not found!\n";
                }
                break;
            }

            case 4:
                loggedInID.clear();
                cout << "Logged out successfully!\n";
                break;

            case 5:
                cout << "Goodbye!\n";
                return 0;

            default:
                cout << "Invalid menu choice!\n";
        }
    }

    return 0;
}
