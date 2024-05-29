#ifndef ADVISOR_H_INCLUDED
#define ADVISOR_H_INCLUDED

struct Advisor {
    char* username;
    char* password;
    bool isFaculty;
    struct Student* students;
    struct Course* courses;
    struct Ledger* ledger;
    struct Advisor* next;
};

void advisorFunctionality(struct Advisor* advisor);
void addAdvisor(const char* username, const char* password, const char* initials, const char** courses, int numCourses, const char** timings);
int checkAdvisorCredentials(const char* username, const char* password);
struct Advisor* findAdvisorByUsername(const char* username);

#endif
