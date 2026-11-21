#include "../include/Course.h"
#include "../include/Input.h"
#include <iostream>

Course::Course() {
    credits = 0;
    capacity = 0;
    enrolledStudents = 0;
}

Course::Course(const string& id, const string& name, int courseCredits,
               const string& teacher, int courseCapacity, int enrolled,
               const string& courseDay, const string& start, const string& end) {
    courseID = id;
    courseName = name;
    credits = courseCredits;
    instructor = teacher;
    capacity = courseCapacity;
    enrolledStudents = enrolled;
    day = normalizeDay(courseDay);
    startTime = start;
    endTime = end;
}

// ---------------- getters ----------------

string Course::getCourseID() const { return courseID; }
string Course::getCourseName() const { return courseName; }
int Course::getCredits() const { return credits; }
string Course::getInstructor() const { return instructor; }
int Course::getCapacity() const { return capacity; }
int Course::getEnrolledStudents() const { return enrolledStudents; }
int Course::getAvailableSeats() const { return capacity - enrolledStudents; }
string Course::getDay() const { return day; }
string Course::getStartTime() const { return startTime; }
string Course::getEndTime() const { return endTime; }

// ---------------- seats ----------------

bool Course::isFull() const {
    return enrolledStudents >= capacity;
}

bool Course::enrollOne() {
    if (isFull()) return false;
    enrolledStudents++;
    return true;
}

bool Course::dropOne() {
    if (enrolledStudents <= 0) return false;
    enrolledStudents--;
    return true;
}

// ---------------- validation / editing ----------------

bool Course::isValid() const {
    return !courseID.empty() && safeText(courseID) &&
           safeText(courseName) && safeText(instructor) &&
           credits > 0 && capacity > 0 &&
           enrolledStudents >= 0 && enrolledStudents <= capacity &&
           getDayOrder() != 8 &&
           getStartMinutes() >= 0 && getEndMinutes() >= 0 &&
           getEndMinutes() > getStartMinutes();
}

bool Course::update(const string& name, int newCredits, const string& teacher,
                    int newCapacity, const string& newDay,
                    const string& start, const string& end) {

    string fixedDay = normalizeDay(newDay);
    int s = timeToMinutes(start);
    int e = timeToMinutes(end);

    if (!safeText(name) || !safeText(teacher) ||
        newCredits <= 0 || newCapacity <= 0 ||
        newCapacity < enrolledStudents ||
        dayToOrder(fixedDay) == 8 || s < 0 || e < 0 || e <= s) {
        return false;
    }

    courseName = name;
    credits = newCredits;
    instructor = teacher;
    capacity = newCapacity;
    day = fixedDay;
    startTime = start;
    endTime = end;
    return true;
}

// ---------------- schedule helpers ----------------

string Course::normalizeDay(string text) {
    for (size_t i = 0; i < text.length(); i++) {
        if (text[i] >= 'A' && text[i] <= 'Z') text[i] = text[i] + ('a' - 'A');
    }
    if (!text.empty() && text[0] >= 'a' && text[0] <= 'z') text[0] = text[0] - ('a' - 'A');
    return text;
}

int Course::dayToOrder(const string& dayName) {
    if (dayName == "Monday") return 1;
    if (dayName == "Tuesday") return 2;
    if (dayName == "Wednesday") return 3;
    if (dayName == "Thursday") return 4;
    if (dayName == "Friday") return 5;
    if (dayName == "Saturday") return 6;
    if (dayName == "Sunday") return 7;
    return 8;
}

// "HH:MM" -> minutes since midnight, -1 if invalid
int Course::timeToMinutes(const string& timeText) {
    size_t colon = timeText.find(':');
    if (colon == string::npos) return -1;

    string hoursText = timeText.substr(0, colon);
    string minutesText = timeText.substr(colon + 1);
    int hours = 0;
    int minutes = 0;

    if (hoursText.length() > 2 || minutesText.length() != 2 ||
        !parseNumber(hoursText, hours) || !parseNumber(minutesText, minutes)) {
        return -1;
    }
    if (hours > 23 || minutes > 59) return -1;

    return hours * 60 + minutes;
}

string Course::formatTime(int totalMinutes) {
    int hours = totalMinutes / 60;
    int minutes = totalMinutes % 60;
    string result = "";
    result += char('0' + hours / 10);
    result += char('0' + hours % 10);
    result += ':';
    result += char('0' + minutes / 10);
    result += char('0' + minutes % 10);
    return result;
}

int Course::getDayOrder() const { return dayToOrder(day); }
int Course::getStartMinutes() const { return timeToMinutes(startTime); }
int Course::getEndMinutes() const { return timeToMinutes(endTime); }

// Back-to-back classes are NOT a conflict
bool Course::conflictsWith(const Course& other) const {
    if (getDayOrder() != other.getDayOrder()) return false;
    return getStartMinutes() < other.getEndMinutes() &&
           other.getStartMinutes() < getEndMinutes();
}

// ---------------- display ----------------

void Course::showDetails() const {
    cout << "\nCourse ID: " << courseID
         << "\nCourse Name: " << courseName
         << "\nType: " << getCourseType()
         << "\nCredits: " << credits
         << "\nInstructor: " << instructor
         << "\nTime: " << day << " " << startTime << " - " << endTime
         << "\nAvailable Seats: " << getAvailableSeats() << " / " << capacity
         << "\n";
    showFeatures();
    cout << "---------------------------------\n";
}
