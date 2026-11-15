
#include <iostream>
#include "Student.h"

int main() {
    Student student;

    int choice;

    do {
        std::cout << "\n--- Student Management ---\n";
        std::cout << "1. Create Account\n";
        std::cout << "2. Login\n";
        std::cout << "3. View Student Profile\n";
        std::cout << "4. Exit\n";
        std::cout << "Enter choice: ";

        if (!(std::cin >> choice)) {
            std::cout << "Invalid input!\n";
            break;
        }

        switch (choice) {
            case 1:
                student.createAccount();
                break;

            case 2:
                student.login();
                break;

            case 3: {
                std::string id;
                std::cout << "Enter Student ID: ";
                std::cin >> id;

                Student* found = Student::findStudent(id);

                if (found != nullptr) {
                    found->displayProfile();
                } else {
                    std::cout << "Student not found!\n";
                }
                break;
            }

            case 4:
                std::cout << "Goodbye!\n";
                break;

            default:
                std::cout << "Invalid choice!\n";
        }

    } while (choice != 4);

    return 0;
}
