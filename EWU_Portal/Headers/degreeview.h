#ifndef DEGREEVIEW_H_INCLUDED
#define DEGREEVIEW_H_INCLUDED

struct DegreeView {
    char* studentID;
    char* courseName;
    float credit;
    char* grade;
    struct DegreeView* next;
};


void addDegreeView(const char* studentID, const char* courseName, float credit, const char* grade);
void viewDegree(struct Student* student);
#endif
