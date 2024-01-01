#include <iostream>
#include <fstream>
#include <algorithm>
#include <vector>
#include <iomanip>
#include <typeinfo> // Include this header for typeid

using namespace std;

// Base class representing a person
class Person {
protected:
    

public:
    string name;
    Person(const string& n) : name(n) {}

    // Virtual function for displaying information
    virtual void display() const = 0;

    // Virtual destructor to support polymorphic behavior
    virtual ~Person() = default;
};

// Derived class representing an employee, inheriting from Person
class Employee : public Person {
public:
    int id;
    double salary;


    Employee(const string& n, int i, double s) : Person(n), id(i), salary(s) {}

    // Getter functions
    const string& getName() const { return name; }
    int getID() const { return id; }
    double getSalary() const { return salary; }

    // Overriding the display function from the base class
    void display() const override {
        cout << setw(15) << name << setw(10) << id << setw(10) << salary << endl;
    }

    // Operator overloading for equality
    bool operator==(const Employee& other) const {
        return (id == other.id && name == other.name && salary == other.salary);
    }
};

// Function prototypes
void writeDataToFile(const vector<Person*>& persons);
vector<Person*> readDataFromFile();
void addPerson(vector<Person*>& persons);
void updatePerson(vector<Person*>& persons);
void showPersons(const vector<Person*>& persons);
void deletePerson(vector<Person*>& persons);
void updateEmployee(Employee* emp);

int main() {
    vector<Person*> persons = readDataFromFile();

    int choice;
    do {
        cout << "\nPerson Management System\n";
        cout << "1. Add Person\n";
        cout << "2. Update Person Information\n";
        cout << "3. Show Persons\n";
        cout << "4. Delete Person\n";
        cout << "5. Exit\n";
        cout << "Enter your choice: ";
        cin >> choice;

        switch (choice) {
            case 1:
                addPerson(persons);
                break;
            case 2:
                updatePerson(persons);
                break;
            case 3:
                showPersons(persons);
                break;
            case 4:
                deletePerson(persons);
                break;
            case 5:
                writeDataToFile(persons);

                // Deallocate memory for dynamically allocated objects
                for (const auto& person : persons) {
                    delete person;
                }

                cout << "Exiting the program.\n";
                break;
            default:
                cout << "Invalid choice. Please try again.\n";
        }
    } while (choice != 5);

    return 0;
}

void writeDataToFile(const vector<Person*>& persons) {
    ofstream file("persons.txt");
    if (file.is_open()) {
        for (const auto& person : persons) {
            const type_info& ti = typeid(*person);
            if (ti == typeid(Employee)) {
                const Employee* emp = dynamic_cast<const Employee*>(person);
                file << "Employee " << setw(15) << emp->getName() << setw(10) << emp->getID() << setw(10) << emp->getSalary() << endl;
            }
        }
        file.close();
    } else {
        cout << "Unable to open the file for writing.\n";
    }
}

vector<Person*> readDataFromFile() {
    vector<Person*> persons;
    ifstream file("persons.txt");
    if (file.is_open()) {
        string type;
        while (file >> type) {
            if (type == "Employee") {
                string name;
                int id;
                double salary;
                file >> name >> id >> salary;
                persons.push_back(new Employee(name, id, salary));
            }
        }
        file.close();
    } else {
        cout << "Unable to open the file for reading. Creating a new file.\n";
    }
    return persons;
}

void addPerson(vector<Person*>& persons) {
    string name;
    int id;
    double salary;

    cout << "Enter person name: ";
    cin >> name;

    int personType;
    cout << "Enter person type (1 for Employee): ";
    cin >> personType;

    if (personType == 1) {
        cout << "Enter employee ID: ";
        cin >> id;
        cout << "Enter employee salary: ";
        cin >> salary;
        persons.push_back(new Employee(name, id, salary));
        cout << "Employee added successfully.\n";
    } else {
        cout << "Invalid person type.\n";
    }
}

void updatePerson(vector<Person*>& persons) {
    int id;
    cout << "Enter the employee ID to update: ";
    cin >> id;

    auto it = find_if(persons.begin(), persons.end(), [id](const Person* person) {
        if (const Employee* emp = dynamic_cast<const Employee*>(person)) {
            return emp->getID() == id;
        }
        return false;
    });

    if (it != persons.end()) {
        cout << "Enter new person information:\n";
        updateEmployee(dynamic_cast<Employee*>(*it));
        cout << "Person information updated successfully.\n";
    } else {
        cout << "Person not found.\n";
    }
}

void updateEmployee(Employee* emp) {
    string name;
    int id;
    double salary;

    cout << "Enter employee name: ";
    cin >> name;
    cout << "Enter employee ID: ";
    cin >> id;
    cout << "Enter employee salary: ";
    cin >> salary;

    emp->Employee::name = name;
    emp->id = id; // Making id accessible
    emp->salary = salary; // Making salary accessible
}

void showPersons(const vector<Person*>& persons) {
    cout << setw(15) << "Name" << setw(10) << "ID" << setw(10) << "Salary" << "\n";
    for (const auto& person : persons) {
        person->display();  // Polymorphic behavior
    }
}

void deletePerson(vector<Person*>& persons) {
    int id;
    cout << "Enter the employee ID to delete: ";
    cin >> id;

    auto it = find_if(persons.begin(), persons.end(), [id](const Person* person) {
        if (const Employee* emp = dynamic_cast<const Employee*>(person)) {
            return emp->getID() == id;
        }
        return false;
    });

    if (it != persons.end()) {
        delete *it;  // Delete the object before erasing it from the vector
        persons.erase(it);
        cout << "Person deleted successfully.\n";
    } else {
        cout << "Person not found.\n";
    }
}
