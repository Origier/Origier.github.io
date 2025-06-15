/* CS300: Project 2 - Reading Course Data and creating a Binary Search Tree
*  Author: Aaron McDonald
*  Date: 04/21/2024
*/

#include <iostream>
#include <stack>
#include <string>
#include <vector>
#include <fstream>

//============================================================================
// Binary Search Tree class definition
//============================================================================

/**
 * General purpose binary search tree containing data type T with a key of type of K
 * Can be used with any data type K that utilizes comparison operators
 * To print the values correctly type T must have an overloaded friend std::ostream& operator<< function defined
 */
template<typename T, typename K>
class BinarySearchTree {
private:
    // Internal structure for tree node
    struct Node {
        T data;
        K key;
        Node* left;
        Node* right;

        // initialize with a bid
        Node(const T data, const K key) {
            this->data = data;
            this->key = key;
            left = nullptr;
            right = nullptr;
        }
    };

    Node* root;

    void addNode(Node* const node, const T data, const K key);
    void removeNode(Node* tree, Node* parent, const K key);

public:
    BinarySearchTree();
    virtual ~BinarySearchTree();
    void Insert(const T data, const K key);
    void Remove(const K key);
    T Search(const K key) const;
    bool empty() const;
    void InOrder() const;
    void PostOrder() const;
    void PreOrder() const;
    void inOrder(Node* const node) const;
    void postOrder(Node* const node) const;
    void preOrder(Node* const node) const;
};

/**
 * Default constructor
 */
template <typename T, typename K> BinarySearchTree<T, K>::BinarySearchTree() {
    // Initialize the root of the tree
    root = nullptr;
}

/**
 * Destructor
 */
template <typename T, typename K> BinarySearchTree<T, K>::~BinarySearchTree() {
    // Continue removing the root pointer until there are no more root pointers remaining
    while (root != nullptr) {
        K key = root->key;
        removeNode(root, nullptr, key);
    }
}

/**
 * Traverse the tree in order
 */
template <typename T, typename K> void BinarySearchTree<T, K>::InOrder() const {
    // Calling the built-in function for traversing
    inOrder(root);
}

/**
 * Traverse the tree in post-order
 */
template <typename T, typename K> void BinarySearchTree<T, K>::PostOrder() const {
    // Calling the built-in function for traversing
    postOrder(root);
}

/**
 * Traverse the tree in pre-order
 */
template <typename T, typename K> void BinarySearchTree<T, K>::PreOrder() const {
    // Calling the built-in function for traversing
    preOrder(root);
}

/**
 * Insert a new node with data type T and key type of K
 */
template <typename T, typename K> void BinarySearchTree<T, K>::Insert(const T data, const K key) {
    // If there is not root node then this becomes the root node
    if (root == nullptr) {
        root = new Node(data, key);
    }
    // Otherwise recursively add this node to the tree
    else {
        addNode(root, data, key);
    }
}

/**
 * Remove the node described by the key
 */
template <typename T, typename K> void BinarySearchTree<T, K>::Remove(const K key) {
    // Recursively remove the node from the root node
    removeNode(root, nullptr, key);
}

/**
 * Search key K
 */
template <typename T, typename K> T BinarySearchTree<T, K>::Search(const K key) const {
    // set current node equal to root
    T data_found;
    Node* currNode = root;
    while (currNode != nullptr) {
        // If a match is found then return that match
        if (currNode->key == key) {
            data_found = currNode->data;
            break;
            // The current key is larger then search left
        }
        else if (currNode->key > key) {
            currNode = currNode->left;
            // Otherwise search right
        }
        else {
            currNode = currNode->right;
        }
    }
    return data_found;
}

// Returns if the current BST is empty
template <typename T, typename K> bool BinarySearchTree<T, K>::empty() const {
    return root == nullptr;
}

/**
 * Add a bid to some node (recursive)
 *
 * @param node Current node in tree
 * @param data T is the new data of type T to be added
 * @param key K is the key to that data to be added
 */
template <typename T, typename K> void BinarySearchTree<T, K>::addNode(Node* const node, const T data, const K key) {
    // If the current node is larger than the key then search the left
    Node* newNode = new Node(data, key);
    Node* currNode = node;
    while (true) {
        // Checking the left side first
        if (currNode->key > key) {
            if (currNode->left == nullptr) {
                currNode->left = newNode;
                return;
            }
            else {
                currNode = currNode->left;
            }
        }
        // Otherwise checking the right side
        else {
            if (currNode->right == nullptr) {
                currNode->right = newNode;
                return;
            }
            else {
                currNode = currNode->right;
            }
        }
    }
}

/**
 * Remove a node from the tree
 *
 * @param node Current node in tree
 * @param parent parent to the tree node
 * @param key K is the key for the node to be removed
 */
template <typename T, typename K> void BinarySearchTree<T, K>::removeNode(Node* const node, Node* const parent, const K key) {
    // If the node is null then do nothing - this key is not in the tree
    if (node == nullptr) {
        return;
    }

    //// Non-recursive version ////
    Node* currNode = node;
    Node* currPar = parent;
    // Looping structure to search the tree
    bool notFound = true;
    while (notFound) {
        // Searching left
        if (currNode->key > key) {
            currPar = currNode;
            currNode = currNode->left;
        }
        // Searching right
        else if (currNode->key < key) {
            currPar = currNode;
            currNode = currNode->right;
        }
        // Otherwise this must be the node to remove
        else {
            notFound = false;
        }
        // Case where this node doesn't exist, simply return
        if (currNode == nullptr) {
            return;
        }
    }

    // Loop to remove successor nodes if needed
    bool removeMore = true;
    while (removeMore) {
        removeMore = false;
        // Case where the node is an internal node with 2 children
        if (currNode->left != nullptr && currNode->right != nullptr) {
            // Find the successor and the successors parent
            Node* succNode = currNode->right;
            Node* succParent = currNode;
            // Anything that was to the right of the original node must be greater than that node
            // Therefore we are looking for the smallest node that meets that criteria
            while (succNode->left != nullptr) {
                succParent = succNode;
                succNode = succNode->left;
            }

            // Copying the values for the succNode into the node
            currNode->data = succNode->data;
            currNode->key = succNode->key;
            currNode->left = succNode->left;
            currNode->right = succNode->right;

            // Setting the local variables to remove the successor node 
            currNode = succNode;
            currPar = succParent;
            removeMore = true;
        }
        // Case where the node is the root with 1 or 0 children
        else if (currNode == root) {
            // Deallocate the current root and replace it with the left node
            if (currNode->left != nullptr) {
                Node* temp = root->left;
                delete root;
                root = temp;
                // Otherwise deallocate the current root and replace it with the right node
            }
            else {
                Node* temp = root->right;
                delete root;
                root = temp;
            }
        }
        // Case where it is an internal node with a left node
        else if (currNode->left != nullptr) {
            // Replacing the left parent pointer with the child
            if (currPar->left == currNode) {
                currPar->left = currNode->left;
                // Otherwise replace the right parent pointer with the child
            }
            else {
                currPar->right = currNode->left;
            }
            // Deallocating the node
            delete currNode;
        }
        // Case where it is an internal node with a right node or no nodes
        else {
            // Replacing the left parent pointer with the child
            if (currPar->left == currNode) {
                currPar->left = currNode->right;
                // Otherwise replace the right parent pointer with the child
            }
            else {
                currPar->right = currNode->right;
            }
            // Deallocating the node
            delete currNode;
        }
    }
}

// Prints the given nodes subtree in order
template <typename T, typename K> void BinarySearchTree<T, K>::inOrder(Node* const node) const {
    //// Non recursive version ////
    // Ensuring the current node is not null
    if (node == nullptr) {
        return;
    }

    // Struct to help set flags for if certain aspects of a node have been printed
    struct NodePrinted {
        Node* node = nullptr;
        bool printedLeft = false;

        NodePrinted(Node* node) {
            this->node = node;
        }
    };

    std::stack<NodePrinted*> nodesToPrint;
    NodePrinted* currNode = new NodePrinted(node);
    // Push the sub-tree root node provided onto the stack
    nodesToPrint.push(currNode);
    // While there are still items in the stack
    while (!nodesToPrint.empty()) {
        // Grabbing the top of the stack
        currNode = nodesToPrint.top();
        // Have we printed the left of this node
        if (currNode->printedLeft) {
            // Print the current node and remove it from the stack
            Node* dataNode = currNode->node;
            std::cout << dataNode->data << std::endl;
            nodesToPrint.pop();
            // Adding the right node to the stack if it is not null
            if (currNode->node->right != nullptr) {
                nodesToPrint.push(new NodePrinted(currNode->node->right));
            }
            // Deallocating memory created
            delete currNode;
        }
        // If the left has not been printed then add it to the stack
        else {
            // If the left node is null then we will not print it
            if (currNode->node->left == nullptr) {
                currNode->printedLeft = true;
            }
            // Otherwise add it to the stack
            else {
                nodesToPrint.push(new NodePrinted(currNode->node->left));
                // Marking as true as this is now in the stack
                currNode->printedLeft = true;
            }
        }
    }
}

// Prints the given nodes subtree in post order
template <typename T, typename K> void BinarySearchTree<T, K>::postOrder(Node* const node) const {
    //if node is not equal to null ptr
    if (node != nullptr) {
        //postOrder left
        postOrder(node->left);
        //postOrder right
        postOrder(node->right);
        //output the data type T
        std::cout << node->data << std::endl;
    }
}

// Prints the given nodes subtree in pre order
template <typename T, typename K> void BinarySearchTree<T, K>::preOrder(Node* const node) const {
    //if node is not equal to null ptr
    if (node != nullptr) {
        //output the data type T
        std::cout << node->data << std::endl;
        //postOrder left
        preOrder(node->left);
        //postOrder right
        preOrder(node->right);
    }
}

// Namespace to encapsulate the functionality of the project
namespace bst_courses {
    // Default values for comparison
    const std::string DEFAULT_COURSE_NUMBER = "NoNumber";
    const std::string DEFAULT_COURSE_NAME = "NoName";

    // Course data structure containing prerequisites and course data
    struct Course {
        std::string courseNumber;
        std::string name;
        std::vector<std::string> prerequisites;

        // Default constructor
        Course() {
            courseNumber = DEFAULT_COURSE_NUMBER;
            name = DEFAULT_COURSE_NAME;
        }

        // Main constructor
        Course(const std::string courseNumber, const std::string name) {
            this->courseNumber = courseNumber;
            this->name = name;
        }

        // Adds prerequisites to this course
        void appendPrerequisite(std::string prereq) {
            prerequisites.push_back(prereq);
        }

        // Allows the course to be utilized with an ostream operator
        friend std::ostream& operator<<(std::ostream& os, const Course c) {
            return (os << c.courseNumber << ", " << c.name);
        }
    };

    // Reads in course data from the provided file name, using the delimiter to separate values on the same line and loads them into the provided tree
    void read_course_data(const std::string f_name, const char delimiter, BinarySearchTree<Course, std::string>& courses) {
        // Opening the file and ensuring there were no issues with opening it
        std::ifstream data_file;
        data_file.open(f_name);
        if (!data_file.good()) {
            std::cout << "There was an issue trying to open your file." << std::endl;
            if (data_file.fail()) {
                std::cout << "There was a logical error in opening your file" << std::endl;
            }
            else {
                std::cout << "There was a read/write error in opening your file" << std::endl;
            }
            return;
        }

        // Read each line of the file until the end of the file
        std::string next_line;
        while (std::getline(data_file, next_line)) {
            std::vector<std::string> line_words;
            // Finding the next delimiter until there isn't any
            int starting_pos = 0;
            int delimiter_loc = next_line.find(delimiter);
            std::string next_word;
            while (delimiter_loc != std::string::npos) {
                next_word = next_line.substr(starting_pos, delimiter_loc - starting_pos);
                // Adding the comma separated value to the list
                line_words.push_back(next_word);
                // Shifting the starting position and finding the next delimiter
                starting_pos  = delimiter_loc + 1;
                // Finding the next delimiter
                delimiter_loc = next_line.find(delimiter, starting_pos);
            }
            // Finally, add the last item that had no delimiter
            next_word = next_line.substr(starting_pos, next_line.size() - 1);
            line_words.push_back(next_word);

            // Checking the vector size to ensure there are at least 2 items
            if (line_words.size() < 2) {
                std::cout << "The file isn't formatted correctly, there should be at least a course number and course name separated by one \'" << delimiter << "\' ." << std::endl;
                return;
            }

            // Otherwise creating the course data structure
            Course new_course(line_words.at(0), line_words.at(1));
            bool add_course = true;
            // Adding prerequisites if needed
            if (line_words.size() > 2) {
                for (int i = 2; i < line_words.size(); ++i) {
                    std::string prereq = line_words.at(i);
                    Course prereq_found = courses.Search(prereq);
                    // Ensuring the prerequisite course is within the binary search tree
                    if (prereq_found.courseNumber == DEFAULT_COURSE_NUMBER) {
                        std::cout << new_course << " is missing the following prerequisite course from the course list: " << prereq << std::endl;
                        std::cout << "This course will not be added..." << std::endl;
                        std::cout << std::endl;
                        add_course = false;
                        break;
                    }
                    // The prerequisite course was found
                    else {
                        new_course.appendPrerequisite(prereq);
                    }
                }
            }

            // Finally insert the new course into the binary search tree if all conditions were met
            if (add_course) {
                courses.Insert(new_course, new_course.courseNumber);
            }
        }
    }
}

using namespace bst_courses;
using namespace std;

int main()
{
    // Welcome statement and setup
    cout << "Welcome to the course planner." << endl;
    string user_input = "";
    int user_chioce = 0;
    BinarySearchTree<Course, string> courses;
    Course user_course;

    // Flag for continuing the main function loop
    bool continue_loop = true;
    // Main interface loop
    while (continue_loop) {
        // User interface
        cout << "   1. Load Data Structure." << endl;
        cout << "   2. Print course list." << endl;
        cout << "   3. Print course." << endl;
        cout << "   9. Exit" << endl;
        cout << "What would you like to do? ";
        cin >> user_input;
        user_chioce = stoi(user_input);

        // Checking the user input and providing needed functionality
        switch (user_chioce) {
        // Reading and loading data from a file
        case 1:
            cout << "What is the name of the file you would like to use? Please make sure it is the complete path: ";
            cin >> user_input;
            read_course_data(user_input, ',', courses);

            // Ensuring the data was read correctly
            if (courses.empty()) {
                cout << "Unsuccessful at reading the data file..." << endl;
            }
            else {
                cout << "Data file successfully read and loaded into a binary search tree." << endl;
            }
            break;
        case 2:
            // Ensuring there is currently working data
            if (courses.empty()) {
                cout << "There are currently no courses to print. Try loading courses first and then print them." << endl;
            }
            // Printing what has been loaded
            else {
                cout << "Here are the currently loaded courses:" << endl;
                courses.InOrder();
            }
            break;
        case 3:
            // Ensuring there is currently working data
            if (courses.empty()) {
                cout << "There are currently no courses to print. Try loading courses first and then print them." << endl;
            }
            // Reading in user input and finding that course
            else {
                cout << "What course do you want to know more about? ";
                cin >> user_input;
                user_course = courses.Search(user_input);
                // Ensuring the course exists in the tree
                if (user_course.courseNumber == DEFAULT_COURSE_NUMBER) {
                    cout << "Sorry that isn't one of the available courses." << endl;
                }
                // Printing the course data
                else {
                    cout << user_course << endl;
                    // Printing prerequisites
                    if (user_course.prerequisites.size() > 0) {
                        cout << "Prerequisites: ";
                        for (int i = 0; i < user_course.prerequisites.size(); ++i) {
                            cout << user_course.prerequisites.at(i);
                            if (i != user_course.prerequisites.size() - 1) {
                                cout << ", ";
                            }
                        }
                        cout << endl;
                    }
                    else {
                        cout << "This course has no prerequisites." << endl;
                    }
                }
            }
            break;
        // Program exit condition
        case 9:
            continue_loop = false;
            cout << "Thank you for using the course planner!" << endl;
            break;
        // Error catching for invalid input
        default:
            cout << user_chioce << " isn't a valid option." << endl;
        }
    }
}