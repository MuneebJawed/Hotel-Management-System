#include <iostream>
#include <fstream>
#include <string>

using namespace std;

// Room class to represent each room in the hotel
class Room {
public:
    int roomNumber;
    bool isOccupied;
    string guestName;
    bool needsRoomService;
    Room() {}
    Room(int number) : roomNumber(number), isOccupied(false), needsRoomService(false) {}
};

// User class to represent users (Manager and Receptionist)
class User {
public:
    string username;
    string password;
    User() {}
    User(string username, string password) : username(username), password(password) {}
};

// Hotel class to manage rooms and reservations
class Hotel {
private:
    Room rooms[10];
    User users[2];

public:
    Hotel() {
        for (int i = 1; i <= 10; i++) {
            rooms[i - 1].roomNumber = i;
        }
        for (int i = 0; i < 2; i++) {
            users[i].username = "";
            users[i].password = "";
        }
    }

    void addUser(string username, string password, string role) {
        if (role == "Manager") {
            users[0].username = username;
            users[0].password = password;
        }
        else if (role == "Receptionist") {
            users[1].username = username;
            users[1].password = password;
        }
    }

    bool login(string username, string password) {
        bool success = false;
        if (username == users[0].username && password == users[0].password) {
            success = true;
        }
        else if (username == users[1].username && password == users[1].password) {
            success = true;
        }

        // Log the login attempt
        ofstream logFile("login_attempts.txt", ios::app);
        if (logFile.is_open()) {
            logFile << "Username: " << username << ", Password: " << password << ", Success: " << (success ? "true" : "false") << endl;
            logFile.close();
        }
        else {
            cout << "Unable to open log file." << endl;
        }

        return success;
    }

    void updateInventory(int amount) {
        ofstream inventoryFile("inventory_log.txt", ios::app);
        if (inventoryFile.is_open()) {
            inventoryFile << "Inventory updated by: " << amount << endl;
            inventoryFile.close();
        }
        else {
            cout << "Unable to open inventory log file." << endl;
        }
    }

    void logRoomOccupancy() {
        int vacantRooms = 0;
        int occupiedRooms = 0;
        for (int i = 0; i < 10; i++) {
            if (rooms[i].isOccupied) {
                occupiedRooms++;
            }
            else {
                vacantRooms++;
            }
        }

        ofstream roomOccupancyFile("room_occupancy_log.txt", ios::app);
        if (roomOccupancyFile.is_open()) {
            roomOccupancyFile << "Vacant Rooms: " << vacantRooms << ", Occupied Rooms: " << occupiedRooms << endl;
            roomOccupancyFile.close();
        }
        else {
            cout << "Unable to open room occupancy log file." << endl;
        }
    }

    void checkInGuest(int roomNumber, string guestName) {
        if (roomNumber >= 1 && roomNumber <= 10 && !rooms[roomNumber - 1].isOccupied) {
            rooms[roomNumber - 1].isOccupied = true;
            rooms[roomNumber - 1].guestName = guestName;
            cout << "Guest " << guestName << " checked into Room " << roomNumber << "." << endl;
        }
        else {
            cout << "Invalid room number or room already occupied." << endl;
        }
    }

    void checkOutGuest(int roomNumber) {
        if (roomNumber >= 1 && roomNumber <= 10 && rooms[roomNumber - 1].isOccupied) {
            rooms[roomNumber - 1].isOccupied = false;
            rooms[roomNumber - 1].guestName = "";
            rooms[roomNumber - 1].needsRoomService = false;
            cout << "Room " << roomNumber << " has been checked out." << endl;
        }
        else {
            cout << "Invalid room number or room is not occupied." << endl;
        }
    }

    void requestRoomService(int roomNumber) {
        if (roomNumber >= 1 && roomNumber <= 10 && rooms[roomNumber - 1].isOccupied) {
            rooms[roomNumber - 1].needsRoomService = true;
            cout << "Room service requested for Room " << roomNumber << "." << endl;
        }
        else {
            cout << "Invalid room number or room is not occupied." << endl;
        }
    }

};

// Manager class inherits from Hotel and adds inventory management
class Manager : public Hotel {
private:
    int inventory;

public:
    Manager(int initialInventory) : Hotel(), inventory(initialInventory) {}

    void updateInventory(int amount) {
        inventory += amount;
        cout << "Inventory updated by: " << amount << ". New inventory: " << inventory << endl;

        // Log the inventory update
        Hotel::updateInventory(amount);
    }
};

int main() {
    Hotel hotel;

    hotel.addUser("manager", "pass1", "Manager");
    hotel.addUser("reception", "pass2", "Receptionist");

    string username, password;
    cout << "Enter your username: ";
    cin >> username;
    cout << "Enter your password: ";
    cin >> password;

    if (hotel.login(username, password)) {
        if (username == "Manager") {
            Manager manager(50);
            manager.updateInventory(20);
        }
        else{
            int choice;
            cout << "Welcome, Receptionist!" << endl;
            do {
                cout << "\nReceptionist Tasks:\n";
                cout << "1. Check-in guest\n";
                cout << "2. Check-out guest\n";
                cout << "3. Request room service\n";
                cout << "4. Log room occupancy\n";
                cout << "5. Exit\n";
                cout << "Enter your choice: ";
                cin >> choice;

                switch (choice) {
                case 1: {
                    int roomNumber;
                    string guestName;
                    cout << "Enter room number: ";
                    cin >> roomNumber;
                    cout << "Enter guest name: ";
                    cin >> guestName;
                    hotel.checkInGuest(roomNumber, guestName);
                    break;
                }
                case 2: {
                    int roomNumber;
                    cout << "Enter room number: ";
                    cin >> roomNumber;
                    hotel.checkOutGuest(roomNumber);
                    break;
                }
                case 3: {
                    int roomNumber;
                    cout << "Enter room number: ";
                    cin >> roomNumber;
                    hotel.requestRoomService(roomNumber);
                    break;
                }
                case 4: {
                    hotel.logRoomOccupancy();
                    break;
                }
                case 5: {
                    cout << "Exiting..." << endl;
                    break;
                }
                default:
                    cout << "Invalid choice. Please try again." << endl;
                }
            } while (choice != 5);
        }
    }
    else {
        cout << "Invalid username or password." << endl;
    }

    return 0;
}