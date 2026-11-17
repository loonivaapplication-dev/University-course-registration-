
#ifndef COURSE_H
#define COURSE_H

#include <string>
#include <vector>

class Course {
private:
    std::string courseID;
    std::string courseName;
    int credits;
    std::string instructor;
    int capacity;
    int enrolledStudents;

    static std::vector<Course> courses;
    static bool coursesLoaded;

    static void loadCourses();
    static void saveCourses();

public:
    Course();

    Course(std::string id, std::string name,
           int courseCredits, std::string teacher,
           int courseCapacity, int enrolled = 0);

    static void addCourse();
    static void editCourse();
    static void deleteCourse();
    static void displayCourses();

    bool checkAvailability() const;

    std::string getCourseID() const;
    std::string getCourseName() const;
    int getCredits() const;
    std::string getInstructor() const;
    int getCapacity() const;
    int getEnrolledStudents() const;
    int getAvailableSeats() const;

    bool enrollStudent();
};

#endif
