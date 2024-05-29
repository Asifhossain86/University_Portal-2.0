#ifndef ENROLLEDCOURSE_H_INCLUDED
#define ENROLLEDCOURSE_H_INCLUDED

struct EnrolledCourse {
    char* courseCode;
    char* section;
    char* enrolledBy;
    struct EnrolledCourse* next;
};

void addCourse(const char* code, const char* name, int credits, const char* prerequisite, const char** sections, int numSections);
void enrollCourse(struct Student* student, const char* courseCode, const char* section, const char* enrolledBy);
void dropCourse(struct Student* student, const char* courseCode);
void saveDropHistory(const char* studentID, const char* courseCode);
#endif
