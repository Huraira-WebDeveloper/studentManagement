

#include <iostream>
#include <string>
#include <vector>
#include <algorithm>
#include <numeric>
#include <limits>
#include <iomanip>  // For formatted output

class Student {
private:
    std::string name;
    int id;
    std::vector<int> grades;

public:
    Student(std::string name, int id);
    void addGrade(int grade);
    double calculateAverage() const;
    void displayInfo() const;
    int getID() const { return id; }
    std::string getName() const { return name; }
};

Student::Student(std::string n, int id) : name(n), id(id) {}

void Student::addGrade(int grade) {
    if (grade >= 0 && grade <= 100) {
        grades.push_back(grade);
    }
}

double Student::calculateAverage() const {
    if (grades.empty()) return -1;
    return std::accumulate(grades.begin(), grades.end(), 0.0) / grades.size();
}

void Student::displayInfo() const {
    std::cout << "\nStudent ID: " << std::setw(8) << std::left << id
              << " Name: " << std::setw(20) << std::left << name;
    
    if (!grades.empty()) {
        std::cout << "\nGrades: ";
        for (int grade : grades) std::cout << grade << " ";
        std::cout << "\nAverage: " << std::fixed << std::setprecision(2) 
                  << calculateAverage();
    }
    std::cout << "\n" << std::string(40, '-') << "\n";
}

class GradeManager {
private:
    std::vector<Student> students;

public:
    void addStudent(const Student& s);
    bool removeStudent(int id);
    Student* searchByID(int id);
    Student* searchByName(const std::string& name);
    void sortByID();
    void displayAllStudents() const;
    bool isIDExists(int id) const;
};

// GradeManager member functions
void GradeManager::addStudent(const Student& s) {
    students.push_back(s);
}

bool GradeManager::removeStudent(int id) {
    auto it = std::remove_if(students.begin(), students.end(),
        [id](const Student& s) { return s.getID() == id; });
    bool removed = it != students.end();
    students.erase(it, students.end());
    return removed;
}

Student* GradeManager::searchByID(int id) {
    auto it = std::find_if(students.begin(), students.end(),
        [id](const Student& s) { return s.getID() == id; });
    return it != students.end() ? &*it : nullptr;
}

Student* GradeManager::searchByName(const std::string& name) {
    auto it = std::find_if(students.begin(), students.end(),
        [&name](const Student& s) { return s.getName() == name; });
    return it != students.end() ? &*it : nullptr;
}

void GradeManager::sortByID() {
    std::sort(students.begin(), students.end(),
        [](const Student& a, const Student& b) { return a.getID() < b.getID(); });
}

void GradeManager::displayAllStudents() const {
    if (students.empty()) {
        std::cout << "\nNo students in the system.\n";
        return;
    }
    std::cout << "\n" << std::string(40, '=') << "\n";
    std::cout << "          STUDENT LIST\n";
    std::cout << std::string(40, '=') << "\n";
    for (const auto& student : students) student.displayInfo();
}

bool GradeManager::isIDExists(int id) const {
    return std::any_of(students.begin(), students.end(),
        [id](const Student& s) { return s.getID() == id; });
}

// Input validation utilities
int getValidInt(const std::string& prompt, int min = INT_MIN, int max = INT_MAX) {
    int value;
    while (true) {
        std::cout << prompt;
        if (std::cin >> value && value >= min && value <= max) {
            std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
            return value;
        }
        std::cin.clear();
        std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
        std::cout << "Invalid input. Please enter a value between "
                  << min << " and " << max << ".\n";
    }
}

std::string getString(const std::string& prompt) {
    std::string input;
    std::cout << prompt;
    std::getline(std::cin, input);
    return input;
}

// Menu functions
void addStudent(GradeManager& gm) {
    std::string name = getString("Enter student name: ");
    int id;

    do {
        id = getValidInt("Enter student ID (positive number): ", 1);
        if (gm.isIDExists(id)) {
            std::cout << "ID already exists! Try again.\n";
        }
    } while (gm.isIDExists(id));

    gm.addStudent(Student(name, id));
    std::cout << "\nStudent added successfully!\n";
}

void manageGrades(Student* student) {
    if (!student) {
        std::cout << "Student not found!\n";
        return;
    }

    std::cout << "\nCurrent student: " << student->getName() << "\n";
    student->displayInfo();

    int choice;
    do {
        std::cout << "\n1. Add grades\n2. Clear all grades\n3. Return\nChoice: ";
        choice = getValidInt("", 1, 3);
        
        if (choice == 1) {
            int count = getValidInt("How many grades to add? ", 1, 10);
            for (int i = 0; i < count; i++) {
                int grade = getValidInt("Enter grade (0-100): ", 0, 100);
                student->addGrade(grade);
            }
            std::cout << count << " grades added!\n";
        } else if (choice == 2) {
            // Create new student with same ID/name but empty grades
            Student newStudent(student->getName(), student->getID());
            *student = newStudent;
            std::cout << "All grades cleared!\n";
        }
    } while (choice != 3);
}

void searchStudents(GradeManager& gm) {
    std::cout << "\nSearch by:\n1. ID\n2. Name\n3. Return\nChoice: ";
    int choice = getValidInt("", 1, 3);
    
    if (choice == 3) return;

    Student* found = nullptr;
    if (choice == 1) {
        int id = getValidInt("Enter student ID: ");
        found = gm.searchByID(id);
    } else {
        std::string name = getString("Enter student name: ");
        found = gm.searchByName(name);
    }

    manageGrades(found);
}

void removeStudent(GradeManager& gm) {
    int id = getValidInt("Enter student ID to remove: ");
    if (gm.removeStudent(id)) {
        std::cout << "Student removed successfully!\n";
    } else {
        std::cout << "Student not found!\n";
    }
}

int main() {
    GradeManager gm;
    
    while (true) {
        std::cout << "\n===== GRADE MANAGEMENT SYSTEM =====\n"
                  << "1. Add Student\n2. Manage Student\n3. View All Students\n"
                  << "4. Remove Student\n5. Exit\n";
        
        int choice = getValidInt("Enter your choice: ", 1, 5);
        
        switch (choice) {
            case 1: addStudent(gm); break;
            case 2: searchStudents(gm); break;
            case 3: gm.sortByID(); gm.displayAllStudents(); break;
            case 4: removeStudent(gm); break;
            case 5: std::cout << "Exiting program...\n"; return 0;
        }
    }
}