# University-course-registration-
This is a project assignment given by professor kaleb for our OOP-II class
### GitHub Instructions for Everyone

1. Before coding, switch your branch from `main` to your assigned branch (your name).
2. Create your files and paste/write your code in the appropriate files.
3. Once you're done, click **Commit Changes** and push your changes to your own branch.
4. If GitHub shows that your branch is "4 commits behind main", don't worry. We'll merge everyone's branches into `main` once everyone has finished coding.
5. You can also create a Pull Request if you've created new files, as long as your changes don't overlap with another member's files or cause conflicts.
6. **DO NOT DELETE YOUR BRANCH.** Keep it until the entire project is completed.

**Remember:** Work only on your assigned branch. Don't directly edit or push to `main`.

---

## Project Structure

```
UniversityRegistration/
├── main.cpp          program entry point (main menu)
├── include/          all header files (.h), including Input.h (input helpers)
├── src/              all source files (.cpp)
└── data/             students.txt, courses.txt, registrations.txt
```

## Classes

```
User (abstract)                 Course (abstract)
 ├── Student                     ├── LectureCourse
 └── Admin                       ├── LabCourse
                                 └── SeminarCourse

Registration                    RegistrationManager
(student + course record)       (owns all courses, students and registrations,
                                 and saves / loads the files)
```

## OOP concepts used

| Concept | Where |
|---|---|
| **Inheritance** | `Student` and `Admin` inherit from `User`; `LectureCourse`, `LabCourse` and `SeminarCourse` inherit from `Course` |
| **Polymorphism** | `User* user` in `main.cpp` can be a `Student` or an `Admin` and calls the right `login()` / `menu()`. `Course::showDetails()` calls the virtual `showFeatures()` / `getCourseType()` of each course type |
| **Abstraction** | `User` and `Course` are abstract (pure virtual functions), so they cannot be created directly |
| **Encapsulation** | All data members are `private`; they are read through getters and changed through setters or checked functions such as `Course::update()` |
| **Composition** | `RegistrationManager` contains the students, courses and registrations |

## data/courses.txt format
`ID|Name|Credits|Instructor|Capacity|Enrolled|Day|Start|End|Type`  (Type = Lecture, Lab or Seminar; lines without a Type are loaded as Lecture)

## Build
Compile `main.cpp` and every file in `src/`, then run the program from the project root so that `data/` is found:

`g++ -std=c++11 main.cpp src/*.cpp -o UniversityRegistration`
