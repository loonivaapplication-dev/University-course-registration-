
#ifndef STUDENT_H
#define STUDENT_H

#include "User.h"
#include <string>
#include <vector>

class Student : public User {
private:
    std::string studentID;
    static std::vector<Student> students;

public:
    Student();

    Student(const std::string& id,
            const std::string& name,
            const std::string& email,
            const std::string& password);

    std::string getStudentID() const;

    static Student* findStudent(const std::string& id);

    void createAccount();
    bool login();
    void displayProfile() const override;
};

#endif
