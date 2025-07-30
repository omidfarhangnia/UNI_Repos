#include <iostream>
using namespace std;

class Person {
    string FirstName;
    string LastName;

public:
    Person(string firstname, string lastname) : FirstName(firstname), LastName(lastname) {}

    void showInfo() {
        cout << "Name: " << FirstName << " " << LastName << endl;
    }
};
class Student : public Person {
private:
    float Grade;

public:
    Student(string firstname, string lastname, float grade) : Person(firstname, lastname), Grade(grade) {}

    void showStudentInfo() {
        showInfo();
        cout << "Grade: " << this->Grade << endl;
    }
};
class Teacher : public Person {
private:
    int Salary;

public:
    Teacher(string firstname, string lastname, int salary) : Person(firstname, lastname), Salary(salary) {}

    void showTeacherInfo() {
        showInfo();
        cout << "salary: " << this->Salary << endl;
    }
};

int main() {
    Student student("firstNameOne", "lastNameOne", 19.5);
    Teacher teacher("firstNameTwo", "lastNameTwo", 15000000);

    cout << "Student Info: " << "\n";
    student.showStudentInfo();

    cout << "Teacher Info: " << "\n";
    teacher.showTeacherInfo();

    return 0;
}