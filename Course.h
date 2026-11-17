
#ifndef COURSE_H
#define COURSE_H

#include <string>

class Course {
private:
    std::string day;
    std::string startTime;
    std::string endTime;
    std::string courseID;
    std::string courseName;
    int credits;
    std::string instructor;
    int capacity;
    int enrolledStudents;

    // Store courses using a fixed-size array
    static Course courses[100];

    // Tracks the number of courses stored
    static int courseCount;

    // Checks whether courses have already been loaded
    static bool coursesLoaded;

    // File handling functions
    static void loadCourses();
    static void saveCourses();

public:
    // Default constructor
    Course();

    // Parameterized constructor
    Course(std::string id, std::string name,
           int courseCredits, std::string teacher,
           int courseCapacity, int enrolled,
           std::string courseDay,
           std::string start,
           std::string end);

    // Course management functions
    static void addCourse();
    static void editCourse();
    static void deleteCourse();
    static void displayCourses();

    // Availability checking
    bool checkAvailability() const;

    // Getter functions
    std::string getCourseID() const;
    std::string getCourseName() const;
    int getCredits() const;
    std::string getInstructor() const;
    int getCapacity() const;
    int getEnrolledStudents() const;
    int getAvailableSeats() const;

    // Student enrollment
    bool enrollStudent();
};

#endif
