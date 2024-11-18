#include <iostream>
#include <string>
#include <limits>
using namespace std;

// Exception Class for Validation
class ValidationException {
private:
    string message;
public:
    explicit ValidationException(const string& msg) : message(msg) {}
    string what() const { return message; }
};

// Abstract User Class for Admin and Customer
class User {
protected:
    string name, username, email, contactNumber, password;
    bool isAdmin;
public:
    User(string nm, string uname, string mail, string contact, string pass, bool adminFlag)
        : name(nm), username(uname), email(mail), contactNumber(contact), password(pass), isAdmin(adminFlag) {}

    virtual void displayProfile() const {
        cout << "Name: " << name << "\nUsername: " << username
             << "\nEmail: " << email << "\nContact: " << contactNumber
             << "\nRole: " << (isAdmin ? "Admin" : "Customer") << endl;
    }

    bool getIsAdmin() { return isAdmin; }
    string getUsername() { return username; }
    string getPassword() { return password; }

    virtual void menu() = 0;  // Pure virtual function for strategy pattern

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

    void menu() override {
        int choice;
        bool continueMenu = true;
        while (continueMenu) {
            cout << "\nAdmin Menu:\n1 - View All Users\n2 - Logout\n3 - Exit\nEnter your choice: ";
            cin >> choice;

            switch (choice) {
            case 1:
                cout << "Displaying users...\n";
                break;
            case 2:
                logout();  // logout method
                continueMenu = false;
                break;
            case 3:
                cout << endl << "Exiting the system."; // exit rekta
                exit(0);
                break;
            default:
                cout << "Invalid choice. Please try again.\n";
            }
        }
    }
};

class Customer : public User {
private:
    // dto naka store ticketzz
    struct Ticket {
        string movieName;
        string location;
        string showTime;
        double price;
        int quantity;
        bool isValid = false;
    };
    
    Ticket ticketHistory[50];
    int ticketCount = 0;

public:
    Customer(string nm, string uname, string mail, string contact, string pass)
        : User(nm, uname, mail, contact, pass, false) {}

    void menu() override {
        int choice;
        bool continueMenu = true;

        while (continueMenu) {
            cout << "\nCustomer Menu:\n"
                 << "1 - View Profile\n"
                 << "2 - Purchase Tickets\n"
                 << "3 - View Movies\n"
                 << "4 - My Tickets\n"
                 << "5 - Logout\n"
                 << "6 - Exit\n"
                 << "Enter your choice: ";
            cin >> choice;

            switch (choice) {
            case 1:
                cout << "Viewing profile...\n";
                break;
            case 2:
                purchaseTickets();
                break;
            case 3:
                browseMovies();
                break;
            case 4:
                viewTickets();
                break;
            case 5:
                logout();
                continueMenu = false;
                break;
            case 6:
                cout << endl << "Exiting the system." << endl;
                exit(0);
                break;
            default:
                cout << "Invalid choice. Please try again.\n";
            }
        }
    }

    void browseMovies() {
        string movies[5][6] = {
            {"Movie A", "Action-packed thriller", "120 min", "10", "30", "Location A"},
            {"Movie B", "Romantic comedy", "90 min", "12", "50", "Location B"},
            {"Movie C", "Sci-fi adventure", "140 min", "15", "20", "Location A"},
            {"Movie D", "Horror mystery", "100 min", "8", "10", "Location C"},
            {"Movie E", "Animated fantasy", "110 min", "10", "25", "Location B"}
        };

        cout << "\nAvailable Movies:\n";
        for (int i = 0; i < 5; i++) {
            cout << "\n" << (i + 1) << ". Movie Name: " << movies[i][0] << "\n";
            cout << "   Synopsis: " << movies[i][1] << "\n";
            cout << "   Runtime: " << movies[i][2] << "\n";
            cout << "   Price: $" << movies[i][3] << "\n";
            cout << "   Available Seats: " << movies[i][4] << "\n";
            cout << "   Location: " << movies[i][5] << "\n";
        }
    }

void purchaseTickets() {
    string movies[5][6] = {
        {"Movie A", "Action-packed thriller", "120 min", "10", "30", "Location A"},
        {"Movie B", "Romantic comedy", "90 min", "12", "50", "Location B"},
        {"Movie C", "Sci-fi adventure", "140 min", "15", "20", "Location A"},
        {"Movie D", "Horror mystery", "100 min", "8", "10", "Location C"},
        {"Movie E", "Animated fantasy", "110 min", "10", "25", "Location B"}
    };

    char purchaseMore;
    do {
        // show available movies
        browseMovies();

        // select movie with validation
        int movieChoice;
        bool validMovie = false;
        do {
            cout << "\nEnter the number of the movie you want to watch (1-5): ";
            if (!(cin >> movieChoice)) {
                cin.clear();
                cin.ignore(numeric_limits<streamsize>::max(), '\n');
                cout << "Please enter a valid number.\n";
                continue;
            }
            if (movieChoice >= 1 && movieChoice <= 5) {
                validMovie = true;
            } else {
                cout << "Invalid movie selection. Please try again.\n";
            }
        } while (!validMovie);
        
        movieChoice--; // array indexing adjustment (kasi 0 ang start ng index tapos sa choices, 1 ang start so babawasan ng isa para mag match)

        // show and select location with validation
        cout << "\nAvailable Locations for " << movies[movieChoice][0] << ":\n";
        cout << "1. Location A\n";
        cout << "2. Location B\n";
        cout << "3. Location C\n";
        
        int locationChoice;
        bool validLocation = false;

        do {
            cout << "Enter location number (1-3): ";
            if (!(cin >> locationChoice)) {
                cin.clear();
                cin.ignore(numeric_limits<streamsize>::max(), '\n');
                cout << "Please enter a valid number.\n";
                continue;
            }
            if (locationChoice >= 1 && locationChoice <= 3) {
                validLocation = true;
            } else {
                cout << "Invalid location selection. Please try again.\n";
            }
        } while (!validLocation);

        string selectedLocation;

        switch (locationChoice) {
            case 1: selectedLocation = "Location A"; break;
            case 2: selectedLocation = "Location B"; break;
            case 3: selectedLocation = "Location C"; break;
        }

        // booking details
        cout << "\nBooking Details:\n";
        cout << "Movie: " << movies[movieChoice][0] << endl;
        cout << "Synopsis: " << movies[movieChoice][1] << endl;
        cout << "Runtime: " << movies[movieChoice][2] << endl;
        cout << "Location: " << selectedLocation << endl;
        cout << "Price per ticket: $" << movies[movieChoice][3] << endl;

        char proceed;
        do {
            cout << "\nDo you want to proceed with the booking? (Y/N): ";
            cin >> proceed;
            proceed = toupper(proceed);
            if (proceed != 'Y' && proceed != 'N') {
                cout << "Please enter Y or N.\n";
            }
        } while (proceed != 'Y' && proceed != 'N');

        if (proceed != 'Y') {
            cout << "Booking cancelled.\n";
            break;
        }

        // get number of tickets with validation
        int numTickets;
        bool validTickets = false;
        do {
            cout << "Enter number of tickets (max 15): ";
            if (!(cin >> numTickets)) {
                cin.clear();
                cin.ignore(numeric_limits<streamsize>::max(), '\n');
                cout << "Please enter a valid number.\n";
                continue;
            }
            if (numTickets >= 1 && numTickets <= 15) {
                validTickets = true;
            } else {
                cout << "Invalid number of tickets. Please enter a number between 1 and 15.\n";
            }
        } while (!validTickets);

        // payment method
        int paymentMethod;
        bool validPayment = false;
        do {
            cout << "\nSelect payment method:\n";
            cout << "1. Card\n2. Bank\n3. E-Wallet\n";
            cout << "Enter choice (1-3): ";

            if (!(cin >> paymentMethod)) {
                cin.clear();
                cin.ignore(numeric_limits<streamsize>::max(), '\n');
                cout << "Please enter a valid number.\n";
                continue;
            }
            if (paymentMethod >= 1 && paymentMethod <= 3) {
                validPayment = true;
            } else {
                cout << "Invalid payment method. Please try again.\n";
            }
        } while (!validPayment);

        cout << "Processing payment...\n";
        cout << "Payment successful!\n";

        // store ticket in history
        if (ticketCount < 50) {
            ticketHistory[ticketCount].movieName = movies[movieChoice][0];
            ticketHistory[ticketCount].location = selectedLocation;
            ticketHistory[ticketCount].showTime = "Today";  // Simplified
            ticketHistory[ticketCount].price = stod(movies[movieChoice][3]);
            ticketHistory[ticketCount].quantity = numTickets;
            ticketHistory[ticketCount].isValid = true;
            ticketCount++;
            
            cout << "Tickets booked successfully!\n";
        } else {
            cout << "Ticket history is full.\n";
        }

        // ask user to purchase more tickets (if y, uulitin ang func, if n, return to menu)
        do {
            cout << "\nWould you like to purchase more tickets? (Y/N): ";
            cin >> purchaseMore;
            purchaseMore = toupper(purchaseMore);
            if (purchaseMore != 'Y' && purchaseMore != 'N') {
                cout << "Please enter Y or N.\n";
            }
        } while (purchaseMore != 'Y' && purchaseMore != 'N');

    } while (purchaseMore == 'Y');
}

    void viewTickets() {
        cout << "\nMy Tickets:\n";
        bool hasTickets = false;
        
        for (int i = 0; i < 50; i++) {
            if (ticketHistory[i].isValid) {
                hasTickets = true;
                cout << "\nTicket #" << (i + 1) << ":\n";
                cout << "Movie: " << ticketHistory[i].movieName << endl;
                cout << "Location: " << ticketHistory[i].location << endl;
                cout << "Show Time: " << ticketHistory[i].showTime << endl;
                cout << "Quantity: " << ticketHistory[i].quantity << endl;
                cout << "Price per ticket: $" << ticketHistory[i].price << endl;
                cout << "Total price: $" << (ticketHistory[i].price * ticketHistory[i].quantity) << endl;
            }
        }
        
        if (!hasTickets) {
            cout << "No tickets found in your history.\n";
        }
    }
};

// CinemaSystem Class (Singleton Pattern)
class CinemaSystem {
private:
    // static ++ private constructor for Singleton (ganto dba)
    static CinemaSystem instance;
    User** users; 
    int userCount;
    int maxUsers;

    CinemaSystem() : userCount(0), maxUsers(100) {
        users = new User*[maxUsers];  // DMA for users
    }

public:
    ~CinemaSystem() {
        for (int i = 0; i < userCount; i++) {
            delete users[i];  // delete user
        }
        delete[] users;  // delete the array
    }

    // getter for instance
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
                return users[i]; // login function
            }
        }
        return nullptr;  // if no user is found, return null
    }

    void displayUsers() const {
        for (int i = 0; i < userCount; i++) {
            users[i]->displayProfile();
        }
    }
    // getter
    int getUserCount() const {
        return userCount;
    }
};

// definition of the static instance outside the class (for Singleton)
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

                Customer* customer = dynamic_cast<Customer*>(user);
                if (customer) {
                    customer->menu();  // if user == Customer, call Customer menu
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

                // check if admin based on predefined usernames
                for (const string& adminUsername : adminUsernames) {
                    if (username == adminUsername) {
                        isAdmin = true;
                        break;
                    }
                }
                try {
                    // creation of new admin/user acc
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