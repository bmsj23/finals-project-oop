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
        cout << "Name: " << name << endl << "Username: " << username
             << endl << "Email: " << email << endl << "Contact: " << contactNumber
             << endl << "Role: " << (isAdmin ? "Admin" : "Customer") << endl;
    }

    bool getIsAdmin() { return isAdmin; }
    string getUsername() { return username; }
    string getPassword() { return password; }

    virtual void menu() = 0;  // Pure virtual function for strategy pattern

    // Logout functionality
    void logout() const {
        cout << "Logging out..." << endl;
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
            cout << endl << "Admin Menu:\n1 - View All Users\n2 - Logout\n3 - Exit\nEnter your choice: ";
            cin >> choice;

            switch (choice) {
            case 1:
                cout << "Displaying users..." << endl;
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
                cout << "Invalid choice. Please try again." << endl;
            }
        }
    }
};

class Ticket {
private:
    string movieName;
    string location;
    string showTime;
    double price;
    int quantity;
    bool isValid;

public:
    // Default constructor
    Ticket() : movieName(""), location(""), showTime(""), price(0.0), quantity(0), isValid(false) {}

    // Parameterized constructor
    Ticket(string movie, string loc, string time, double ticketPrice, int qty)
        : movieName(movie), location(loc), showTime(time), price(ticketPrice), quantity(qty), isValid(true) {}

    // Getter methods
    string getMovieName() const { return movieName; }
    string getLocation() const { return location; }
    string getShowTime() const { return showTime; }
    double getPrice() const { return price; }
    int getQuantity() const { return quantity; }
    bool getIsValid() const { return isValid; }

    // Setter methods
    void setMovieName(const string& movie) { movieName = movie; }
    void setLocation(const string& loc) { location = loc; }
    void setShowTime(const string& time) { showTime = time; }
    void setPrice(double ticketPrice) { price = ticketPrice; }
    void setQuantity(int qty) { quantity = qty; }
    void setIsValid(bool valid) { isValid = valid; }

    // Calculate total price
    double getTotalPrice() const {
        return price * quantity;
    }

    // Display ticket details
    void displayTicketDetails() const {
        cout << "Movie: " << movieName << endl;
        cout << "Location: " << location << endl;
        cout << "Show Time: " << showTime << endl;
        cout << "Quantity: " << quantity << endl;
        cout << "Price per ticket: P" << price << endl;
        cout << "Total price: P" << getTotalPrice() << endl;
    }
};

class Customer : public User {
private:
    Ticket ticketHistory[50];
    int ticketCount = 0;

public:
    Customer(string nm, string uname, string mail, string contact, string pass)
        : User(nm, uname, mail, contact, pass, false) {}

    void menu() override {
        int choice;
        bool continueMenu = true;

        while (continueMenu) {
            cout << endl << "Customer Menu:\n"
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
                cout << "Viewing profile..." << endl;
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
                cout << "Invalid choice. Please try again." << endl;
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

        cout << endl << "Available Movies:" << endl;
        for (int i = 0; i < 5; i++) {
            cout << endl << "" << (i + 1) << ". Movie Name: " << movies[i][0] << endl;
            cout << "   Synopsis: " << movies[i][1] << endl;
            cout << "   Runtime: " << movies[i][2] << endl;
            cout << "   Price: P" << movies[i][3] << endl;
            cout << "   Available Seats: " << movies[i][4] << endl;
            cout << "   Location: " << movies[i][5] << endl;
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
            cout << endl << "Enter the number of the movie you want to watch (1-5): ";
            if (!(cin >> movieChoice)) {
                cin.clear();
                cin.ignore(numeric_limits<streamsize>::max(), '\n');
                cout << "Please enter a valid number." << endl;
                continue;
            }
            if (movieChoice >= 1 && movieChoice <= 5) {
                validMovie = true;
            } else {
                cout << "Invalid movie selection. Please try again." << endl;
            }
        } while (!validMovie);
        
        movieChoice--; // array indexing adjustment (kasi 0 ang start ng index tapos sa choices, 1 ang start so babawasan ng isa para mag match)

        // show and select location with validation
        cout << endl << "Available Locations for " << movies[movieChoice][0] << ":" << endl;
        cout << "1. Location A" << endl;
        cout << "2. Location B" << endl;
        cout << "3. Location C" << endl;
        
        int locationChoice;
        bool validLocation = false;

        do {
            cout << "Enter location number (1-3): ";
            if (!(cin >> locationChoice)) {
                cin.clear();
                cin.ignore(numeric_limits<streamsize>::max(), '\n');
                cout << "Please enter a valid number." << endl;
                continue;
            }
            if (locationChoice >= 1 && locationChoice <= 3) {
                validLocation = true;
            } else {
                cout << "Invalid location selection. Please try again." << endl;
            }
        } while (!validLocation);

        string selectedLocation;

        switch (locationChoice) {
            case 1: selectedLocation = "Location A"; break;
            case 2: selectedLocation = "Location B"; break;
            case 3: selectedLocation = "Location C"; break;
        }

        // booking details
        cout << endl << "Booking Details:" << endl;
        cout << "Movie: " << movies[movieChoice][0] << endl;
        cout << "Synopsis: " << movies[movieChoice][1] << endl;
        cout << "Runtime: " << movies[movieChoice][2] << endl;
        cout << "Location: " << selectedLocation << endl;
        cout << "Price per ticket: P" << movies[movieChoice][3] << endl;

        char proceed;
        do {
            cout << endl << "Do you want to proceed with the booking? (Y/N): ";
            cin >> proceed;
            proceed = toupper(proceed);
            if (proceed != 'Y' && proceed != 'N') {
                cout << "Please enter Y or N." << endl;
            }
        } while (proceed != 'Y' && proceed != 'N');

        if (proceed != 'Y') {
            cout << "Booking cancelled." << endl;
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
                cout << "Please enter a valid number." << endl;
                continue;
            }
            if (numTickets >= 1 && numTickets <= 15) {
                validTickets = true;
            } else {
                cout << "Invalid number of tickets. Please enter a number between 1 and 15." << endl;
            }
        } while (!validTickets);

        // payment method
        int paymentMethod;
        bool validPayment = false;
        do {
            cout << endl << "Select payment method:" << endl;
            cout << "1. Card\n2. Bank\n3. E-Wallet" << endl;
            cout << "Enter choice (1-3): ";

            if (!(cin >> paymentMethod)) {
                cin.clear();
                cin.ignore(numeric_limits<streamsize>::max(), '\n');
                cout << "Please enter a valid number." << endl;
                continue;
            }
            if (paymentMethod >= 1 && paymentMethod <= 3) {
                validPayment = true;
            } else {
                cout << "Invalid payment method. Please try again." << endl;
            }
        } while (!validPayment);

        cout << "Processing payment..." << endl;
        cout << "Payment successful!" << endl;

        // store ticket in history
        if (ticketCount < 50) {
            ticketHistory[ticketCount] = Ticket(movies[movieChoice][0], selectedLocation, "Today", stod(movies[movieChoice][3]), numTickets);
            ticketCount++;
            cout << "Tickets booked successfully!" << endl;
        }else {
            cout << "Ticket history is full." << endl;
        }

        // ask user to purchase more tickets (if y, uulitin ang func, if n, return to menu)
        do {
            cout << endl << "Would you like to purchase more tickets? (Y/N): ";
            cin >> purchaseMore;
            purchaseMore = toupper(purchaseMore);
            if (purchaseMore != 'Y' && purchaseMore != 'N') {
                cout << "Please enter Y or N." << endl;
            }
        } while (purchaseMore != 'Y' && purchaseMore != 'N');

    } while (purchaseMore == 'Y');
}

    void viewTickets() {
        cout << endl << "My Tickets:" << endl;
        bool hasTickets = false;
        
        for (int i = 0; i < ticketCount; i++) {
            if (ticketHistory[i].getIsValid()) {
                hasTickets = true;
                cout << endl << "Ticket #" << (i + 1) << ":" << endl;
                ticketHistory[i].displayTicketDetails();
            }
        }
        
        if (!hasTickets) {
            cout << "No tickets found in your history." << endl;
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
        cout << endl << "Main Menu:" << endl;
        cout << "1 - Login\n2 - Sign Up\n3 - Exit" << endl;
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
                        cout << "Invalid credentials. Please try again." << endl;
                    }
                }

                cout << "Login successful." << endl;

                Customer* customer = dynamic_cast<Customer*>(user);
                if (customer) {
                    customer->menu();  // if user == Customer, call Customer menu
                } else {
                    cout << "This user is not a customer." << endl;
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
                    cout << "Sign-up successful. You can now log in." << endl;
                } catch (ValidationException& ex) {
                    cout << "Error: " << ex.what() << endl;
                }

                break;
            }
            case 3:
                cout << "Exiting the system. Goodbye!" << endl;
                continueMenu = false;
                break;
            default:
                cout << "Invalid choice. Please try again." << endl;
        }
    }
}

int main() {
    CinemaSystem& system = CinemaSystem::getInstance();
    mainMenu(system);

    return 0;
}