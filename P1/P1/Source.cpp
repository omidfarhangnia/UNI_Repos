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

int main() {

	const int numsLength = 7;
	int nums[numsLength] = { 0, -30, 20, 150, 2000, -300, 500 };

	cout << "   " << Mod(5) << "        " << Mod(-10) << "        " << Mod(0) << "        " << Mod(-25) << endl;
	// output : "   5    10      0       25"
	cout << "find the max (0, -30, 20, 150, 2000, -300, 500)" << " ==> " << Max(nums, numsLength) << endl;
	// output : "2000"
	cout << "find the min (0, -30, 20, 150, 2000, -300, 500)" << " ==> " << Min(nums, numsLength) << endl;
	// output : "-300"
	PERSON randomPerson = Person("randomName", "randomLastName", 30);
	cout << randomPerson.firstName << "        " << randomPerson.lastName << "        " << randomPerson.age;
	// output : randomName        randomLastName        30

	return 0;
}