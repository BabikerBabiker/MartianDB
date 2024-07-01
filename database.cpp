#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <cassert>
#include <algorithm>
#include <iomanip>
#include <vector>   
using namespace std;
const int MAX_MARTIANS = 100;

void printMenu();

struct Martian {
    string fname_;
    string lname_;
    string SSN_;
    Martian(string fn, string ln, string ssn)
        : fname_(fn), lname_(ln), SSN_(ssn) {}
    // you may add other things to the structure	
};

enum class Criteria { first, last, ssn };

class MartianDatabase {
public:
    // creates a new empty Martian Database initialized with the
    // contents of filename.
    MartianDatabase(std::istream& infile);

    // add item m to database, return true if successfully added.
    bool addMartian(const Martian& m);

    // delete item with the ssn in database, return true if successfully 
    // deleted.
    bool deleteMartian(std::string ssn);

    // return true if the database does not contain any items, false
    // otherwise.
    bool isEmpty();

    // return true if the database is at its maximum storage, false
    // otherwise.
    bool isFull();

    // prints the list of Martians, sorted by last name, sorted
    // by first name and by MSSN.
    void printMartians(Criteria type);

    // deletes all the dynamically created memory
    ~MartianDatabase();
private:
    Martian* m_unsorted[MAX_MARTIANS]{};
    Martian* m_fname[MAX_MARTIANS]{};
    Martian* m_lname[MAX_MARTIANS]{};
    Martian* m_ssn[MAX_MARTIANS]{};
    int count{};

    // you may add other private members to the class
};

bool compareFname(Martian* a, Martian* b) {
    return (a->fname_ < b->fname_);
}

bool compareLname(Martian* a, Martian* b) {
    return (a->lname_ < b->lname_);
}

bool compareSsn(Martian* a, Martian* b) {
    return (a->SSN_ < b->SSN_);
}

void show(Martian* m_array[], int N);

int main() {
    string filename;
    cout << "Enter the name of the file: ";
    cin >> filename;
    ifstream infile(filename);

    if (!infile) {
        cout << "Error: That File Doesnt Exist!" << endl;
        exit(-1);
    }

    MartianDatabase dataBase(infile);
    if (dataBase.isEmpty()) {
        cout << "File is empty" << endl;
        exit(-2);
    }

    int i = 0;

    while (i != 6) {
        printMenu();

        if (!cin) {
            cout << "Error. exiting.." << endl;
            break;
        }

        i = -1;

        bool flag = true;
        do {
            if (flag)
                flag = false;
            else
                cout << "Enter an integer 1 - 6" << endl;

            if (!(cin >> i)) {
                cout << "Invalid input. exiting.." << endl;
                exit(-3);
            }
        } while (!((i >= 1) && (i <= 6)));

        switch (i) {
        case 1: {
            string first, last, ssn;
            cout << "Please enter a first name: ";
            cin >> first;
            cout << "Please enter a last name: ";
            cin >> last;
            cout << "Please enter a SSN: ";
            cin >> ssn;
            Martian newMartian(first, last, ssn);

            bool add = dataBase.addMartian(newMartian);
            if (!add)
                cout << "Can't add a new martian " << endl;
            break;
        }
        case 2: {
            string ssn;
            cout << "Please enter a ssn: ";
            cin >> ssn;

            bool checkDelete = dataBase.deleteMartian(ssn);
            if (!checkDelete)
                cout << "SSN not found" << endl;
            break;
        }
        case 3: {
            dataBase.printMartians(Criteria::first);
            break;
        }
        case 4: {
            dataBase.printMartians(Criteria::last);
            break;
        }
        case 5: {
            dataBase.printMartians(Criteria::ssn);
            break;
        }
        case 6:
            exit(0);
        default:
            break;
        }
    }

    return 0;
}

MartianDatabase::MartianDatabase(istream& infile) {
    this->count = 0;

    if (!infile) {
        cout << "Error: Unable to open file." << endl;
        return;
    }

    string fname, lname, ssn;
    while (count < MAX_MARTIANS && infile >> fname >> lname >> ssn) {
        m_unsorted[count] = new Martian(fname, lname, ssn);

        m_fname[count] = m_unsorted[count];
        m_lname[count] = m_unsorted[count];
        m_ssn[count] = m_unsorted[count];

        count++;
    }

    if (infile.bad()) {
        cout << "Error: Problem reading file." << endl;
    }
    else if (!infile.eof()) {
        cout << "Error: File too large." << endl;
    }
}

bool MartianDatabase::addMartian(const Martian& m) {
    if (count == MAX_MARTIANS) {
        return false;
    }

    m_unsorted[count] = new Martian(m.fname_, m.lname_, m.SSN_);
    m_fname[count] = m_unsorted[count];
    m_lname[count] = m_unsorted[count];
    m_ssn[count] = m_unsorted[count];

    count++;
    return true;
}

bool MartianDatabase::deleteMartian(string ssn) {
    Martian* p{};
    int x;
    bool isFound = false;

    for (x = 0; x < count; x++) {
        if (m_unsorted[x]->SSN_ == ssn) {
            isFound = true;
            p = m_unsorted[x];

            for (int y = x; y < count - 1; y++)
                m_unsorted[y] = m_unsorted[y + 1];

            break;
        }
    }

    if (isFound) {
        delete p;
        this->count -= 1;
    }

    return isFound;
}


bool MartianDatabase::isEmpty() {
    return this->count == 0;
}

bool MartianDatabase::isFull() {
    return this->count == MAX_MARTIANS;
}

void MartianDatabase::printMartians(Criteria type) {
    switch (type) {
    case Criteria::first:
        cout << "\n" << left << setw(24) << "Sorted By First Name";
        cout << "\n------------------------------------------------" << endl;
        sort(m_unsorted, m_unsorted + count, compareFname);
        show(m_unsorted, this->count);
        break;
    case Criteria::last:
        cout << "\n" << left << setw(24) << "Sorted By Last Name";
        cout << "\n------------------------------------------------" << endl;
        sort(m_unsorted, m_unsorted + count, compareLname);
        show(m_unsorted, this->count);
        break;
    case Criteria::ssn:
        cout << "\n" << left << setw(24) << "Sorted By SSN";
        cout << "\n------------------------------------------------" << endl;
        sort(m_unsorted, m_unsorted + count, compareSsn);
        show(m_unsorted, this->count);
        break;
    }
}

MartianDatabase::~MartianDatabase() {
    for (int x = 0; x < MAX_MARTIANS; x++) {
        delete m_unsorted[x];
    }
}

void show(Martian* m_array[], int N) {
    cout << left << setw(15) << "First Name"
        << left << setw(15) << "Last Name"
        << left << setw(4) << "SSN" << endl;
    cout << "------------------------------------------------";
    for (int x = 0; x < N; x++) {
        cout << "\n" << left << setw(15) << m_array[x]->fname_
            << left << setw(15) << m_array[x]->lname_
            << left << setw(4) << m_array[x]->SSN_;
    }
}

void printMenu() {
    cout << "\n\nMartian DataBase" << endl;
    cout << "---------------------" << endl;
    cout << "1. Add Martian" << endl;
    cout << "2. Delete Martian" << endl;
    cout << "3. Print by First Name" << endl;
    cout << "4. Print by Last Name" << endl;
    cout << "5. Print by SSN" << endl;
    cout << "6. Quit" << endl;
    cout << "---------------------" << endl;
    cout << "> ";
}