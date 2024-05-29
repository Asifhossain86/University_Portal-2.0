#ifndef CONFLICTION_H_INCLUDED
#define CONFLICTION_H_INCLUDED

struct Confliction {
    char* courseCode1;
    char* courseCode2;
    struct Confliction* next;
};

void checkCourseConflict();
void displayConflicts();
void addConflict(const char* courseCode1, const char* courseCode2);

#endif
