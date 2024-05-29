#ifndef FACULTY_H_INCLUDED
#define FACULTY_H_INCLUDED

struct Faculty {
    char* username;
    char* password;
    char* initials;
    char** courses;
    char** timings;
    bool isAdvisor;
    int numCourses;
    int numStudents;
    struct Student* enrolledStudents;
    struct GradeReport* allGradeReports;
    struct Advisor* advisorDetails;
    struct Faculty* next;
};

void facultyFunctionality(struct Faculty* faculty);
void addFaculty(const char* initials, const char** courses, int numCourses, const char** timings);
struct Faculty* findFacultyByInitials(const char* initials);

#endif
