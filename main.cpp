#include <iostream>
#include <fstream>
#include <map>
#include <string>
#include <thread>
#include <mutex>
#include <chrono>
#include <iomanip>
#include <ctime>

using namespace std;

class Discount
{
private:
    map<string, double> discounts;

public:
    Discount()
    {
        loadDiscounts();
    }

    void loadDiscounts()
    {
        ifstream file("discounts.txt");
        if (file.is_open())
        {
            string code;
            double percentage;
            while (file >> code >> percentage)
            {
                discounts[code] = percentage;
            }
            file.close();
        }
        else
        {
            // If file doesn't exist, create default discounts
            discounts["EMP001"] = 0.5; // 50% discount
            discounts["EMP002"] = 0.3; // 30% discount
            discounts["PROF01"] = 0.4; // 40% discount
            saveDiscounts();
        }
    }

    void saveDiscounts()
    {
        ofstream file("discounts.txt");
        for (const auto &pair : discounts)
        {
            file << pair.first << " " << pair.second << endl;
        }
        file.close();
    }

    double getDiscount(const string &code) const
    {
        auto it = discounts.find(code);
        return (it != discounts.end()) ? it->second : 0.0;
    }

    bool isEmployee(const string &code) const
    {
        return discounts.find(code) != discounts.end();
    }
};

class Vehicle
{
private:
    string code;
    chrono::system_clock::time_point entryTime;
    bool active;

public:
    Vehicle() : active(false) {}

    Vehicle(const string &cod) : code(cod), active(true)
    {
        entryTime = chrono::system_clock::now();
    }

    friend class ParkingLot;

    string &getCode() { return code; }
    const string &getCode() const { return code; }

    bool isActive() const { return active; }

    double calculateTime() const
    {
        if (!active)
            return 0.0;
        auto now = chrono::system_clock::now();
        auto duration = chrono::duration_cast<chrono::seconds>(now - entryTime);
        return duration.count() / 60.0; // 1 real minute = 1 parking hour (so seconds/60 = hours)
    }

    string getEntryTime() const
    {
        auto time_t = chrono::system_clock::to_time_t(entryTime);
        stringstream ss;
        ss << put_time(localtime(&time_t), "%H:%M:%S");
        return ss.str();
    }

    void exit() { active = false; }
};

class ParkingLot
{
private:
    map<string, Vehicle> vehicles;
    Discount discounts;
    static const double HOURLY_RATE;
    mutable mutex mtx;
    bool showingClock;
    thread clockThread;

public:
    ParkingLot() : showingClock(true)
    {
        clockThread = thread(&ParkingLot::displayClock, this);
    }

    ~ParkingLot()
    {
        showingClock = false;
        if (clockThread.joinable())
        {
            clockThread.join();
        }
    }

    void displayClock()
    {
        while (showingClock)
        {
            {
                lock_guard<mutex> lock(mtx);
                auto now = chrono::system_clock::now();
                auto time_t = chrono::system_clock::to_time_t(now);

                cout << "\r🕐 Current time: " << put_time(localtime(&time_t), "%H:%M:%S") << " | ";
                cout << "Vehicles in parking: " << countActiveVehicles() << "     " << flush;
            }
            this_thread::sleep_for(chrono::milliseconds(500)); // Updates every 0.5 seconds (faster)
        }
    }

    void pauseClock()
    {
        showingClock = false;
        if (clockThread.joinable())
        {
            clockThread.join();
        }
    }

    void resumeClock()
    {
        showingClock = true;
        clockThread = thread(&ParkingLot::displayClock, this);
    }

    int countActiveVehicles() const
    {
        int counter = 0;
        for (const auto &pair : vehicles)
        {
            if (pair.second.isActive())
            {
                counter++;
            }
        }
        return counter;
    }

    bool processCode(const string &code)
    {
        lock_guard<mutex> lock(mtx);

        cout << "\n\n=== PROCESSING CODE: " << code << " ===" << endl;

        auto it = vehicles.find(code);

        if (it == vehicles.end() || !it->second.isActive())
        {
            // Vehicle entering
            vehicles[code] = Vehicle(code);
            cout << "🚗 ENTRY registered" << endl;
            cout << "Entry time: " << vehicles[code].getEntryTime() << endl;

            if (discounts.isEmployee(code))
            {
                double disc = discounts.getDiscount(code);
                cout << "✅ Employee detected - Discount: " << (disc * 100) << "%" << endl;
            }
            else
            {
                cout << "👤 Regular customer - Normal rate" << endl;
            }
            return true;
        }
        else
        {
            // Vehicle exiting
            double hours = it->second.calculateTime(); // This already returns hours
            double baseCost = hours * HOURLY_RATE;
            double discount = discounts.getDiscount(code);
            double finalCost = baseCost * (1.0 - discount);

            cout << "🚗 EXIT registered" << endl;
            cout << "Time parked: " << fixed << setprecision(2) << hours << " hours" << endl;
            cout << "Base cost: $" << fixed << setprecision(2) << baseCost << endl;

            if (discount > 0)
            {
                cout << "Applied discount: " << (discount * 100) << "%" << endl;
                cout << "Savings: $" << fixed << setprecision(2) << (baseCost - finalCost) << endl;
            }

            cout << "💰 TOTAL TO PAY: $" << fixed << setprecision(2) << finalCost << endl;

            it->second.exit();
            return false;
        }
    }

    void showStatus() const
    {
        lock_guard<mutex> lock(mtx);
        cout << "\n\n=== PARKING LOT STATUS ===" << endl;
        cout << "Currently parked vehicles:" << endl;

        for (const auto &pair : vehicles)
        {
            if (pair.second.isActive())
            {
                cout << "🚗 Code: " << pair.first
                     << " | Entry: " << pair.second.getEntryTime()
                     << " | Time: " << fixed << setprecision(2)
                     << pair.second.calculateTime() << " hours" << endl;
            }
        }
    }

    friend void showMenu(ParkingLot &parkingLot);
};

const double ParkingLot::HOURLY_RATE = 25.0;

void showMenu(ParkingLot &parkingLot)
{
    cout << "\n\n=== PARKING LOT MANAGEMENT SYSTEM ===" << endl;
    cout << "1. Enter vehicle code" << endl;
    cout << "2. View parking lot status" << endl;
    cout << "3. Exit" << endl;
    cout << "Option: ";
}

int main()
{
    ParkingLot parkingLot;
    string code;
    int option;

    cout << "🅿️  PARKING LOT SYSTEM STARTED" << endl;
    cout << "Rate: $25.00 per hour" << endl;
    cout << "Press Enter to continue...";
    cin.get();

    while (true)
    {
        showMenu(parkingLot);
        cin >> option;

        switch (option)
        {
        case 1:
            parkingLot.pauseClock();
            cout << "\n\nEnter vehicle code: ";
            cin >> code;
            parkingLot.processCode(code);
            parkingLot.resumeClock();
            break;

        case 2:
            parkingLot.showStatus();
            break;

        case 3:
            cout << "\n🚗 Thank you for using the Parking Lot System!" << endl;
            return 0;

        default:
            cout << "\n❌ Invalid option" << endl;
        }

        cout << "\nPress Enter to continue...";
        cin.ignore();
        cin.get();
    }

    return 0;
}