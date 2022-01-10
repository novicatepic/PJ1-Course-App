#include "Lecturer.h"
#include <fstream>
#include <iostream>
#include <vector>
#include <filesystem>

size_t howManyStudents(std::string courseName) {
	auto readFile = std::ifstream("./KURSEVI/" + courseName + "/ZAHTJEVI.txt", std::ios::in);
	size_t studentCounter = 0;
	if (readFile) {
		while (readFile.good()) {
			User u;
			readFile >> u;
			if (u.getUserName() != "") {
				studentCounter++;
			}
		}
	}
	return studentCounter;
}

size_t howManyLecturers(std::string courseName) {
	size_t counter = 0;
	auto readHowManyLecturers = std::ifstream("./KURSEVI/" + courseName + "/PREDAVAC.txt", std::ios::in);
	if (readHowManyLecturers) {
		while (readHowManyLecturers.good()) {
			User u;
			readHowManyLecturers >> u;
			if (u.getUserName() != "" && u.getPassword() != "" && u.getType() == "Lecturer") {
				counter++;
			}
		}
	}
	readHowManyLecturers.close();
	return counter;
}

Lecturer::Lecturer(std::string userName, std::string password) : User(userName, password) {
	this->setType("Lecturer");

	auto writeToStudents = std::ifstream("./STUDENTI/" + this->userName + "/" + "FRIENDS.TXT");
	if (!writeToStudents) {
		namespace fs = std::filesystem;
		fs::path path = std::filesystem::current_path();
		fs::path studentSubfolderPath = path / "STUDENTI" / this->userName;
		create_directory(studentSubfolderPath);
		fs::path chatPath = path / "STUDENTI" / this->userName / "CHATS";
		create_directory(chatPath);
		std::ofstream studentFile("./STUDENTI/" + this->userName + "/" + "FRIENDS.TXT");
		std::ofstream requestFile("./STUDENTI/" + this->userName + "/" + "FRIENDREQUESTS.TXT");
		studentFile.close();
		requestFile.close();
		//auto write = std::ofstream("./STUDENTI/" + this->userName + "/" + "FRIENDS.TXT", std::ios::out | std::ios::app);
		//write << *this;
	}

	if (!checkIfUserIsAlreadyInAFile(userName) && userName != "") {
		auto writeUsers = std::ofstream("./KORISNICI/Korisnici.txt", std::ios::app | std::ios::out);
		this->typePassword = true;
		writeUsers << *this;
		this->typePassword = false;
		writeUsers.close();
	}
	else {
		//std::cout << "This person is already written in this file!" << std::endl;
	}
}

void Lecturer::signStudentToCourse(std::string courseName) {
	try {
		if (!checkIfIsLecturer(courseName)) {
			throw std::exception("You are not a lecturer in this course!");
		}

		std::vector<Student> students;
		auto readFile = std::ifstream("./KURSEVI/" + courseName + "/ZAHTJEVI.txt", std::ios::in);
		if (readFile) {
			std::cout << "You have " << howManyStudents(courseName) << "students who want to go to this course!" << std::endl;
			std::cout << "Here they are: " << std::endl;
			while (readFile.good()) {
				Student u;
				readFile >> u;
				std::cout << u;
				students.push_back(u);
			}
			readFile.close();
		}
		auto writeFile = std::ofstream("./KURSEVI/" + courseName + "/STUDENTI.txt", std::ios::out | std::ios::app);
		std::cout << "Enter username of student who you want to assign to this course: " << std::endl;
		std::string userName;
		//std::cin >> userName;
		std::getline(std::cin, userName, '\n');
		auto readAgain = std::ifstream("./KURSEVI/" + courseName + "/ZAHTJEVI.txt", std::ios::in);
		if (readAgain && writeFile) {
			while (readAgain.good()) {
				Student u;
				readAgain >> u;
				if (u.getUserName() == userName) {
					writeFile << u;
				}
				if (!u.checkIfIsAlreadyAFriend(this->getUserName())) {
					u.automaticLecturerFriend(this->getUserName(), this->getPassword());
				}
			}
		}
		writeFile.close();
		readAgain.close();

		auto rewriteRequests = std::ofstream("./KURSEVI/" + courseName + "/ZAHTJEVI.txt", std::ios::out);
		if (rewriteRequests) {
			for (auto elem : students) {
				if (elem.getUserName() != userName && elem.getUserName() != "") {
					rewriteRequests << elem;
				}
			}
		}
		rewriteRequests.close();

	}
	catch (const std::exception& e) {
		std::cout << e.what() << std::endl;
	}
}



void Lecturer::signLecturerToCourse(std::string courseName) {
	try {
		auto readFromCourse = std::ifstream("./KURSEVI/" + courseName + "/PREDAVAC.txt", std::ios::in);
		if (readFromCourse) {
			readFromCourse.close();

			auto writeToCourse = std::ofstream("./KURSEVI/" + courseName + "/PREDAVAC.txt", std::ios::out | std::ios::app);
			if (writeToCourse) {
				if (howManyLecturers(courseName) == 0) {
					writeToCourse << *this;
					writeToCourse.close();
				}
				else {
					throw std::exception("There is already one lecturer in this course!");
				}

			}
			writeToCourse.close();

			auto makeFriends = std::ifstream("./KURSEVI/" + courseName + "/STUDENTI.txt", std::ios::in);


			while (makeFriends.good()) {
				Student s;
				makeFriends >> s;
				if (s.getUserName() != "") {
					s.automaticLecturerFriend(this->userName, this->password);
				}
			}
			makeFriends.close();
		}
		else {
			throw std::exception("This course doesn't exist!");
		}
		}
	catch (const std::exception& e) {
		std::cout << e.what() << std::endl;
	}
}

void Lecturer::replaceLecturer(std::string courseName) const {
	auto replaceFile = std::ofstream("./KURSEVI/" + courseName + "/PREDAVAC.txt");
}

void Lecturer::writeGrade(std::string courseName) const {
	auto openFile = std::ifstream("./KURSEVI/" + courseName + "/STUDENTI.txt", std::ios::in);
	try {
		if (!openFile) {
			throw std::exception("Course doesn't exist!");
		}

		std::cout << "Enter student name: " << std::endl;
		std::string studentName; 
		std::getline(std::cin, studentName, '\n');
		std::cout << "Enter grade: " << std::endl;
		int grade;
		std::cin >> grade;
		if (grade < 5 && grade > 10) {
			throw std::exception("Grade must be between 5 and 10, exiting option, sorry!");
		}
		//if(!checkIfIsEitherStudentOrLecturer(courseName))
		//FUNKCIJA DA PROVJERI DA LI POSTOJI KORISNIK U FAJLU

		std::vector<Student> remainingStudents;

		while (openFile.good()) {
			Student s;
			openFile >> s;
			if (s.getUserName() == studentName) {
				auto writeGrade = std::ofstream("./KURSEVI/" + courseName + "/POLOZILI.txt", std::ios::out | std::ios::app);
				writeGrade << s;
				writeGrade << grade;
				writeGrade.close();
			}
			else {
				remainingStudents.push_back(s);
			}
		}

		openFile.close();

		auto openFile2 = std::ofstream("./KURSEVI/" + courseName + "/STUDENTI.txt", std::ios::out);

		if (openFile2) {
			for (auto elem : remainingStudents) {
				openFile2 << elem;
			}
		}

		openFile2.close();

	}
	catch (const std::exception& e) {
		std::cout << e.what() << std::endl;
	}
}
