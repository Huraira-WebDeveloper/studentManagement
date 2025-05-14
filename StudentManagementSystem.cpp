#include <iostream>
#include <string>
#include <vector>
#include <algorithm>
#include <numeric>
#include <limits>
#include <iomanip>

class Student {
private:
    std::string name;
    int id;
    std::vector<int> grades;

public:
    Student(const std::string& name, int id);
    void addGrade(int grade);
    void clearGrades();
    double getAverage() const;
    void display() const;

    int getID() const { return id; }
    std::string getName() const { return name; }
};

Student::Student(const std::string& name, int id) : name(name), id(id) {}

void Student::addGrade(int grade) {
    if (grade >= 0 && grade <= 100)
        grades.push_back(grade);
}

void Student::clearGrades() {
    grades.clear();
}

double Student::getAverage() const {
    if (grades.empty()) return -1;
    return std::accumulate(grades.begin(), grades.end(), 0.0) / grades.size();
}

void Student::display() const {
    std::cout << "\nID: " << std::setw(5) << id
              << " | Name: " << std::setw(20) << std::left << name;

    if (!grades.empty()) {
        std::cout << "\nGrades: ";
        for (int grade : grades)
            std::cout << grade << " ";
        std::cout << "\nAverage: " << std::fixed << std::setprecision(2) << getAverage();
    } else {
        std::cout << "\nGrades: None";
    }

    std::cout << "\n" << std::string(40, '-') << "\n";
}

class GradeManager {
private:
    std::vector<Student> students;

public:
    void addStudent(const Student& s);
    bool removeStudent(int id);
    Student* findByID(int id);
    Student* findByName(const std::string& name);
    void showAll() const;
    void sortStudents();
    bool idExists(int id) const;
};

void GradeManager::addStudent(const Student& s) {
    students.push_back(s);
}

bool GradeManager::removeStudent(int id) {
    auto it = std::remove_if(students.begin(), students.end(),
        [id](const Student& s) { return s.getID() == id; });

    bool removed = (it != students.end());
    students.erase(it, students.end());
    return removed;
}

Student* GradeManager::findByID(int id) {
    for (auto& s : students) {
        if (s.getID() == id)
            return &s;
    }
    return nullptr;
}

Student* GradeManager::findByName(const std::string& name) {
    for (auto& s : students) {
        if (s.getName() == name)
            return &s;
    }
    return nullptr;
}

void GradeManager::showAll() const {
    if (students.empty()) {
        std::cout << "\nNo students found.\n";
        return;
    }

    std::cout << "\n========== STUDENT LIST ==========\n";
    for (const auto& s : students)
        s.display();
}

void GradeManager::sortStudents() {
    std::sort(students.begin(), students.end(),
        [](const Student& a, const Student& b) { return a.getID() < b.getID(); });
}

bool GradeManager::idExists(int id) const {
    return std::any_of(students.begin(), students.end(),
        [id](const Student& s) { return s.getID() == id; });
}

// Utility: Input functions
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
        std::cout << "Invalid input! Please try again.\n";
    }
}

std::string getInput(const std::string& prompt) {
    std::string input;
    std::cout << prompt;
    std::getline(std::cin, input);
    return input;
}

// Menu handlers
void addNewStudent(GradeManager& gm) {
    std::string name = getInput("Enter student name: ");
    int id;

    do {
        id = getValidInt("Enter student ID (positive number): ", 1);
        if (gm.idExists(id))
            std::cout << "This ID already exists. Try another one.\n";
    } while (gm.idExists(id));

    gm.addStudent(Student(name, id));
    std::cout << "Student added successfully!\n";
}

void manageStudent(Student* student) {
    if (!student) {
        std::cout << "Student not found!\n";
        return;
    }

    int option;
    do {
        std::cout << "\nManaging: " << student->getName() << "\n";
        student->display();

        std::cout << "1. Add Grades\n2. Clear Grades\n3. Back\n";
        option = getValidInt("Select option: ", 1, 3);

        if (option == 1) {
            int count = getValidInt("How many grades? ", 1, 10);
            for (int i = 0; i < count; ++i) {
                int grade = getValidInt("Enter grade (0-100): ", 0, 100);
                student->addGrade(grade);
            }
            std::cout << "Grades added!\n";
        } else if (option == 2) {
            student->clearGrades();
            std::cout << "Grades cleared!\n";
        }
    } while (option != 3);
}

void searchStudentMenu(GradeManager& gm) {
    std::cout << "\nSearch By:\n1. ID\n2. Name\n3. Back\n";
    int option = getValidInt("Choose: ", 1, 3);

    if (option == 3) return;

    Student* found = nullptr;
    if (option == 1) {
        int id = getValidInt("Enter ID: ");
        found = gm.findByID(id);
    } else {
        std::string name = getInput("Enter name: ");
        found = gm.findByName(name);
    }

    manageStudent(found);
}

void deleteStudent(GradeManager& gm) {
    int id = getValidInt("Enter ID to remove: ");
    if (gm.removeStudent(id))
        std::cout << "Student removed!\n";
    else
        std::cout << "Student not found!\n";
}

int main() {
    GradeManager gm;
    int choice;

    while (true) {
        std::cout << "\n====== GRADE MANAGEMENT SYSTEM ======\n"
                  << "1. Add Student\n"
                  << "2. Manage Student\n"
                  << "3. View All Students\n"
                  << "4. Remove Student\n"
                  << "5. Exit\n";

        choice = getValidInt("Choose an option: ", 1, 5);

        switch (choice) {
            case 1: addNewStudent(gm); break;
            case 2: searchStudentMenu(gm); break;
            case 3: gm.sortStudents(); gm.showAll(); break;
            case 4: deleteStudent(gm); break;
            case 5: std::cout << "Goodbye!\n"; return 0;
        }
    }
}
