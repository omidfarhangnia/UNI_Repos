#include <iostream>
using namespace std;

int Mod(int num) {
	if (num >= 0) {
		return num;
	}
	else {
		return -num;
	}
}

int Max(int nums[], int length) {
	int maxNum = nums[0];
	
	for (int i = 1; i < length; i++) {
		if (maxNum < nums[i]) {
			maxNum = nums[i];
		}
	}

	return maxNum;
}

int Min(int nums[], int length) {
	int minNum = nums[0];

	for (int i = 1; i < length; i++) {
		if (minNum > nums[i]) {
			minNum = nums[i];
		}
	}

	return minNum;
}

struct PERSON {
	string firstName;
	string lastName;
	int age;
};

PERSON Person(string personName, string personLastName, int personAge) {
	PERSON randomPerson;

	randomPerson.firstName = personName;
	randomPerson.lastName = personLastName;
	randomPerson.age = personAge;

	return randomPerson;
}

double peopleAgeAvg(PERSON people[], int length) {
	double avg = 0;

	for (int i = 0; i < length; i++) {
		avg += people[i].age;
	}

	return (avg / length);
}

int main() {

	const int numsLength = 7;
	int nums[numsLength] = { 0, -30, 20, 150, 2000, -300, 500 };

	cout << "   " << Mod(5) << "        " << Mod(-10) << "        " << Mod(0) << "        " << Mod(-25) << endl;
	// output : "   5    10      0       25"
	cout << "find the max (0, -30, 20, 150, 2000, -300, 500)" << " ==> " << Max(nums, numsLength) << endl;
	// output : "2000"
	cout << "find the min (0, -30, 20, 150, 2000, -300, 500)" << " ==> " << Min(nums, numsLength) << endl;
	// output : "-300"
	PERSON person1 = Person("randomName", "randomLastName", 22);
	PERSON person2 = Person("randomName", "randomLastName", 60);
	PERSON person3 = Person("randomName", "randomLastName", 19);
	PERSON person4 = Person("randomName", "randomLastName", 39);
	PERSON person5 = Person("randomName", "randomLastName", 60);
	PERSON person6 = Person("randomName", "randomLastName", 2);
	PERSON person7 = Person("randomName", "randomLastName", 6);
	PERSON person8 = Person("randomName", "randomLastName", 12);
	PERSON person9 = Person("randomName", "randomLastName", 89);
	PERSON person10 = Person("randomName", "randomLastName", 15);
	const int peopleLength = 10;
	PERSON people[peopleLength] = { person1, person2, person3, person4, person5, person6, person7, person8, person9, person10 };
	cout << "Avg is : " << peopleAgeAvg(people, peopleLength);
	return 0;
}