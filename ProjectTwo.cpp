#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include <algorithm>

using namespace std;

// course structure
struct Course {
    string courseNumber;
    string title;
    vector<string> prerequisites;

    Course() {}

    Course(string number, string name, vector<string> prereqs) {
        courseNumber = number;
        title = name;
        prerequisites = prereqs;
    }
};

// node structure for binary search tree
struct Node {
    Course course;
    Node* left;
    Node* right;

    Node(Course c) {
        course = c;
        left = nullptr;
        right = nullptr;
    }
};

// binary search tree class
class BinarySearchTree {
private:
    Node* root;

    // insert helper
    void insert(Node*& node, Course course) {
        if (node == nullptr) {
            node = new Node(course);
        }
        else if (course.courseNumber < node->course.courseNumber) {
            insert(node->left, course);
        }
        else {
            insert(node->right, course);
        }
    }

    // in-order traversal
    void inOrder(Node* node) const {
        if (node == nullptr) {
            return;
        }

        inOrder(node->left);
        cout << node->course.courseNumber << ", "
            << node->course.title << endl;
        inOrder(node->right);
    }

    // search helper
    Course* search(Node* node, string courseNumber) const {
        if (node == nullptr) {
            return nullptr;
        }

        if (node->course.courseNumber == courseNumber) {
            return &(node->course);
        }

        if (courseNumber < node->course.courseNumber) {
            return search(node->left, courseNumber);
        }

        return search(node->right, courseNumber);
    }

public:
    BinarySearchTree() {
        root = nullptr;
    }

    void Insert(Course course) {
        insert(root, course);
    }

    void PrintAll() const {
        inOrder(root);
    }

    Course* Search(string courseNumber) const {
        return search(root, courseNumber);
    }
};

// trim whitespace
string trim(const string& str) {
    size_t first = str.find_first_not_of(" \t");
    size_t last = str.find_last_not_of(" \t");
    if (first == string::npos) return "";
    return str.substr(first, (last - first + 1));
}

// load data from csv file
bool loadCourseData(string fileName, BinarySearchTree& bst) {

    ifstream file(fileName);

    if (!file.is_open()) {
        cout << "Error: Could not open file." << endl;
        return false;
    }

    string line;

    while (getline(file, line)) {

        if (line.empty()) {
            continue;
        }

        stringstream ss(line);
        string token;
        vector<string> tokens;

        while (getline(ss, token, ',')) {
            tokens.push_back(trim(token));
        }

        if (tokens.size() < 2) {
            continue;
        }

        string courseNumber = tokens[0];
        string title = tokens[1];

        vector<string> prereqs;

        for (size_t i = 2; i < tokens.size(); ++i) {
            prereqs.push_back(tokens[i]);
        }

        Course course(courseNumber, title, prereqs);
        bst.Insert(course);
    }

    file.close();
    return true;
}

// print single course
void printCourse(Course* course) {

    if (course == nullptr) {
        cout << "Course not found." << endl;
        return;
    }

    cout << course->courseNumber << ", "
        << course->title << endl;

    if (course->prerequisites.empty()) {
        cout << "Prerequisites: None" << endl;
    }
    else {
        cout << "Prerequisites: ";
        for (size_t i = 0; i < course->prerequisites.size(); ++i) {
            cout << course->prerequisites[i];
            if (i < course->prerequisites.size() - 1) {
                cout << ", ";
            }
        }
        cout << endl;
    }
}

// main menu
int main() {

    BinarySearchTree bst;
    bool dataLoaded = false;
    int choice = 0;

    cout << "Welcome to the course planner." << endl;

    while (choice != 9) {

        cout << endl;
        cout << "1. Load Data Structure." << endl;
        cout << "2. Print Course List." << endl;
        cout << "3. Print Course." << endl;
        cout << "9. Exit" << endl;
        cout << "What would you like to do? ";

        cin >> choice;

        if (cin.fail()) {
            cin.clear();
            cin.ignore(1000, '\n');
            cout << "Invalid input." << endl;
            continue;
        }

        if (choice == 1) {

            string fileName;
            cout << "Enter file name: ";
            cin >> fileName;

            dataLoaded = loadCourseData(fileName, bst);
        }
        else if (choice == 2) {

            if (!dataLoaded) {
                cout << "Please load the data structure first." << endl;
            }
            else {
                cout << "Here is a sample schedule:" << endl;
                bst.PrintAll();
            }
        }
        else if (choice == 3) {

            if (!dataLoaded) {
                cout << "Please load the data structure first." << endl;
            }
            else {
                string courseNumber;
                cout << "What course do you want to know about? ";
                cin >> courseNumber;

                transform(courseNumber.begin(),
                    courseNumber.end(),
                    courseNumber.begin(),
                    ::toupper);

                Course* course = bst.Search(courseNumber);
                printCourse(course);
            }
        }
        else if (choice == 9) {
            cout << "Thank you for using the course planner!" << endl;
        }
        else {
            cout << choice << " is not a valid option." << endl;
        }
    }

    return 0;
}