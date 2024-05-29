#ifndef UTILITY_H_INCLUDED
#define UTILITY_H_INCLUDED

#include "Student.h"
#include "Faculty.h"
#include "Advisor.h"
#include "Course.h"
#include "EnrolledCourse.h"
#include "Confliction.h"
#include "DegreeView.h"
#include "GradeReport.h"
#include "Ledger.h"

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <time.h>
#include <string.h>

int authenticateUser(const char* username, const char* password);
int generateRandomNumber(int min, int max);
int generateRandomSum();
void displayMenu();
void handleStudentMenu(struct Student* student);
void handleAdvisorMenu(struct Advisor* advisor);
void handleFacultyMenu(struct Faculty* faculty);

#endif
