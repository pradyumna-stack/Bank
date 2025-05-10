#include <iostream>
#include <fstream>
#include <cstdlib>
#include <cstdio>
#include <cstring>  

using std::cout;
using std::cin;
using std::endl;
using std::fstream;
using std::ofstream;
using std::ifstream;
using std::ios;

class account_query {
private:
    char account_number[20];
    char firstName[10];
    char lastName[10];
    float total_Balance;

public:
    void read_data();
    void show_data();
    void write_rec();
    void read_rec();
    void search_rec();
    void edit_rec();
    void delete_rec();
};

// Function to auto-generate account number
int generate_account_number() {
    int acc_no = 1000; // Starting account number
    ifstream infile("acc_no.txt");
    if (infile) {
        infile >> acc_no;
        infile.close();
    }
    acc_no++; // Increment for next account
    ofstream outfile("acc_no.txt");
    outfile << acc_no;
    outfile.close();
    return acc_no;
}

void account_query::read_data() {
    int acc_num = generate_account_number();
    snprintf(account_number, sizeof(account_number), "%d", acc_num);
    cout << "\nGenerated Account Number: " << account_number << endl;
    cout << "Enter First Name: ";
    cin >> firstName;
    cout << "Enter Last Name: ";
    cin >> lastName;
    cout << "Enter Balance: ";
    cin >> total_Balance;
    cout << endl;
}

void account_query::show_data() {
    cout << "Account Number: " << account_number << endl;
    cout << "First Name: " << firstName << endl;
    cout << "Last Name: " << lastName << endl;
    cout << "Current Balance: Rs. " << total_Balance << endl;
    cout << "-------------------------------" << endl;
}

void account_query::write_rec() {
    ofstream outfile;
    outfile.open("record.bank", ios::binary | ios::app);
    read_data();
    outfile.write(reinterpret_cast<char*>(this), sizeof(*this));
    outfile.close();
}

void account_query::read_rec() {
    ifstream infile;
    infile.open("record.bank", ios::binary);
    if (!infile) {
        cout << "Error in Opening! File Not Found!!" << endl;
        return;
    }

    cout << "\n**** Data from file ****" << endl;

    while (infile.read(reinterpret_cast<char*>(this), sizeof(*this))) {
        show_data();
    }

    infile.close();
}

void account_query::search_rec() {
    int n;
    ifstream infile;
    infile.open("record.bank", ios::binary);
    if (!infile) {
        cout << "\nError in opening! File Not Found!!" << endl;
        return;
    }

    infile.seekg(0, ios::end);
    int count = infile.tellg() / sizeof(*this);
    cout << "\nThere are " << count << " records in the file";
    cout << "\nEnter Record Number to Search: ";
    cin >> n;

    infile.seekg((n - 1) * sizeof(*this));
    infile.read(reinterpret_cast<char*>(this), sizeof(*this));
    show_data();

    infile.close();
}

void account_query::edit_rec() {
    int n;
    fstream iofile;
    iofile.open("record.bank", ios::in | ios::binary);
    if (!iofile) {
        cout << "\nError in opening! File Not Found!!" << endl;
        return;
    }

    iofile.seekg(0, ios::end);
    int count = iofile.tellg() / sizeof(*this);
    cout << "\nThere are " << count << " records in the file";
    cout << "\nEnter Record Number to edit: ";
    cin >> n;

    iofile.seekg((n - 1) * sizeof(*this));
    iofile.read(reinterpret_cast<char*>(this), sizeof(*this));
    cout << "Record " << n << " has the following data" << endl;
    show_data();
    iofile.close();

    iofile.open("record.bank", ios::out | ios::in | ios::binary);
    iofile.seekp((n - 1) * sizeof(*this));
    cout << "\nEnter data to Modify: " << endl;
    
    // Retain the same account number
    char old_acc_num[20];
    std::strcpy(old_acc_num, account_number);

    read_data();
    std::strcpy(account_number, old_acc_num); // Restore old account number

    iofile.write(reinterpret_cast<char*>(this), sizeof(*this));
    iofile.close();
}

void account_query::delete_rec() {
    int n;
    ifstream infile;
    infile.open("record.bank", ios::binary);
    if (!infile) {
        cout << "\nError in opening! File Not Found!!" << endl;
        return;
    }

    infile.seekg(0, ios::end);
    int count = infile.tellg() / sizeof(*this);
    cout << "\nThere are " << count << " records in the file";
    cout << "\nEnter Record Number to Delete: ";
    cin >> n;

    ofstream tmpfile;
    tmpfile.open("tmpfile.bank", ios::out | ios::binary);
    infile.seekg(0);

    for (int i = 0; i < count; i++) {
        infile.read(reinterpret_cast<char*>(this), sizeof(*this));
        if (i == (n - 1))
            continue;
        tmpfile.write(reinterpret_cast<char*>(this), sizeof(*this));
    }

    infile.close();
    tmpfile.close();
    remove("record.bank");
    rename("tmpfile.bank", "record.bank");
}

int main() {
    account_query A;
    int choice;
    cout << "*** Account Information System ***" << endl;

    while (true) {
        cout << "Select one option below ";
        cout << "\n\t1 --> Add record to file";
        cout << "\n\t2 --> Show all records from file";
        cout << "\n\t3 --> Search Record from file";
        cout << "\n\t4 --> Update Record";
        cout << "\n\t5 --> Delete Record";
        cout << "\n\t6 --> Quit";
        cout << "\nEnter your choice: ";
        cin >> choice;

        switch (choice) {
        case 1:
            A.write_rec();
            break;
        case 2:
            A.read_rec();
            break;
        case 3:
            A.search_rec();
            break;
        case 4:
            A.edit_rec();
            break;
        case 5:
            A.delete_rec();
            break;
        case 6:
            exit(0);
        default:
            cout << "\nEnter correct choice!" << endl;
        }
    }

    return 0;
}
