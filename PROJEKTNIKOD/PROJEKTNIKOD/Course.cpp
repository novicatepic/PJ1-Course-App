#include "Course.h"
#include <iostream>
#include <fstream>
#include <filesystem>
#include <set>
#include <algorithm>

namespace fs = std::filesystem;

Course::Course(std::string courseName, std::string typeOfCourse, double minimumCourseGrade) : courseName(courseName), typeOfCourse(typeOfCourse), 	minimumCourseGrade(minimumCourseGrade){
	auto openCourse = std::ifstream("./KURSEVI/" + courseName + "/STUDENTI.txt", std::ios::in);
	if (!openCourse) {
		fs::path path = std::filesystem::current_path();
		fs::path courseFolderPath = path / "KURSEVI" / this->courseName;
		create_directory(courseFolderPath);
		std::ofstream("./KURSEVI/" + courseName + "/PREDAVAC.txt");
		std::ofstream("./KURSEVI/" + courseName + "/STUDENTI.txt");
		std::ofstream("./KURSEVI/" + courseName + "/ZAHTJEVI.txt");
		std::ofstream("./KURSEVI/" + courseName + "/POLOZILI.txt");
		/*std::ofstream writeTypeOfCourse = std::ofstream("./KURSEVI/" + courseName + "/" + typeOfCourse + ".txt", std::ios::out | std::ios::app);
		writeTypeOfCourse << typeOfCourse << std::endl;
		writeTypeOfCourse << minimumCourseGrade << std::endl;
		writeTypeOfCourse.close();

		writeTypeOfCourse = std::ofstream("./KURSEVI/" + courseName + "/" + "PREDUSLOVI.txt", std::ios::out | std::ios::app);
		std::cout << "Enter first condition -> course needed to go to this course: " << std::endl;
		std::string courseName;
		std::getline(std::cin, courseName, '\n');
		if (!doesCourseExist(courseName)) {
			std::cout << "Course doesn't exist!" << std::endl;
			writeTypeOfCourse << std::endl;
		}
		else {
			writeTypeOfCourse << courseName << std::endl;
		}

		std::cout << "Enter second condition -> type of course and number of those courses needed to finish the course: " << std::endl;
		std::getline(std::cin, courseName, '\n');
		size_t number;
		std::cin >> number;
		writeTypeOfCourse << courseName << ":" << number << std::endl;
		
		size_t counter = 0;
		std::vector<std::string> courseArray;
		std::string option;
		do {
			std::cout << "Enter option, -exit to exit option: " << std::endl;
			std::getline(std::cin, option, '\n');

			if (option == "-exit") {

			}
			else if (doesCourseExist(option)) {
				Course c(option);
				courseArray.push_back(option);
				counter++;
				//std::cout << c.getCourseName() << std::endl;
			}
			else {
				std::cout << "Unknown option!" << std::endl;
			}

		} while (option != "-exit");

		std::cout << counter << std::endl;
		for (auto elem : courseArray)
			writeTypeOfCourse << elem << std::endl;

	
		do {
			std::cout << "Enter option, -exit to exit option: " << std::endl;
			std::getline(std::cin, option, '\n');

		} while (option != "-exit");

		writeTypeOfCourse.close();*/



	}
	else {
		User u;
		while (openCourse.good()) {
			openCourse >> u;
			Student st(u.getUserName());
			studentArray.push_back(st);
		}
		openCourse.close();
		openCourse = std::ifstream("./KURSEVI/" + courseName + "/PREDAVAC.txt", std::ios::in);
		User us;
		openCourse >> us;
		Lecturer l(us.getUserName());
		this->lecturer = l;
		openCourse.close();
		openCourse = std::ifstream("./KURSEVI/" + courseName + "/" + typeOfCourse + ".txt");
		std::string type;
		std::getline(openCourse, type, '\n');
		this->typeOfCourse = type;
		double minGrade;
		openCourse >> minGrade;
		this->minimumCourseGrade = minGrade;
		openCourse.close();
		//l.signLecturerToCourse(this->courseName);
	}
	//openCourse.close();
}

Lecturer Course::returnLecturer() const {
	return this->lecturer;
}

std::string Course::getCourseName() const {
	return this->courseName;
}

std::string Course::getTypeOfCourse() const {
	return this->typeOfCourse;
}

std::set<Student> Course::findUnionIntersectionDifference() {
	std::string courseName = userInput();
	std::set<Student> resVector;
	try {
		if (!doesCourseExist(courseName)) {
			throw std::exception("This course doesn't exist!");
		}

		std::string choice = userInput3();

		std::set<Student> set1;
		std::set<Student> set2;

		std::ifstream readFromCourse;
		std::ifstream readFromCourse2;

		if (choice == "1") {
			readFromCourse = std::ifstream("./KURSEVI/" + this->courseName + "/STUDENTI.txt", std::ios::in);
			readFromCourse2 = std::ifstream("./KURSEVI/" + courseName + "/STUDENTI.txt", std::ios::in);
		}
		else if (choice == "2") {
			readFromCourse = std::ifstream("./KURSEVI/" + this->courseName + "/POLOZILI.txt", std::ios::in);
			readFromCourse2 = std::ifstream("./KURSEVI/" + courseName + "/POLOZILI.txt", std::ios::in);
			/*if (!readFromCourse) {
				auto makeNew = std::ofstream("./KURSEVI/" + this->courseName + "/POLOZILI.txt");
			}
			if (!readFromCourse2) {
				auto makeNew2 = std::fstream("./KURSEVI/" + courseName + "/POLOZILI.txt");
			}*/
		}
		else {
			throw std::exception("It must be either 1 for current students or 2 for students that finished this course!");
		}


		if (readFromCourse && readFromCourse2) {
			while (readFromCourse.good()) {
				Student s;
				readFromCourse >> s;
				if (s.getUserName() != "") {
					set1.emplace(s);
				}
			}
			readFromCourse.close();
			while (readFromCourse2.good()) {
				Student s;
				readFromCourse2 >> s;
				if (s.getUserName() != "") {
					set2.emplace(s);
				}
			}
			readFromCourse2.close();
		}

		std::string userInput = userInput2();
		//IZ NEKOG RAZLOGA NE RADI STL BIBLIOTEKA
		if (userInput == "Union") {
			/*std::set_union(set2.cbegin(), set2.cend(),
				set1.cbegin(), set1.cend(),
				std::inserter(resVector, resVector.begin()));*/
			for (auto iterator = set1.begin(); iterator != set1.end(); iterator++) {
				resVector.insert(*iterator);
			}
			for (auto iterator = set2.begin(); iterator != set2.end(); iterator++) {
				if (std::find(set1.begin(), set1.end(), *iterator) == set1.end()) {
					resVector.insert(*iterator);
				}
			}
		}
		else if (userInput == "Intersection") {
			/*std::set_intersection(set1.cbegin(), set1.cend(),
				set2.cbegin(), set2.cend(),
				std::inserter(resVector, resVector.begin()));*/
			for (auto iterator = set1.begin(); iterator != set1.end(); iterator++) {
				if(std::find(set2.begin(), set2.end(), *iterator) != set2.end())
					resVector.insert(*iterator);
			}

			//BESPOTREBNO
			/*for (auto iterator = set2.begin(); iterator != set2.end(); iterator++) {
				if (std::find(set1.begin(), set1.end(), *iterator) != set1.end() &&
					std::find(resVector.begin(), resVector.end(), *iterator) == resVector.end()) {
					resVector.insert(*iterator);
				}					
			}*/

		}
		//NE RADI SET DIFFERENCE, MORA SE NA SILU
		else if (userInput == "Difference") {
			/*std::set_difference(set1.cbegin(), set1.cend(),
				set2.cbegin(), set2.cend(),
				std::inserter(resVector, resVector.begin())); NE RADI IZ NEKOG RAZLOGA*/

			if (set1.size() == 0) {
				for (auto iterator = set2.begin(); iterator != set2.end(); iterator++) {
					resVector.insert(*iterator);
				}
			}

			if (set2.size() == 0) {
				for (auto iterator = set1.begin(); iterator != set1.end(); iterator++) {
					resVector.insert(*iterator);
				}
			}

			//for (auto iterator1 = set1.begin(); iterator1 != set1.end(); iterator1++) {
				for (auto iterator2 = set2.begin(); iterator2 != set2.end(); iterator2++) {
					if (std::find(set1.begin(), set1.end(), *iterator2) == set1.end()) {
						//std::cout << *iterator1;
						resVector.insert(*iterator2);
					}
				}
			//}
			//for (auto iterator1 = set2.begin(); iterator1 != set2.end(); iterator1++) {
				for (auto iterator2 = set1.begin(); iterator2 != set1.end(); iterator2++) {
					if (std::find(set2.begin(), set2.end(), *iterator2) == set2.end()) {
						resVector.insert(*iterator2);
					}
				}
			//}
		}
		else {
			throw std::exception("Input must be Union or Intersection or Difference");
		}
	}
	catch (const std::exception& e) {
		std::cout << e.what() << std::endl;
	}
	return resVector;
}

std::string Course::userInput() const {
	std::cout << "Input course name: " << std::endl;
	std::string courseName;
	std::getline(std::cin, courseName, '\n');
	return courseName;
}

std::string Course::userInput2() const {
	std::cout << "Union/Intersection/Difference: " << std::endl;
	std::string answer;
	std::getline(std::cin, answer, '\n');
	return answer;
}

std::string Course::userInput3() const {
	std::cout << "1 for students that are going to the course, 2 for students that finished the course :" << std::endl;
	std::string answer;
	std::getline(std::cin, answer, '\n');
	return answer;
}

bool Course::doesCourseExist(std::string courseName) {
	namespace fs = std::filesystem;
	fs::path path = std::filesystem::current_path();

	for (auto const& entry : fs::directory_iterator(path / "KURSEVI")) {
		if (entry.path().filename() == courseName)
			return true;
	}

	return false;
}

double Course::getMinimumCourseGrade() const {
	return this->minimumCourseGrade;
}