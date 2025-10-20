#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <cstdlib>
#include <ctime>
using namespace std;

// these are the constants for random ranges
const int MIN_NR = 10, MAX_NR = 99, MIN_LS = 5, MAX_LS = 20;

// below is the Doublyllinkedlist class which is pretty much used to the customer line
class DoublyLinkedList {
private:
// node structure for the customers
    struct Node {
        string data;
        Node* prev;
        Node* next;
        Node(string val, Node* p = nullptr, Node* n = nullptr) {
            data = val;
            prev = p;
            next = n;
        }
    };

    Node* head;
    Node* tail;

public:
    // constructor just gives empty list
    DoublyLinkedList() { head = nullptr; tail = nullptr; }

    // inserting a node after the position
    void insert_after(string value, int position) {
        if (position < 0) {
            cout << "Position must be >= 0." << endl;
            return;
        }

        Node* newNode = new Node(value);
        if (!head) {
            head = tail = newNode;
            return;
        }

        Node* temp = head;
        for (int i = 0; i < position && temp; ++i)
            temp = temp->next;

        if (!temp) {
            cout << "Position exceeds list size. Node not inserted.\n";
            delete newNode;
            return;
        }

        newNode->next = temp->next;
        newNode->prev = temp;
        if (temp->next)
            temp->next->prev = newNode;
        else
            tail = newNode;
        temp->next = newNode;
    }
    // deletes a node on its given value
    void delete_val(string value) {
        if (!head) return;

        Node* temp = head;
        while (temp && temp->data != value)
            temp = temp->next;

        if (!temp) return;

        if (temp->prev)
            temp->prev->next = temp->next;
        else
            head = temp->next;

        if (temp->next)
            temp->next->prev = temp->prev;
        else
            tail = temp->prev;

        delete temp;
    }
    // delete a node by its given position
    void delete_pos(int pos) {
        if (!head) {
            cout << "List is empty." << endl;
            return;
        }

        if (pos == 1) {
            pop_front();
            return;
        }

        Node* temp = head;

        for (int i = 1; i < pos; i++) {
            if (!temp) {
                cout << "Position doesn't exist." << endl;
                return;
            } else
                temp = temp->next;
        }
        if (!temp) {
            cout << "Position doesn't exist." << endl;
            return;
        }

        if (!temp->next) {
            pop_back();
            return;
        }

        Node* tempPrev = temp->prev;
        tempPrev->next = temp->next;
        temp->next->prev = tempPrev;
        delete temp;
    }

    // adds a new customer at the end of line
    void push_back(string v) {
        Node* newNode = new Node(v);
        if (!tail)
            head = tail = newNode;
        else {
            tail->next = newNode;
            newNode->prev = tail;
            tail = newNode;
        }
    }

    // adds a new customer at the front of line
    void push_front(string v) {
        Node* newNode = new Node(v);
        if (!head)
            head = tail = newNode;
        else {
            newNode->next = head;
            head->prev = newNode;
            head = newNode;
        }
    }

    // gets rid of th first customer
    void pop_front() {
        if (!head) {
            cout << "List is empty." << endl;
            return;
        }

        Node* temp = head;

        if (head->next) {
            head = head->next;
            head->prev = nullptr;
        } else
            head = tail = nullptr;
        delete temp;
    }

    // removes the last customer
    void pop_back() {
        if (!tail) {
            cout << "List is empty." << endl;
            return;
        }
        Node* temp = tail;

        if (tail->prev) {
            tail = tail->prev;
            tail->next = nullptr;
        } else
            head = tail = nullptr;
        delete temp;
    }
    // free memory
    ~DoublyLinkedList() {
        while (head) {
            Node* temp = head;
            head = head->next;
            delete temp;
        }
    }
    // below prints the list forward 
    void print() {
        Node* current = head;
        if (!current) {
            cout << "List is empty." << endl;
            return;
        }
        while (current) {
            cout << current->data << " ";
            current = current->next;
        }
        cout << endl;
    }
     // belows prints the lists in reverse
    void print_reverse() {
        Node* current = tail;
        if (!current) {
            cout << "List is empty." << endl;
            return;
        }
        while (current) {
            cout << current->data << " ";
            current = current->prev;
        }
        cout << endl;
    }
};

int main() {
    srand(time(0)); // this is our random seed

    // reads all the names in the names.txt file
    vector<string> allNames;
    ifstream file("names.txt");
    string name;
    if (!file.is_open()) {
        cout << "Error: could not open names.txt" << endl;
        return 1;
    }

    while (getline(file, name)) {
        if (!name.empty()) allNames.push_back(name);
    }
    file.close();

    // below creates a customer line
    DoublyLinkedList line;
    cout << "Store opens:" << endl;

    // below adds 5 random customers from the names.txt file to the start
    for (int i = 0; i < 5; ++i) {
        int randomIndex = rand() % allNames.size();
        string person = allNames[randomIndex];
        line.push_back(person);
        cout << person << " joins the line" << endl;
    }

    cout << "Resulting line: ";
    line.print();
    cout << endl;

        // 20 min simulation that also include random events
    for (int minute = 1; minute <= 20; ++minute) {
        cout << "\n----- Minute " << minute << " -----" << endl;

        int prob = rand() % 100 + 1;

        if (prob <= 40) {
            // 40% chance a customer is served
            cout << "A customer was served." << endl;
            line.pop_front();
        }
        else if (prob <= 60) {
            // 20% chance the last customer leaves (impatient)
            cout << "A customer at the end got impatient and left!" << endl;
            line.pop_back();
        }
        else if (prob <= 70) {
            // 10% chance a VIP joins the front
            int randomIndex = rand() % allNames.size();
            string vip = allNames[randomIndex];
            line.push_front(vip);
            cout << vip << " (VIP) joins the FRONT of the line!" << endl;
        }
        else {
            // everyone else (remaining 30%) new customer joins normally
            int randomIndex = rand() % allNames.size();
            string newPerson = allNames[randomIndex];
            line.push_back(newPerson);
            cout << newPerson << " joins the line." << endl;
        }

        cout << "Line after minute " << minute << ": ";
        line.print();
    }

    cout << "\n--- Store closes ---" << endl;
    cout << "Final line: ";
    line.print();

    return 0;
}

