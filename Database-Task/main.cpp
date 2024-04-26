#include <algorithm>
#include <iostream>
#include <memory>
#include <vector>

struct Student {
  Student(int id, const std::string &name, int age)
    : id_(id), name_(name), age_(age)
  {}

  int id_;
  std::string name_;
  int age_;
};

class StudentDatabase {
  std::vector<std::shared_ptr<Student>> students_;
public:
  void addStudent(int id, const std::string &name, int age) {
    students_.emplace_back(std::make_shared<Student>(id, name, age));
    std::cout << "Student added successfully." << std::endl;
  }

  void removeStudent(int id) {
    auto it = std::find_if(students_.begin(), students_.end(),
                           [id](const std::shared_ptr<Student> &student) {
                             return student->id_ == id;
                           });

    if (it != students_.end()) {
      students_.erase(it);
      std::cout << "Student with ID " << id << " removed from the database."
                << std::endl;
    } else {
      std::cout << "Student with ID " << id << " not found in the database."
                << std::endl;
    }
  }

  std::shared_ptr<Student> getStudentById(int id) {
    auto it = std::find_if(students_.begin(), students_.end(),
                           [id](const std::shared_ptr<Student> &student) {
                             return student->id_ == id;
                           });

    if (it != students_.end()) {
      return *it;
    } else {
      std::cout << "Student with ID " << id << " not found in the database."
                << std::endl;
      return nullptr;
    }
  }
};

int main() {
  StudentDatabase database;

  database.addStudent(1, "Ivan", 22);
  database.addStudent(2, "Petya", 18);
  database.addStudent(3, "Anna", 25);

  std::shared_ptr<Student> student = database.getStudentById(2);
  if (student) {
    std::cout << "Student ID: " << student->id_ << ", Name: " << student->name_
              << ", Age: " << student->age_ << std::endl;
  }
  database.removeStudent(2);

  return 0;
}
