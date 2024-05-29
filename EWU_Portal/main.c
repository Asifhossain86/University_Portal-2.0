
#include"Portal_Headers.h"

struct Course* courses = NULL;
int numCourses = 0;
struct Student* students = NULL;
struct Advisor* advisors = NULL;
struct Faculty* faculties = NULL;
struct Confliction* conflictList = NULL;

void addGradeReport(const char* studentID, const char* courseCode, const char* grade)
{
    struct GradeReport* newReport = (struct GradeReport*)malloc(sizeof(struct GradeReport));
    newReport->studentID = strdup(studentID);
    newReport->courseCode = strdup(courseCode);
    newReport->grade = strdup(grade);
    newReport->next = NULL;

    struct Student* student = findStudentByID(studentID);
    if (student == NULL)
    {
        printf("Student not found!\n");
        free(newReport->studentID);
        free(newReport->courseCode);
        free(newReport->grade);
        free(newReport);
        return;
    }

    if (student->gradeReports == NULL)
    {
        student->gradeReports = newReport;
    }
    else
    {
        struct GradeReport* currentReport = student->gradeReports;
        while (currentReport->next != NULL)
        {
            currentReport = currentReport->next;
        }
        currentReport->next = newReport;
    }
}

void viewGradeReports(struct Student* student)
{
    if (student->gradeReports == NULL)
    {
        printf("No grade reports available for this student.\n");
        return;
    }

    printf("Grade Reports for Student %s:\n", student->studentName);
    struct GradeReport* currentReport = student->gradeReports;
    while (currentReport != NULL)
    {
        printf("Course: %s, Grade: %s\n", currentReport->courseCode, currentReport->grade);
        currentReport = currentReport->next;
    }
}

void addDegreeView(const char* studentID, const char* courseName, float credit, const char* grade)
{
    struct DegreeView* newView = (struct DegreeView*)malloc(sizeof(struct DegreeView));
    newView->studentID = strdup(studentID);
    newView->courseName = strdup(courseName);
    newView->credit = credit;
    newView->grade = strdup(grade);
    newView->next = NULL;

    struct Student* student = findStudentByID(studentID);
    if (student == NULL)
    {
        printf("Student not found!\n");
        free(newView->studentID);
        free(newView->courseName);
        free(newView->grade);
        free(newView);
        return;
    }

    if (student->degreeView == NULL)
    {
        student->degreeView = newView;
    }
    else
    {
        struct DegreeView* currentView = student->degreeView;
        while (currentView->next != NULL)
        {
            currentView = currentView->next;
        }
        currentView->next = newView;
    }
}

void viewDegree(struct Student* student)
{
    if (student->degreeView == NULL)
    {
        printf("Degree information not available for this student.\n");
        return;
    }

    printf("Degree View for Student %s:\n", student->studentName);
    struct DegreeView* currentView = student->degreeView;
    while (currentView != NULL)
    {
        printf("Course: %s, Credit: %.2f, Grade: %s\n", currentView->courseName, currentView->credit, currentView->grade);
        currentView = currentView->next;
    }
}

void addLedgerEntry(struct Ledger* ledger, const char* transactionType, const char* transactionDetails)
{
    struct LedgerEntry* newEntry = (struct LedgerEntry*)malloc(sizeof(struct LedgerEntry));
    newEntry->transactionType = strdup(transactionType);
    newEntry->transactionDetails = strdup(transactionDetails);
    newEntry->next = NULL;

    if (ledger->head == NULL)
    {
        ledger->head = newEntry;
    }
    else
    {
        struct LedgerEntry* currentEntry = ledger->head;
        while (currentEntry->next != NULL)
        {
            currentEntry = currentEntry->next;
        }
        currentEntry->next = newEntry;
    }
}

void viewLedger(struct Ledger* ledger)
{
    if (ledger->head == NULL)
    {
        printf("No ledger entries available.\n");
        return;
    }

    printf("Ledger Entries:\n");
    struct LedgerEntry* currentEntry = ledger->head;
    while (currentEntry != NULL)
    {
        printf("Transaction Type: %s, Details: %s\n", currentEntry->transactionType, currentEntry->transactionDetails);
        currentEntry = currentEntry->next;
    }
}



const char* getPrerequisite(const char* courseCode)
{
    for (int i = 0; i < numCourses; i++)
    {
        if (strcmp(courses[i].code, courseCode) == 0)
        {
            return courses[i].prerequisite;
        }
    }
    return "None"; // Return "None" if no prerequisite is found
}


int checkPrerequisites(const char* courseCode, struct Student* student)
{
    const char* prerequisite = getPrerequisite(courseCode);
    if (strcmp(prerequisite, "None") == 0) {
        return true;
    }

    struct EnrolledCourse* currentCourse = student->enrolledCourses;
    while (currentCourse != NULL)
    {
        if (strcmp(currentCourse->courseCode, prerequisite) == 0)
        {
            return true; // Prerequisite met
        }
        currentCourse = currentCourse->next;
    }

    return false;
}


void addCourse(const char* code, const char* name, int credits, const char* prerequisite, const char** sections, int numSections)
{
    struct Course* newCourse = (struct Course*)malloc(sizeof(struct Course));
    newCourse->code = strdup(code);
    newCourse->name = strdup(name);
    newCourse->credits = credits;
    newCourse->prerequisite = strdup(prerequisite);
    newCourse->numSections = numSections;
    newCourse->sections = (char**)malloc(numSections * sizeof(char*));
    for (int i = 0; i < numSections; i++)
    {
        newCourse->sections[i] = strdup(sections[i]);
    }
    courses = realloc(courses, (numCourses + 1) * sizeof(struct Course));
    courses[numCourses++] = *newCourse;
    free(newCourse);
}

void enrollCourse(struct Student* student, const char* courseCode, const char* section, const char* enrolledBy)
{
    struct EnrolledCourse* newEnrollment = (struct EnrolledCourse*)malloc(sizeof(struct EnrolledCourse));
    newEnrollment->courseCode = strdup(courseCode);
    newEnrollment->section = strdup(section);
    newEnrollment->enrolledBy = strdup(enrolledBy);
    newEnrollment->next = student->enrolledCourses;
    student->enrolledCourses = newEnrollment;

    FILE* file = fopen("enrollment_data.txt", "a");
    if (file == NULL)
    {
        printf("Error: Could not open file to save enrollment data.\n");
        return;
    }
    fprintf(file, "Student ID: %s, Course Code: %s, Section: %s, Enrolled By: %s\n", student->studentID, courseCode, section, enrolledBy);
    fclose(file);
}

void dropCourse(struct Student* student, const char* courseCode)
{
    struct EnrolledCourse* current = student->enrolledCourses;
    struct EnrolledCourse* prev = NULL;
    while (current != NULL)
    {
        if (strcmp(current->courseCode, courseCode) == 0)
        {
            if (prev == NULL)
            {
                student->enrolledCourses = current->next;
            }
            else
            {
                prev->next = current->next;
            }
            free(current);
            saveDropHistory(student->studentID, courseCode);
            return;
        }
        prev = current;
        current = current->next;
    }
}

void checkCourseConflict()
{
    struct EnrolledCourse* currentCourse1;
    struct EnrolledCourse* currentCourse2;
    struct Student* currentStudent = students;
    while (currentStudent != NULL)
    {
        currentCourse1 = currentStudent->enrolledCourses;
        while (currentCourse1 != NULL)
        {
            currentCourse2 = currentCourse1->next;
            while (currentCourse2 != NULL)
            {
                if (strcmp(currentCourse1->section, currentCourse2->section) == 0)
                {
                    addConflict(currentCourse1->courseCode, currentCourse2->courseCode);
                }
                currentCourse2 = currentCourse2->next;
            }
            currentCourse1 = currentCourse1->next;
        }
        currentStudent = currentStudent->next;
    }
}

void displayConflicts()
{
    struct Confliction* temp = conflictList;
    while (temp != NULL)
    {
        printf("Conflict between %s and %s\n", temp->courseCode1, temp->courseCode2);
        temp = temp->next;
    }
}

void addConflict(const char* courseCode1, const char* courseCode2)
{
    struct Confliction* newConflict = (struct Confliction*)malloc(sizeof(struct Confliction));
    newConflict->courseCode1 = strdup(courseCode1);
    newConflict->courseCode2 = strdup(courseCode2);
    newConflict->next = conflictList;
    conflictList = newConflict;
}

struct Student* findStudentByID(const char* studentID)
{
    struct Student* currentStudent = students;
    while (currentStudent != NULL)
    {
        if (strcmp(currentStudent->studentID, studentID) == 0)
        {
            return currentStudent;
        }
        currentStudent = currentStudent->next;
    }
    return NULL;
}

void addStudent(const char* studentName, const char* studentID, float studentCgpa, float completedCredit)
{
    struct Student* newStudent = (struct Student*)malloc(sizeof(struct Student));
    newStudent->studentName = strdup(studentName);
    newStudent->studentID = strdup(studentID);
    newStudent->studentCgpa = studentCgpa;
    newStudent->completedCredit = completedCredit;
    newStudent->enrolledCourses = NULL;
    newStudent->next = students;
    students = newStudent;
}

void addAdvisor(const char* username, const char* password)
{
    struct Advisor* newAdvisor = (struct Advisor*)malloc(sizeof(struct Advisor));
    newAdvisor->username = strdup(username);
    newAdvisor->password = strdup(password);
    newAdvisor->next = advisors;
    advisors = newAdvisor;
}

void saveDropHistory(const char* studentID, const char* courseCode)
{
    FILE* file = fopen("drop_history.txt", "a");
    if (file == NULL)
    {
        printf("Error: Could not open file.\n");
        return;
    }
    fprintf(file, "%s %s\n", studentID, courseCode);
    fclose(file);
}

int checkAdvisorCredentials(const char* username, const char* password)
{
    struct Advisor* advisor = findAdvisorByUsername(username);
    if (advisor != NULL && strcmp(advisor->password, password) == 0)
    {
        return 1;
    }
    return 0;
}

struct Advisor* findAdvisorByUsername(const char* username)
{
    struct Advisor* currentAdvisor = advisors;
    while (currentAdvisor != NULL)
    {
        if (strcmp(currentAdvisor->username, username) == 0)
        {
            return currentAdvisor;
        }
        currentAdvisor = currentAdvisor->next;
    }
    return NULL;
}

struct Course* findCourseByCode(const char* code)
{
    for (int i = 0; i < numCourses; i++)
    {
        if (strcmp(courses[i].code, code) == 0)
        {
            return &courses[i];
        }
    }
    return NULL;
}

void advisorFunctionality(struct Advisor* advisor)
{
    printf("Welcome, %s!\n", advisor->username);
    int choice;
    do {
        printf("===========================\n");
        printf("\tAdvisor Menu\n");
        printf("===========================\n");
        printf("1. Enroll student in a course\n");
        printf("2. Drop course for a student\n");
        printf("3. Search for a course by code\n");
        printf("4. Exit\n");
        printf("===========================\n");
        printf("Enter your choice: ");
        scanf("%d", &choice);
        switch (choice)
        {
            case 1:
                printf("Enter student ID: ");
                char studentID[20];
                scanf("%s", studentID);
                struct Student* student = findStudentByID(studentID);
                if (student == NULL) {
                    printf("Student not found!\n");
                } else {
                    printf("Enrolled courses for %s:\n", student->studentName);
                    struct EnrolledCourse* currentCourse = student->enrolledCourses;
                    while (currentCourse != NULL) {
                        printf("- %s\n", currentCourse->courseCode);
                        currentCourse = currentCourse->next;
                    }

                    printf("Enter course code to enroll: ");
                    char courseCode[10];
                    scanf("%s", courseCode);
                    struct Course* course = findCourseByCode(courseCode);
                    if (course == NULL) {
                        printf("Course not found!\n");
                    } else {
                        printf("Available Sections for %s:\n", course->name);
                        for (int j = 0; j < course->numSections; j++) {
                            printf("%d. %s\n", j + 1, course->sections[j]);
                        }
                        int sectionChoice;
                        printf("Enter section number to enroll: ");
                        scanf("%d", &sectionChoice);
                        if (sectionChoice >= 1 && sectionChoice <= course->numSections) {
                            enrollCourse(student, courseCode, course->sections[sectionChoice - 1], advisor->username);
                            printf("Course %s enrolled successfully for student %s.\n", courseCode, student->studentName);
                        } else {
                            printf("Invalid section number!\n");
                        }
                    }
                }
                break;

            case 2:
                printf("Enter student ID: ");
                scanf("%s", studentID);
                student = findStudentByID(studentID);
                if (student == NULL) {
                    printf("Student not found!\n");
                } else {
                    printf("Enrolled courses for %s:\n", student->studentName);
                    struct EnrolledCourse* currentCourse = student->enrolledCourses;
                    while (currentCourse != NULL)
                    {
                        printf("- %s\n", currentCourse->courseCode);
                        currentCourse = currentCourse->next;
                    }
                    char courseCode[10];
                    printf("Enter course code to drop: ");
                    scanf("%s", courseCode);
                    dropCourse(student, courseCode);
                    printf("Course %s dropped successfully for student %s.\n", courseCode, student->studentName);
                }
                break;
            case 3:
                printf("Enter course code to search: ");
                char searchCode[10];
                scanf("%s", searchCode);
                struct Course* foundCourse = findCourseByCode(searchCode);
                if (foundCourse != NULL)
                {
                    printf("Course found:\n");
                    printf("Code: %s, Name: %s, Credits: %d, Prerequisite: %s\n", foundCourse->code, foundCourse->name, foundCourse->credits, foundCourse->prerequisite);
                    printf("Sections:\n");
                    for (int i = 0; i < foundCourse->numSections; i++)
                    {
                        printf("%s\n", foundCourse->sections[i]);
                    }
                }
                else
                {
                    printf("Course not found!\n");
                }
                break;
            case 4:
                printf("Exiting...\n");
                break;
            default:
                printf("Invalid choice! Please enter a number between 1 and 4.\n");
        }
    } while (choice != 4);
}

void displayMenu()
{
    printf("===========================\n");
    printf("\tMain Menu\n");
    printf("===========================\n");
    printf("1. Student Login\n");
    printf("2. Advisor Login\n");
    printf("3. Faculty Login\n");
    printf("4. Exit\n");
    printf("===========================\n");
}

void studentFunctionality(struct Student* student)
{
    printf("Welcome, %s!\n", student->studentName);
    int choice;
    do {
        printf("===========================\n");
        printf("\tStudent Menu\n");
        printf("===========================\n");
        printf("1. Enroll in a course\n");
        printf("2. Drop a course\n");
        printf("3. View enrolled courses\n");
        printf("4. Exit\n");
        printf("===========================\n");
        printf("Enter your choice: ");
        scanf("%d", &choice);
        switch (choice)
        {
            case 1:
                printf("Available Courses:\n");
                for (int i = 0; i < numCourses; i++)
                {
                    printf("Code: %s, Name: %s, Credits: %d\n", courses[i].code, courses[i].name, courses[i].credits);
                }
                char courseCode[10];
                printf("Enter course code to enroll: ");
                scanf("%s", courseCode);
                struct Course* course = findCourseByCode(courseCode);
                if (course == NULL)
                {
                    printf("Course not found!\n");
                }
                else
                {
                    if (checkPrerequisites(courseCode, student))
                    {
                        printf("Available Sections for %s:\n", course->name);
                        for (int j = 0; j < course->numSections; j++)
                        {
                            printf("%d. %s\n", j + 1, course->sections[j]);
                        }
                        int sectionChoice;
                        printf("Enter section number to enroll: ");
                        scanf("%d", &sectionChoice);
                        if (sectionChoice >= 1 && sectionChoice <= course->numSections)
                        {
                            enrollCourse(student, courseCode, course->sections[sectionChoice - 1], student->studentID);
                            printf("Course %s enrolled successfully.\n", courseCode);
                        }
                        else
                        {
                            printf("Invalid section number!\n");
                        }
                    }
                    else
                    {
                        printf("You don't meet the prerequisites for this course!\n");
                    }
                }
                break;
            case 2:
                printf("Enrolled courses:\n");
                struct EnrolledCourse* currentCourse = student->enrolledCourses;
                while (currentCourse != NULL)
                {
                    printf("- %s\n", currentCourse->courseCode);
                    currentCourse = currentCourse->next;
                }
                printf("Enter course code to drop: ");
                scanf("%s", courseCode);
                dropCourse(student, courseCode);
                printf("Course %s dropped successfully.\n", courseCode);
                break;
            case 3:
                printf("Enrolled courses:\n");
                currentCourse = student->enrolledCourses;
                while (currentCourse != NULL)
                {
                    printf("- %s\n", currentCourse->courseCode);
                    currentCourse = currentCourse->next;
                }
                break;
            case 4:
                printf("Exiting...\n");
                break;
            default:
                printf("Invalid choice! Please enter a number between 1 and 4.\n");
        }
    } while (choice != 4);
}

void addFaculty(const char* initials, const char** courses, int numCourses, const char** timings)
{
    struct Faculty* newFaculty = (struct Faculty*)malloc(sizeof(struct Faculty));
    newFaculty->initials = strdup(initials);
    newFaculty->courses = (char**)malloc(numCourses * sizeof(char*));
    newFaculty->timings = (char**)malloc(numCourses * sizeof(char*));
    for (int i = 0; i < numCourses; i++)
    {
        newFaculty->courses[i] = strdup(courses[i]);
        newFaculty->timings[i] = strdup(timings[i]);
    }
    newFaculty->numCourses = numCourses;
    faculties = realloc(faculties, (numCourses + 1) * sizeof(struct Faculty));
    faculties[numCourses++] = *newFaculty;
    free(newFaculty);
}

struct Faculty* findFacultyByInitials(const char* initials)
{
    for (int i = 0; i < numCourses; i++)
    {
        if (strcmp(faculties[i].initials, initials) == 0)
        {
            return &faculties[i];
        }
    }
    return NULL;
}

void facultyFunctionality(struct Faculty* faculty)
{
    printf("Welcome, Faculty with initials %s!\n", faculty->initials);
    int choice;
    do {
        printf("===========================\n");
        printf("\tFaculty Menu\n");
        printf("===========================\n");
        printf("1. View courses and timings\n");
        printf("2. Search student information by ID\n");
        printf("3. Give grade\n");
        printf("4. Exit\n");
        printf("===========================\n");
        printf("Enter your choice: ");
        scanf("%d", &choice);
        switch (choice)
        {
            case 1:
                printf("Your courses and timings:\n");
                for (int i = 0; i < faculty->numCourses; i++)
                {
                    printf("Course: %s, Timing: %s\n", faculty->courses[i], faculty->timings[i]);
                }
                break;
            case 2:
                printf("Enter student ID to search: ");
                char studentID[20];
                scanf("%s", studentID);
                struct Student* student = findStudentByID(studentID);
                if (student != NULL)
                {
                    printf("Student Found:\n");
                    printf("Name: %s, ID: %s, CGPA: %.2f\n", student->studentName, student->studentID, student->studentCgpa);
                    printf("Enrolled Courses:\n");
                    struct EnrolledCourse* currentCourse = student->enrolledCourses;
                    while (currentCourse != NULL)
                    {
                        printf("- %s\n", currentCourse->courseCode);
                        currentCourse = currentCourse->next;
                    }
                }
                else
                {
                    printf("Student not found!\n");
                }
                break;
            case 3:
                printf("Enter student ID: ");
                scanf("%s", studentID);
                student = findStudentByID(studentID);
                if (student == NULL)
                {
                    printf("Student not found!\n");
                }
                else
                {
                    printf("Enter course code: ");
                    char courseCode[10];
                    scanf("%s", courseCode);
                    printf("Enter grade for %s in course %s: ", student->studentName, courseCode);
                    char grade[3];
                    scanf("%s", grade);
                    printf("Grade %s given successfully for student %s in course %s.\n", grade, student->studentName, courseCode);
                }
                break;
            case 4:
                printf("Exiting...\n");
                break;
            default:
                printf("Invalid choice! Please enter a number between 1 and 4.\n");
        }
    } while (choice != 4);
}

int main()
{

    addStudent("Lajim", "2022-3-043", 3.5, 90);
    addStudent("Asif", "2022-3-60-007", 3.89, 90);
    addStudent("Ishrat jahan Anika", "2022-3-60-195", 3.67, 90);
    addAdvisor("Hardcore", "push");
    addStudent("Alamin","787",3.7,50);

    addAdvisor("hasan", "121");



    const char* eng101_sections[] = {"Section 1: SUN & THU, 08:30 AM - 10:00 AM, Faculty: MARIA", "Section 2: SUN & THU, 11:50 AM - 01:20 PM, Faculty: SOLIKA", "Section 3: MON & WED, 08:30 AM - 10:00 AM, Faculty: SJS"};
    const char* eng102_sections[] = {"Section 1: MON & WED, 08:30 AM - 10:00 AM, Faculty: KNF", "Section 2: MON & WED, 08:30 AM - 10:00 AM, Faculty: FSPI", "Section 3: SUN & TUE, 08:30 AM - 10:00 AM, Faculty: KNF"};
    const char* phy109_sections[] = {"Section 1: SUN & TUE, 08:30 AM - 10:10 AM, Faculty: MRA", "Section 2: MON & WED, 08:30 AM - 10:10 AM, Faculty: PKM", "Section 3: SUN & TUE, 10:10 AM - 11:40 AM, Faculty: GDM"};
    const char* cse103_sections[] = {"Section 1: MON & WED, 08:30 AM - 10:10 AM, Faculty: TJ", "Section 2: SUN & TUE, 10:10 AM - 11:40 AM, Faculty: MI", "Section 3: SUN & THU, 08:30 AM - 10:10 AM, Faculty: DHMAI"};

    const char* eng101_prerequisite = "None";
    const char* eng102_prerequisite = "ENG101";
    const char* phy109_prerequisite = "MAT102";
    const char* cse103_prerequisite = "None";

    const char* eng101_courses[] = {"ENG101"};
    const char* eng102_courses[] = {"ENG102"};
    const char* phy109_courses[] = {"PHY109"};
    const char* cse103_courses[] = {"CSE103"};

    const char* eng101_timings[] = {"SUN & THU, 08:30 AM - 10:00 AM", "SUN & THU, 11:50 AM - 01:20 PM", "MON & WED, 08:30 AM - 10:00 AM"};
    const char* eng102_timings[] = {"MON & WED, 08:30 AM - 10:00 AM", "MON & WED, 08:30 AM - 10:00 AM", "SUN & TUE, 08:30 AM - 10:00 AM"};
    const char* phy109_timings[] = {"SUN & TUE, 08:30 AM - 10:10 AM", "MON & WED, 08:30 AM - 10:10 AM", "SUN & TUE, 10:10 AM - 11:40 AM"};
    const char* cse103_timings[] = {"MON & WED, 08:30 AM - 10:10 AM", "SUN & TUE, 10:10 AM - 11:40 AM", "SUN & THU, 08:30 AM - 10:10 AM"};

    addCourse("ENG101", "Basic English", 3, eng101_prerequisite, eng101_sections, 3);
    addCourse("ENG102", "Composition and Communication Skills", 3, eng102_prerequisite, eng102_sections, 3);
    addCourse("PHY109", "Engineering Physics-I", 4, phy109_prerequisite, phy109_sections, 3);
    addCourse("CSE103", "Structured Programming", 4.5, cse103_prerequisite, cse103_sections, 3);

    addFaculty("MARIA", eng101_courses, 1, eng101_timings);
    addFaculty("SOLIKA", eng101_courses, 1, eng101_timings);
    addFaculty("SJS", eng101_courses, 1, eng101_timings);
    addFaculty("KNF", eng102_courses, 1, eng102_timings);
    addFaculty("FSPI", eng102_courses, 1, eng102_timings);
    addFaculty("MRA", phy109_courses, 1, phy109_timings);
    addFaculty("PKM", phy109_courses, 1, phy109_timings);
    addFaculty("GDM", phy109_courses, 1, phy109_timings);
    addFaculty("TJ", cse103_courses, 1, cse103_timings);
    addFaculty("MI", cse103_courses, 1, cse103_timings);
    addFaculty("DHMAI", cse103_courses, 1, cse103_timings);




    int loginChoice;
    do {
        displayMenu();
        printf("Enter your choice: ");
        scanf("%d", &loginChoice);
        switch (loginChoice)
        {
            case 1:
                printf("Enter student ID: ");
                char studentID[20];
                scanf("%s", studentID);
                struct Student* student = findStudentByID(studentID);
                if (student != NULL)
                {
                    studentFunctionality(student);
                }
                else
                {
                    printf("Student not found!\n");
                }
                break;
            case 2:
                printf("Enter advisor username: ");
                char advisorUsername[20];
                scanf("%s", advisorUsername);
                printf("Enter advisor password: ");
                char advisorPassword[20];
                scanf("%s", advisorPassword);
                if (checkAdvisorCredentials(advisorUsername, advisorPassword))
                {
                    struct Advisor* advisor = findAdvisorByUsername(advisorUsername);
                    advisorFunctionality(advisor);
                }
                else
                {
                    printf("Invalid credentials!\n");
                }
                break;
            case 3:
                printf("Enter faculty initials: ");
                char facultyInitials[3];
                scanf("%s", facultyInitials);
                struct Faculty* faculty = findFacultyByInitials(facultyInitials);
                if (faculty != NULL)
                {
                    facultyFunctionality(faculty);
                }
                else
                {
                    printf("Faculty not found!\n");
                }
                break;
            case 4:
                printf("Exiting...\n");
                break;
            default:
                printf("Invalid choice! Please enter a number between 1 and 4.\n");
        }
    } while (loginChoice != 4);

    // Clean up allocated memory
    for (int i = 0; i < numCourses; i++)
    {
        free(courses[i].code);
        free(courses[i].name);
        free(courses[i].prerequisite);
        for (int j = 0; j < courses[i].numSections; j++)
        {
            free(courses[i].sections[j]);
        }
        free(courses[i].sections);
    }
    free(courses);

    struct Student* currentStudent = students;
    struct Student* nextStudent;
    while (currentStudent != NULL)
    {
        nextStudent = currentStudent->next;
        struct EnrolledCourse* currentEnrollment = currentStudent->enrolledCourses;
        struct EnrolledCourse* nextEnrollment;
        while (currentEnrollment != NULL)
        {
            nextEnrollment = currentEnrollment->next;
            free(currentEnrollment->courseCode);
            free(currentEnrollment->section);
            free(currentEnrollment->enrolledBy);
            free(currentEnrollment);
            currentEnrollment = nextEnrollment;
        }
        free(currentStudent->studentName);
        free(currentStudent->studentID);
        free(currentStudent->enrolledBy);
        free(currentStudent);
        currentStudent = nextStudent;
    }

    struct Advisor* currentAdvisor = advisors;
    struct Advisor* nextAdvisor;
    while (currentAdvisor != NULL)
    {
        nextAdvisor = currentAdvisor->next;
        free(currentAdvisor->username);
        free(currentAdvisor->password);
        free(currentAdvisor);
        currentAdvisor = nextAdvisor;
    }

    for (int i = 0; i < numCourses; i++)
    {
        free(faculties[i].initials);
        for (int j = 0; j < faculties[i].numCourses; j++)
        {
            free(faculties[i].courses[j]);
            free(faculties[i].timings[j]);
        }
        free(faculties[i].courses);
        free(faculties[i].timings);
    }
    free(faculties);

    struct Confliction* currentConflict = conflictList;
    struct Confliction* nextConflict;
    while (currentConflict != NULL)
    {
        nextConflict = currentConflict->next;
        free(currentConflict->courseCode1);
        free(currentConflict->courseCode2);
        free(currentConflict);
        currentConflict = nextConflict;
    }

    return 0;
}



















