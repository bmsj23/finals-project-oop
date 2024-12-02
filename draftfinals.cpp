#include <iostream>
#include <string>
#include <limits>
#include <iomanip>
using namespace std;

class Ticket;
class Customer;

// seating array for different movies and locations
char movieSeats[5][3][10][10];  // [movie][location][row][column]

string movies[5][5] = {
    {"Movie A", "Action-packed thriller", "120 min", "300", "Location A"},
    {"Movie B", "Romantic comedy", "90 min", "250", "Location B"},
    {"Movie C", "Sci-fi adventure", "140 min", "200", "Location A"},
    {"Movie D", "Horror mystery", "100 min", "350", "Location C"},
    {"Movie E", "Animated fantasy", "110 min", "270", "Location B"}
};

// initialization of seats to available ('O') for all movies and locations
void initializeAllSeats() {
    for (int movie = 0; movie < 5; ++movie) {
        for (int location = 0; location < 3; ++location) {
            for (int row = 0; row < 10; ++row) {
                for (int col = 0; col < 10; ++col) {
                    movieSeats[movie][location][row][col] = 'O';
                }
            }
        }
    }
}

// display available seats for a specific movie and location
void displaySeats(int movieIndex, int locationIndex) {
    cout << "\nSeating Arrangement (O = Available, X = Taken):\n\n";
    cout << "  ";
    for (int col = 1; col <= 10; ++col) {
        cout << setw(2) << col << " ";
    }
    cout << endl;

    for (int row = 0; row < 10; ++row) {
        cout << char('A' + row) << " ";
        for (int col = 0; col < 10; ++col) {
            cout << setw(2) << movieSeats[movieIndex][locationIndex][row][col] << " ";
        }
        cout << endl;
    }
}

// book seats func
string* bookSeats(int movieIndex, int locationIndex, int numTickets) {
    static string selectedSeats[15];  // max 15 tickets allowed
    
    for (int i = 0; i < numTickets; ++i) {
        string seatChoice;
        bool validSeat = false;

        while (!validSeat) {
            cout << "Enter seat " << (i + 1) << " (e.g., A1, B10): ";
            cin >> seatChoice;

            if (seatChoice.length() < 2 || seatChoice.length() > 3) {
                cout << "Invalid format. Try again." << endl;
                continue;
            }

            char rowChar = toupper(seatChoice[0]);
            int col = stoi(seatChoice.substr(1)) - 1;

            if (rowChar < 'A' || rowChar > 'J' || col < 0 || col > 9) {
                cout << "Seat out of range. Try again." << endl;
            continue;
            }

            int row = rowChar - 'A';
            
            if (movieSeats[movieIndex][locationIndex][row][col] == 'X') {
                cout << "Seat already taken. Choose another seat." << endl;
            } else {
                movieSeats[movieIndex][locationIndex][row][col] = 'X'; // mark seat as taken
                selectedSeats[i] = seatChoice;
                cout << "Seat " << seatChoice << " successfully booked." << endl;
                validSeat = true;
            }
        }
    }
    return selectedSeats;
}

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

    CinemaSystem(const CinemaSystem&) = delete;
    CinemaSystem& operator=(const CinemaSystem&) = delete;

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
            if (users[i]->getUsername() == username && 
                users[i]->getPassword() == password) {
                return users[i];
            }
        }
        return nullptr;
    }

    // admin authentication method
    bool authenticateAdmin(const string& username, const string& password) {
        for (int i = 0; i < userCount; i++) {
            // check if the user is an admin and credentials match
            if (users[i]->getUsername() == username && 
                users[i]->getPassword() == password && 
                users[i]->getIsAdmin()) {
                return true;
            }
        }
        return false;
    }

    void deleteUser(int index) {
        if (index >= 0 && index < userCount) {
            delete users[index]; // delete the user
            for (int i = index; i < userCount - 1; i++) {
                users[i] = users[i + 1]; // yung logic for adjustment ng users sa array index
            }
            userCount--;
        } else {
            cout << "Invalid user index." << endl;
        }
    }

    void displayUsers() const {
        for (int i = 0; i < userCount; i++) {
            users[i]->displayProfile();
            cout << endl;
        }
    }
    // getter
    int getUserCount() const {
        return userCount;
    }

    User** getUsers() {
        return users;
    }
};

// definition of the static instance outside the class (for Singleton)
CinemaSystem CinemaSystem::instance;

class Ticket {
private:
    string movieName;
    string location;
    string showTime;
    double price;
    int quantity;
    bool isValid;
    string seats;

    string formatSeats(const string& seatInput, int quantity) const {

        string formattedSeats[100];
        int seatCount = 0;
        string currentSeat = "";

        for (char ch : seatInput) {
            if (isalpha(ch) && currentSeat.empty()) {
                // start a new seat with a single uppercase letter
                currentSeat += toupper(ch);
            } else if (isdigit(ch) && currentSeat.size() == 1) {
                // add a single digit to complete the seat format
                currentSeat += ch;

                // then save the valid seat and reset for the next one
                formattedSeats[seatCount++] = currentSeat;
                currentSeat = "";

                //stop processing if the required quantity of seats is reached
                if (seatCount >= quantity) {
                    break;
                }
            } else {
                // reset on invalid characters pati excess digits
                currentSeat = "";
            }
        }

        // combine formatted seats into a single string
        string result = "";
        
        for (int i = 0; i < seatCount; ++i) {
            result += formattedSeats[i];
            if (i < seatCount - 1) {
                result += ", ";
            }
        }
        return result;
    }

public:
    Ticket() : movieName(""), location(""), showTime(""), price(0.0), quantity(0), isValid(false), seats("") {}

    Ticket(string movie, string loc, string time, double ticketPrice, int qty, string selectedSeats)
        : movieName(movie), location(loc), showTime(time), price(ticketPrice),
        quantity(qty), isValid(true), seats(formatSeats(selectedSeats, qty)) {}

    string getMovieName() const { return movieName; }
    string getLocation() const { return location; }
    string getShowTime() const { return showTime; }
    double getPrice() const { return price; }
    int getQuantity() const { return quantity; }
    bool getIsValid() const { return isValid; }
    string getSeats() const { return seats; }

    void setMovieName(const string& movie) { movieName = movie; }
    void setLocation(const string& loc) { location = loc; }
    void setShowTime(const string& time) { showTime = time; }
    void setPrice(double ticketPrice) { price = ticketPrice; }
    void setQuantity(int qty) { quantity = qty; }
    void setIsValid(bool valid) { isValid = valid; }
    void setSeats(const string& selectedSeats) { seats = formatSeats(selectedSeats, quantity); }

    double getTotalPrice() const {
        return price * quantity;
    }

    void displayTicketDetails() const {
        cout << "Movie: " << movieName << endl;
        cout << "Location: " << location << endl;
        cout << "Show Time: " << showTime << endl;
        cout << "Quantity: " << quantity << endl;
        cout << "Selected Seats: " << seats << endl;
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
        : User(nm, uname, mail, contact, pass, false), ticketCount(0) {}

    int getTicketCount() const { return ticketCount; }
    Ticket* getTickets() { return ticketHistory; }

    void addTicket(const Ticket& ticket) {
        if (ticketCount < 50) {
            ticketHistory[ticketCount++] = ticket;
        }
    }

        void refundTickets() {
        if (ticketCount == 0) {
            cout << "No tickets available for refund." << endl;
            return;
        }

        viewTickets();
        
        int refundChoice;
        cout << "Enter the ticket number you want to refund (0 to cancel): ";
        cin >> refundChoice;

        if (refundChoice == 0) return;

        if (refundChoice < 1 || refundChoice > ticketCount) {
            cout << "Invalid ticket number." << endl;
            return;
        }

        refundChoice--;

        if (!ticketHistory[refundChoice].getIsValid()) {
            cout << "This ticket is already invalid." << endl;
            return;
        }

        double originalPrice = ticketHistory[refundChoice].getPrice();
        double refundAmount = originalPrice * 0.7;  // 70% refund

        cout << "Refund Details:" << endl;
        cout << "Original Ticket Price: P" << originalPrice << endl;
        cout << "Refund Amount (70%): P" << refundAmount << endl;

        char confirm;
        cout << "Confirm refund? (Y/N): ";
        cin >> confirm;

        if (toupper(confirm) == 'Y') {
            // void ticket para sa refund
            ticketHistory[refundChoice].setIsValid(false);

            cout << "Ticket refunded successfully." << endl;
            cout << "Refund amount: P" << refundAmount << endl;
        } else {
            cout << "Refund cancelled." << endl;
        }
    }

    void menu() override {
        int choice;
        bool continueMenu = true;

        while (continueMenu) {
            cout << endl << "Customer Menu:" << endl
                 << "1 - View Profile" << endl
                 << "2 - Purchase Tickets" << endl
                 << "3 - View Movies" << endl
                 << "4 - My Tickets" << endl
                 << "5 - Refund Tickets" << endl                
                 << "6 - Logout" << endl                  
                 << "7 - Exit" << endl << endl
                 << "Enter your choice: ";
            
            cin >> choice;

            switch (choice) {
            case 1:
                profileMenu();
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
                refundTickets();
                break;
            case 6:
                logout();
                continueMenu = false;
                break;
            case 7:
                cout << endl << "Exiting the system." << endl;
                exit(0);
                break;
            default:
                cout << "Invalid choice. Please try again." << endl;
            }
        }
    }

      void profileMenu() {
        int profileChoice;
        bool continueProfileMenu = true;

        while (continueProfileMenu) {
            cout << endl << "Profile Details" << endl << endl;
            cout << "Name: " << name << endl;
            cout << "Email: " << email << endl;
            cout << "Phone Number: " << contactNumber << endl;
            cout << "Username: " << username << endl;
            cout << "Password: " << password << endl << endl;

            cout << "What would you like to do?" << endl;
            cout << endl << "1 - Update Profile Details";
            cout << endl << "2 - Return to Main Menu" << endl;
            cout << endl << "Enter your choice: ";
            cin >> profileChoice;

            switch (profileChoice) {
            case 1:
                updateProfile();
                break;
            case 2:
                continueProfileMenu = false;
                break;
            default:
                cout << "Invalid choice. Please try again." << endl;
            }
        }
    }

    void updateProfile() {
        int updateChoice;
        bool continueUpdate = true;

        while (continueUpdate) {
            cout << endl << "Update Profile Menu:" << endl;
            cout << "1 - Edit Username" << endl;
            cout << "2 - Edit Password" << endl;
            cout << "3 - Edit Name" << endl;
            cout << "4 - Edit Phone Number" << endl;
            cout << "5 - Edit Email" << endl;
            cout << "6 - Return to Profile Menu" << endl;
            cout << endl << "Enter your choice: ";
            cin >> updateChoice;

            string newValue;

            switch (updateChoice) {
            case 1:
                cout << "Enter new username: ";
                cin >> newValue;
                username = newValue;
                cout << "Username updated successfully." << endl;
                break;
            case 2:
                cout << "Enter new password: ";
                cin >> newValue;
                password = newValue;
                cout << "Password updated successfully." << endl;
                break;
            case 3:
                cout << "Enter new name: ";
                cin.ignore();
                getline(cin, newValue);
                name = newValue;
                cout << "Name updated successfully." << endl;
                break;
            case 4:
                cout << "Enter new phone number: ";
                cin >> newValue;
                contactNumber = newValue;
                cout << "Phone number updated successfully." << endl;
                break;
            case 5:
                cout << "Enter new email: ";
                cin >> newValue;
                email = newValue;
                cout << "Email updated successfully." << endl;
                break;

            case 6:
                continueUpdate = false;
                break;
            default:
                cout << "Invalid choice. Please try again." << endl;
            }
        }
    }

    void browseMovies() {
        cout << endl << "Available Movies:" << endl;
        for (int i = 0; i < 5; i++) {
            
            // calculate available seats for each movie and location
            int availableSeats = 0;
            
            for (int j = 0; j < 3; j++) {  // locations iteration
                for (int row = 0; row < 10; ++row) {
                    for (int col = 0; col < 10; ++col) {
                        if (movieSeats[i][j][row][col] == 'O') {
                            availableSeats++;
                        }
                    }
                }
            }  

            cout << endl << "" << (i + 1) << ". Movie Name: " << movies[i][0] << endl;
            cout << "   Synopsis: " << movies[i][1] << endl;
            cout << "   Runtime: " << movies[i][2] << endl;
            cout << "   Price: P" << movies[i][3] << endl;
            cout << "   Available Seats: " << availableSeats << endl;
            cout << "   Location: " << movies[i][4] << endl;
        }
    }

    int countAvailableSeats(int movieIndex, int locationIndex) {
        int availableSeats = 0;
        for (int row = 0; row < 10; row++) {
            for (int col = 0; col < 10; ++col) {
                if (movieSeats[movieIndex][locationIndex][row][col] == 'O') {
                    availableSeats++;
                }
            }
        }
        return availableSeats;
    }

    void purchaseTickets() {

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

            int availableSeats = countAvailableSeats(movieChoice, locationChoice - 1);

            // booking details
            cout << endl << "Booking Details" << endl << endl;
            cout << "Movie: " << movies[movieChoice][0] << endl;
            cout << "Synopsis: " << movies[movieChoice][1] << endl;
            cout << "Runtime: " << movies[movieChoice][2] << endl;
            cout << "Location: " << selectedLocation << endl;
            cout << "Price per ticket: P" << movies[movieChoice][3] << endl;
            cout << "Available Seats: " << availableSeats << endl;

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

            // display and book seats
            displaySeats(movieChoice, locationChoice - 1);
            string* bookedSeats = bookSeats(movieChoice, locationChoice - 1, numTickets);

            // Convert array of seats to string
            string selectedSeats;
            for (int i = 0; i < numTickets; ++i) {
                if (i > 0) selectedSeats += ", ";
                selectedSeats += bookedSeats[i];
            }

            for (int i = 0; i < numTickets; ++i) {
                if (i > 0) selectedSeats += ", ";
                char rowChar = 'A' + (movieChoice * 2);  // adjust row based on movie
                selectedSeats += rowChar + to_string(i + 1);
            }

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

            switch (paymentMethod) {
                case 1: {  // Card Payment
                    string cardNumber, expDate, cvc;
                    cout << "Enter Card Number (16 digits): ";
                    cin >> cardNumber;
                    cout << "Enter Expiration Date (MM/YY): ";
                    cin >> expDate;
                    cout << "Enter CVC (3 digits): ";
                    cin >> cvc;
                    cout << endl << "Processing card payment..." << endl;
                    break;
                }
                case 2: {  // Bank Payment
                    string accountNumber, accountName;
                    cout << "Enter Account Number: ";
                    cin >> accountNumber;
                    cout << "Enter Account Name: ";
                    cin.ignore();
                    getline(cin, accountName);
                    cout << "Processing bank transfer..." << endl;
                    break;
                }
                case 3: {  // E-Wallet Payment
                    int ewalletChoice;
                    string phoneNumber;
                    cout << "Select E-Wallet:" << endl;
                    cout << "1. GCash\n2. Maya" << endl;
                    cout << endl << "Enter your choice: ";
                    cin >> ewalletChoice;
                    
                    string ewalletName = (ewalletChoice == 1) ? "GCash" : "Maya";
                    
                    cout << "Enter phone number linked to " << ewalletName << ": ";
                    cin >> phoneNumber;
                    cout << "Processing " << ewalletName << " payment..." << endl;
                    break;
                }
            }

            cout << "Payment successful!" << endl;

            // modify ticket storage to include selected seats
            if (ticketCount < 50) {
                ticketHistory[ticketCount] = Ticket(
                    movies[movieChoice][0], 
                    selectedLocation, 
                    "Today", 
                    stod(movies[movieChoice][3]), 
                    numTickets,
                    selectedSeats
                );
                ticketCount++;
                cout << endl << "Tickets booked successfully!" << endl;
            } else {
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
                cout << endl << "Order #" << (i + 1) << ":" << endl;
                ticketHistory[i].displayTicketDetails();
            }
        }
        
        if (!hasTickets) {
            cout << "No tickets found in your history." << endl;
        }
    }
};

// Admin Class - Derived from User (Polymorphism)
class Admin : public User {
public:
    Admin(string nm, string uname, string mail, string contact, string pass)
        : User(nm, uname, mail, contact, pass, true) {}

     void viewAllUsers(CinemaSystem& system) {
        if (system.getUserCount() == 0) {
            cout << "No users found." << endl;
            return;
        }

        system.displayUsers();
    }

        void updateMovieDetails() {
        int movieChoice, detailChoice;
        bool continueUpdate = true;

        while (continueUpdate) {
            // display movie list
            for (int i = 0; i < 5; i++) {
                cout << (i + 1) << ". " << movies[i][0] << endl;
            }

            cout << "Select movie to update (0 to exit): ";
            cin >> movieChoice;

            if (movieChoice == 0) break;
            movieChoice--;

            if (movieChoice < 0 || movieChoice >= 5) {
                cout << "Invalid movie selection." << endl;
                continue;
            }

            cout << endl << "Select detail to update:" << endl;
            cout << "1. Movie Name" << endl;
            cout << "2. Synopsis" << endl;
            cout << "3. Runtime" << endl;
            cout << "4. Price" << endl;
            cout << "5. Locations" << endl;
            cout << "6. Additional Details" << endl;
            cout << "0. Exit" << endl;

            cout << endl << "Enter choice: ";

            cin >> detailChoice;

            string newValue;
            cin.ignore();

            switch (detailChoice) {
                case 1:
                    cout << "Enter new movie name: ";
                    getline(cin, newValue);
                    movies[movieChoice][0] = newValue;
                    break;
                case 2:
                    cout << "Enter new synopsis: ";
                    getline(cin, newValue);
                    movies[movieChoice][1] = newValue;
                    break;
                case 3:
                    cout << "Enter new runtime: ";
                    getline(cin, newValue);
                    movies[movieChoice][2] = newValue;
                    break;
                case 4:
                    cout << "Enter new price: ";
                    getline(cin, newValue);
                    movies[movieChoice][3] = newValue;
                    break;
                case 5:
                    cout << "Enter new locations (comma-separated): ";
                    getline(cin, newValue);
                    movies[movieChoice][4] = newValue;
                    break;
                case 6:
                    cout << "Enter new additional details: ";
                    getline(cin, newValue);
                    movies[movieChoice][5] = newValue;
                    break;
                case 0:
                    continueUpdate = false;
                    break;
                default:
                    cout << "Invalid choice." << endl;
            }

            cout << "Update successful!" << endl;
        }
    }

    void deleteMovie() {
        bool hasMovies = false;

        for (int i = 0; i < 5; i++) {
            if (!movies[i][0].empty()) {
                hasMovies = true;
                break;
            }
        }

        if (!hasMovies) {
            cout << "No movies available to delete." << endl;
            return;
        }

        // display available movies
        cout << "\nAvailable Movies:" << endl;
        for (int i = 0; i < 5; i++) {
            if (!movies[i][0].empty()) {
                cout << (i + 1) << ". " << movies[i][0] << endl;
            }
        }

        int movieChoice;
        bool validChoice = false;

        do {
            cout << "\nEnter the number of the movie to delete (0 to cancel): ";
            if (!(cin >> movieChoice)) {
                cin.clear();
                cin.ignore(numeric_limits<streamsize>::max(), '\n');
                cout << "Invalid input. Please try again." << endl;
                continue;
            }

            if (movieChoice == 0) {
                cout << "Movie deletion cancelled." << endl;
                return;
            }

            if (movieChoice >= 1 && movieChoice <= 5 && !movies[movieChoice - 1][0].empty()) {
                validChoice = true;
            } else {
                cout << "Invalid movie selection. Please try again." << endl;
            }
        } while (!validChoice);

        // Confirm deletion
        char confirm;
        cout << "Are you sure you want to delete " << movies[movieChoice - 1][0] << "? (Y/N): ";
        cin >> confirm;

        if (toupper(confirm) == 'Y') {

            // clear movie details
            for (int j = 0; j < 5; j++) {
                movies[movieChoice - 1][j] = "";
            }

            // clear corresponding seats
            for (int location = 0; location < 3; ++location) {
                for (int row = 0; row < 10; ++row) {
                    for (int col = 0; col < 10; ++col) {
                        movieSeats[movieChoice - 1][location][row][col] = 'O';
                    }
                }
            }

            cout << "Movie deleted successfully." << endl;
        } else {
            cout << "Movie deletion cancelled." << endl;
        }
    }

void refundManagement(CinemaSystem& system) {
    User** users = system.getUsers();
    int userCount = system.getUserCount();
    int refundedSeats = 0;

    Ticket* refundableTickets[5000];
    int refundableCount = 0;

    for (int i = 0; i < userCount; ++i) {
        Customer* customer = dynamic_cast<Customer*>(users[i]);
        if (customer) {
            for (int j = 0; j < customer->getTicketCount(); ++j) {
                if (customer->getTickets()[j].getIsValid()) {
                    refundableTickets[refundableCount++] = &customer->getTickets()[j];
                }
            }
        }
    }

    if (refundableCount == 0) {
        cout << "No tickets available for refund." << endl;
        return;
    }

    cout << endl << "Refundable Tickets:" << endl;
    for (int i = 0; i < refundableCount; ++i) {
        cout << endl << "Ticket #" << i + 1 << ". " << endl;
        refundableTickets[i]->displayTicketDetails();
    }

    int choice;
    cout << "Enter the number of the ticket to refund (0 to cancel): ";
    cin >> choice;

    if (choice <= 0 || choice > refundableCount) {
        cout << "Refund cancelled." << endl;
        return;
    }

    Ticket* ticket = refundableTickets[choice - 1];
    double refundAmount = ticket->getTotalPrice() * 0.7;

    // under construction pa ang reversing the availablity of seats after a refund
    // increase seat count
    // refundedSeats++;

    ticket->setIsValid(false);
    cout << "Refund processed: P" << refundAmount << endl;
}

    void deleteUser(CinemaSystem& system) {
        system.displayUsers();
        int choice;

        cout << "Enter the user number to delete (0 to cancel): ";
        cin >> choice;

        if (choice <= 0 || choice > system.getUserCount()) {
            cout << "Deletion cancelled." << endl;
            return;
        }

        system.deleteUser(choice - 1);
        cout << "User deleted successfully." << endl;
    }

    void menu() override {
        int choice;
        CinemaSystem& system = CinemaSystem::getInstance();
        bool continueMenu = true;

        while (continueMenu) {
            cout << endl << "Admin Menu:" << endl
                 << "1 - View All Users" << endl
                 << "2 - View and Update Movie Details" << endl
                 << "3 - Delete Movies" << endl
                 << "4 - Accommodate Refunds and Cancellations" << endl
                 << "5 - Delete Users" << endl
                 << "6 - Logout" << endl
                 << "7 - Exit" << endl << endl
                 << "Enter your choice: ";
            cin >> choice;

            switch (choice) {
            case 1:
                viewAllUsers(system);
                break;
            case 2:
                updateMovieDetails();
                break;
            case 3:
                deleteMovie();
                break;
            case 4:
                refundManagement(system);
                break;
            case 5:
                deleteUser(system);
                break;
            case 6:
                logout();
                continueMenu = false;
                break;
            case 7:
                cout << endl << "Exiting the system." << endl;
                exit(0);
                break;
            default:
                cout << "Invalid choice. Please try again." << endl;
            }
        }
    }
};

// Main Menu Function
void mainMenu(CinemaSystem& system) {
    string adminUsernames[5] = {"admin1", "admin2", "admin3", "admin4", "admin5"};
    int choice;
    bool continueMenu = true;

    while (continueMenu) {
        cout << endl << "Main Menu" << endl << endl;
        cout << "1 - Login" << endl << "2 - Sign Up" << endl << "3 - Exit" << endl;
        cout << endl << "Enter your choice: ";
        cin >> choice;

        switch (choice) {
            case 1: {

                bool loginFinished = false;
                bool returnToMainMenu = false;

                while (!loginFinished && !returnToMainMenu) {

                    string username, password;
                    User* user = nullptr;

                    cout << "Enter username (or '0' to return to main menu): ";
                    cin >> username;                     

                    if (username == "0") {
                        returnToMainMenu = true;
                        break;
                    }
                    cout << "Enter password: ";
                    cin >> password;

                    // check if it's an admin login
                    bool isAdmin = system.authenticateAdmin(username, password);
                    if (isAdmin) {

                        // Find the admin user
                        for (int i = 0; i < system.getUserCount(); i++) {
                            
                            user = system.authenticateUser(username, password);

                            if (user && user->getIsAdmin()) {

                                Admin* admin = dynamic_cast<Admin*>(user);

                                if (admin) {
                                    cout << "Admin login successful." << endl;
                                    admin->menu();
                                    loginFinished = true;
                                    break;
                                }
                            }
                        }
                        
                        if (loginFinished) {
                            break;
                        }
                    }

                    // if hindi admin, check for customer login
                    user = system.authenticateUser(username, password);

                    if (user != nullptr) {
                        cout << "Login successful." << endl;

                        Customer* customer = dynamic_cast<Customer*>(user);
                        
                        if (customer) {
                            customer->menu();
                        }
                        loginFinished = true;
                    } else {
                        cout << "Invalid credentials. Please try again." << endl;
                    }
                }
                break;
            }
            case 2: {
                string name, username, email, contact, password;
                bool isAdmin = false;

                cout << endl << "Sign Up" << endl << endl;
                cout << "Enter username: ";
                cin >> username;
                cout << "Enter password: ";
                cin >> password;
                cout << "Enter name: ";
                cin.ignore();
                getline(cin, name);
                cout << "Enter email: ";
                cin >> email;
                cout << "Enter contact number: ";
                cin >> contact;

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
                    cout << endl << "Sign-up successful. You can now log in." << endl;
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
    initializeAllSeats();
    mainMenu(system);

    return 0;
}