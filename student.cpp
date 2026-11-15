
#include "Student.h"
#include <iostream>
#include <limits>

std::vector<Student> Student::students;

Student::Student() {}

Student::Student(const std::string& id,
                 const std::string& name,
                 const std::string& email,
                 const std::string& password)
    : User(name, email, password), studentID(id) {}

std::string Student::getStudentID() const {
    return studentID;
}

Student* Student::findStudent(const std::string& id) {
    for (Student& student : students) {
        if (student.getStudentID() == id) {
            return &student;
        }
    }
    return nullptr;
}

void Student::createAccount() {
    std::string id, name, email, password;

    std::cout << "\n--- Student Registration ---\n";

    std::cout << "Enter Student ID: ";
    std::getline(std::cin >> std::ws, id);

    if (id.empty() || findStudent(id) != nullptr) {
        std::cout << "Invalid or duplicate Student ID!\n";
        return;
    }

    std::cout << "Enter Name: ";
    std::getline(std::cin, name);

    std::cout << "Enter Email: ";
    std::getline(std::cin, email);

    for (const Student& student : students) {
        if (student.getEmail() == email) {
            std::cout << "Email already registered!\n";
            return;
        }
    }

    if (name.empty() || email.empty() ||
        email.find('@') == std::string::npos) {
        std::cout << "Invalid name or email!\n";
        return;
    }

    std::cout << "Enter Password: ";
    std::getline(std::cin, password);

    if (password.empty()) {
        std::cout << "Password cannot be empty!\n";
        return;
    }

    students.emplace_back(id, name, email, password);

    std::cout << "Account created successfully!\n";
}

bool Student::login() {
    std::string id, password;

    std::cout << "\n--- Student Login ---\n";

    std::cout << "Enter Student ID: ";
    std::cin >> id;

    std::cout << "Enter Password: ";
    std::cin >> password;

    Student* student = findStudent(id);

    if (student != nullptr &&
        student->checkPassword(password)) {
        std::cout << "Login successful! Welcome, "
                  << student->getName() << "!\n";
        return true;
    }

    std::cout << "Invalid Student ID or Password!\n";
    return false;
}

void Student::displayProfile() const {
    std::cout << "\n--- Student Profile ---\n";
    std::cout << "Student ID: " << studentID << '\n';
    User::displayProfile();
}
