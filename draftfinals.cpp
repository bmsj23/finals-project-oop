#include <iostream>
#include <string>
using namespace std;

// Exception Class for Validation
class ValidationException {
private:
    string message;
public:
    explicit ValidationException(const string& msg) : message(msg) {}
    string what() const { return message; }
};

// Abstract User Class for Generalizing Admin and Customer
class User {
protected:
    string name, username, email, contactNumber, password;
    bool isAdmin;  // Field to distinguish Admins and Customers
public:
    User(string nm, string uname, string mail, string contact, string pass, bool adminFlag)
        : name(nm), username(uname), email(mail), contactNumber(contact), password(pass), isAdmin(adminFlag) {}

    virtual void displayProfile() const {
        cout << "Name: " << name << "\nUsername: " << username
             << "\nEmail: " << email << "\nContact: " << contactNumber
             << "\nRole: " << (isAdmin ? "Admin" : "Customer") << endl;
    }

    bool getIsAdmin() const { return isAdmin; }
    string getUsername() const { return username; }
    string getPassword() const { return password; }

    virtual void menu() const = 0;  // Pure virtual function for strategy pattern

    // Logout functionality
    void logout() const {
        cout << "Logging out...\n";
    }
};

// Admin Class - Derived from User (Polymorphism)
class Admin : public User {
public:
    Admin(string nm, string uname, string mail, string contact, string pass)
        : User(nm, uname, mail, contact, pass, true) {}

    void menu() const override {
        int choice;
        bool continueMenu = true;
        while (continueMenu) {
            cout << "\nAdmin Menu:\n1 - View All Users\n2 - Logout\nEnter your choice: ";
            cin >> choice;

            switch (choice) {
            case 1:
                cout << "Displaying users...\n";
                break;
            case 2:
                logout();  // Call logout method
                continueMenu = false;  // End the loop
                break;
            default:
                cout << "Invalid choice. Please try again.\n";
            }
        }
    }
};

// Customer Class - Derived from User (Polymorphism)
class Customer : public User {
public:
    Customer(string nm, string uname, string mail, string contact, string pass)
        : User(nm, uname, mail, contact, pass, false) {}

    void menu() const override {
        int choice;
        bool continueMenu = true;
        while (continueMenu) {
            cout << "\nCustomer Menu:\n1 - View Profile\n2 - Purchase Tickets\n3 - View Movies\n4 - Logout\nEnter your choice: ";
            cin >> choice;

            switch (choice) {
            case 1:
                cout << "Viewing profile...\n";
                break;
            case 2:
                purchaseTickets();
                break;
            case 3:
                browseMovies();  // Call browseMovies method
                break;
            case 4:
                logout();  // Call logout method
                continueMenu = false;  // End the loop
                break;
            default:
                cout << "Invalid choice. Please try again.\n";
            }
        }
    }

    // Ticket Purchase Logic
    void purchaseTickets() const {
        int ticketsToBuy;
        int totalTickets = 0;
        char continueOrder = 'Y';  // Initialize with 'Y' to start the loop

        while (toupper(continueOrder) == 'Y') {
            cout << "Enter number of tickets to purchase (max 15 per order): ";
            cin >> ticketsToBuy;

            if (ticketsToBuy > 15) {
                cout << "You can only purchase up to 15 tickets per order.\n";
            } else {
                totalTickets += ticketsToBuy;
                cout << ticketsToBuy << " tickets purchased successfully.\n";
                if (totalTickets > 15) {
                    cout << "You've reached the 15-ticket limit for this order. You can start a new order for more tickets.\n";
                }
            }

            cout << "Do you want to continue purchasing? (Y/N): ";
            cin >> continueOrder;
        }

        cout << "Total tickets purchased: " << totalTickets << endl;
    }

    // Movie Listings - Viewing Available Movies
    void browseMovies() const {
        string movies[5][6] = {
            {"Movie A", "Action-packed thriller", "120 min", "10", "30", "Location A"},
            {"Movie B", "Romantic comedy", "90 min", "12", "50", "Location B"},
            {"Movie C", "Sci-fi adventure", "140 min", "15", "20", "Location A"},
            {"Movie D", "Horror mystery", "100 min", "8", "10", "Location C"},
            {"Movie E", "Animated fantasy", "110 min", "10", "25", "Location B"}
        };

        cout << "\nAvailable Movies:\n";
        for (int i = 0; i < 5; i++) {
            cout << "Movie Name: " << movies[i][0] << "\n";
            cout << "Synopsis: " << movies[i][1] << "\n";
            cout << "Runtime: " << movies[i][2] << "\n";
            cout << "Price: $" << movies[i][3] << "\n";
            cout << "Available Seats: " << movies[i][4] << "\n";
            cout << "Location: " << movies[i][5] << "\n\n";
        }
    }
};

// CinemaSystem Class (Singleton Pattern)
class CinemaSystem {
private:
    static CinemaSystem instance;
    User** users; 
    int userCount;
    int maxUsers;

    CinemaSystem() : userCount(0), maxUsers(100) {
        users = new User*[maxUsers];  // Dynamically allocate array of pointers
    }

public:
    ~CinemaSystem() {
        for (int i = 0; i < userCount; i++) {
            delete users[i];  // Delete each user
        }
        delete[] users;  // Delete the array
    }

    static CinemaSystem& getInstance() {
        return instance;
    }

    void addUser(User* user) {  
        if (userCount < maxUsers) {
            users[userCount++] = user;
        } else {
            throw ValidationException("Maximum users reached!");
        }
    }

    User* authenticateUser(const string& username, const string& password) {
        for (int i = 0; i < userCount; i++) {
            if (users[i]->getUsername() == username && users[i]->getPassword() == password) {
                return users[i];
            }
        }
        return nullptr;  // If no user is found, return null
    }

    void displayUsers() const {
        for (int i = 0; i < userCount; i++) {
            users[i]->displayProfile();
        }
    }

    int getUserCount() const {
        return userCount;
    }

};

// Define the static instance outside the class
CinemaSystem CinemaSystem::instance;

// Main Menu Function
void mainMenu(CinemaSystem& system) {
    string adminUsernames[5] = {"admin1", "admin2", "admin3", "admin4", "admin5"};
    int choice;
    bool continueMenu = true;

    while (continueMenu) {
        cout << "\nMain Menu:\n";
        cout << "1 - Login\n2 - Sign Up\n3 - Exit\n";
        cout << "Enter your choice: ";
        cin >> choice;

        switch (choice) {
            case 1: {
                string username, password;
                User* user = nullptr;

                while (user == nullptr) {
                    cout << "Enter username: ";
                    cin >> username;
                    cout << "Enter password: ";
                    cin >> password;

                    user = system.authenticateUser(username, password);
                    if (user == nullptr) {
                        cout << "Invalid credentials. Please try again.\n";
                    }
                }

                cout << "Login successful.\n";

                // Safely call the correct menu function based on user type
                Customer* customer = dynamic_cast<Customer*>(user);
                if (customer) {
                    customer->menu();  // If user is a Customer, call Customer's menu
                } else {
                    cout << "This user is not a customer.\n";
                }

                break;
            }
            case 2: {
                string name, username, email, contact, password;
                bool isAdmin = false;

                cout << "Enter name: ";
                cin.ignore();
                getline(cin, name);
                cout << "Enter username: ";
                cin >> username;
                cout << "Enter email: ";
                cin >> email;
                cout << "Enter contact number: ";
                cin >> contact;
                cout << "Enter password: ";
                cin >> password;

                // Check if it's an Admin based on predefined usernames
                for (const string& adminUsername : adminUsernames) {
                    if (username == adminUsername) {
                        isAdmin = true;
                        break;
                    }
                }

                try {
                    User* newUser;
                    if (isAdmin) {
                        newUser = new Admin(name, username, email, contact, password);
                    } else {
                        newUser = new Customer(name, username, email, contact, password);
                    }
                    system.addUser(newUser);
                    cout << "Sign-up successful. You can now log in.\n";
                } catch (ValidationException& ex) {
                    cout << "Error: " << ex.what() << endl;
                }

                break;
            }
            case 3:
                cout << "Exiting the system. Goodbye!\n";
                continueMenu = false;
                break;
            default:
                cout << "Invalid choice. Please try again.\n";
        }
    }
}



int main() {
    CinemaSystem& system = CinemaSystem::getInstance();
    mainMenu(system);

    return 0;
}
