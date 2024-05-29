#ifndef PORTAL_HEADERS_H_INCLUDED
#define PORTAL_HEADERS_H_INCLUDED

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

struct EnrolledCourse
{
    char* courseCode;
    char* section;
    char* enrolledBy;
    struct EnrolledCourse* next;
};

struct Course
{
    char* code;
    char* name;
    int credits;
    char* prerequisite;
    char** sections;
    int numSections;
};

struct Student
{
    char* studentName;
    char* studentID;
    float studentCgpa;
    float completedCredit;
    char* enrolledBy;
    char* password;
    struct EnrolledCourse* enrolledCourses;
    struct GradeReport* gradeReports;
    struct DegreeView* degreeView;
    struct Ledger* ledger;
    struct Student* next;
};

struct Advisor
{
    char* username;
    char* password;
    struct Advisor* next;
};

struct Faculty
{
    char* initials;
    char* password;
    char** courses;
    int numCourses;
    char** timings;
};

struct Confliction
{
    char* courseCode1;
    char* courseCode2;
    struct Confliction* next;
};

struct GradeReport
{
    char* studentID;
    char* courseCode;
    char* grade;
    struct GradeReport* next;
};

struct DegreeView
{
    char* studentID;
    char* courseName;
    float credit;
    char* grade;
    struct DegreeView* next;
};

struct LedgerEntry {
    char* transactionType;
    char* transactionDetails;
    struct LedgerEntry* next;
};

struct Ledger
{
    struct LedgerEntry* head;
};


void addGradeReport(const char* studentID, const char* courseCode, const char* grade);
void viewGradeReports(struct Student* student);
void addDegreeView(const char* studentID, const char* courseName, float credit, const char* grade);
void viewDegree(struct Student* student);
void addLedgerEntry(struct Ledger* ledger, const char* transactionType, const char* transactionDetails);
void viewLedger(struct Ledger* ledger);
void addCourse(const char* code, const char* name, int credits, const char* prerequisite, const char** sections, int numSections);
void enrollCourse(struct Student* student, const char* courseCode, const char* section, const char* enrolledBy);
void dropCourse(struct Student* student, const char* courseCode);
void checkCourseConflict();
void displayConflicts();
void addConflict(const char* courseCode1, const char* courseCode2);
struct Student* findStudentByID(const char* studentID);
void addStudent(const char* studentName, const char* studentID, float studentCgpa, float completedCredit,const char* password);
void addAdvisor(const char* username, const char* password);
void saveDropHistory(const char* studentID, const char* courseCode);
int checkAdvisorCredentials(const char* username, const char* password);
struct Advisor* findAdvisorByUsername(const char* username);
struct Course* findCourseByCode(const char* code);
void advisorFunctionality(struct Advisor* advisor);
void displayMenu();
void studentFunctionality(struct Student* student);
void addFaculty(const char* initials, const char** courses, int numCourses, const char** timings);
struct Faculty* findFacultyByInitials(const char* initials);
void facultyFunctionality(struct Faculty* faculty);
void initializeLedger();
int authenticateUser(const char* username, const char* password);
int generateRandomNumber(int min, int max);
int generateRandomSum();
void handleStudentMenu(struct Student* student);
void handleAdvisorMenu(struct Advisor* advisor);
void handleFacultyMenu(struct Faculty* faculty);


#endif
