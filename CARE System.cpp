#include <fstream>
#include <iostream>
#include <string>
#include <algorithm>
#include <vector>
#include <queue>
#include <stack>
#include <cctype>
#include <windows.h>

using namespace std;

// ---------------- Recursion helpers ----------------
HANDLE ConsoleColour = GetStdHandle(STD_OUTPUT_HANDLE);


void greetUser(int times) {
    if (times <= 0) return;
    SetConsoleTextAttribute(ConsoleColour, 11);
    cout << "==Welcome to the CRISES AID RESOURCE ENGINE System!==\n" << endl;;
    SetConsoleTextAttribute(ConsoleColour, 7);
    greetUser(times - 1);
}
void thankUser(int times) {
    if (times <= 0) return;
    SetConsoleTextAttribute(ConsoleColour, 11);
    cout << "==Thank you for using our system. Stay safe!==\n";
    SetConsoleTextAttribute(ConsoleColour, 7);
    thankUser(times - 1);
}

// ---------------- Login Manager ----------------
class LoginManager {
private:
    string username;
    string password;
    string role;

public:
    bool runLogin = false;

    string getRole() const { return role; }

    void login() {
        cout << "=== Login ===\nUsername: ";
        cin >> username;
        cout << "Password: ";
        cin >> password;

        if (username == "admin" && password == "admin123") {
            role = "admin";
            runLogin = true;
            cout << "Login successful. Welcome, Admin!" << endl;
            return;
        }

        if ((username == "m1@capetown" && password == "pass1") ||
            (username == "m2@bloemfontein" && password == "pass2") ||
            (username == "m3@johannesburg" && password == "pass3") ||
            (username == "m4@kimberley" && password == "pass4") ||
            (username == "m5@mahikeng" && password == "pass5") ||
            (username == "m6@bisho" && password == "pass6") ||
            (username == "m7@pietermaritzburg " && password == "pass7") ||
            (username == "m8@polokwane" && password == "pass8") ||
            (username == "m9@mbombela" && password == "pass9"))
        {
            role = "manager";
            runLogin = true;
            cout << "Login successful. Welcome, Manager!" << endl;
            return;
        }
        SetConsoleTextAttribute(ConsoleColour, 11);
        cout << "Invalid credentials! Try again.\n" << endl;
        SetConsoleTextAttribute(ConsoleColour, 7);
        role = "guest";
        runLogin = false;
    }
};

// ---------------- Supply arrays & manager ----------------
const int NUM_ITEMS = 7;

struct Supply {
    int ID;
    string name;
    int Quantity;
    string expiry;
};

class SupplyManager {
public:
    Supply supplies[NUM_ITEMS];

    SupplyManager() {
        supplies[0] = { 101, "Food", 2000, "2025-12" };
        supplies[1] = { 102, "Water", 2010, "None" };
        supplies[2] = { 103, "Medicine", 150, "2029-09" };
        supplies[3] = { 104, "Clothes", 150, "None" };
        supplies[4] = { 105, "Blankets", 160, "None" };
        supplies[5] = { 106, "Tents", 60, "None" };
        supplies[6] = { 107, "HygieneKits", 100, "2029-03" };
    }

    static string toLowerCopy(string s) {
        transform(s.begin(), s.end(), s.begin(), ::tolower);
        return s;
    }

    int searchSupply(const string& key_in) {
        string key = toLowerCopy(key_in);
        for (int i = 0; i < NUM_ITEMS; ++i) {
            if (toLowerCopy(supplies[i].name) == key)
                return i;
        }
        return -1;
    }

    void displaySupplies() {
        cout << "\nWarehouse Supplies:\n";
        for (int i = 0; i < NUM_ITEMS; ++i) {
            cout << "- " << supplies[i].name << " (ID: " << supplies[i].ID
                << ") : " << supplies[i].Quantity << " units, Expiry: " << supplies[i].expiry << '\n';
        }
    }

    bool reduceSupplyByIndex(int idx, int qty) {
        if (idx < 0 || idx >= NUM_ITEMS) return false;
        if (supplies[idx].Quantity < qty) return false;
        supplies[idx].Quantity -= qty;
        return true;
    }

    // Simple bubble sort by quantity (keeps arrays aspect requested)
    void bubbleSortByQuantity() {
        for (int i = 0; i < NUM_ITEMS - 1; ++i) {
            for (int j = 0; j < NUM_ITEMS - i - 1; ++j) {
                if (supplies[j].Quantity > supplies[j + 1].Quantity) {
                    Supply temp = supplies[j];
                    supplies[j] = supplies[j + 1];
                    supplies[j + 1] = temp;
                }
            }
        }
    }
};

// ---------------- Shipment doubly-linked list ----------------
struct Shipment {
    int id;
    string type;
    string destination;
    int quantity;
    Shipment* prev;
    Shipment* next;
    Shipment(int i, string t, string d, int q) : id(i), type(t), destination(d), quantity(q), prev(nullptr), next(nullptr) {}
};

int generateID() {
    static int counter = 1;
    return counter++;
}

class ShipmentList {
private:
    Shipment* head;
    Shipment* tail;
    SupplyManager& manager;

    // valid capital cities (simple list)
    vector<string> validDestinations = {
        "Pretoria", "Bloemfontein", "Pietermaritzburg", "Polokwane",
        "Mbombela", "Kimberley", "Mahikeng", "Bhisho", "CapeTown", "Johannesburg", "Durban"
    };

    static string toLowerCopy(string s) {
        transform(s.begin(), s.end(), s.begin(), [](unsigned char c) { return std::tolower(c); });
        return s;
    }

    bool isValidDestination(const string& city) {
        string cLower = toLowerCopy(city);
        for (auto& v : validDestinations) {
            if (toLowerCopy(v) == cLower) return true;
        }
        return false;
    }

public:
    ShipmentList(SupplyManager& m) : head(nullptr), tail(nullptr), manager(m) {}

    void addShipment(const string& type, const string& destination, int quantity) {
        int idx = manager.searchSupply(type);
        if (idx == -1) {
            cout << "Request denied! '" << type << "' is not in the supply list.\n";
            return;
        }

        if (!isValidDestination(destination)) {
            cout << "Invalid destination! Please use a valid capital city.\n";
            return;
        }

        if (quantity <= 0) {
            cout << "Quantity must be > 0.\n";
            return;
        }

        if (quantity > manager.supplies[idx].Quantity) {
            cout << "Request denied! Only " << manager.supplies[idx].Quantity
                << " units of " << manager.supplies[idx].name << " available.\n";
            return;
        }

        // deduct
        manager.reduceSupplyByIndex(idx, quantity);

        int id = generateID();
        Shipment* node = new Shipment(id, manager.supplies[idx].name, destination, quantity);

        if (!head) {
            head = tail = node;
        }
        else {
            tail->next = node;
            node->prev = tail;
            tail = node;
        }

        // Ticket
        cout << "\n=========================\n";
        cout << "   Shipment Ticket   \n";
        cout << "-------------------------\n";
        cout << "Shipment ID : " << id << "\n";
        cout << "Type        : " << node->type << "\n";
        cout << "Destination : " << node->destination << "\n";
        cout << "Quantity    : " << node->quantity << "\n";
        cout << "=========================\n";
    }

    void displayForward() {
        Shipment* cur = head;
        if (!cur) {
            cout << "No shipments to display.\n";
            return;
        }
        cout << "\nAll Shipments:\n";
        while (cur) {
            cout << "ID: " << cur->id << " | " << cur->type << " -> " << cur->destination
                << " | Qty: " << cur->quantity << '\n';
            cur = cur->next;
        }
    }
};

// ---------------- Binary Search Trees (warehouse supplies & orders) ----------------
class SupplyNode {
public:
    string itemName;
    int quantity;
    SupplyNode* left; SupplyNode* right;
    SupplyNode(const string& item, int qty) : itemName(item), quantity(qty), left(nullptr), right(nullptr) {}
};

class SupplyBST {
private:
    SupplyNode* root;
    SupplyNode* insert(SupplyNode* node, const string& item, int qty) {
        if (!node) return new SupplyNode(item, qty);
        if (item < node->itemName) node->left = insert(node->left, item, qty);
        else if (item > node->itemName) node->right = insert(node->right, item, qty);
        else node->quantity += qty; // update existing
        return node;
    }
    void inorder(SupplyNode* n) {
        if (!n) return;
        inorder(n->left);
        cout << "- " << n->itemName << " : " << n->quantity << '\n';
        inorder(n->right);
    }
    SupplyNode* search(SupplyNode* n, const string& item) {
        if (!n) return nullptr;
        if (item == n->itemName) return n;
        if (item < n->itemName) return search(n->left, item);
        return search(n->right, item);
    }
public:
    SupplyBST() : root(nullptr) {}
    void addSupply(const string& item, int qty) { root = insert(root, item, qty); }
    void displaySupplies() {
        if (!root) { cout << "Warehouse BST is empty.\n"; return; }
        cout << "Warehouse Supplies (BST):\n"; inorder(root);
    }
    bool reduceSupply(const string& item, int qty) {
        SupplyNode* f = search(root, item);
        if (!f) { cout << "Item not found in warehouse BST.\n"; return false; }
        if (f->quantity < qty) { cout << "Not enough " << item << " in BST warehouse. Available: " << f->quantity << '\n'; return false; }
        f->quantity -= qty;
        cout << "BST: dispatched " << qty << " " << item << ". Remaining: " << f->quantity << '\n';
        return true;
    }
};

class OrderNode {
public:
    string hubName;
    string itemName;
    int quantity;
    OrderNode* left; OrderNode* right;
    OrderNode(const string& hub, const string& item, int qty) : hubName(hub), itemName(item), quantity(qty), left(nullptr), right(nullptr) {}
};

class OrderBST {
private:
    OrderNode* root;
    OrderNode* insert(OrderNode* node, const string& hub, const string& item, int qty) {
        if (!node) return new OrderNode(hub, item, qty);
        if (hub < node->hubName) node->left = insert(node->left, hub, item, qty);
        else if (hub > node->hubName) node->right = insert(node->right, hub, item, qty);
        else {
            if (node->itemName == item) {
                node->quantity += qty;
                cout << "Updated order for " << hub << " (" << node->itemName << " now " << node->quantity << " units)\n";
            }
            else {
                cout << "Hub " << hub << " already ordered a different item (" << node->itemName << "). New item not added to this same node.\n";
            }
        }
        return node;
    }
    void inorder(OrderNode* n) {
        if (!n) return;
        inorder(n->left);
        cout << "- " << n->hubName << " ordered " << n->quantity << " " << n->itemName << '\n';
        inorder(n->right);
    }
public:
    OrderBST() : root(nullptr) {}
    void addOrder(const string& hub, const string& item, int qty) { root = insert(root, hub, item, qty); cout << "Order added: " << hub << " " << qty << " " << item << '\n'; }
    void displayOrders() {
        if (!root) { cout << "No orders placed yet.\n"; return; }
        cout << "Orders by hubs (BST):\n"; inorder(root);
    }
};

// ---------------- Priority queue & stack for aid requests ----------------
struct AidRequest {
    int requestID;
    int supplyCount;
    int quantities[3];
    string priority;
    string location;
    string supplyTypes[3];
    string disasterType;
};

int getPriorityValue(const string& priority) {
    if (priority == "High" || priority == "high") return 2;
    if (priority == "Medium" || priority == "medium") return 1;
    return 0;
}
struct ComparePriority {
    bool operator()(const AidRequest& a, const AidRequest& b) const {
        return getPriorityValue(a.priority) < getPriorityValue(b.priority);
    }
};

class ReliefRequestSystem {
private:
    priority_queue<AidRequest, vector<AidRequest>, ComparePriority> requestQueue;
    stack<AidRequest> processedStack;
    int requestCounter = 1;
    const int MAX_REQUESTS = 500;
    const int MAX_QUANTITY = 1000;

    SupplyManager& supplyMgr;
    ShipmentList& shipList;
    SupplyBST& warehouseBST; // optional additional warehouse
    vector<string> allowedSupplies = { "Water", "Food", "Medicine", "Clothes", "Blankets", "Tents", "HygieneKits" };
    vector<string> allowedLocations = { "Johannesburg", "Cape Town", "Pietermaritzburg", "Bloemfontein", "Mbombela", "Polokwane", "Mahikeng", "Bhisho", "Kimberley", "Pretoria", "Durban" };
    vector<string> allowedDisasters = { "Flood", "Fire", "Earthquake" };

    static bool isValidOptionCI(const string& in, const vector<string>& list) {
        string low = in; transform(low.begin(), low.end(), low.begin(), ::tolower);
        for (auto s : list) {
            string lows = s; transform(lows.begin(), lows.end(), lows.begin(), ::tolower);
            if (low == lows) return true;
        }
        return false;
    }
    void saveRequestToFile(const AidRequest& r) {
        ofstream outFile("requests.txt", ios::app);
        if (!outFile) { cout << "Error opening requests file!\n"; return; }

        outFile << r.requestID << "," << r.location << "," << r.disasterType << ","
            << r.supplyCount;
        for (int i = 0; i < r.supplyCount; ++i) {
            outFile << "," << r.supplyTypes[i] << "," << r.quantities[i];
        }
        outFile << "," << r.priority << "\n";
        outFile.close();
    }

public:
    ReliefRequestSystem(SupplyManager& sm, ShipmentList& sl, SupplyBST& wb) : supplyMgr(sm), shipList(sl), warehouseBST(wb) {}

    void addRequestInteractive() {
        if (requestCounter > MAX_REQUESTS) {
            cout << "Request limit reached.\n";
            return;
        }

        AidRequest r;
        r.requestID = requestCounter++;
        r.supplyCount = 0;
        cin.ignore(10000, '\n');

        // location
        do {
            cout << "Enter location: ";
            getline(cin, r.location);
        } while (!isValidOptionCI(r.location, allowedLocations));

        // responsible for the input on what type of disater
        do {
            cout << "Enter disaster type (Flood/Fire/Earthquake): ";
            getline(cin, r.disasterType);
        } while (!isValidOptionCI(r.disasterType, allowedDisasters));

        // Now this checks for the number of supplies
        do {
            cout << "Enter number of supplies required (1-3): ";
            string tmp; getline(cin, tmp);
            try { r.supplyCount = stoi(tmp); }
            catch (...) { r.supplyCount = 0; }
        } while (r.supplyCount < 1 || r.supplyCount > 3);

        bool highPriority = false;
        for (int i = 0; i < r.supplyCount; ++i) {
            int choice = 0;
            do {
                cout << "Select supply #" << (i + 1) << " by number:\n";
                for (int j = 0; j < (int)allowedSupplies.size(); ++j)
                    cout << j + 1 << ". " << allowedSupplies[j] << '\n';
                string tmp; getline(cin, tmp);
                try { choice = stoi(tmp); }
                catch (...) { choice = 0; }
            } while (choice < 1 || choice >(int)allowedSupplies.size());
            r.supplyTypes[i] = allowedSupplies[choice - 1];
            if (r.supplyTypes[i] == "Food" || r.supplyTypes[i] == "Water" || r.supplyTypes[i] == "Medicine")
                highPriority = true;

            int qty = 0;
            do {
                cout << "Enter quantity (1-" << MAX_QUANTITY << "): ";
                string tmp; getline(cin, tmp);
                try { qty = stoi(tmp); }
                catch (...) { qty = 0; }
            } while (qty <= 0 || qty > MAX_QUANTITY);
            r.quantities[i] = qty;
        }
        r.priority = highPriority ? "High" : "Medium";
        requestQueue.push(r);
        saveRequestToFile(r);
        cout << "Request #" << r.requestID << " added to queue with priority " << r.priority << ".\n";
    }

    // Process highest priority request: attempt to fulfill using SupplyManager first (array), then warehouseBST fallback.
    void processRequest() {
        if (requestQueue.empty()) {
            cout << "No requests to process.\n";
            return;
        }

        AidRequest r = requestQueue.top();
        requestQueue.pop();

        bool allSatisfied = true;
        for (int i = 0; i < r.supplyCount; ++i) {
            string item = r.supplyTypes[i];
            int qty = r.quantities[i];
            int idx = supplyMgr.searchSupply(item);
            if (idx != -1 && supplyMgr.supplies[idx].Quantity >= qty) {
                supplyMgr.reduceSupplyByIndex(idx, qty);
                // create shipment record for this item
                shipList.addShipment(supplyMgr.supplies[idx].name, r.location, qty);
            }
            else {
                // try BST warehouse fallback
                if (warehouseBST.reduceSupply(item, qty)) {
                    // creating shipment
                    shipList.addShipment(item, r.location, qty);
                }
                else {
                    cout << "Unable to fulfill " << qty << " of " << item << " for request #" << r.requestID << ".\n";
                    allSatisfied = false;
                }
            }
        }

        if (allSatisfied) r.priority += " (Fulfilled)";
        else r.priority += " (Partial/Failed)";
        processedStack.push(r);
        cout << "Processed request #" << r.requestID << " | Priority now: " << r.priority << '\n';
    }
    void loadRequestsForApproval() {
        ifstream inFile("requests.txt");
        if (!inFile) {
            cout << "No saved requests found.\n";
            return;
        }

        AidRequest r;
        requestQueue = priority_queue<AidRequest, vector<AidRequest>, ComparePriority>(); // clear queue

        while (inFile >> r.requestID) {
            inFile.ignore();
            getline(inFile, r.location, ',');
            getline(inFile, r.disasterType, ',');
            inFile >> r.supplyCount;
            inFile.ignore();

            for (int i = 0; i < r.supplyCount; i++) {
                getline(inFile, r.supplyTypes[i], ',');
                inFile >> r.quantities[i];
                inFile.ignore();
            }

            getline(inFile, r.priority);
            requestQueue.push(r);
        }

        inFile.close();
        cout << "Requests loaded into queue for admin approval.\n";
    }
    void approveRequest() {
        if (requestQueue.empty()) {
            cout << "No requests to approve.\n";
            return;
        }

        int approveID;
        cout << "Enter Request ID to approve: ";
        cin >> approveID;

        priority_queue<AidRequest, vector<AidRequest>, ComparePriority> tempQueue;
        bool found = false;

        while (!requestQueue.empty()) {
            AidRequest r = requestQueue.top();
            requestQueue.pop();

            if (r.requestID == approveID) {
                r.priority += " (Approved)";
                cout << "Request #" << r.requestID << " approved.\n";
                found = true;
            }

            tempQueue.push(r);
        }

        requestQueue = tempQueue;

        if (!found) cout << "Request ID not found.\n";
    }

    void loadRequestsForAdmin() {
        ifstream inFile("requests.txt");
        if (!inFile) {
            cout << "No saved requests found.\n";
            return;
        }

        string line;
        cout << "\n=== All Saved Requests ===\n";
        while (getline(inFile, line)) {
            cout << line << '\n'; // just display each request
        }

        inFile.close();
    }

    void viewPendingRequests() {
        if (requestQueue.empty()) { cout << "No pending requests.\n"; return; }
        auto copy = requestQueue;
        cout << "Pending requests (highest priority first):\n";
        while (!copy.empty()) {
            AidRequest r = copy.top(); copy.pop();
            cout << "Req#" << r.requestID << " | Loc: " << r.location << " | Priority: " << r.priority << '\n';
        }
    }

    void viewProcessedRequests() {
        if (processedStack.empty()) { cout << "No processed requests yet.\n"; return; }
        auto copy = processedStack;
        cout << "Processed requests (last processed first):\n";
        while (!copy.empty()) {
            AidRequest r = copy.top(); copy.pop();
            cout << "Req#" << r.requestID << " | Loc: " << r.location << " | Priority: " << r.priority << '\n';
        }
    }
};

// ---------------- Main program (menu) ----------------
int main() {
    // managers inf
    SupplyManager supplyManager;
    ShipmentList shipmentList(supplyManager);
    SupplyBST warehouseBST;
    OrderBST orderBST;

    // Add initial BST stock (separate tree for demonstration)
    warehouseBST.addSupply("Food", 500);
    warehouseBST.addSupply("Water", 1000);
    warehouseBST.addSupply("Medicine", 200);
    warehouseBST.addSupply("Blankets", 300);
    warehouseBST.addSupply("Tents", 100);

    ReliefRequestSystem requestSystem(supplyManager, shipmentList, warehouseBST);
    LoginManager loginMgr;

    greetUser(1);


    cout << "Select role to login:\n1. Admin\n2. Manager\nEnter choice: ";
    int roleChoice = 0;
    if (!(cin >> roleChoice)) {
        cout << "Invalid input. Exiting.\n";
        return 0;
    }

    if (roleChoice == 1) {
        // Admin login
        while (!loginMgr.runLogin) loginMgr.login();
        if (loginMgr.getRole() != "admin") {
            cout << "Admin login required. Exiting.\n";
            return 0;
        }

        int adminChoice = -1;
        do {
            cout << "\n====== Admin Menu ======\n";
            cout << "1. View supplies (array manager)\n";
            cout << "2. View warehouse (BST)\n";
            cout << "3. Add supply to BST warehouse\n";
            cout << "4. Add shipment (manual)\n";
            cout << "5. View shipments\n";
            cout << "6. View orders (BST)\n";
            cout << "7.View All Request\n";
            cout << "8.Admin aproval\n";
            cout << "9. View shipments\n";
            cout << "0. Logout\n";
            cout << "Choose: ";
            if (!(cin >> adminChoice)) { cin.clear(); string tmp; getline(cin, tmp); adminChoice = -1; continue; }

            switch (adminChoice) {
            case 1:
                supplyManager.displaySupplies();
                break;
            case 2:
                warehouseBST.displaySupplies();
                break;
            case 3: {
                cin.ignore(10000, '\n');
                cout << "Enter item name to add to BST (e.g. Water): ";
                string item; getline(cin, item);
                cout << "Enter quantity to add: ";
                int q; if (!(cin >> q)) { cin.clear(); cout << "Invalid qty.\n"; break; }
                warehouseBST.addSupply(item, q);
                break;
            }
            case 4: {
                string type, dest; int qty;
                cout << "Enter shipment type (name): "; cin >> type;
                cout << "Enter destination (capital): "; cin >> dest;
                cout << "Enter quantity: "; cin >> qty;
                shipmentList.addShipment(type, dest, qty);
                break;
            }
            case 5:
                shipmentList.displayForward();
                break;
            case 6:
                orderBST.displayOrders();
                break;
            case 7:
                requestSystem.loadRequestsForAdmin();
                break;
            case 8:
                requestSystem.loadRequestsForApproval();
                requestSystem.approveRequest();
                break;
            case 9:
                //ReliefRequestSystem.loadShipmentsFromFile();
                shipmentList.displayForward();
                break;
            case 0:
                cout << "\nLogging out admin...\n";
                break;
            default:
                cout << "Invalid choice.\n";
            }
        } while (adminChoice != 0);

    }
    else if (roleChoice == 2) {

        while (!loginMgr.runLogin) loginMgr.login();
        if (loginMgr.getRole() != "manager") {
            cout << "Manager login required. Exiting.\n";
            return 0;
        }

        int mChoice = -1;
        do {
            cout << "\n--- Manager Menu ---\n";
            cout << "1. Add a new aid request\n";
            cout << "2. Process next request\n";
            cout << "3. View pending requests\n";
            cout << "4. View processed requests\n";
            cout << "5. View supplies (array)\n";
            cout << "6. Place hub order (BST)\n";
            cout << "7. View orders (BST)\n";

            cout << "0. Logout\n";
            cout << "Enter choice: ";
            if (!(cin >> mChoice)) {
                cin.clear();
                string tmp;
                getline(cin, tmp);
                mChoice = -1;
                continue;
            }

            switch (mChoice) {
            case 1:
                requestSystem.addRequestInteractive();
                break;
            case 2:
                requestSystem.processRequest();
                break;
            case 3:
                requestSystem.viewPendingRequests();
                break;
            case 4:
                requestSystem.viewProcessedRequests();
                break;
            case 5:
                supplyManager.displaySupplies();
                break;
            case 6: {
                cin.ignore(10000, '\n');
                cout << "Enter hub (e.g. Johannesburg): ";
                string hub; getline(cin, hub);
                cout << "Enter item to order (e.g. Food): ";
                string it; getline(cin, it);
                cout << "Enter quantity: ";
                int q; if (!(cin >> q)) { cin.clear(); cout << "Invalid qty.\n"; break; }
                // attempt to reduce from BST warehouse
                if (warehouseBST.reduceSupply(it, q)) {
                    orderBST.addOrder(hub, it, q);
                }
                else {
                    cout << "Unable to place order: insufficient BST warehouse stock.\n";
                }
                break;
            }
            case 7:
                orderBST.displayOrders();
                break;
            case 0:
                cout << "Manager logging out...\n";
                break;
            default:
                cout << "Invalid option.\n";
            }
        } while (mChoice != 0);

    }
    else {
        cout << "Invalid role selection. Exiting.\n";
    }

    thankUser(1);
    SetConsoleTextAttribute(ConsoleColour, 4);
    cout << "Program terminated.\n";
    SetConsoleTextAttribute(ConsoleColour, 7);
    return 0;
}