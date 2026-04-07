#include "Student.h"

Student::Student() {}

Student::Student(const std::string& name, const std::string& id)
    : name(name), id(id) {}

std::string Student::getName() const {
    return name;
}

std::string Student::getId() const {
    return id;
}