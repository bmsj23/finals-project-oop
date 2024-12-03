    #include <iostream>
    #include <string>
    #include <sstream>
    #include <limits>
    #include <iomanip>
    using namespace std;

    class Ticket;
    class Customer;

    // seating array for different movies and locations
    char movieSeats[5][3][5][4][10][10];  // [movies][locations][dates][showtimes][rows][columns]

    string movies[5][5] = {
        {"Movie A", "Action-packed thriller", "120 min", "300", "Location A"},
        {"Movie B", "Romantic comedy", "90 min", "250", "Location B"},
        {"Movie C", "Sci-fi adventure", "140 min", "200", "Location A"},
        {"Movie D", "Horror mystery", "100 min", "350", "Location C"},
        {"Movie E", "Animated fantasy", "110 min", "270", "Location B"}
    };

    // array for dates (e.g., Today + 4 future dates)
    string availableDates[5] = {"Today", "December 5", "December 6", "December 7", "December 8"};
    // times are based on the movie runtime; start time is 11 AM
    string times[4] = {"11 AM", "1 PM", "3 PM", "5 PM"};

    // initialization of seats to available ('O') for all movies and locations
    void initializeAllSeats() {
        for (int movie = 0; movie < 5; movie++) {
            for (int location = 0; location < 3; location++) {
                for (int date = 0; date < 5; date++) {
                    for (int time = 0; time < 4; time++) {
                        for (int row = 0; row < 10; row++) {
                            for (int col = 0; col < 10; col++) {
                                movieSeats[movie][location][date][time][row][col] = 'O';
                            }
                        }
                    }
                }
            }
        }
    }

    // display available seats for a specific movie and location
    void displaySeats(int movieIndex, int locationIndex, int dateIndex, int timeIndex) {
        cout << "\nSeating Arrangement (O = Available, X = Taken):\n\n";
        cout << "  ";
        for (int col = 1; col <= 10; col++) {
            cout << setw(2) << col << " ";
        }
        cout << endl;

        for (int row = 0; row < 10; row++) {
            cout << char('A' + row) << " ";
            for (int col = 0; col < 10; col++) {
                cout << setw(2) << movieSeats[movieIndex][locationIndex][dateIndex][timeIndex][row][col] << " ";
            }
            cout << endl;
        }
    }

    string* bookSeats(int movieIndex, int locationIndex, int dateIndex, int timeIndex, int numTickets) {
        static string selectedSeats[15];

        for (int i = 0; i < numTickets; i++) {
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

                if (movieSeats[movieIndex][locationIndex][dateIndex][timeIndex][row][col] == 'X') {
                    cout << "Seat already taken. Choose another seat." << endl;
                } else {
                    movieSeats[movieIndex][locationIndex][dateIndex][timeIndex][row][col] = 'X';
                    selectedSeats[i] = seatChoice;
                    cout << "Seat " << seatChoice << " successfully booked." << endl;
                    validSeat = true;
                }
            }
        }
        return selectedSeats;
    }

    // exception Class for Validation
    class ValidationException {
    private:
        string message;
    public:
        explicit ValidationException(const string& msg) : message(msg) {}
        string what() const { return message; }
    };

    // abstract User Class for Admin and Customer
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

        virtual void menu() = 0;  // pure virtual function for strategy pattern

        // logout functionality
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
        // to search for the username
        bool isUsernameExists(const string& username) {
            for (int i = 0; i < userCount; i++) {
                if(users[i]->getUsername() == username) {
                    return true;
                }
            }
            return false;
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

    void inputValidName() {
        string name;
        bool isValid = true;
        
        do {
            cout << "Enter name: ";
            cin.ignore();
            getline(cin, name);

            if (name.empty()) {
                cout << "Input should not be empty.\n";
                isValid = false;
            }
            else {
                for (char c : name) {
                    if (!isalpha(c) && c != ' ') {
                        isValid = false;
                        cout << "Invalid input. Name should only contain letters.\n";
                    }
                }
            }
        } while (!isValid);
    }

    void inputValidEmail() {
        string email;
        bool isValid = false;

        do {
            cout << "Enter email: ";
            cin >> email;

            // check if '@' exists exactly once and there is at least one '.' after '@'
            size_t atPos = email.find('@');
            size_t dotPos = email.find('.', atPos);

            isValid = (atPos != string::npos && dotPos != string::npos && atPos < dotPos);

            if (!isValid) {
                cout << "Invalid input. Email should contain '@' and '.'.\n";
            }
        } while (!isValid);
    }

    void inputValidContactNum() {
        string contactNum;
        bool isValid = true;

        do {
            cout << "Enter contact number: ";
            cin >> contactNum;

            if (contactNum.empty()) {
                cout << "Contact Number should not be empty.\n";
                isValid = false;
            }
            else if (contactNum.length() != 11) {
                cout << "Invalid input. Contact number should contain 11 numbers only.\n";
                isValid = false;
            }
            else {
                for (char c : contactNum) {
                    if (!isdigit(c)) {
                        isValid = false;
                        cout << "Invalid input. Contact number should only contain numbers and without spaces.";
                    }
                }
            }
        } while (!isValid);
    }

    void loginValidationUsername(CinemaSystem& system) {
        string username;
        bool isFound;

        do {
            cout << "Enter username: ";            
            cin >> username;

            if (username.empty()) {
                cout << "Username should not be empty.\n";
                isFound = false;
            }
            else if (!isFound) {
                isFound = system.isUsernameExists(username);
                if (!isFound) {
                    cout << "This username does not exist. Please re-enter your username.\n";
                }
            }
        } while (!isFound);
    }

    

    void signupValidationUsername(CinemaSystem& system) {
        string username;
        bool isFound;

        do {
            cout << "Enter username: ";            
            cin >> username;

            if (username.empty()) {
                cout << "Username should not be empty.\n";
                isFound = true;
            }
            else if (username.length() < 5) {
                cout << "Invalid input. Username should contain a minimum of 5 characters.\n";
                isFound = true;
            }
            else {
                isFound = system.isUsernameExists(username);
                if (isFound) {
                    cout << "This username already exists. Please re-enter your username.\n";
                }
            }
        } while (username.empty() || username.length() < 5 || isFound);
    }

    void inputValidPassword() {
        string password;
        bool hasLetter = false;
        bool hasDigit = false;

        do {
            cout << "Enter password: ";
            getline(cin, password);

            if (password.empty()) {
                cout << "Password should not be empty.\n";
            }
            else if (password.length() < 8) {
                cout << "Invalid input. Password should contain a minimum of 8 characters.\n";
            }
            else {
                for (char c : password) {
                    if (isalnum(c)) {
                        if (isalpha(c)) hasLetter = true;
                        if (isdigit(c)) hasDigit = true;
                    }
            
                    if (!hasLetter || !hasDigit) {
                        cout << "Invalid input. Password should contain both number and password.\n";
                    }
                }
            }
        } while (password.empty() || password.length() < 8 || !hasLetter || !hasDigit);
    }

class Ticket {
    private:
        string movieName;
        string location;
        string showTime;
        string selectedDate;
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
                    currentSeat += toupper(ch);
                } else if (isdigit(ch) && currentSeat.size() == 1) {
                    currentSeat += ch;
                    formattedSeats[seatCount++] = currentSeat;
                    currentSeat = "";

                    if (seatCount >= quantity) break;
                } else {
                    currentSeat = "";
                }
            }

            string result = "";
            for (int i = 0; i < seatCount; i++) {
                result += formattedSeats[i];
                if (i < seatCount - 1) {
                    result += ", ";
                }
            }
            return result;
        }

    public:
        Ticket() : movieName(""), location(""), showTime(""), selectedDate(""), price(0.0), quantity(0), isValid(false), seats("") {}

        Ticket(string movie, string loc, string time, string date, double ticketPrice, int qty, string selectedSeats)
            : movieName(movie), location(loc), showTime(time), selectedDate(date), price(ticketPrice),
            quantity(qty), isValid(true), seats(formatSeats(selectedSeats, qty)) {}

        string getMovieName() const { return movieName; }
        string getLocation() const { return location; }
        string getShowTime() const { return showTime; }
        string getSelectedDate() const { return selectedDate; }
        double getPrice() const { return price; }
        int getQuantity() const { return quantity; }
        bool getIsValid() const { return isValid; }
        string getSeats() const { return seats; }

        void setMovieName(const string& movie) { movieName = movie; }
        void setLocation(const string& loc) { location = loc; }
        void setShowTime(const string& time) { showTime = time; }
        void setDate(const string& date) { selectedDate = date; }
        void setPrice(double ticketPrice) { price = ticketPrice; }
        void setQuantity(int qty) { quantity = qty; }
        void setIsValid(bool valid) { isValid = valid; }
        void setSeats(const string& selectedSeats) { seats = formatSeats(selectedSeats, quantity); }

        double getTotalPrice() const {
            return price * quantity;
        }

        // method to assign seats
        void assignSeats(string* selectedSeats, int seatCount) {
            string seatList = "";
            for (int i = 0; i < seatCount; i++) {
                if (i > 0) seatList += ", ";
                seatList += selectedSeats[i];
            }
            setSeats(seatList); // update the seats
        }

        void displayTicketDetails() const {
            cout << "Movie: " << movieName << endl;
            cout << "Location: " << location << endl;
            cout << "Show Time: " << showTime << endl;
            cout << "Date: " << selectedDate << endl;
            cout << "Quantity: " << quantity << endl;
            cout << "Selected Seats: " << seats << endl;
            cout << "Price per ticket: P" << price << endl;
            cout << "Total price: P" << getTotalPrice() << endl;
        }
};

    class Cart {
private:
    Ticket cartItems[15];  // max 15 tix in cart
    int itemCount;

public:
    Cart() : itemCount(0) {}

    void addToCart(const Ticket& ticket) {
        if (itemCount < 15) {
            cartItems[itemCount++] = ticket;
            cout << "Ticket added to cart successfully!" << endl;
        } else {
            cout << "Cart is full. Cannot add more tickets." << endl;
        }
    }

    void removeFromCart(int index) {
        if (index >= 0 && index < itemCount) {
            // shift elements to remove the item at the specified index
            for (int i = index; i < itemCount - 1; i++) {
                cartItems[i] = cartItems[i + 1];
            }
            itemCount--; // decrease the item count
        } else {
            cout << "Invalid cart index." << endl;
        }
    }

    void viewCart() {
        if (itemCount == 0) {
            cout << "Your cart is empty." << endl;
            return;
        }

        cout << "\nCart Contents:" << endl;
        double totalCartPrice = 0.0;
        for (int i = 0; i < itemCount; i++) {
            cout << "\nItem #" << (i + 1) << ":" << endl;
            cartItems[i].displayTicketDetails();
            totalCartPrice += cartItems[i].getTotalPrice();
        }
        cout << "\nTotal Cart Value: P" << totalCartPrice << endl;
    }

    void clearCart() {
        itemCount = 0;
        cout << "Cart cleared." << endl;
    }

    int getItemCount() const {
        return itemCount;
    }

    Ticket* getCartItems() {
        return cartItems;
    }
};

    class Customer : public User {
    private:
        Ticket ticketHistory[50];
        int ticketCount = 0;
        Cart cart;
        int movieIndex;
        int locationIndex;
        int dateIndex;
        int timeIndex;

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
        void setRefundDetails(int movie, int location, int date, int time) {
            movieIndex = movie;
            locationIndex = location;
            dateIndex = date;
            timeIndex = time;
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

    refundChoice--; // adjust for zero-based index

    if (!ticketHistory[refundChoice].getIsValid()) {
        cout << "This ticket is already invalid." << endl;
        return;
    }

    double originalPrice = ticketHistory[refundChoice].getPrice();
    double refundAmount = originalPrice * 0.7; // 70% refund

    cout << "Refund Details:" << endl;
    cout << "Original Ticket Price: P" << originalPrice << endl;
    cout << "Refund Amount (70%): P" << refundAmount << endl;

    char confirm;
    cout << "Confirm refund? (Y/N): ";
    cin >> confirm;

    if (toupper(confirm) == 'Y') {
        string seats = ticketHistory[refundChoice].getSeats();
        int numSeats = ticketHistory[refundChoice].getQuantity();

        // free the seats using member variables
for (int i = 0; i < numSeats; i++) {
    string seatChoice = seats.substr(i * 3, 2);  // get the seat label (e.g., "A1", "B2")
    
    if (seatChoice.size() < 2) {
        cout << "Invalid seat format: " << seatChoice << endl;
        continue;
    }

    char rowChar = toupper(seatChoice[0]); // get the row letter (e.g., 'A', 'B')
    int row = rowChar - 'A'; // convert row letter to index (e.g., 'A' -> 0, 'B' -> 1)

    // extract column from the seat (e.g., "A1" -> column "1")
    int col = seatChoice[1] - '1';  // convert '1' to 0, '2' to 1, etc.
    
    // check if row and col are valid indices before accessing movieSeats
    if (row < 0 || row >= 10 || col < 0 || col >= 10) {
        cout << "Invalid seat position: " << seatChoice << endl;
        continue;
    }

    // use the indices for the specific movie, location, date, and time
    movieSeats[movieIndex][locationIndex][dateIndex][timeIndex][row][col] = 'O';
}


        ticketHistory[refundChoice].setIsValid(false);
        cout << "Ticket refunded successfully. Refund amount: P" << refundAmount << endl;
    } else {
        cout << "Refund cancelled." << endl;
    }
}

        void menu() override {
            int choice;
            bool continueMenu = true;

            while (continueMenu) {
                cout << endl << "Customer Menu" << endl << endl
                    << "1 - View Profile" << endl
                    << "2 - Purchase Tickets" << endl
                    << "3 - View Movies" << endl
                    << "4 - Search Movies" << endl
                    << "5 - Sort Movies" << endl
                    << "6 - My Cart" << endl
                    << "7 - My Tickets" << endl
                    << "8 - Refund Tickets" << endl                
                    << "9 - Logout" << endl                  
                    << "10 - Exit" << endl << endl
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
                    searchMoviesMenu();
                    break;
                case 5:
                    sortMoviesMenu();
                    break;
                case 6:
                    cartMenu();
                    break;
                case 7:
                    viewTickets();
                    break;
                case 8:
                    refundTickets();
                    break;
                case 9:
                    logout();
                    continueMenu = false;
                    break;
                case 10:
                    cout << endl << "Exiting the system." << endl;
                    exit(0);
                    break;
                default:
                    cout << "Invalid choice. Please try again." << endl;
                }
            }
        }

        void searchMoviesMenu() {
            string keyword;
            char choice;
            
            do {
                cout << "Enter search keyword (movie title or synopsis): ";
                cin.ignore();
                getline(cin, keyword);
                
                searchMovies(movies, keyword);

                cout << "Do you want to search again? (Y/N): ";
                cin >> choice;

            } while (choice == 'Y' || choice == 'y'); 
        }

        void sortMoviesMenu() {
            int sortChoice;
            char choice;
            
            do {
                cout << endl << "Sort Movies By:" << endl;
                cout << "1 - Title" << endl;
                cout << "2 - Genre" << endl;
                cout << "3 - Price" << endl;
                cout << "Enter your choice (1-3): ";
                cin >> sortChoice;

                switch (sortChoice) {
                case 1:
                    sortMovies(movies, 5, 0);  // sort by Title (column 0)
                    break;
                case 2:
                    sortMovies(movies, 5, 1);  // sort by Genre (column 1)
                    break;
                case 3:
                    sortMovies(movies, 5, 3);  // sort by Price (column 3)
                    break;
                default:
                    cout << "Invalid choice. Returning to main menu." << endl;
                    return;
                }

                // display sorted movies
                cout << endl << "Movies sorted successfully:" << endl;
                for (int i = 0; i < 5; i++) {
                    cout << "Title: " << movies[i][0] 
                        << ", Genre: " << movies[i][1] 
                        << ", Price: P" << movies[i][3] << endl;
                }

                cout << "Do you want to sort again? (Y/N): ";
                cin >> choice;

            } while (choice == 'Y' || choice == 'y'); 
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
            for (int movie = 0; movie < 5; movie++) {
                int availableSeats = 0;
                
                // array to store available locations
                string availableLocations[3];
                int availableLocationsCount = 0;
                
                // check available seats for each location
                for (int locationIndex = 0; locationIndex < 3; locationIndex++) {
                    int locationSeats = 0;
                    for (int dateIndex = 0; dateIndex < 5; dateIndex++) {
                        for (int timeIndex = 0; timeIndex < 4; timeIndex++) {
                            for (int row = 0; row < 10; row++) {
                                for (int col = 0; col < 10; col++) {
                                    if (movieSeats[movie][locationIndex][dateIndex][timeIndex][row][col] == 'O') {
                                        locationSeats++;
                                        availableSeats++;
                                    }
                                }
                            }
                        }
                    }
                    
                    // if location has available seats, add it to the array
                    if (locationSeats > 0) {
                        string locationName = (locationIndex == 0 ? "Location A" :
                                            locationIndex == 1 ? "Location B" : "Location C");
                        availableLocations[availableLocationsCount++] = locationName;
                    }
                }

                // display movie details
                cout << endl << (movie + 1) << ". Movie Name: " << movies[movie][0] << endl;
                cout << "   Synopsis: " << movies[movie][1] << endl;
                cout << "   Runtime: " << movies[movie][2] << endl;
                cout << "   Price: P" << movies[movie][3] << endl;
                cout << "   Available Seats: " << availableSeats << endl;
                
                // display available locations
                cout << "   Available Locations: ";
                for (int i = 0; i < availableLocationsCount; i++) {
                    cout << availableLocations[i];
                    if (i < availableLocationsCount - 1) {
                        cout << ", ";
                    }
                }
                cout << endl;
                
                cout << "   Primary Location: " << movies[movie][4] << endl;
            }
        }

        bool searchKeyword(const string& text, const string& keyword) {
            int textLength = text.length();
            int keywordLength = keyword.length();

            for (int i = 0; i <= textLength - keywordLength; i++) {
                bool match = true;
                for (int j = 0; j < keywordLength; j++) {
                    if (text[i + j] != keyword[j]) {
                        match = false;
                        break;
                    }
                }
                if (match) {
                    return true;
                }
            }
            return false;
        }

        void searchMovies(string movies[5][5], const string& keyword) {
            cout << "Search results for \"" << keyword << "\":" << endl;
            bool found = false;

            for (int i = 0; i < 5; i++) {
                if (searchKeyword(movies[i][0], keyword) || searchKeyword(movies[i][1], keyword)) {
                    cout << "Title: " << movies[i][0] << ", Genre: " << movies[i][1] 
                        << ", Duration: " << movies[i][2] << ", Price: " << movies[i][3]
                        << ", Location: " << movies[i][4] << endl;
                    found = true;
                }
            }

            if (!found) {
                cout << "No movies found matching the keyword." << endl;
            }
        }

        // function to sort movies using bubble sort
        void sortMovies(string movies[][5], int size, int column) {
            for (int i = 0; i < size - 1; i++) {
                for (int j = 0; j < size - i - 1; j++) {
                    if (movies[j][column] > movies[j + 1][column]) {
                        // swap entire rows
                        for (int k = 0; k < 5; k++) {
                            string temp = movies[j][k];
                            movies[j][k] = movies[j + 1][k];
                            movies[j + 1][k] = temp;
                        }
                    }
                }
            }
        }

        int countAvailableSeats(int movieIndex, int locationIndex, int dateIndex, int timeIndex) {
            int availableSeats = 0;
            for (int row = 0; row < 10; row++) {
                for (int col = 0; col < 10; col++) {
                    if (movieSeats[movieIndex][locationIndex][dateIndex][timeIndex][row][col] == 'O') {
                        availableSeats++;
                    }
                }
            }
            return availableSeats;
        }

        void cartMenu() {
            int choice;
            bool continueCartMenu = true;

            while (continueCartMenu) {
                cout << endl << "Cart Menu:" << endl;
                cout << "1 - View Cart" << endl;
                cout << "2 - Remove Tickets from Cart" << endl;
                cout << "3 - Clear Cart" << endl;
                cout << "4 - Checkout Tickets" << endl;
                cout << "5 - Return to Main Menu" << endl;
                cout << "Enter your choice: ";
                cin >> choice;

                switch (choice) {
                    case 1:
                        cart.viewCart();
                        break;
                    case 2: {
                        if (cart.getItemCount() == 0) {
                            cout << "Cart is empty." << endl;
                            break;
                        }
                        cart.viewCart();
                        int removeIndex;
                        cout << "Enter the number of the item to remove: ";
                        cin >> removeIndex;
                        cart.removeFromCart(removeIndex - 1);
                        break;
                    }
                    case 3:
                        cart.clearCart();
                        break;
                    case 4:
                        checkoutCart();
                        break;
                    case 5:
                        continueCartMenu = false;
                        break;
                    default:
                        cout << "Invalid choice. Please try again." << endl;
                }
            }
        }

void checkoutCart() {
    if (cart.getItemCount() == 0) {
        cout << "Cart is empty. Nothing to checkout." << endl;
        return;
    }

    // if more than one item in cart, let user choose which to checkout
    int selectedItemIndex = 0;
    if (cart.getItemCount() > 1) {
        cart.viewCart(); // display cart contents
        
        bool validSelection = false;
        do {
            cout << "\nEnter the item number you want to checkout: ";
            
            if (!(cin >> selectedItemIndex)) {
                cin.clear();
                cin.ignore(numeric_limits<streamsize>::max(), '\n');
                cout << "Please enter a valid number." << endl;
                continue;
            }
            
            // adjust for zero-based indexing
            selectedItemIndex--;
            
            if (selectedItemIndex >= 0 && selectedItemIndex < cart.getItemCount()) {
                validSelection = true;
            } else {
                cout << "Invalid item number. Please try again." << endl;
            }
        } while (!validSelection);
    }

    // get ticket details from the cart item
    
    Ticket* selectedTicket = &cart.getCartItems()[selectedItemIndex];

    string movieName = selectedTicket->getMovieName();
    string location = selectedTicket->getLocation();
    string date = selectedTicket->getSelectedDate();
    string time = selectedTicket->getShowTime();
    int seatCount = selectedTicket->getQuantity();

    // display available seats
    displaySeats(movieIndex, locationIndex, dateIndex, timeIndex);

    cout << endl;

    // book the selected seats
    string* selectedSeats = bookSeats(movieIndex, locationIndex, dateIndex, timeIndex, seatCount);
    
    // assign the selected seats to the ticket
    selectedTicket->assignSeats(selectedSeats, seatCount);

    // payment method selection (unchanged)
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

    // payment processing (unchanged)
    switch (paymentMethod) {
        case 1: {  // card Payment
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
        case 2: {  // bank Payment
            string accountNumber, accountName;
            cout << "Enter Account Number: ";
            cin >> accountNumber;
            cout << "Enter Account Name: ";
            cin.ignore();
            getline(cin, accountName);
            cout << endl << "Processing bank transfer..." << endl;
            break;
        }
        case 3: {  // e-Wallet Payment
            int ewalletChoice;
            string phoneNumber;
            cout << "Select E-Wallet:" << endl;
            cout << "1. GCash\n2. Maya" << endl;
            cout << endl << "Enter your choice: ";
            cin >> ewalletChoice;
            
            string ewalletName = (ewalletChoice == 1) ? "GCash" : "Maya";
            
            cout << "Enter phone number linked to " << ewalletName << ": ";
            cin >> phoneNumber;
            cout << endl << "Processing " << ewalletName << " payment..." << endl;
            break;
        }
    }

    cout << "Payment successful!" << endl;

    // process the selected ticket or all tickets
    Ticket* cartItems = cart.getCartItems();
    int cartItemCount = cart.getItemCount();

    // add selected ticket to ticket history
    if (cartItemCount > 1) {
        if (ticketCount < 50) {
            ticketHistory[ticketCount++] = cartItems[selectedItemIndex];
            // remove the selected item from cart
            cart.removeFromCart(selectedItemIndex);
        } else {
            cout << "Ticket history is full. Cannot add more tickets." << endl;
        }
    } else {
        // if only one item, process the entire cart
        for (int i = 0; i < cartItemCount; i++) {
            if (ticketCount < 50) {
                ticketHistory[ticketCount++] = cartItems[i];
            } else {
                cout << "Ticket history is full. Cannot add more tickets." << endl;
                break;
            }
        }
        cart.clearCart();
    }

    cout << "Checkout completed successfully!" << endl;
}


        void purchaseTickets() {
            char purchaseMore;
            char addToCartChoice;
            
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

                int dateChoice;
                bool validDate = false;
                do {
                    cout << endl << "Choose Date:" << endl;
                    for (int i = 0; i < 5; i++) {
                        cout << (i + 1) << " - " << availableDates[i] << endl;
                    }
                    cout << "Enter the number corresponding to your choice (1-5): ";
                    if (!(cin >> dateChoice)) {
                        cin.clear();
                        cin.ignore(numeric_limits<streamsize>::max(), '\n');
                        cout << "Please enter a valid number." << endl;
                        continue;
                    }
                    if (dateChoice >= 1 && dateChoice <= 5) {
                        validDate = true;
                    } else {
                        cout << "Invalid date selection. Please try again." << endl;
                    }
                } while (!validDate);

                // choose a time
                int timeChoice;
                bool validTime = false;
                do {
                    cout << endl << "Choose Time:" << endl;
                    for (int i = 0; i < 4; i++) {
                        cout << (i + 1) << " - " << times[i] << endl;
                    }
                    cout << "Enter the number corresponding to your choice (1-4): ";
                    if (!(cin >> timeChoice)) {
                        cin.clear();
                        cin.ignore(numeric_limits<streamsize>::max(), '\n');
                        cout << "Please enter a valid number." << endl;
                        continue;
                    }
                    if (timeChoice >= 1 && timeChoice <= 4) {
                        validTime = true;
                    } else {
                        cout << "Invalid time selection. Please try again." << endl;
                    }
                } while (!validTime);

                // store the selected date and time
                string selectedDate = availableDates[dateChoice - 1];
                string selectedTime = times[timeChoice - 1];

                int dateIndex = dateChoice - 1;
                int timeIndex = timeChoice - 1;

                int availableSeats = countAvailableSeats(movieChoice, locationChoice - 1, dateIndex, timeIndex);

                cout << endl << "Booking Details" << endl << endl;
                cout << "Movie: " << movies[movieChoice][0] << endl;
                cout << "Synopsis: " << movies[movieChoice][1] << endl;
                cout << "Runtime: " << movies[movieChoice][2] << endl;
                cout << "Location: " << selectedLocation << endl;
                cout << "Date: " << selectedDate << endl;  // display selected date
                cout << "Time: " << selectedTime << endl; // display selected time
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
                //displaySeats(movieChoice, locationChoice - 1, dateIndex, timeIndex);
                //string* bookedSeats = nullptr;

                // add to cart or checkout prompt
                do {
                    cout << endl << "What do you want to do?" << endl;
                    cout << "1 - Add to Cart" << endl;
                    cout << "2 - Checkout" << endl;
                    cout << "Enter your choice (1-2): ";
                    cin >> addToCartChoice;

                    if (addToCartChoice != '1' && addToCartChoice != '2') {
                        cout << "Invalid choice. Please enter 1 or 2." << endl;
                    }
                } while (addToCartChoice != '1' && addToCartChoice != '2');

                // create ticket object
                Ticket currentTicket(
                    movies[movieChoice][0], 
                    selectedLocation, 
                    selectedTime, 
                    selectedDate,
                    stod(movies[movieChoice][3]), 
                    numTickets,
                    ""
                );

                if (addToCartChoice == '1') {
                    // add to cart
                    cart.addToCart(currentTicket);

                    // ask if user wants to purchase more tickets
                    do {
                        cout << endl << "Would you like to purchase more tickets? (Y/N): ";
                        cin >> purchaseMore;
                        purchaseMore = toupper(purchaseMore);
                        if (purchaseMore != 'Y' && purchaseMore != 'N') {
                            cout << "Please enter Y or N." << endl;
                        }
                    } while (purchaseMore != 'Y' && purchaseMore != 'N');
                }
                else { // checkout current ticket
                    // checkout process for the current ticket only
                    // Now reserve the seats for checkout

                    displaySeats(movieChoice, locationChoice - 1, dateIndex, timeIndex);
                    string* bookedSeats = bookSeats(movieChoice, locationChoice - 1, dateIndex, timeIndex, numTickets);

                    string selectedSeats;
                    for (int i = 0; i < numTickets; i++) {
                        if (i > 0) selectedSeats += ", ";
                        selectedSeats += bookedSeats[i];
                    }

                    // update ticket with booked seats
                    currentTicket.setSeats(selectedSeats);
                    
                    cout << endl << "Proceeding to checkout for the current booking:" << endl;
                    currentTicket.displayTicketDetails();

                    // payment method selection
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

                    // handle payment based on selected method
                    switch (paymentMethod) {
                        case 1: {  // card Payment
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
                        case 2: {  // bank Payment
                            string accountNumber, accountName;
                            cout << "Enter Account Number: ";
                            cin >> accountNumber;
                            cout << "Enter Account Name: ";
                            cin.ignore();
                            getline(cin, accountName);
                            cout << "Processing bank transfer..." << endl;
                            break;
                        }
                        case 3: {  // e-Wallet Payment
                            int ewalletChoice;
                            string phoneNumber;
                            cout << "Select E-Wallet:" << endl;
                            cout << "1. GCash\n2. Maya" << endl;
                            cout << "Enter your choice: ";
                            cin >> ewalletChoice;

                            string ewalletName = (ewalletChoice == 1) ? "GCash" : "Maya";
                            cout << "Enter phone number linked to " << ewalletName << ": ";
                            cin >> phoneNumber;
                            cout << "Processing " << ewalletName << " payment..." << endl;
                            break;
                        }
                    }

                    cout << "Payment successful!" << endl;

                    // add current ticket directly to ticket history
                    if (ticketCount < 50) {
                        ticketHistory[ticketCount++] = currentTicket;
                    } else {
                        cout << "Ticket history is full. Cannot add more tickets." << endl;
                    }

                    cout << "Checkout for current booking completed successfully!" << endl;

                    // ask if the user wants to purchase more tickets
                    do {
                        cout << endl << "Would you like to purchase more tickets? (Y/N): ";
                        cin >> purchaseMore;
                        purchaseMore = toupper(purchaseMore);
                        if (purchaseMore != 'Y' && purchaseMore != 'N') {
                            cout << "Please enter Y or N." << endl;
                        }
                    } while (purchaseMore != 'Y' && purchaseMore != 'N');
                }
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

    // admin Class - Derived from User (Polymorphism)
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
                    cout << endl << "Movie " << (i + 1) << "." << endl;
                    cout << endl << "Movie Name: " << movies[i][0] << endl;
                    cout << "   Synopsis: " << movies[i][1] << endl;
                    cout << "   Runtime: " << movies[i][2] << endl;
                    cout << "   Price: P" << movies[i][3] << endl;
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

            char confirm;
            cout << "Are you sure you want to delete " << movies[movieChoice - 1][0] << "? (Y/N): ";
            cin >> confirm;

            if (toupper(confirm) == 'Y') {

                // clear movie details
                for (int j = 0; j < 5; j++) {
                    movies[movieChoice - 1][j] = "";
                }

                // clear corresponding seats for the deleted movie
                for (int location = 0; location < 3; location++) {
                    for (int date = 0; date < 5; date++) {
                        for (int time = 0; time < 4; time++) {
                            for (int row = 0; row < 10; row++) {
                                for (int col = 0; col < 10; col++) {
                                    movieSeats[movieChoice - 1][location][date][time][row][col] = 'O';  // reset to 'O'
                                }
                            }
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

    Ticket* refundableTickets[5000];  // array to store refundable tickets
    int refundableCount = 0;

    // collect all refundable tickets from all customers
    for (int i = 0; i < userCount; i++) {
        Customer* customer = dynamic_cast<Customer*>(users[i]);
        if (customer) {
            Ticket* tickets = customer->getTickets();
            for (int j = 0; j < customer->getTicketCount(); j++) {
                // only add tickets that are valid and not yet refunded
                if (tickets[j].getIsValid()) {
                    refundableTickets[refundableCount++] = &tickets[j];
                }
            }
        }
    }

    // check if there are any refundable tickets
    if (refundableCount == 0) {
        cout << "No tickets available for refund." << endl;
        return;
    }

    // display refundable tickets
    cout << endl << "Refundable Tickets:" << endl;
    for (int i = 0; i < refundableCount; i++) {
        cout << endl << "Ticket #" << (i + 1) << endl;
        refundableTickets[i]->displayTicketDetails();
    }

    int choice;
    cout << "Enter the number of the ticket to refund (0 to cancel): ";
    cin >> choice;

    // validate choice
    if (choice <= 0 || choice > refundableCount) {
        cout << "Refund cancelled." << endl;
        return;
    }

    // select the ticket for refund
    Ticket* ticket = refundableTickets[choice - 1];

    // calculate refund amount (70% of total price)
    double refundAmount = ticket->getTotalPrice() * 0.7;

    // get ticket details
    string movieName = ticket->getMovieName();
    string location = ticket->getLocation();
    string showtime = ticket->getShowTime();
    string selectedDate = ticket->getSelectedDate();

    // find movie index
    int movieIndex = -1;
    for (int i = 0; i < 5; i++) {
        if (movies[i][0] == movieName) {
            movieIndex = i;
            break;
        }
    }

    // find location index
    int locationIndex = -1;
    string locations[] = {"Location A", "Location B", "Location C"};
    for (int i = 0; i < 3; i++) {
        if (locations[i] == location) {
            locationIndex = i;
            break;
        }
    }

    // find date index
    int dateIndex = -1;
    for (int i = 0; i < 5; i++) {
        if (availableDates[i] == selectedDate) {
            dateIndex = i;
            break;
        }
    }

    // find time index
    int timeIndex = -1;
    for (int i = 0; i < 4; i++) {
        if (times[i] == showtime) {
            timeIndex = i;
            break;
        }
    }

    // validate all indices
    if (movieIndex == -1 || locationIndex == -1 || dateIndex == -1 || timeIndex == -1) {
        cout << "Error: Unable to process refund. Invalid ticket details." << endl;
        return;
    }

    // get seats and free them
    string seats = ticket->getSeats();
    int numSeats = ticket->getQuantity();

    // parse and free seats
    stringstream ss(seats);
    string seat;
    while (getline(ss, seat, ',')) {
        // trim whitespace
        seat.erase(0, seat.find_first_not_of(" "));
        seat.erase(seat.find_last_not_of(" ") + 1);

        // convert seat to row and column
        char rowChar = seat[0];
        int row = toupper(rowChar) - 'A';
        int col = stoi(seat.substr(1)) - 1;

        // ensure valid indices
        if (row >= 0 && row < 10 && col >= 0 && col < 10) {
            movieSeats[movieIndex][locationIndex][dateIndex][timeIndex][row][col] = 'O';
        } else {
            cout << "Invalid seat: " << seat << endl;
        }
    }

    // mark ticket as invalid
    ticket->setIsValid(false);

    // process refund
    cout << "Refund processed: P" << fixed << setprecision(2) << refundAmount << endl;
    cout << "Ticket for " << movieName << " at " << location << " has been refunded." << endl;
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

    // main Menu Function
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

                            // find the admin user
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