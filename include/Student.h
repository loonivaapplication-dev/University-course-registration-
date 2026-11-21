#ifndef STUDENT_H
#define STUDENT_H

#include "User.h"

// A Student is a kind of User (inheritance)
class Student : public User {

private:
    // Actions available in the student menu
    void viewAvailableCourses(RegistrationManager& manager);
    void registerForCourse(RegistrationManager& manager);
    void dropCourse(RegistrationManager& manager);
    void viewSchedule(RegistrationManager& manager);
    void checkScheduleConflicts(RegistrationManager& manager);

public:
    Student();
    Student(const string& id, const string& name, const string& email);

    bool login(RegistrationManager& manager) override;   // log in by ID (or create an account)
    void menu(RegistrationManager& manager) override;    // student menu
    string getRole() const override;
};

#endif
