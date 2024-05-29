#ifndef STUDENT_H_INCLUDED
#define STUDENT_H_INCLUDED

struct Student {
    char* studentName;
    char* studentID;
    float studentCgpa;
    float completedCredit;
    char* password;
    char* enrolledBy;
    struct EnrolledCourse* enrolledCourses;
    struct GradeReport* gradeReports;
    struct DegreeView* degreeView;
    struct Ledger* ledger;
    struct Student* next;
};

void addGradeReport(const char* studentID, const char* courseCode, const char* grade);
void viewGradeReports(struct Student* student);
void addDegreeView(const char* studentID, const char* courseName, float credit, const char* grade);
void viewDegree(struct Student* student);
void enrollCourse(struct Student* student, const char* courseCode, const char* section, const char* enrolledBy);
void dropCourse(struct Student* student, const char* courseCode);
struct Student* findStudentByID(const char* studentID);
void addStudent(const char* studentName, const char* studentID, float studentCgpa, float completedCredit);

#endif
