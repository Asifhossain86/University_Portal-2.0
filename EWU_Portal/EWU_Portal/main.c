#include"Portal_Headers.h"

struct Course* courses = NULL;
int numCourses = 0,numFaculties=0,MAX_COURSES=3;
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
    return "None";
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
            return true;
        }
        currentCourse = currentCourse->next;
    }

    return false;
}


void addCourse(const char* code, const char* name, int credits, const char* prerequisite, const char** sections, int numSections)
{

    struct Course* newCourse = (struct Course*)malloc(sizeof(struct Course));
    if (newCourse == NULL)
    {
        fprintf(stderr, "Memory allocation failed for new course.\n");
        return;
    }
    newCourse->code = strdup(code);
    newCourse->name = strdup(name);
    newCourse->credits = credits;
    newCourse->prerequisite = strdup(prerequisite);

    newCourse->numSections = numSections;
    newCourse->sections = (char**)malloc(numSections * sizeof(char*));
    if (newCourse->sections == NULL) {
        fprintf(stderr, "Memory allocation failed for sections array.\n");
        free(newCourse->code);
        free(newCourse->name);
        free(newCourse->prerequisite);
        free(newCourse);
        return;
    }
    for (int i = 0; i < numSections; i++)
    {
        newCourse->sections[i] = strdup(sections[i]);
        if (newCourse->sections[i] == NULL) {
            fprintf(stderr, "Memory allocation failed for section name.\n");
            for (int j = 0; j < i; j++)
            {
                free(newCourse->sections[j]);
            }
            free(newCourse->sections);
            free(newCourse->code);
            free(newCourse->name);
            free(newCourse->prerequisite);
            free(newCourse);
            return;
        }
    }

    courses = realloc(courses, (numCourses + 1) * sizeof(struct Course));
    if (courses == NULL)
        {
        fprintf(stderr, "Memory reallocation failed for courses array.\n");
        for (int i = 0; i < numSections; i++) {
            free(newCourse->sections[i]);
        }
        free(newCourse->sections);
        free(newCourse->code);
        free(newCourse->name);
        free(newCourse->prerequisite);
        free(newCourse);
        return;
    }
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

void addStudent(const char* studentName, const char* studentID, float studentCgpa, float completedCredit, const char* password)
{
    struct Student* newStudent = (struct Student*)malloc(sizeof(struct Student));
    newStudent->studentName = strdup(studentName);
    newStudent->studentID = strdup(studentID);
    newStudent->studentCgpa = studentCgpa;
    newStudent->completedCredit = completedCredit;
    newStudent->password = strdup(password);
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
    faculties = realloc(faculties, (numFaculties + 1) * sizeof(struct Faculty)); // Assuming numFaculties is a global variable
    faculties[numFaculties++] = *newFaculty;
}


struct Faculty* findFacultyByInitials(const char* initials)
{
    for (int i = 0; i < numFaculties; i++)
    {
        if (strcmp(faculties[i].initials, initials) == 0)
        {
            return &faculties[i];
        }
    }
    return NULL;
}


void facultyFunctionality(struct Faculty* faculty) {
    printf("Welcome, Faculty with initials %s!\n", faculty->initials);
    int choice;
    do {
        printf("===========================\n");
        printf("\tFaculty Menu\n");
        printf("===========================\n");
        printf("1. View courses and timings\n");
        printf("2. Search student information by ID\n");
        printf("3. Add a new course with timings\n");
        printf("4. Give grade\n");
        printf("5. Exit\n");
        printf("===========================\n");
        printf("Enter your choice: ");
        scanf("%d", &choice);
        switch (choice) {
            case 1:
                printf("Your courses and timings:\n");
                for (int i = 0; i < faculty->numCourses; i++) {
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
                addNewCourse(faculty);
                break;
            case 4:
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
            case 5:
                printf("Exiting...\n");
                break;
            default:
                printf("Invalid choice! Please enter a number between 1 and 5.\n");
        }
    } while (choice != 5);
}
void addNewCourse(struct Faculty* faculty)
{
    char code[10], name[50];
    int numSections;
    const char** sections;
    const char** timings;

    printf("Enter course code: ");
    scanf("%s", code);
    printf("Enter course name: ");
    scanf("%s", name);
    printf("Enter number of sections: ");
    scanf("%d", &numSections);

    sections = malloc(numSections * sizeof(char*));
    if (sections == NULL) {
        fprintf(stderr, "Memory allocation failed for sections array.\n");
        return;
    }
    for (int i = 0; i < numSections; i++) {
        sections[i] = malloc(50 * sizeof(char));
        if (sections[i] == NULL)
        {
            fprintf(stderr, "Memory allocation failed for section name.\n");
            for (int j = 0; j < i; j++) {
                free(sections[j]);
            }
            free(sections);
            return;
        }
        printf("Enter section %d: ", i + 1);
        scanf("%s", sections[i]);
    }

    addCourse(code, name, 0, NULL, sections, numSections);
}


int main()
{

addStudent("Lajim", "2022-3-60-043", 3.5, 90, "maverick");
addStudent("Asif", "2022-3-60-007", 3.89, 90, "pass");
addStudent("Ishrat jahan Anika", "2022-3-60-195", 3.67, 90, "password");
addStudent("Alamin", "787", 3.7, 50, "password4");

    addAdvisor("Hardcore", "push");

    addAdvisor("hasan", "121");


//Sections
const char* eng101_sections[] = {"Section 1: SUN & THU, 08:30 AM - 10:00 AM, Faculty: MARIA", "Section 2: SUN & THU, 11:50 AM - 01:20 PM, Faculty: SOLIKA", "Section 3: MON & WED, 08:30 AM - 10:00 AM, Faculty: SJS"};
const char* eng102_sections[] = {"Section 1: MON & WED, 08:30 AM - 10:00 AM, Faculty: KNF", "Section 2: MON & WED, 08:30 AM - 10:00 AM, Faculty: FSPI", "Section 3: SUN & TUE, 08:30 AM - 10:00 AM, Faculty: KNF"};
const char* phy109_sections[] = {"Section 1: SUN & TUE, 08:30 AM - 10:10 AM, Faculty: MRA", "Section 2: MON & WED, 08:30 AM - 10:10 AM, Faculty: PKM", "Section 3: SUN & TUE, 10:10 AM - 11:40 AM, Faculty: GDM"};
const char* cse103_sections[] = {"Section 1: MON & WED, 08:30 AM - 10:10 AM, Faculty: TJ", "Section 2: SUN & TUE, 10:10 AM - 11:40 AM, Faculty: MI", "Section 3: SUN & THU, 08:30 AM - 10:10 AM, Faculty: DHMAI"};
const char* gen226_sections[] = {"Section 1: MON & WED, 11:50 AM - 01:30 PM, Faculty: TMD", "Section 2: MON & WED, 01:30 PM - 03:00 PM, Faculty: NAB", "Section 3: SUN & THU, 10:10 AM - 11:40 AM, Faculty: NHAM"};
const char* eco101_sections[] = {"Section 1: SUN & TUE, 11:50 AM - 01:20 PM, Faculty: MC", "Section 2: MON & WED, 03:10 PM - 04:40 PM, Faculty: MASM", "Section 3: TUE & THU, 01:30 PM - 03:00 PM, Faculty: DMTH"};
const char* gen203_sections[] = {"Section: TUE & THU, 10:10 AM - 11:40 AM, Faculty: MRWS"};
const char* gen214_sections[] = {"Section: MON & WED, 01:30 PM - 03:00 PM, Faculty: SHM"};
const char* gen201_sections[] = {"Section 1: MON & WED, 10:10 AM - 11:40 AM, Faculty: DKN", "Section 2: MON & WED, 01:30 PM - 03:00 PM, Faculty: NAB"};
const char* gen210_sections[] = {"Section 1: MON & WED, 01:30 PM - 03:00 PM, Faculty: EH", "Section 2: MON & WED, 03:10 PM - 04:40 PM, Faculty: EH"};
const char* act101_sections[] = {"Section 1: SUN & TUE, 10:10 AM - 11:40 AM, Faculty: DACY", "Section 2: SUN & TUE, 11:50 AM - 01:20 PM, Faculty: DACY"};
const char* bus231_sections[] = {"Section 1: SUN & TUE, 10:10 AM - 11:40 AM, Faculty: DACY", "Section 2: SUN & TUE, 11:50 AM - 01:20 PM, Faculty: DACY"};
const char* mgt337_sections[] = {"Section 1: MON & WED, 10:10 AM - 11:40 AM, Faculty: SAM", "Section 2: SUN & THU, 03:10 PM - 04:40 PM, Faculty: SNS"};
const char* fin101_sections[] = {"Section 1: TUE & THU, 03:10 PM - 04:40 PM, Faculty: MNS", "Section 2: SUN & TUE, 10:10 AM - 11:40 AM, Faculty: DSBA", "Section 3: SUN & TUE, 03:10 PM - 04:40 PM, Faculty: RJ"};
const char* cse106_sections[] = {
    "Section 1: SUN & TUE, 10:10 AM - 11:40 AM, Faculty: MHAK",
    "Section 2: SUN & TUE, 01:10 PM - 03:00 PM, Faculty: MHAK",
    "Section 3: MON & WED, 11:50 AM - 01:20 PM, Faculty: DSU"
};

const char* cse110_sections[] = {
    "Section 1: SUN & THU",
    "Section 2: MON & WED, 01:30 PM - 03:00 PM, Faculty: DSU",
    "Section 3: SUN & THU, 10:10 AM - 01:10 PM, Faculty: TM"
};

const char* cse200_sections[] = {
    "Section 1: MON, 01:30 PM - 03:30 PM, Faculty: ALI",
    "Section 2: MON, 04:40 PM - 06:50 PM, Faculty: MDH",
    "Section 3: WED, 04:40 PM - 06:50 PM, Faculty: MMRA"
};

const char* cse209_sections[] = {
    "Section 1: MON & WED, 10:10 AM - 01:10 PM, Faculty: RDA",
    "Section 2: MON & WED, 01:30 PM - 04:40 PM, Faculty: SHK",
    "Section 3: MON & WED, 10:10 AM - 11:40 AM, Faculty: MKN"
};

const char* cse207_sections[] = {
    "Section 1: SUN & THU, 08:30 AM - 10:10 AM, Faculty: MI",
    "Section 2: SUN & THU, 11:50 AM - 01:20 PM, Faculty: DAKP",
    "Section 3: TUE & THU, 01:30 PM - 03:30 PM, Faculty: DHMAI"
};

const char* cse251_sections[] = {
    "Section 1: MON & WED, 01:30 PM - 03:30 PM, Faculty: MHR",
    "Section 2: SUN & THU, 10:10 AM - 11:40 AM, Faculty: SJ",
    "Section 3: MON & WED, 10:10 AM - 11:40 AM, Faculty: SHK"
};

const char* cse246_sections[] = {
    "Section 1: MON & WED, 08:30 AM - 10:00 AM, Faculty: AMITM",
    "Section 2: MON & WED, 10:10 AM - 12:10 PM, Faculty: RARZ",
    "Section 3: TUE & THU, 08:30 AM - 10:00 AM, Faculty: AASR"
};

const char* cse302_sections[] = {
    "Section 1: SUN & THU, 08:30 AM - 10:00 AM, Faculty: DMRH",
    "Section 2: MON & WED, 11:50 AM - 01:20 PM, Faculty: DMAZ",
    "Section 3: TUE & THU, 03:10 PM - 04:40 PM, Faculty: MMSU"
};
const char* mat101_sections[] = {
    "Section 1: MON & WED, 10:10 AM - 11:40 AM, Faculty: AP",
    "Section 2: MON & WED, 10:10 AM - 11:40 AM, Faculty: SIS",
    "Section 3: MON & WED, 03:10 PM - 04:40 PM, Faculty: IKI"
};

const char* mat102_sections[] = {
    "Section 1: MON & WED, 01:10 PM - 03:00 PM, Faculty: AP",
    "Section 2: SUN & TUE, 10:10 AM - 11:40 AM, Faculty: AP",
    "Section 3: SUN & TUE, 03:10 PM - 04:40 PM, Faculty: AKHN"
};

const char* mat104_sections[] = {
    "Section 1: MON & WED, 10:10 AM - 11:40 AM, Faculty: DFK",
    "Section 2: MON & WED, 03:10 PM - 04:40 PM, Faculty: AKHN",
    "Section 3: MON & WED, 03:10 PM - 04:40 PM, Faculty: AKMN"
};

const char* mat205_sections[] = {
    "Section 1: MON & WED, 11:50 AM - 01:20 PM, Faculty: DNCR",
    "Section 2: SUN - THU, 11:50 AM - 01:20 PM, Faculty: AKMN",
    "Section 3: SUN - TUE, 01:30 PM - 03:00 PM, Faculty: DFK"
};

const char* sta102_sections[] = {
    "Section 1: TUE & THU, 11:50 AM - 01:20 PM, Faculty: DSR",
    "Section 2: MON & WED, 10:10 AM - 11:40 AM, Faculty: IKI",
    "Section 3: SUN - THU, 01:30 PM - 03:00 PM, Faculty: IKI"
};



//Prereq
const char* eng101_prerequisite = "None";
const char* eng102_prerequisite = "ENG101";
const char* phy109_prerequisite = "MAT102";
const char* cse103_prerequisite = "None";
const char* gen226_prerequisite = "ENG102";
const char* eco101_prerequisite = "MAT101";
const char* gen203_prerequisite = "ENG102";
const char* gen214_prerequisite = "ENG102";
const char* gen201_prerequisite = "ENG102";
const char* gen210_prerequisite = "NONE";
const char* act101_prerequisite = "MAT101";
const char* bus231_prerequisite = "ENG101";
const char* mgt337_prerequisite = "ENG102";
const char* fin101_prerequisite = "MAT101";
const char* cse106_prerequisite = "CSE103";
const char* cse110_prerequisite = "CSE106";
const char* cse200_prerequisite = "None";
const char* cse209_prerequisite = "None";
const char* cse207_prerequisite = "CSE110";
const char* cse251_prerequisite = "CSE209";
const char* cse246_prerequisite = "CSE207";
const char* cse302_prerequisite = "CSE106";
const char* mat101_prerequisite = "None";
const char* mat102_prerequisite = "MAT101";
const char* mat104_prerequisite = "MAT101";
const char* mat205_prerequisite = "MAT102";
const char* sta102_prerequisite = "None";



//Cour
const char* eng101_courses[] = {"ENG101"};
const char* eng102_courses[] = {"ENG102"};
const char* phy109_courses[] = {"PHY109"};
const char* cse103_courses[] = {"CSE103"};
const char* gen226_courses[] = {"GEN226"};
const char* eco101_courses[] = {"ECO101"};
const char* gen203_courses[] = {"GEN203"};
const char* gen214_courses[] = {"GEN214"};
const char* gen201_courses[] = {"GEN201"};
const char* gen210_courses[] = {"GEN210"};
const char* act101_courses[] = {"ACT101"};
const char* bus231_courses[] = {"BUS231"};
const char* mgt337_courses[] = {"MGT337"};
const char* fin101_courses[] = {"FIN101"};
const char* cse106_courses[] = {"CSE106"};
const char* cse110_courses[] = {"CSE110"};
const char* cse200_courses[] = {"CSE200"};
const char* cse209_courses[] = {"CSE209"};
const char* cse207_courses[] = {"CSE207"};
const char* cse251_courses[] = {"CSE251"};
const char* cse246_courses[] = {"CSE246"};
const char* cse302_courses[] = {"CSE302"};
const char* mat101_courses[] = {"MAT101"};
const char* mat102_courses[] = {"MAT102"};
const char* mat104_courses[] = {"MAT104"};
const char* mat205_courses[] = {"MAT205"};
const char* sta102_courses[] = {"STA102"};






const char* eng101_timings[] = {"SUN & THU, 08:30 AM - 10:00 AM", "SUN & THU, 11:50 AM - 01:20 PM", "MON & WED, 08:30 AM - 10:00 AM"};
const char* eng102_timings[] = {"MON & WED, 08:30 AM - 10:00 AM", "MON & WED, 08:30 AM - 10:00 AM", "SUN & TUE, 08:30 AM - 10:00 AM"};
const char* phy109_timings[] = {"SUN & TUE, 08:30 AM - 10:10 AM", "MON & WED, 08:30 AM - 10:10 AM", "SUN & TUE, 10:10 AM - 11:40 AM"};
const char* cse103_timings[] = {"MON & WED, 08:30 AM - 10:10 AM", "SUN & TUE, 10:10 AM - 11:40 AM", "SUN & THU, 08:30 AM - 10:10 AM"};
const char* gen226_timings[] = {"MON & WED, 11:50 AM - 01:30 PM", "MON & WED, 01:30 PM - 03:00 PM", "SUN & THU, 10:10 AM - 11:40 AM"};
const char* eco101_timings[] = {"SUN & TUE, 11:50 AM - 01:20 PM", "MON & WED, 03:10 PM - 04:40 PM", "TUE & THU, 01:30 PM - 03:00 PM"};
const char* gen203_timings[] = {"TUE & THU, 10:10 AM - 11:40 AM"};
const char* gen214_timings[] = {"MON & WED, 01:30 PM - 03:00 PM"};
const char* gen201_timings[] = {"MON & WED, 10:10 AM - 11:40 AM", "MON & WED, 01:30 PM - 03:00 PM"};
const char* gen210_timings[] = {"MON & WED, 01:30 PM - 03:00 PM", "MON & WED, 03:10 PM - 04:40 PM"};
const char* act101_timings[] = {"SUN & TUE, 10:10 AM - 11:40 AM", "SUN & TUE, 11:50 AM - 01:20 PM"};
const char* bus231_timings[] = {"SUN & TUE, 10:10 AM - 11:40 AM", "SUN & TUE, 11:50 AM - 01:20 PM"};
const char* mgt337_timings[] = {"MON & WED, 10:10 AM - 11:40 AM", "SUN & THU, 03:10 PM - 04:40 PM"};
const char* fin101_timings[] = {"TUE & THU, 03:10 PM - 04:40 PM", "SUN & TUE, 10:10 AM - 11:40 AM", "SUN & TUE, 03:10 PM - 04:40 PM"};

const char* mat101_timings[] = {
    "MON & WED, 10:10 AM - 11:40 AM",
    "MON & WED, 03:10 PM - 04:40 PM"
};
const char* mat102_timings[] = {
    "MON & WED, 01:10 PM - 03:00 PM",
    "SUN & TUE, 10:10 AM - 11:40 AM",
    "SUN & TUE, 03:10 PM - 04:40 PM"
};
const char* mat104_timings[] = {
    "MON & WED, 10:10 AM - 11:40 AM",
    "MON & WED, 03:10 PM - 04:40 PM"
};
const char* mat205_timings[] = {
    "MON & WED, 11:50 AM - 01:20 PM",
    "SUN - THU, 11:50 AM - 01:20 PM",
    "SUN - TUE, 01:30 PM - 03:00 PM"
};
const char* sta102_timings[] = {
    "TUE & THU, 11:50 AM - 01:20 PM",
    "MON & WED, 10:10 AM - 11:40 AM",
    "SUN - THU, 01:30 PM - 03:00 PM"
};
const char* cse106_timings[] = {
    "SUN & TUE, 10:10 AM - 11:40 AM",
    "SUN & TUE, 01:10 PM - 03:00 PM",
    "MON & WED, 11:50 AM - 01:20 PM"
};
const char* cse110_timings[] = {
    "SUN & THU",
    "MON & WED, 01:30 PM - 03:00 PM",
    "SUN & THU, 10:10 AM - 01:10 PM"
};
const char* cse200_timings[] = {
    "MON, 01:30 PM - 03:30 PM",
    "MON, 04:40 PM - 06:50 PM",
    "WED, 04:40 PM - 06:50 PM"
};
const char* cse209_timings[] = {
    "MON & WED, 10:10 AM - 01:10 PM",
    "MON & WED, 01:30 PM - 04:40 PM",
    "MON & WED, 10:10 AM - 11:40 AM"
};
const char* cse207_timings[] = {
    "SUN & THU, 08:30 AM - 10:10 AM",
    "SUN & THU, 11:50 AM - 01:20 PM",
    "TUE & THU, 01:30 PM - 03:30 PM"
};
const char* cse251_timings[] = {
    "MON & WED, 01:30 PM - 03:30 PM",
    "SUN & THU, 10:10 AM - 11:40 AM",
    "MON & WED, 10:10 AM - 11:40 AM"
};
const char* cse246_timings[] = {
    "MON & WED, 08:30 AM - 10:00 AM",
    "MON & WED, 10:10 AM - 12:10 PM",
    "TUE & THU, 08:30 AM - 10:00 AM"
};
const char* cse302_timings[] = {
    "SUN & THU, 08:30 AM - 10:00 AM",
    "MON & WED, 11:50 AM - 01:20 PM",
    "TUE & THU, 03:10 PM - 04:40 PM"
};





addCourse("ENG101", "Basic English", 3, eng101_prerequisite, eng101_sections, 3);
addCourse("ENG102", "Composition and Communication Skills", 3, eng102_prerequisite, eng102_sections, 3);
addCourse("PHY109", "Engineering Physics-I", 4, phy109_prerequisite, phy109_sections, 3);
addCourse("CSE103", "Structured Programming", 4.5, cse103_prerequisite, cse103_sections, 3);
addCourse("GEN226", "Emergence of Bangladesh", 3, gen226_prerequisite, gen226_sections, 3);
addCourse("ECO101", "Principles of Microeconomics", 3, eco101_prerequisite, eco101_sections, 3);
addCourse("GEN203", "Ecological System and Environment", 3, gen203_prerequisite, gen203_sections, 1);
addCourse("GEN214", "Development Studies", 3, gen214_prerequisite, gen214_sections, 1);
addCourse("GEN201", "Bangladesh Studies", 3, gen201_prerequisite, gen201_sections, 2);
addCourse("GEN210", "International Relation", 3, gen210_prerequisite, gen210_sections, 2);
addCourse("ACT101", "Financial Accounting", 3, act101_prerequisite, act101_sections, 2);
addCourse("BUS231", "Business Communication", 3, bus231_prerequisite, bus231_sections, 2);
addCourse("MGT337", "Production Operations Management", 3, mgt337_prerequisite, mgt337_sections, 2);
addCourse("FIN101", "Principles of Finance", 3, fin101_prerequisite, fin101_sections, 3);
addCourse("MAT101", "Differential and Integral Calculus", 3, mat101_prerequisite, mat101_sections, 3);
addCourse("MAT102", "Differential Equations and Special Functions", 3, mat102_prerequisite, mat102_sections, 3);
addCourse("MAT104", "Coordinate Geometry and Vector Analysis", 3, mat104_prerequisite, mat104_sections, 3);
addCourse("MAT205", "Linear Algebra and Complex Variable", 3, mat205_prerequisite, mat205_sections, 3);
addCourse("STA102", "Statistics and Probability", 3, sta102_prerequisite, sta102_sections, 3);
addCourse("CSE106", "Discrete Mathematics", 3, cse106_prerequisite, cse106_sections, 3);
addCourse("CSE110", "Object Oriented Programming", 4.5, cse110_prerequisite, cse110_sections, 3);
addCourse("CSE200", "Computer-Aided Engineering Drawing", 1, cse200_prerequisite, cse200_sections, 3);
addCourse("CSE209", "Electrical Circuits", 4, cse209_prerequisite, cse209_sections, 3);
addCourse("CSE207", "Data Structures", 4, cse207_prerequisite, cse207_sections, 3);
addCourse("CSE251", "Electronic Circuits", 4, cse251_prerequisite, cse251_sections, 3);
addCourse("CSE246", "Algorithms", 4.5, cse246_prerequisite, cse246_sections, 3);
addCourse("CSE302", "Database Systems", 4.5, cse302_prerequisite, cse302_sections, 3);



addFaculty("AP", mat101_courses, 1, mat101_timings);
addFaculty("SIS", mat101_courses, 1, mat101_timings);
addFaculty("IKI", mat101_courses, 1, mat101_timings);
addFaculty("AP", mat102_courses, 1, mat102_timings);
addFaculty("AKHN", mat102_courses, 1, mat102_timings);
addFaculty("DFK", mat104_courses, 1, mat104_timings);
addFaculty("AKHN", mat104_courses, 1, mat104_timings);
addFaculty("AKMN", mat104_courses, 1, mat104_timings);
addFaculty("DNCR", mat205_courses, 1, mat205_timings);
addFaculty("AKMN", mat205_courses, 1, mat205_timings);
addFaculty("DFK", mat205_courses, 1, mat205_timings);
addFaculty("DSR", sta102_courses, 1, sta102_timings);
addFaculty("IKI", sta102_courses, 1, sta102_timings);
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
addFaculty("TMD", gen226_courses, 1, gen226_timings);
addFaculty("NAB", gen226_courses, 1, gen226_timings);
addFaculty("NHAM", gen226_courses, 1, gen226_timings);
addFaculty("MC", eco101_courses, 1, eco101_timings);
addFaculty("MASM", eco101_courses, 1, eco101_timings);
addFaculty("DMTH", eco101_courses, 1, eco101_timings);
addFaculty("MRWS", gen203_courses, 1, gen203_timings);
addFaculty("SHM", gen214_courses, 1, gen214_timings);
addFaculty("DKN", gen201_courses, 1, gen201_timings);
addFaculty("NAB", gen201_courses, 1, gen201_timings);
addFaculty("EH", gen210_courses, 1, gen210_timings);
addFaculty("EH", gen210_courses, 1, gen210_timings); // Intentional duplicate to match input
addFaculty("DACY", act101_courses, 1, act101_timings);
addFaculty("DACY", act101_courses, 1, act101_timings); // Intentional duplicate to match input
addFaculty("DACY", bus231_courses, 1, bus231_timings);
addFaculty("DACY", bus231_courses, 1, bus231_timings); // Intentional duplicate to match input
addFaculty("SAM", mgt337_courses, 1, mgt337_timings);
addFaculty("SNS", mgt337_courses, 1, mgt337_timings);
addFaculty("MNS", fin101_courses, 1, fin101_timings);
addFaculty("DSBA", fin101_courses, 1, fin101_timings);
addFaculty("RJ", fin101_courses, 1, fin101_timings);
addFaculty("MHAK", cse106_courses, 1, cse106_timings);
addFaculty("DSU", cse106_courses, 1, cse106_timings);
addFaculty("TM", cse110_courses, 1, cse110_timings);
addFaculty("ALI", cse200_courses, 1, cse200_timings);
addFaculty("MDH", cse200_courses, 1, cse200_timings);
addFaculty("MMRA", cse200_courses, 1, cse200_timings);
addFaculty("RDA", cse209_courses, 1, cse209_timings);
addFaculty("SHK", cse209_courses, 1, cse209_timings);
addFaculty("MKN", cse209_courses, 1, cse209_timings);
addFaculty("MI", cse207_courses, 1, cse207_timings);
addFaculty("DAKP", cse207_courses, 1, cse207_timings);
addFaculty("DHMAI", cse207_courses, 2, cse207_timings);
addFaculty("MHR", cse251_courses, 1, cse251_timings);
addFaculty("SJ", cse251_courses, 1, cse251_timings);
addFaculty("SHK", cse251_courses, 1, cse251_timings);
addFaculty("AMITM", cse246_courses, 1, cse246_timings);
addFaculty("RARZ", cse246_courses, 1, cse246_timings);
addFaculty("AASR", cse246_courses, 1, cse246_timings);
addFaculty("DMRH", cse302_courses, 1, cse302_timings);
addFaculty("DMAZ", cse302_courses, 1, cse302_timings);
addFaculty("MMSU", cse302_courses, 1, cse302_timings);





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
                    char password[20];
                    printf("Enter password: ");
                    scanf("%s", password);
                    if (strcmp(password, student->password) == 0) {
                        studentFunctionality(student);
                    } else {
                        printf("Incorrect password. Access denied.\n");
                    }
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



















