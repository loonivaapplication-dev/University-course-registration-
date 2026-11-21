#include "../include/Registration.h"
#include <iostream>

Registration::Registration() {
}

Registration::Registration(const string& id, const string& student, const string& course)
    : registrationID(id), studentID(student), courseID(course) {
}

string Registration::getRegistrationID() const { return registrationID; }
string Registration::getStudentID() const { return studentID; }
string Registration::getCourseID() const { return courseID; }

void Registration::showDetails() const {
    cout << "Registration: " << registrationID
         << " | Student ID: " << studentID
         << " | Course ID: " << courseID << endl;
}
