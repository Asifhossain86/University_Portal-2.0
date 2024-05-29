
+---------------------+       +-------------------+      +-------------------+      +-----------------+
|      Student        |------>|   EnrolledCourse  |----->|   GradeReport     |      |   DegreeView    |
+---------------------+       +-------------------+      +-------------------+      +-----------------+
| studentName         |       | courseCode        |      | studentID         |      | studentID       |
| studentID           |       | section           |      | courseCode        |      | courseName      |
| studentCgpa         |       | enrolledBy        |      | grade             |      | credit          |
| completedCredit     |       +-------------------+      +-------------------+      | grade           |
| password            |                                                             +-----------------+
| enrolledBy          |                                                                     |
| enrolledCourses     |                                                                     |
| gradeReports        |                                                                     v                                                                     
| degreeView          |-----------------------------------------------------|---------------+
+---------------------+                                                     v
          |                                                       +-------------------+
          |                                                       |      Ledger       |
          |                                                       +-------------------+
          |                                                       | head              |
          |                                                       +-------------------+
          |                                                                     |
          |                                                                     |
          V                                                                     |
+---------------------+                                                         |
|      Faculty        |                                                         |
+---------------------+                                                         |
| username            |                                                         |
| password            |                                                         |
| initials            |                                                         |
| courses             |                                                         |
| timings             |                                                         |
| isAdvisor           |                                                         |
| numCourses          |                                                         |
| numStudents         |                                                         |
| enrolledStudents    |                                                         |
| allGradeReports     |                                                         |
| advisorDetails      |                                                         |
| next                |                                                         |
+---------------------+                                                         |
          |                                                                     |
          |                                                                     |
          V                                                                     |
+---------------------+                                                         |
|      Advisor        |                                                         |
+---------------------+                                                         |
| username            |                                                         |
| password            |                                                         |
| isFaculty           |                                                         |
| students            |                                                         |
| courses             |                                                         |
| ledger              |                                                         |
| next                |                                                         |
+---------------------+                                                         |
          |                                                                     |
          |                                                                     |
          V                                                                     |
+---------------------+                                                         |
|      Course         |                                                         |
+---------------------+                                                         |
| code                |                                                         |
| name                |                                                         |
| credits             |                                                         |
| sections            |                                                         |
| numSections         |                                                         |
| prerequisite        |                                                         |
| confliction         |                                                         |
+---------------------+                                                         |
          |                                                                     |
          |                                                                     |
          V                                                                     |
+---------------------+                                                         |
|    Confliction      |                                                         |
+---------------------+                                                         |
| courseCode1         |                                                         |
| courseCode2         |                                                         |
| next                |                                                         |
+---------------------+                                                         |
                                                                                |
                                                                                |
                                                                                V
+---------------------+                                                         |
|   LedgerEntry       |                                                         |
+---------------------+                                                         |
| transactionType     |                                                         |
| transactionDetails  |                                                         |
| next                |---------------------------------------------------------+
+---------------------+



1. **Student**:
   - Connected to:
     - EnrolledCourse: Through the enrolledCourses pointer, which maintains a linked list of courses in which the student is enrolled.
     - GradeReport: Through the gradeReports pointer, which maintains a linked list of grade reports for the student.
     - DegreeView: Through the degreeView pointer, which maintains a linked list of the student's degree views.
     - Ledger: Not directly connected, but ledger entries might contain transactions related to students.

2. **Faculty**:
   - Connected to:
     - Student: Through the enrolledStudents pointer, which maintains a linked list of students advised by the faculty.
     - GradeReport: Through the allGradeReports pointer, which may contain grade reports of students advised by the faculty.
     - Advisor: Through the advisorDetails pointer, which contains information about the faculty's advisor role.

3. **Advisor**:
   - Connected to:
     - Student: Through the students pointer, which maintains a linked list of students advised by the advisor.
     - Ledger: Through the ledger pointer, which might contain transactions related to advisor activities.

4. **Course**:
   - Connected to:
     - EnrolledCourse: Courses are enrolled by students via the EnrolledCourse structure.
     - Confliction: Courses may have conflicts with other courses, which are represented through the Confliction structure.

5. **Confliction**:
   - Connected to:
     - Course: Through the courseCode1 and courseCode2 pointers, which represent conflicting courses.

6. **EnrolledCourse**:
   - Connected to:
     - Student: Through the enrolledBy field, which indicates the student who enrolled in the course.
     - Course: Through the courseCode field, which indicates the enrolled course.

7. **GradeReport**:
   - Connected to:
     - Student: Through the studentID field, which indicates the student associated with the grade report.
     - Course: Through the courseCode field, which indicates the course associated with the grade report.

8. **DegreeView**:
   - Connected to:
     - Student: Through the studentID field, which indicates the student associated with the degree view.

9. **Ledger**:
   - Connected to:
     - Student: Indirectly, ledger entries may contain transactions related to student activities.
     - Faculty: Indirectly, ledger entries may contain transactions related to faculty activities.
     - Advisor: Indirectly, ledger entries may contain transactions related to advisor activities.

10. **LedgerEntry**:
    - Connected to:
      - Ledger: Through the next pointer, forming a linked list of ledger entries.
      - 
