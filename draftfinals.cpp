#include <iostream>
#include <iomanip>
#include <cctype>
#include <limits>
#include <sstream>

using namespace std;

class Ticket;
class Customer;

// seating array for different movies and locations
    char movieSeats[10][3][5][4][10][10]; // [movies][locations][dates][showtimes][rows][columns]

int movieCount = 5; // initial value

string movies[10][5] = {
    {"Wicked", "Fantasy", "120 min", "300", "SM Lipa, Robinsons Lipa, Evia"},
    {"Mufasa", "Adventure", "118 min", "250", "SM Lipa, Robinsons Lipa, Evia"},
    {"Hello, Love, Again", "Romance", "130 min", "200", "SM Lipa, Robinsons Lipa, Evia"},
    {"Heretic", "Horror ", "100 min", "350", "SM Lipa, Robinsons Lipa, Evia"},
    {"Moana 2", "Adventure", "110 min", "270", "SM Lipa, Robinsons Lipa, Evia"}
};

// array for dates (e.g., Today + 4 future dates)
string availableDates[5] = {"Today", "December 5", "December 6", "December 7", "December 8"};
// times are based on the movie runtime; start time is 11 AM
string times[4] = {"10 AM", "1 PM", "4 PM", "7 PM"};

// initialization of seats to available ('O') for all movies and locations
void initializeAllSeats() {
    for (int movie = 0; movie < movieCount; movie++){
        for (int location = 0; location < 3; location++){
            for (int date = 0; date < 5; date++){
                for (int time = 0; time < 4; time++){
                    for (int row = 0; row < 10; row++){
                        for (int col = 0; col < 10; col++){
                            movieSeats[movie][location][date][time][row][col] = 'O';
                        }
                    }
                }
            }
        }
    }
}

    bool isEmpty(string input){

        bool isEmpty = false;

        if (input.length() == 0){
            isEmpty = true;
        }

        return isEmpty;
    }

    string inputValidName() {
        string name;
        bool isValid;

        do {
            bool isValid = true;
            
            cout << "Enter name: ";
            getline(cin, name);

            if (isEmpty(name)) {
                cout << "Input should not be empty." << endl;
                isValid = false;
            }

        } while (!isValid);

        return name;
    }

string inputValidUsername() {
        string username;
        bool isValid = true;
        
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
        do {
            cout << "Enter usename: ";
            getline(cin, username);

            if (isEmpty(username)) {
                cout << "Input should not be empty." << endl;
                isValid = false;
                continue;
            }

            if (username.length() < 5) {
                cout << "Minimum of 5 characters!" << endl;
                isValid = false;
                continue;
            }
            
            isValid = true;
            for (char c : username) {
                if (!isalpha(c) && c != ' ') {
                    cout << "Userame should only contain letters." << endl;
                    isValid = false;
                    break;
                }
            }
        } while (!isValid);
        return username;
    }

string inputValidPassword() {
    string password;
    bool hasLetter;
    bool hasDigit;
    bool isValid;

    do {
        hasLetter = false;
        hasDigit = false;
        isValid = true;

        cout << "Enter password: ";
        
        // Clear any previous error states
        cin.clear();
          
        // Use getline for complete input handling
        getline(cin, password);

        if (password.empty()) {
            cout << "Password should not be empty.\n";
            isValid = false;
            continue;
        }
        
        if (password.length() < 8) {
            cout << "Invalid input. Password should contain a minimum of 8 characters.\n";
            isValid = false;
            continue;
        }

        // Reset flags for checking letters and digits
        hasLetter = false;
        hasDigit = false;

        for (char c : password) {
            if (isalpha(c)) {
                hasLetter = true;
            }
            if (isdigit(c)) {
                hasDigit = true;
            }
        }

        if (!hasLetter || !hasDigit) {
            cout << "Invalid input. Password should contain both letters and numbers.\n";
            isValid = false;
        }
    } while (!isValid);
    
    return password;
}

    string inputValidEmail() {
        string email;
        bool isValid;

        do {
            isValid = true;

            cout << "Enter email: ";
            getline(cin, email);

            if (isEmpty(email)) {
                cout << "Input should not be empty." << endl;
                isValid = false;
            }

            // check if '@' exists exactly once and there is at least one '.' after '@'
            size_t atPos = email.find('@');
            size_t dotPos = email.find('.', atPos);

            isValid = (atPos != string::npos && dotPos != string::npos && atPos < dotPos);

            if (!isValid) {
                cout << "Invalid input. Email should contain '@' and a '.' after '@'." << endl;
            }
        } while (!isValid);
        
        return email;
    }

    string inputValidContactNum() {
        string contactNum;
        bool isValid;

        do {
            isValid = true;

            cout << "Enter contact number: ";
            getline(cin, contactNum);

            if (isEmpty(contactNum)) {
                cout << "Contact Number should not be empty." << endl;
                isValid = false;
            }
            else if (contactNum.length() != 11) {
                cout << "Invalid input. Contact number should contain 11 numbers only." << endl;
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
        return contactNum;
    }

int inputValidPaymentMethod() {
    int paymentMethod;
    bool validPayment;

    do {
        validPayment = true;

        cin.ignore(numeric_limits<streamsize>::max(), '\n');

        cout << endl << "Select payment method:" << endl;
        cout << "1. Card" << endl;
        cout << "2. Bank" << endl;
        cout << "3. E-Wallet" << endl;
        cout << "Enter choice (1-3): ";

        // Input validation
        if (!(cin >> paymentMethod)) {
            cin.clear();
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
            cout << "Please enter a valid number." << endl;
            validPayment = false;
            continue;
        }

        // Check if input is within valid range
        if (paymentMethod >= 1 && paymentMethod <= 3) {
            validPayment = true;
        } else {
            cout << "Invalid payment method. Please try again." << endl;
        }
    } while (!validPayment);

    return paymentMethod;
}

string inputValidCardNumber() {
    string cardNumber;
    bool isValid;

    do {
        isValid = true;
        
        cout << "Enter Card Number (16 digits): ";
        getline(cin, cardNumber);
        
        // Check for empty input
        if (isEmpty(cardNumber)) {
            cout << "Card number should not be empty." << endl;
            isValid = false;
            continue;
        }

        // Check for exactly 16 digits
        if (cardNumber.length() != 16) {
            cout << "Card number must be exactly 16 digits." << endl;
            isValid = false;
            continue;
        }

        // Verify all characters are digits
        for (char c : cardNumber) {
            if (!isdigit(c)) {
                cout << "Card number should only contain digits." << endl;
                isValid = false;
                break;
            }
        }
    } while (!isValid);

    return cardNumber;
}

string inputValidExpirationDate() {
    string expDate;
    bool isValid;

    do {
        isValid = true;

        cout << "Enter Expiration Date (MM/YY): ";
        getline(cin, expDate);

        // Check for empty input
        if (isEmpty(expDate)) {
            cout << "Expiration date should not be empty." << endl;
            isValid = false;
            continue;
        }

        // Check for MM/YY format
        if (expDate.length() != 5 || expDate[2] != '/') {
            cout << "Invalid format. Use MM/YY format." << endl;
            isValid = false;
            continue;
        }

        // Validate month
        int month = stoi(expDate.substr(0, 2));
        if (month < 1 || month > 12) {
            cout << "Invalid month. Month must be between 01 and 12." << endl;
            isValid = false;
            continue;
        }

        // Validate digits
        string monthStr = expDate.substr(0, 2);
        string yearStr = expDate.substr(3, 2);
        for (char c : monthStr + yearStr) {
            if (!isdigit(c)) {
                cout << "Expiration date must contain only digits and '/'." << endl;
                isValid = false;
                break;
            }
        }
    } while (!isValid);

    return expDate;
}

string inputValidCVC() {
    string cvc;
    bool isValid;

    do {
        isValid = true;

        cout << "Enter CVC (3 digits): ";
        getline(cin, cvc);
        
        // Check for empty input
        if (isEmpty(cvc)) {
            cout << "CVC should not be empty." << endl;
            isValid = false;
            continue;
        }

        // Check for exactly 3 digits
        if (cvc.length() != 3) {
            cout << "CVC must be exactly 3 digits." << endl;
            isValid = false;
            continue;
        }

        // Verify all characters are digits
        for (char c : cvc) {
            if (!isdigit(c)) {
                cout << "CVC should only contain digits." << endl;
                isValid = false;
                break;
            }
        }
    } while (!isValid);

    return cvc;
}

string inputValidBankAccountNumber() {
    string accountNumber;
    bool isValid;

    do {
        isValid = true;

        cin.ignore(numeric_limits<streamsize>::max(), '\n');
        
        cout << "Enter Account Number: ";
        getline(cin, accountNumber);       

        // Check for empty input
        if (isEmpty(accountNumber)) {
            cout << "Account number should not be empty." << endl;
            isValid = false;
            continue;
        }
        // Check for empty input
        if (accountNumber.length() < 12) {
            cout << "Account number should be at least 12 digits." << endl;
            isValid = false;
            continue;
        }

        // Verify all characters are digits
        for (char c : accountNumber) {
            if (!isdigit(c)) {
                cout << "Account number should only contain digits." << endl;
                isValid = false;
                break;
            }
        }
    } while (!isValid);

    return accountNumber;
}

string inputValidBankAccountName() {
    string accountName;
    bool isValid;

    do {
        cout << "Enter Account Name: ";
        getline(cin, accountName);

        isValid = true;
        
        // Check for empty input
        if (isEmpty(accountName)) {
            cout << "Account name should not be empty." << endl;
            isValid = false;
            continue;
        }

        for (char c : accountName) {
            if (isdigit(c)){
                cout << "Account name should not contain letters." << endl;
                isValid = false;
                continue;
            }
        }
    } while (!isValid);

    return accountName;
}

int inputValidEWalletChoice() {
    int ewalletChoice;
    bool validChoice;

    do {
        validChoice = true;
        cout << "Select E-Wallet:" << endl;
        cout << "1. GCash" << endl;
        cout << "2. Maya" << endl;
        cout << "Enter your choice: ";

        // Input validation
        if (!(cin >> ewalletChoice)) {
            cin.clear();
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
            cout << "Please enter a valid number." << endl;
            validChoice = false;
            continue;
        }

        // Check if input is 1 or 2
        if (ewalletChoice == 1 || ewalletChoice == 2) {
            validChoice = true;
        } else {
            cout << "Invalid e-wallet selection. Please try again." << endl;
        }
    } while (!validChoice);

    return ewalletChoice;
}

string inputValidEWalletPhoneNumber() {
    string phoneNumber;
    bool isValid;

    do {
        isValid = true;

        cout << "Enter phone number: ";
        getline(cin, phoneNumber);

        // Check for empty input
        if (isEmpty(phoneNumber)) {
            cout << "Phone number should not be empty." << endl;
            isValid = false;
            continue;
        }

        // Check for exactly 11 digits
        if (phoneNumber.length() != 11) {
            cout << "Phone number must be exactly 11 digits." << endl;
            isValid = false;
            continue;
        }

        // Verify all characters are digits
        for (char c : phoneNumber) {
            if (!isdigit(c)) {
                cout << "Phone number should only contain digits." << endl;
                isValid = false;
                break;
            }
        }
    } while (!isValid);

    return phoneNumber;
}

string capitalizeFirstLetters(string input) {
    bool capitalize = true;
    for (size_t i = 0; i < input.length(); i++) {
        if (capitalize && isalpha(input[i])) {
            input[i] = toupper(input[i]);
            capitalize = false;
        } else {
            input[i] = tolower(input[i]);
        }
        if (isspace(input[i])) {
            capitalize = true; // next word
        }
    }
    return input;
}

// display available seats for a specific movie and location
void displaySeats(int movieIndex, int locationIndex, int dateIndex, int timeIndex) {
    cout << endl << "Seating Arrangement (O = Available, X = Taken):\n" << endl;
    cout << " ";
    for (int col = 1; col <= 10; col++){
        cout << setw(2) << col << " ";
    }
    cout << endl;

    for (int row = 0; row < 10; row++){
        cout << char('A' + row) << " ";
        for (int col = 0; col < 10; col++){
            cout << setw(2) << movieSeats[movieIndex][locationIndex][dateIndex][timeIndex][row][col] << " ";
        }
        cout << endl;
    }
}

string *bookSeats(int movieIndex, int locationIndex, int dateIndex, int timeIndex, int numTickets) {
    static string selectedSeats[15];

    for (int i = 0; i < numTickets; i++){
        string seatChoice;
        bool validSeat = false;

        while (!validSeat){
            cout << "Enter seat " << (i + 1) << " (e.g., A1, B10): ";
            cin >> seatChoice;

            if (seatChoice.length() < 2 || seatChoice.length() > 3){
                cout << "Invalid format. Try again." << endl;
                continue;
            }

            char rowChar = toupper(seatChoice[0]);
            int col = stoi(seatChoice.substr(1)) - 1;

            if (rowChar < 'A' || rowChar > 'J' || col < 0 || col > 10){
                cout << "Seat out of range. Try again." << endl;
                continue;
            }

            int row = rowChar - 'A';

            if (movieSeats[movieIndex][locationIndex][dateIndex][timeIndex][row][col] == 'X'){
                cout << "Seat already taken. Choose another seat." << endl;
            }
            else {
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
    explicit ValidationException(const string &msg) : message(msg) {}
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
        cout << "Name: " << name << endl
             << "Username: " << username << endl
             << "Email: " << email << endl
             << "Contact: " << contactNumber << endl
             << "Role: " << (isAdmin ? "Admin" : "Customer") << endl;
    }

    bool getIsAdmin() { return isAdmin; }
    string getUsername() { return username; }
    string getPassword() { return password; }

    virtual void menu() = 0; // pure virtual function for strategy pattern

    // logout functionality
    void logout() const {
        cout << endl << "Logging out..." << endl;

        system("pause");
        system("cls");
    }
};

// cinemaSystem Class (Singleton Pattern)
class CinemaSystem {
private:
    // static ++ private constructor for Singleton (ganto dba)
    static CinemaSystem instance;
    User **users;
    int userCount;
    int maxUsers;

    CinemaSystem() : userCount(0), maxUsers(100){
        users = new User *[maxUsers]; // dMA for users
    }

public:
    CinemaSystem(const CinemaSystem &) = delete;
    CinemaSystem &operator=(const CinemaSystem &) = delete;

    ~CinemaSystem(){
        for (int i = 0; i < userCount; i++){
            delete users[i]; // delete user
        }
        delete[] users; // delete the array
    }

    // getter for instance
    static CinemaSystem &getInstance(){
        return instance;
    }

    void addUser(User *user){
        if (userCount < maxUsers){
            users[userCount++] = user;
        }
        else {
            throw ValidationException("Maximum users reached!");
        }
    }

    User *authenticateUser(const string &username, const string &password){
        for (int i = 0; i < userCount; i++){
            if (users[i]->getUsername() == username &&
                users[i]->getPassword() == password){
                return users[i];
            }
        }
        return nullptr;
    }

    // admin authentication method
    bool authenticateAdmin(const string &username, const string &password){
        for (int i = 0; i < userCount; i++){
            // check if the user is an admin and credentials match
            if (users[i]->getUsername() == username &&
                users[i]->getPassword() == password &&
                users[i]->getIsAdmin()){
                return true;
            }
        }
        return false;
    }

    void deleteUser(int index){
        if (index >= 0 && index < userCount){
            delete users[index]; // delete the user
            for (int i = index; i < userCount - 1; i++){
                users[i] = users[i + 1];
            }
            userCount--;
        }
        else {
            cout << "Invalid user index." << endl;
        }
    }

    void displayUsers() const {
        for (int i = 0; i < userCount; i++){
            cout << "User #" << i + 1 << endl;
            users[i]->displayProfile();
            cout << endl;
        }
    }
    // to search for the username
    bool isUsernameExists(const string &username){
        for (int i = 0; i < userCount; i++){
            if (users[i]->getUsername() == username){
                return true;
            }
        }
        return false;
    }
    // getter
    int getUserCount() const {
        return userCount;
    }

    User **getUsers(){
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
    string selectedDate;
    string seats;
    double price;
    int quantity;
    bool isValid;

string formatSeats(const string &seatInput, int quantity) const {
    string formattedSeats[100];
    int seatCount = 0;
    string currentSeat = "";
    
    // loop through each character in the input string
    for (size_t i = 0; i < seatInput.size(); i++){
        char ch = seatInput[i];

        // if it's the first letter, capture it as the row label
        if (isalpha(ch) && currentSeat.empty()){
            currentSeat = toupper(ch); // store the row character, making sure it's uppercase
        }
        // if it's a digit, it's part of the seat number
        else if (isdigit(ch)){
            currentSeat += ch; // add the digit to the current seat number
        }
    }

    // after processing the seat input, store it if valid
    if (!isEmpty(currentSeat)){
        formattedSeats[seatCount++] = currentSeat;
    }

    // create the formatted string of seats
    string result = "";
    for (int i = 0; i < seatCount; i++){
        result += formattedSeats[i];
        if (i < seatCount - 1){
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

    void setMovieName(const string &movie) { movieName = movie; }
    void setLocation(const string &loc) { location = loc; }
    void setShowTime(const string &time) { showTime = time; }
    void setDate(const string &date) { selectedDate = date; }
    void setPrice(double ticketPrice) { price = ticketPrice; }
    void setQuantity(int qty) { quantity = qty; }
    void setIsValid(bool valid) { isValid = valid; }
    void setSeats(const string &selectedSeats) { seats = formatSeats(selectedSeats, quantity); }

    double getTotalPrice() const {
        return price * quantity;
    }

    // method to assign seats
    void assignSeats(string *selectedSeats, int seatCount){
        string seatList = "";
        for (int i = 0; i < seatCount; i++){
            if (i > 0)
                seatList += ", ";
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
    void displayTicketDetailsOnCart() const {
        cout << "Movie: " << movieName << endl;
        cout << "Location: " << location << endl;
        cout << "Show Time: " << showTime << endl;
        cout << "Date: " << selectedDate << endl;
        cout << "Quantity: " << quantity << endl;
        cout << "Price per ticket: P" << price << endl;
        cout << "Total price: P" << getTotalPrice() << endl;
    }
};

class Cart {
private:
    Ticket cartItems[15]; // max 15 tix in cart
    int itemCount;

public:
    Cart() : itemCount(0) {}

    void addToCart(const Ticket &ticket){
        if (itemCount < 15){
            cartItems[itemCount++] = ticket;
            cout << "Ticket added to cart successfully!" << endl;
        }
        else {
            cout << "Cart is full. Cannot add more tickets." << endl;
        }
    }

    void removeFromCart(int index){
        if (index >= 0 && index < itemCount){
            // shift elements to remove the item at the specified index
            for (int i = index; i < itemCount - 1; i++){
                cartItems[i] = cartItems[i + 1];
            }
            itemCount--; // decrease the item count
        }
        else {
            cout << "Invalid cart index." << endl;
        }
    }

    void viewCart(){
        if (itemCount == 0){
            cout << endl << "Your cart is empty." << endl << endl << "Returning to Main Menu.." << endl;
                    system("pause");
                    system("cls");
            return;
        }

        cout << endl << "Cart Contents:" << endl;
        double totalCartPrice = 0.0;
        for (int i = 0; i < itemCount; i++){
            cout << endl << "Item #" << (i + 1) << ":" << endl;
            cartItems[i].displayTicketDetailsOnCart();
            totalCartPrice += cartItems[i].getTotalPrice();
        }
        cout << endl << "Total Cart Value: P" << totalCartPrice << endl;
    }

    void clearCart(){
        itemCount = 0;
        cout << "Cart cleared." << endl;
    }

    int getItemCount() const {
        return itemCount;
    }

    Ticket *getCartItems(){
        return cartItems;
    }
};

class Customer : public User {
private:
    Ticket ticketHistory[50];
    Cart cart;
    int ticketCount = 0;
    int movieIndex;
    int locationIndex;
    int dateIndex;
    int timeIndex;
    int movieSelected[50];
    int locationSelected[50];
    int dateSelected[50];
    int timeSelected[50];
    int counter;

public:
    Customer(string nm, string uname, string mail, string contact, string pass) : User(nm, uname, mail, contact, pass, false), ticketCount(0), counter(1) {}

    int getTicketCount() const { return ticketCount; }
    Ticket *getTickets() { return ticketHistory; }

    void addTicket(const Ticket &ticket){
        if (ticketCount < 50){
            ticketHistory[ticketCount++] = ticket;
        }
    }
    void setRefundDetails(int movie, int location, int date, int time){
        movieIndex = movie;
        locationIndex = location;
        dateIndex = date;
        timeIndex = time;
    }

    void menu() override {
        int choice;
        bool continueMenu = true;

        while (continueMenu){
            cout << endl << "Customer Menu" << endl << endl << "1 - View Profile" << endl << "2 - Purchase Tickets" << endl << "3 - View Movies" << endl << "4 - Search Movies" << endl << "5 - Sort Movies" << endl << "6 - My Cart" << endl << "7 - My Tickets" << endl << "8 - Refund Tickets" << endl << "9 - Logout" << endl << "10 - Exit" << endl << endl << "Enter your choice: ";

            cin >> choice;

            switch (choice){
            case 1:
                system("cls");
                profileMenu();
                break;
            case 2:
                system("cls");
                purchaseTickets();
                break;
            case 3:
                system("cls");
                browseMovies();
                break;
            case 4:
                system("cls");
                searchMoviesMenu();
                break;
            case 5:
                system("cls");
                sortMoviesMenu();
                break;
            case 6:
                system("cls");
                cartMenu();
                break;
            case 7:
                system("cls");
                viewTickets();
                break;
            case 8:
                system("cls");
                refundTickets();
                break;
            case 9:
                system("cls");
                logout();
                continueMenu = false;
                break;
            case 10:
                system("cls");
                cout << endl << "Exiting the CinemaSystem." << endl;
                exit(0);
                break;
            default:
                cout << "Invalid choice. Please try again." << endl;
            }
        }
    }

    void searchMoviesMenu(){
        string keyword;
        char choice;

        do {
            cout << endl << "Enter search keyword (movie title or genre): ";
            cin.ignore();
            getline(cin, keyword);
            keyword = capitalizeFirstLetters(keyword);

            searchMovies(movies, keyword);

            cout << "Do you want to search again? (Y/N): ";
            cin >> choice;

        } while (choice == 'Y' || choice == 'y');
    }

    void sortMoviesMenu(){
        int sortChoice;
        char choice;

        do {
            cout << endl << "Sort Movies By:" << endl;
            cout << "1 - Title" << endl;
            cout << "2 - Genre" << endl;
            cout << "3 - Price" << endl;
            cout << "Enter your choice (1-3): ";
            cin >> sortChoice;

            switch (sortChoice){
            case 1:
                sortMovies(movies, movieCount, 0); // sort by Title (column 0)
                break;
            case 2:
                sortMovies(movies, movieCount, 1); // sort by Genre (column 1)
                break;
            case 3:
                sortMovies(movies, movieCount, 3); // sort by Price (column 3)
                break;
            default:
                cout << "Invalid choice. Returning to main menu." << endl;
                return;
            }

            // display sorted movies
            cout << endl << "Movies sorted successfully:" << endl;
            for (int i = 0; i < movieCount; i++){
                cout << "Title: " << movies[i][0]
                     << ", Genre: " << movies[i][1]
                     << ", Price: P" << movies[i][3] << endl;
            }

            cout << endl << "Do you want to sort again? (Y/N): ";
            cin >> choice;

        } while (choice == 'Y' || choice == 'y');
    }

    void profileMenu(){
        int profileChoice;
        bool continueProfileMenu = true;

        while (continueProfileMenu){
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

            switch (profileChoice){
            case 1:
            system("cls");
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

void updateProfile(){
        int updateChoice;
        bool continueUpdate = true;

        while (continueUpdate){
            cout << endl << "Update Profile Menu" << endl << endl;
            cout << "1 - Edit Username" << endl;
            cout << "2 - Edit Password" << endl;
            cout << "3 - Edit Name" << endl;
            cout << "4 - Edit Phone Number" << endl;
            cout << "5 - Edit Email" << endl;
            cout << "6 - Return to Profile Menu" << endl;
            cout << endl << "Enter your choice: ";
            cin >> updateChoice;

            string newValue;

            switch (updateChoice){
            case 1:
                cout << "Enter new username: ";
                cin >> newValue;
                username = newValue;
                cout << "Username updated successfully." << endl;
                break;
            case 2:
                newValue = inputValidPassword();
                password = newValue;
                cout << "Password updated successfully." << endl;
                break;
            case 3:
                name = inputValidName();
                //name = newValue;
                cout << "Name updated successfully." << endl;
                break;
            case 4:
                newValue = inputValidContactNum();
                contactNumber = newValue;
                cout << "Phone number updated successfully." << endl;
                break;
            case 5:
                newValue = inputValidEmail();
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

    void browseMovies(){
        cout << endl
             << "Available Movies:" << endl;
        for (int movie = 0; movie < movieCount; movie++){
            int availableSeats = 0;

            // array to store available locations
            string availableLocations[3];
            int availableLocationsCount = 0;

        // check available seats for each location
        for (int locationIndex = 0; locationIndex < 3; locationIndex++){
            int locationSeats = 0;
            for (int dateIndex = 0; dateIndex < 5; dateIndex++){
                for (int timeIndex = 0; timeIndex < 4; timeIndex++){
                    int locationTimeSeats = 0;
                    for (int row = 0; row < 10; row++){
                        for (int col = 0; col < 10; col++){
                            if (movieSeats[movie][locationIndex][dateIndex][timeIndex][row][col] == 'O'){
                                locationTimeSeats++;
                            }
                        }
                    }
                   // cout << locationTimeSeats << " ";
                    locationSeats += locationTimeSeats;
                }
            }

            // if location has available seats, add it to the array
            if (locationSeats > 0){
                string locationName = (locationIndex == 0 ? "SM Lipa" : locationIndex == 1 ? "Robinsons Lipa" : "Evia");
                availableLocations[availableLocationsCount++] = locationName;
            }
            availableSeats += locationSeats;
        }

            // display movie details
            cout << endl << (movie + 1) << ". Movie Name: " << movies[movie][0] << endl;
            cout << "Genre: " << movies[movie][1] << endl;
            cout << "Runtime: " << movies[movie][2] << endl;
            cout << "Price: P" << movies[movie][3] << endl;
            cout << "Available Seats: " << availableSeats << endl;

            // display available locations
            cout << "Available Locations: ";
            for (int i = 0; i < availableLocationsCount; i++){
                cout << availableLocations[i];
                if (i < availableLocationsCount - 1){
                    cout << ", ";
                }
            }
            cout << endl;
        }
    }

    bool searchKeyword(const string &text, const string &keyword){
        int textLength = text.length();
        int keywordLength = keyword.length();

        for (int i = 0; i <= textLength - keywordLength; i++){
            bool match = true;
            for (int j = 0; j < keywordLength; j++){
                if (text[i + j] != keyword[j]){
                    match = false;
                    break;
                }
            }
            if (match){
                return true;
            }
        }
        return false;
    }

    void searchMovies(string movies[][5], const string &keyword){
        cout << "Search results for \"" << keyword << "\":" << endl;
        bool found = false;

        for (int i = 0; i < movieCount; i++){
            if (searchKeyword(movies[i][0], keyword) || searchKeyword(movies[i][1], keyword)){
                cout << "Title: " << movies[i][0] << ", Genre: " << movies[i][1]
                     << ", Duration: " << movies[i][2] << ", Price: " << movies[i][3]
                     << ", Location: " << movies[i][4] << endl;
                found = true;
            }
        }

        if (!found){
            cout << "No movies found matching the keyword." << endl;
        }
    }

    // function to sort movies using bubble sort
    void sortMovies(string movies[][5], int size, int column){
        for (int i = 0; i < size - 1; i++){
            for (int j = 0; j < size - i - 1; j++){
                if (movies[j][column] > movies[j + 1][column]){
                    // swap entire rows
                    for (int k = 0; k < 5; k++){
                        string temp = movies[j][k];
                        movies[j][k] = movies[j + 1][k];
                        movies[j + 1][k] = temp;
                    }
                }
            }
        }
    }

    int countAvailableSeats(int movieIndex, int locationIndex, int dateIndex, int timeIndex){
        int availableSeats = 0;
        for (int row = 0; row < 10; row++){
            for (int col = 0; col < 10; col++){
                if (movieSeats[movieIndex][locationIndex][dateIndex][timeIndex][row][col] == 'O'){
                    availableSeats++;
                }
            }
        }
        return availableSeats;
    }

    void cartMenu(){
        int choice;
        bool continueCartMenu = true;

        while (continueCartMenu){
            cout << endl << "Cart Menu:" << endl;
            cout << "1 - View Cart" << endl;
            cout << "2 - Remove Tickets from Cart" << endl;
            cout << "3 - Clear Cart" << endl;
            cout << "4 - Checkout Tickets" << endl;
            cout << "5 - Return to Main Menu" << endl;
            cout << "Enter your choice: ";
            cin >> choice;

            switch (choice){
            case 1:
                cart.viewCart();
                break;
            case 2: {
                if (cart.getItemCount() == 0){
                    cout << endl << "Cart is empty." << endl;
                    cout << endl << "Returning to Main Menu.." << endl;
                    system("pause");
                    system("cls");
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

    void checkoutCart(){
    if (cart.getItemCount() == 0){
        cout << "Cart is empty. Nothing to checkout." << endl;
        cout << endl << "Returning to Main Menu.." << endl;
        system("pause");
        system("cls");
        return;
    }

    // if more than one item in cart, let user choose which to checkout
    int selectedItemIndex = 0;
    if (cart.getItemCount() > 1){
        cart.viewCart(); // display cart contents

        bool validSelection = false;
        do {
            cout << endl << "Enter the item number you want to checkout: ";

            if (!(cin >> selectedItemIndex)){
                cin.clear();
                cin.ignore(numeric_limits<streamsize>::max(), '\n');
                cout << "Please enter a valid number." << endl;
                continue;
            }

            // adjust for zero-based indexing
            selectedItemIndex--;

            if (selectedItemIndex >= 0 && selectedItemIndex < cart.getItemCount()){
                validSelection = true;
            }
            else {
                cout << "Invalid item number. Please try again." << endl;
            }
        } while (!validSelection);
    }

    // get ticket details from the cart item
    Ticket *selectedTicket = &cart.getCartItems()[selectedItemIndex];

    // Find the movie index for the selected movie
    int movieIndex = -1;
    for (int i = 0; i < movieCount; i++){
        if (selectedTicket->getMovieName() == movies[i][0]){
            movieIndex = i;
            break;
        }
    }

    if (movieIndex == -1){
        cout << "Error: Movie not found." << endl;
        return;
    }

    // Find the location index
    int locationIndex = -1;
    if (selectedTicket->getLocation() == "SM Lipa")
        locationIndex = 0;
    else if (selectedTicket->getLocation() == "Robinsons Lipa")
        locationIndex = 1;
    else if (selectedTicket->getLocation() == "Evia")
        locationIndex = 2;

    // Find the date index
    int dateIndex = -1;
    for (int i = 0; i < 5; i++){
        if (selectedTicket->getSelectedDate() == availableDates[i]){
            dateIndex = i;
            break;
        }
    }

    // Find the time index
    int timeIndex = -1;
    for (int i = 0; i < 4; i++){
        if (selectedTicket->getShowTime() == times[i]){
            timeIndex = i;
            break;
        }
    }

    // display seats using the found indices
    displaySeats(movieIndex, locationIndex, dateIndex, timeIndex);

        cout << endl;

        // book the selected seats
        int seatCount = selectedTicket->getQuantity();
        string *selectedSeats = bookSeats(movieIndex, locationIndex, dateIndex, timeIndex, seatCount);

        // assign the selected seats to the ticket
        selectedTicket->assignSeats(selectedSeats, seatCount);

        int paymentMethod = inputValidPaymentMethod();

        switch (paymentMethod) {
            case 1: { // card Payment
                string cardNumber = inputValidCardNumber();
                string expDate = inputValidExpirationDate();
                string cvc = inputValidCVC();
                cout << endl << "Processing card payment..." << endl;
                break;
            }
            case 2: { // bank Payment
                string accountNumber = inputValidBankAccountNumber();
                string accountName = inputValidBankAccountName();
                cout << endl << "Processing bank transfer..." << endl;
                break;
            }
            case 3: { // e-Wallet Payment
                int ewalletChoice = inputValidEWalletChoice();
                string ewalletName = (ewalletChoice == 1) ? "GCash" : "Maya";
                string phoneNumber = inputValidEWalletPhoneNumber();
                cout << endl << "Processing " << ewalletName << " payment..." << endl;
                break;
            }
        }
        cout << "Payment successful!" << endl;

        // process the selected ticket or all tickets
        Ticket *cartItems = cart.getCartItems();
        int cartItemCount = cart.getItemCount();

        // add selected ticket to ticket history
        if (cartItemCount > 1){
            if (ticketCount < 50){
                ticketHistory[ticketCount++] = cartItems[selectedItemIndex];
                // remove the selected item from cart
                cart.removeFromCart(selectedItemIndex);
            }
            else {
                cout << "Ticket history is full. Cannot add more tickets." << endl;
            }
        }
        else {
            // if only one item, process the entire cart
            for (int i = 0; i < cartItemCount; i++){
                if (ticketCount < 50){
                    ticketHistory[ticketCount++] = cartItems[i];
                }
                else {
                    cout << "Ticket history is full. Cannot add more tickets." << endl;
                    break;
                }
            }
            cart.clearCart();
        }

        cout << "Checkout completed successfully!" << endl;
    }

    void purchaseTickets(){
        char purchaseMore;
        char addToCartChoice;

        do {
            // show available movies
            browseMovies();

            // select movie with validation
            int movieChoice;
            bool validMovie = false;
            do {
                cout << endl << "Enter the number of the movie you want to watch (1-" << movieCount << "): ";
                if (!(cin >> movieChoice)){
                    cin.clear();
                    cin.ignore(numeric_limits<streamsize>::max(), '\n');
                    cout << "Please enter a valid number." << endl;
                    continue;
                }
                if (movieChoice >= 1 && movieChoice <= movieCount){
                    validMovie = true;
                }
                else {
                    cout << "Invalid movie selection. Please try again." << endl;
                }
            } while (!validMovie);

            movieChoice--; // array indexing adjustment (kasi 0 ang start ng index tapos sa choices, 1 ang start so babawasan ng isa para mag match)
            movieSelected[counter] = movieChoice;

            // show and select location with validation
            cout << endl << "Available Locations for " << movies[movieChoice][0] << ":" << endl;
            cout << movies[movieChoice][4] << endl;

            int locationChoice;
            bool validLocation = false;

            do {
                cout << "Enter location number (1-3): ";
                if (!(cin >> locationChoice)){
                    cin.clear();
                    cin.ignore(numeric_limits<streamsize>::max(), '\n');
                    cout << "Please enter a valid number." << endl;
                    continue;
                }
                if (locationChoice >= 1 && locationChoice <= 3){
                    validLocation = true;
                }
                else {
                    cout << "Invalid location selection. Please try again." << endl;
                }
            } while (!validLocation);

            string selectedLocation;

            switch (locationChoice){
            case 1:
                selectedLocation = "SM Lipa";
                break;
            case 2:
                selectedLocation = "Robinsons Lipa";
                break;
            case 3:
                selectedLocation = "Evia";
                break;
            }

            locationSelected[counter] = locationChoice - 1;

            int dateChoice;
            bool validDate = false;
            do {
                cout << endl << "Choose Date:" << endl;
                for (int i = 0; i < 5; i++){
                    cout << (i + 1) << " - " << availableDates[i] << endl;
                }
                cout << "Enter the number corresponding to your choice (1-5): ";
                if (!(cin >> dateChoice)){
                    cin.clear();
                    cin.ignore(numeric_limits<streamsize>::max(), '\n');
                    cout << "Please enter a valid number." << endl;
                    continue;
                }
                if (dateChoice >= 1 && dateChoice <= 5){
                    validDate = true;
                }
                else {
                    cout << "Invalid date selection. Please try again." << endl;
                }
            } while (!validDate);

            dateSelected[counter] = dateChoice - 1;

            // choose a time
            int timeChoice;
            bool validTime = false;
            do {
                cout << endl << "Choose Time:" << endl;
                for (int i = 0; i < 4; i++){
                    cout << (i + 1) << " - " << times[i] << endl;
                }
                cout << "Enter the number corresponding to your choice (1-4): ";
                if (!(cin >> timeChoice)){
                    cin.clear();
                    cin.ignore(numeric_limits<streamsize>::max(), '\n');
                    cout << "Please enter a valid number." << endl;
                    continue;
                }
                if (timeChoice >= 1 && timeChoice <= 4){
                    validTime = true;
                }
                else {
                    cout << "Invalid time selection. Please try again." << endl;
                }
            } while (!validTime);

            timeSelected[counter] = timeChoice - 1;

            // store the selected date and time
            string selectedDate = availableDates[dateChoice - 1];
            string selectedTime = times[timeChoice - 1];

            int dateIndex = dateChoice - 1;
            int timeIndex = timeChoice - 1;

            int availableSeats = countAvailableSeats(movieChoice, locationChoice - 1, dateIndex, timeIndex);

            cout << endl << "Booking Details" << endl << endl;
            cout << "Movie: " << movies[movieChoice][0] << endl;
            cout << "Genre: " << movies[movieChoice][1] << endl;
            cout << "Runtime: " << movies[movieChoice][2] << endl;
            cout << "Location: " << selectedLocation << endl;
            cout << "Date: " << selectedDate << endl; // display selected date
            cout << "Time: " << selectedTime << endl; // display selected time
            cout << "Price per ticket: P" << movies[movieChoice][3] << endl;
            cout << "Available Seats: " << availableSeats << endl;

            char proceed;
            do {
                cout << endl << "Do you want to proceed with the booking? (Y/N): ";
                cin >> proceed;
                proceed = toupper(proceed);
                if (proceed != 'Y' && proceed != 'N'){
                    cout << "Please enter Y or N." << endl;
                }
            } while (proceed != 'Y' && proceed != 'N');

            if (proceed != 'Y'){
                cout << "Booking cancelled." << endl;
                break;
            }

            // get number of tickets with validation
            int numTickets;
            bool validTickets = false;
            do {
                cout << "Enter number of tickets (max 15): ";
                if (!(cin >> numTickets)){
                    cin.clear();
                    cin.ignore(numeric_limits<streamsize>::max(), '\n');
                    cout << "Please enter a valid number." << endl;
                    continue;
                }
                if (numTickets >= 1 && numTickets <= 15){
                    validTickets = true;
                }
                else {
                    cout << "Invalid number of tickets. Please enter a number between 1 and 15." << endl;
                }
            } while (!validTickets);

            // display and book seats
            // displaySeats(movieChoice, locationChoice - 1, dateIndex, timeIndex);
            // string* bookedSeats = nullptr;

            // add to cart or checkout prompt
            do {
                cout << endl << "What do you want to do?" << endl;
                cout << "1 - Add to Cart" << endl;
                cout << "2 - Checkout" << endl;
                cout << "Enter your choice (1-2): ";
                cin >> addToCartChoice;

                if (addToCartChoice != '1' && addToCartChoice != '2'){
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
                "");

            if (addToCartChoice == '1'){
                // add to cart
                cart.addToCart(currentTicket);

                // ask if user wants to purchase more tickets
                do {
                    cout << endl << "Would you like to purchase more tickets? (Y/N): ";
                    cin >> purchaseMore;
                    purchaseMore = toupper(purchaseMore);
                    if (purchaseMore != 'Y' && purchaseMore != 'N'){
                        cout << "Please enter Y or N." << endl;
                    }
                } while (purchaseMore != 'Y' && purchaseMore != 'N');
            }
            else { // checkout current ticket
                // checkout process for the current ticket only
                // now reserve the seats for checkout

                displaySeats(movieChoice, locationChoice - 1, dateIndex, timeIndex);
                string *bookedSeats = bookSeats(movieChoice, locationChoice - 1, dateIndex, timeIndex, numTickets);

                string selectedSeats;
                for (int i = 0; i < numTickets; i++){
                    if (i > 0)
                        selectedSeats += ", ";
                    selectedSeats += bookedSeats[i];
                }

                // update ticket with booked seats
                currentTicket.setSeats(selectedSeats);

                cout << endl << "Proceeding to checkout for the current booking:" << endl;
                currentTicket.displayTicketDetails();

                // payment method selection
                int paymentMethod = inputValidPaymentMethod();

                switch (paymentMethod) {
                    case 1: { // card Payment
                        string cardNumber = inputValidCardNumber();
                        string expDate = inputValidExpirationDate();
                        string cvc = inputValidCVC();
                        cout << endl << "Processing card payment..." << endl;
                        break;
                    }
                    case 2: { // bank Payment
                        string accountNumber = inputValidBankAccountNumber();
                        string accountName = inputValidBankAccountName();
                        cout << endl << "Processing bank transfer..." << endl;
                        break;
                    }
                    case 3: { // e-Wallet Payment
                        int ewalletChoice = inputValidEWalletChoice();
                        string ewalletName = (ewalletChoice == 1) ? "GCash" : "Maya";
                        string phoneNumber = inputValidEWalletPhoneNumber();
                        cout << endl << "Processing " << ewalletName << " payment..." << endl;
                        break;
                    }
                }

                cout << "Payment successful!" << endl;

                // add current ticket directly to ticket history
                if (ticketCount < 50){
                    ticketHistory[ticketCount++] = currentTicket;
                }
                else {
                    cout << "Ticket history is full. Cannot add more tickets." << endl;
                }

                cout << "Checkout for current booking completed successfully!" << endl;

                // ask if the user wants to purchase more tickets
                do {
                    cout << endl << "Would you like to purchase more tickets? (Y/N): ";
                    cin >> purchaseMore;
                    purchaseMore = toupper(purchaseMore);
                    if (purchaseMore != 'Y' && purchaseMore != 'N'){
                        cout << "Please enter Y or N." << endl;
                    }
                } while (purchaseMore != 'Y' && purchaseMore != 'N');
            }
        } while (purchaseMore == 'Y');
    counter++;
    }

bool findTicketIndices(const Ticket& ticket, int& movieIndex, int& locationIndex, int& dateIndex, int& timeIndex) {
    // Find movie index
    movieIndex = -1;
    for (int i = 0; i < 10; i++) {
        if (movies[i][0] == ticket.getMovieName()) {
            movieIndex = i;
            break;
        }
    }

    // Find location index
    locationIndex = -1;
    for (int i = 0; i < 5; i++) {
        if (movies[movieIndex][4].find(ticket.getLocation()) != string::npos) {
            locationIndex = i;
            break;
        }
    }

    // default other indices
    dateIndex = 0;
    timeIndex = 0;

    // basic validation
    if (movieIndex == -1 || locationIndex == -1) {
        cout << "Error: Unable to find correct indices for the ticket." << endl;
        return false;
    }

    return true;
}

void refundTickets() {
    if (ticketCount == 0){
        cout << "No tickets available for refund." << endl;
        cout << endl << "Returning to Main Menu.." << endl;
        system("pause");
        system("cls");
        return;
    }

    viewTickets();

    int refundChoice;
    cout << endl << "Enter the ticket number you want to refund (0 to cancel): ";
    cin >> refundChoice;

    if (refundChoice == 0)
        return;

    if (refundChoice < 1 || refundChoice > ticketCount){
        cout << "Invalid ticket number." << endl;
        return;
    }

    refundChoice--; // adjust for zero-based index

    if (!ticketHistory[refundChoice].getIsValid()){
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

    if (toupper(confirm) == 'Y'){
        string seats = ticketHistory[refundChoice].getSeats();
        int numSeats = ticketHistory[refundChoice].getQuantity();

        int movieIndex, locationIndex, dateIndex, timeIndex;
        if (findTicketIndices(ticketHistory[refundChoice], movieIndex, locationIndex, dateIndex, timeIndex)){
            setRefundDetails(movieIndex, locationIndex, dateIndex, timeIndex);

            // Free the seats
            for (int i = 0; i < numSeats; i++){
                // extract the seat label, allow for both 1-digit and 2-digit columns (e.g., "A1" or "B10")
                string seatChoice = seats.substr(i * 3, 3); // maximum of 3 characters

                // extract row and column
                char rowChar = toupper(seatChoice[0]);  // First character is the row (e.g., 'A', 'B', ...)
                int row = rowChar - 'A';  // convert 'A' -> 0, 'B' -> 1, etc.

                // extract column (from the remaining part)
                string colStr = seatChoice.substr(1); // column part (e.g., "1" or "10")
                int col = stoi(colStr);  // convert string to integer for column

                // adjust column for 0-based index
                col--;

                // free the seat (update to 'O')
                movieSeats[movieIndex][locationIndex][dateIndex][timeIndex][row][col] = 'O';
            }

            ticketHistory[refundChoice].setIsValid(false);
            cout << "Ticket refunded successfully. Refund amount: P" << refundAmount << endl;
        }
    }
    else {
        cout << "Refund cancelled." << endl;
    }
}

    void viewTickets(){
        cout << endl
             << "My Tickets:" << endl;
        bool hasTickets = false;

        for (int i = 0; i < ticketCount; i++){
            if (ticketHistory[i].getIsValid()){
                hasTickets = true;
                cout << endl << "Order #" << (i + 1) << ":" << endl;
                ticketHistory[i].displayTicketDetails();
            }
        }

        if (!hasTickets){
            cout << "No tickets found in your history." << endl;
        }
    }
};

// admin Class - Derived from User (Polymorphism)
class Admin : public User {
public:
    Admin(string nm, string uname, string mail, string contact, string pass)
        : User(nm, uname, mail, contact, pass, true) {}

    void viewAllUsers(CinemaSystem &CinemaSystem){
        if (CinemaSystem.getUserCount() == 0){
            cout << "No users found." << endl;
            cout << endl << "Returning to Main Menu.." << endl;
            system("pause");
            system("cls");
            return;
        }

        CinemaSystem.displayUsers();
    }

    void addMovie(string movies[10][5], int &movieCount, char movieSeats[10][3][5][4][10][10]){
        char choice;
        do {
            if (movieCount >= 10){
                cout << "The movie list is full. Cannot add more movies." << endl;
                break;
            }

            cout << "Adding a new movie" << endl << endl;

            string movieTitle;
            cout << "Enter Title: ";
            cin.ignore();

            getline(cin, movieTitle);
            movieTitle = capitalizeFirstLetters(movieTitle);
            movies[movieCount][0] = movieTitle;

            cout << "Enter Genre: ";
            getline(cin, movies[movieCount][1]);

            cout << "Enter Duration (in minutes, e.g., 120): ";
            string duration;
            getline(cin, duration);
            movies[movieCount][2] = duration + " min";

            cout << "Enter Price (P): ";
            string price;
            getline(cin, price);
            movies[movieCount][3] = price;

            movies[movieCount][4] = "SM Lipa, Robinsons Lipa, Evia";

            // initialize seats for this movie
            for (int loc = 0; loc < 3; loc++){ // locations
                for (int row = 0; row < 10; row++){ // rows
                    for (int col = 0; col < 10; col++){ // columns
                        for (int date = 0; date < 5; date++){ // dates
                            for (int time = 0; time < 4; time++){ // times
                                movieSeats[movieCount][loc][date][time][row][col] = 'O';
                            }
                        }
                    }
                }
            }

            movieCount++;

            cout << "Do you want to add another movie? (Y/N): ";
            cin >> choice;
            cin.ignore();
        } while (choice == 'Y' || choice == 'y');

        cout << endl << "Final Movie List:" << endl;

        for (int i = 0; i < movieCount; i++){
            cout << "Movie Name: " << movies[i][0] << endl;
            cout << "Genre: " << movies[i][1] << endl;
            cout << "Runtime: " << movies[i][2] << endl;
            cout << "Price: " << movies[i][3] << endl;
            cout << "Available Locations: " << movies[i][4] << endl;
            cout << endl;
        }
    }

    void updateMovieDetails(){
        int movieChoice, detailChoice;
        bool continueUpdate = true;

        while (continueUpdate){
            // display movie list
            for (int i = 0; i < movieCount; i++){
                cout << endl << "Movie " << (i + 1) << "." << endl;
                cout << endl << "Movie Name: " << movies[i][0] << endl;
                cout << "Genre: " << movies[i][1] << endl;
                cout << "Runtime: " << movies[i][2] << endl;
                cout << "Price: P" << movies[i][3] << endl;
            }

            cout << endl << "Select movie to update (0 to exit): ";
            cin >> movieChoice;

            if (movieChoice == 0) 
                break;
            movieChoice--;

            if (movieChoice < 0 || movieChoice >= movieCount){
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

            switch (detailChoice){
            case 1:
                cout << "Enter new movie name: ";
                getline(cin, newValue);
                movies[movieChoice][0] = newValue;
                break;
            case 2:
                cout << "Enter new genre: ";
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

    void deleteMovie(){
        bool hasMovies = false;

        for (int i = 0; i < movieCount; i++){
            if (!movies[i][0].empty()){
                hasMovies = true;
                break;
            }
        }

        if (!hasMovies){
            cout << "No movies available to delete." << endl;
            return;
        }

        // display available movies
        cout << endl << "Available Movies:" << endl;
        for (int i = 0; i < movieCount; i++){
            if (!movies[i][0].empty()){
                cout << (i + 1) << ". " << movies[i][0] << endl;
            }
        }

        int movieChoice;
        bool validChoice = false;

        do {
            cout << endl << "Enter the number of the movie to delete (0 to cancel): ";
            if (!(cin >> movieChoice)){
                cin.clear();
                cin.ignore(numeric_limits<streamsize>::max(), '\n');
                cout << "Invalid input. Please try again." << endl;
                continue;
            }

            if (movieChoice == 0){
                cout << "Movie deletion cancelled." << endl;
                cout << endl << "Returning to Main Menu.." << endl;
                system("pause");
                system("cls");
                return;
            }

            if (movieChoice >= 1 && movieChoice <= movieChoice && !movies[movieChoice - 1][0].empty()){
                validChoice = true;
            }
            else {
                cout << "Invalid movie selection. Please try again." << endl;
            }
        } while (!validChoice);

        char confirm;
        cout << "Are you sure you want to delete " << movies[movieChoice - 1][0] << "? (Y/N): ";
        cin >> confirm;

        if (toupper(confirm) == 'Y'){

            // clear movie details
            for (int j = 0; j < movieCount; j++){
                movies[movieChoice - 1][j] = "";
            }

            // clear corresponding seats for the deleted movie
            for (int location = 0; location < 3; location++){
                for (int date = 0; date < 5; date++){
                    for (int time = 0; time < 4; time++){
                        for (int row = 0; row < 10; row++){
                            for (int col = 0; col < 10; col++){
                                movieSeats[movieChoice - 1][location][date][time][row][col] = 'O'; // reset to 'O'
                            }
                        }
                    }
                }
            }

            cout << "Movie deleted successfully." << endl;
        }
        else {
            cout << "Movie deletion cancelled." << endl;
        }
    }

    void refundManagement(CinemaSystem &CinemaSystem){
        User **users = CinemaSystem.getUsers();
        int userCount = CinemaSystem.getUserCount();

        Ticket *refundableTickets[5000]; // array to store refundable tickets
        int refundableCount = 0;

        // collect all refundable tickets from all customers
        for (int i = 0; i < userCount; i++){
            Customer *customer = dynamic_cast<Customer *>(users[i]);
            if (customer){
                Ticket *tickets = customer->getTickets();
                for (int j = 0; j < customer->getTicketCount(); j++){
                    // only add tickets that are valid and not yet refunded
                    if (tickets[j].getIsValid()){
                        refundableTickets[refundableCount++] = &tickets[j];
                    }
                }
            }
        }

        // check if there are any refundable tickets
        if (refundableCount == 0){
            cout << "No tickets available for refund." << endl;
            cout << endl << "Returning to Main Menu.." << endl;
            system("pause");
            system("cls");
            return;
        }

        // display refundable tickets
        cout << endl
             << "Refundable Tickets:" << endl;
        for (int i = 0; i < refundableCount; i++){
            cout << endl << "Ticket #" << (i + 1) << endl;
            refundableTickets[i]->displayTicketDetails();
        }

        int choice;
        cout << "Enter the number of the ticket to refund (0 to cancel): ";
        cin >> choice;

        // validate choice
        if (choice <= 0 || choice > refundableCount){
            cout << "Refund cancelled." << endl;
            return;
        }

        // select the ticket for refund
        Ticket *ticket = refundableTickets[choice - 1];

        // calculate refund amount (70% of total price)
        double refundAmount = ticket->getTotalPrice() * 0.7;

        // get ticket details
        string movieName = ticket->getMovieName();
        string location = ticket->getLocation();
        string showtime = ticket->getShowTime();
        string selectedDate = ticket->getSelectedDate();

        // find movie index
        int movieIndex = -1;
        for (int i = 0; i < movieCount; i++){
            if (movies[i][0] == movieName){
                movieIndex = i;
                break;
            }
        }

        // find location index
        int locationIndex = -1;
        string locations[] = {"SM Lipa", "Robinsons Lipa", "Evia"};
        for (int i = 0; i < 3; i++){
            if (locations[i] == location){
                locationIndex = i;
                break;
            }
        }

        // find date index
        int dateIndex = -1;
        for (int i = 0; i < 5; i++){
            if (availableDates[i] == selectedDate){
                dateIndex = i;
                break;
            }
        }

        // find time index
        int timeIndex = -1;
        for (int i = 0; i < 4; i++){
            if (times[i] == showtime){
                timeIndex = i;
                break;
            }
        }

        // validate all indices
        if (movieIndex == -1 || locationIndex == -1 || dateIndex == -1 || timeIndex == -1){
            cout << "Error: Unable to process refund. Invalid ticket details." << endl;
            return;
        }

        // get seats and free them
        string seats = ticket->getSeats();
        int numSeats = ticket->getQuantity();

        // parse and free seats
        stringstream ss(seats);
        string seat;
        while (getline(ss, seat, ',')){
            // trim whitespace
            seat.erase(0, seat.find_first_not_of(" "));
            seat.erase(seat.find_last_not_of(" ") + 1);

            // convert seat to row and column
            char rowChar = seat[0];
            int row = toupper(rowChar) - 'A';
            int col = stoi(seat.substr(1)) - 1;

            // ensure valid indices
            if (row >= 0 && row < 10 && col >= 0 && col < 10){
                movieSeats[movieIndex][locationIndex][dateIndex][timeIndex][row][col] = 'O';
            }
            else {
                // cout << "Invalid seat: " << seat << endl;
            }
        }

        // mark ticket as invalid
        ticket->setIsValid(false);

        // process refund
        cout << "Refund processed: P" << fixed << setprecision(2) << refundAmount << endl;
        cout << "Ticket for " << movieName << " at " << location << " has been refunded." << endl;
    }

    void deleteUser(CinemaSystem &CinemaSystem){
        CinemaSystem.displayUsers();
        int choice;

        cout << "Enter the user number to delete (0 to cancel): ";
        cin >> choice;

        if (choice <= 0 || choice > CinemaSystem.getUserCount()){
            cout << "Deletion cancelled." << endl;
            return;
        }

        CinemaSystem.deleteUser(choice - 1);
        cout << "User deleted successfully." << endl;
    }

    void menu() override {
        int choice;
        CinemaSystem &CinemaSystem = CinemaSystem::getInstance();
        bool continueMenu = true;

        while (continueMenu){
            cout << endl 
            << "Admin Menu:" << endl << "1 - View All Users" << endl << "2 - Add a Movie" << endl << "3 - View and Update Movie Details" << endl << "4 - Delete Movies" << endl << "5 - Accommodate Refunds and Cancellations" << endl << "6 - Delete Users" << endl << "7 - Logout" << endl << "8 - Exit" << endl << endl << "Enter your choice: ";
            cin >> choice;

            switch (choice){
            case 1:
                system("cls");
                viewAllUsers(CinemaSystem);
                break;
            case 2:
                system("cls");
                addMovie(movies, movieCount, movieSeats);
                break;
            case 3:
                system("cls");
                updateMovieDetails();
                break;
            case 4:
                system("cls");
                deleteMovie();
                break;
            case 5:
                refundManagement(CinemaSystem);
                break;
            case 6:
                system("cls");
                deleteUser(CinemaSystem);
                break;
            case 7:
                system("cls");
                logout();
                continueMenu = false;
                break;
            case 8:
                cout << endl << "Exiting TouchMetro." << endl;
                exit(0);
                break;
            default:
                cout << "Invalid choice. Please try again." << endl;
            }
        }
    }
};

// main Menu Function
void mainMenu(CinemaSystem &CinemaSystem) {
    string adminUsernames[5] = {"admin1", "admin2", "admin3", "admin4", "admin5"};
    int choice;
    bool continueMenu = true;

    while (continueMenu){
        try 
        {
            cout << endl
                << "Main Menu" << endl
                << endl;
            cout << "1 - Login" << endl
                << "2 - Sign Up" << endl
                << "3 - Exit" << endl;
            cout << endl
                << "Enter your choice: ";
            cin >> choice;

            if (cin.fail()) {
                cin.clear();
                cin.ignore(numeric_limits<streamsize>::max(), '\n');
                throw runtime_error("Invalid input. Please re-enter your choice.\n");
            }

            if (choice < 1 || choice > 3) {
                throw out_of_range("Choice out of range. Please re-enter your choice.\n");
            }

        } catch (const runtime_error &e) {
            cout << "Error: " << e.what() << endl;
        } catch (const out_of_range &e) {
            cout << "Error: " << e.what() << endl;
        }

            switch (choice){
            case 1: {
                bool loginFinished = false;
                bool returnToMainMenu = false;

                while (!loginFinished && !returnToMainMenu){

                    string username, password;
                    User *user = nullptr;

                    username = inputValidUsername();

                    if (username == "0"){
                        returnToMainMenu = true;
                        break;
                    }
                    
                    password = inputValidPassword();

                    // check if it's an admin login
                    bool isAdmin = CinemaSystem.authenticateAdmin(username, password);
                    if (isAdmin){

                        // find the admin user
                        for (int i = 0; i < CinemaSystem.getUserCount(); i++){

                            user = CinemaSystem.authenticateUser(username, password);

                            if (user && user->getIsAdmin()){

                                Admin *admin = dynamic_cast<Admin *>(user);

                                if (admin){
                                    cout << "Admin login successful." << endl;
                                    admin->menu();
                                    loginFinished = true;
                                    break;
                                }
                            }
                        }
                        if (loginFinished){
                            break;
                        }
                    }
                    // if hindi admin, check for customer login
                    user = CinemaSystem.authenticateUser(username, password);

                    if (user != nullptr){
                        cout << "Login successful." << endl;

                        Customer *customer = dynamic_cast<Customer *>(user);

                    cout << endl << "Heading to Main Menu.." << endl;
                    system("pause");
                    system("cls");

                        if (customer){
                            customer->menu();
                        }
                        loginFinished = true;
                    }
                    else {
                        cout << "Invalid credentials. Please try again." << endl;
                    }
                }
                break;
            }
            case 2: {
                string name, username, email, contact, password;
                bool isAdmin = false;

                system("cls");
                cout << endl << "Sign Up" << endl << endl;

                username = inputValidUsername();
                password = inputValidPassword();
                name = inputValidName();
                email = inputValidEmail();
                contact = inputValidContactNum();

                // check if admin based on predefined usernames
                for (const string &adminUsername : adminUsernames){
                    if (username == adminUsername){
                        isAdmin = true;
                        break;
                    }
                }
                try {
                    // creation of new admin/user acc
                    User *newUser;
                    if (isAdmin){
                        newUser = new Admin(name, username, email, contact, password);
                    }
                    else {
                        newUser = new Customer(name, username, email, contact, password);
                    }
                    CinemaSystem.addUser(newUser);
                    cout << endl << "Sign-up successful. You can now log in." << endl;
                    
                    cout << endl << "Returning to Main Menu.." << endl;
                    system("pause");
                    system("cls");
                }
                catch (ValidationException &ex){
                    cout << "Error: " << ex.what() << endl;
                }
                break;
            }
            case 3:
                cout << "Exiting TouchMetro. Goodbye!" << endl;
                continueMenu = false;
                break;
            default:
                cout << "Invalid choice. Please try again." << endl;
            }
    }
}

int main() {
    
    CinemaSystem &cinemaSystem = CinemaSystem::getInstance();
    initializeAllSeats();
    system("cls");
    mainMenu(cinemaSystem);

    return 0;
}