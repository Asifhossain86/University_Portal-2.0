#ifndef COURSE_H_INCLUDED
#define COURSE_H_INCLUDED

struct Course {
    char* code;
    char* name;
    int credits;
    char** sections;
    int numSections;
    char* prerequisite;
    struct Confliction* confliction;
};


struct Course* findCourseByCode(const char* code);
void addCourse(const char* code, const char* name, int credits, const char* prerequisite, const char** sections, int numSections);
void enrollCourse(struct Student* student, const char* courseCode, const char* section, const char* enrolledBy);
void dropCourse(struct Student* student, const char* courseCode);
#endif
