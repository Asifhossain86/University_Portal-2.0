#ifndef GRADEREPORT_H_INCLUDED
#define GRADEREPORT_H_INCLUDED

struct GradeReport {
    char* studentID;
    char* courseCode;
    char* grade;
    struct GradeReport* next;
};


void addGradeReport(const char* studentID, const char* courseCode, const char* grade);
void viewGradeReports(struct Student* student);
#endif
