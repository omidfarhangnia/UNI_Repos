#include <iostream>
using namespace std;

struct Person {
	string name;
	int age;
};

Person GetPerson(string personName, int personAge) {
	Person randomPerson;

	randomPerson.name = personName;
	randomPerson.age = personAge;

	return randomPerson;
}


class MyClass {
public: 
	double GetAvg() {
		double avg = 0;

		for (int i = 0; i < personLength; i++) {
			avg += People[i].age;
		}

		return (avg / personLength);
	}
private: 
	Person People[10] = {
		GetPerson("name0", 39),
		GetPerson("name1", 30),
		GetPerson("name2", 40),
		GetPerson("name3", 55),
		GetPerson("name4", 33),
		GetPerson("name5", 21),
		GetPerson("name6", 15),
		GetPerson("name7", 3),
		GetPerson("name8", 79),
		GetPerson("name9", 84),
	};
	int personLength = 10;
};

int main() {
	MyClass myObj;

	cout << myObj.GetAvg();

	return 0;
}