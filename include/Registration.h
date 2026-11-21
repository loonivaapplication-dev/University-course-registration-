#ifndef REGISTRATION_H
#define REGISTRATION_H

#include <string>
using namespace std;

// One record: "this student is registered in this course"
class Registration {

private:
    string registrationID;
    string studentID;
    string courseID;

public:
    Registration();
    Registration(const string& id, const string& student, const string& course);

    string getRegistrationID() const;
    string getStudentID() const;
    string getCourseID() const;

    void showDetails() const;
};

#endif
