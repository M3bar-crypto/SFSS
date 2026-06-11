
   #include <iostream>
   #include <string>

   using namespace std;

   #define MAX 5

struct Node {
    string data;
    Node* next;
};

void addTransaction(Node*& head, string details) {
    Node* newNode = new Node();
    newNode->data = details;
    newNode->next = nullptr;
    if (!head) head = newNode;
    else {
        Node* temp = head;
        while (temp->next) temp = temp->next;
        temp->next = newNode;
    }
    cout << ">> [LOGGED] " << details << endl;
}

void displayTransactions(Node* head) {
   
    cout << "  |  STATION TRANSACTION HISTORY  | " << endl;
   
    if (!head) { cout << "No transactions recorded yet ." << endl; return; }
    Node* temp = head;
    int counter = 1;
    while (temp) {
        cout << counter++ << ". " << temp->data << endl;
        temp = temp->next;
    }
    cout << "\n" << endl;
}


struct UndoStack {
    string arr[MAX];
    int top = -1;
};

void pushUndo(UndoStack& s, string action) {
    if (s.top < MAX - 1) s.arr[++s.top] = action;
}

void popUndo(UndoStack& s) {
    if (s.top < 0) cout << "[Undo] Nothing to undo!" << endl;
    else {
        cout << "[Undo] Reverted last action: " << s.arr[s.top--] << endl;
    }
}


struct CarQueue {
    string arr[MAX];
    int front = -1, rear = -1;
};

void enqueueCar(CarQueue& q, string plate) {
    if (q.rear == MAX - 1) {
        cout << " [PUMP FULL] Station queue is full! Car must wait outside ." << endl;
    } else {
        if (q.front == -1) q.front = 0;
        q.arr[++q.rear] = plate;
        cout << "Car [" << plate << "] entered the station pump queue ." << endl;
    }
}

void dequeueCar(CarQueue& q, Node*& history, UndoStack& undo) {
    if (q.front == -1 || q.front > q.rear) {
        cout << "[PUMP EMPTY] No cars waiting at the pump ." << endl;
    } else {
        cout << "[FUELING] Finished fueling car: " << q.arr[q.front] << endl;
        addTransaction(history, "Fueled Car : " + q.arr[q.front]);
        pushUndo(undo, "Fueling Car : " + q.arr[q.front]);
        q.front++;
        if (q.front > q.rear) { q.front = -1; q.rear = -1; } 
    }
}


void pushAlert(Node*& top, string alertMsg) {
    Node* newNode = new Node();
    newNode->data = alertMsg;
    newNode->next = top;
    top = newNode;
    cout << " [CRITICAL ALERT] IoT Sensor : " << alertMsg << endl;
}

void resolveAlert(Node*& top, Node*& history) {
    if (!top) cout << " [ALL CLEAR] No active alerts. Station is safe ." << endl;
    else {
        cout << "[RESOLVED] Fixed critical issue : " << top->data << endl;
        addTransaction(history, "Resolved Alert : " + top->data);
        Node* temp = top;
        top = top->next;
        delete temp;
    }
}


void enqueueTruck(Node*& front, Node*& rear, string truckID) {
    Node* newNode = new Node();
    newNode->data = truckID;
    newNode->next = nullptr;
    if (!rear) front = rear = newNode;
    else { rear->next = newNode; rear = newNode; }
    cout << "Fuel Truck [" << truckID << "] arrived at the unloading zone ." << endl;
}

void dequeueTruck(Node*& front, Node*& rear, Node*& history) {
    if (!front) cout << "[SUPPLY EMPTY] No fuel trucks waiting ." << endl;
    else {
        cout << " [UNLOADING] Main storage refilled from truck : " << front->data << endl;
        addTransaction(history, "Refilled storage from truck : " + front->data);
        Node* temp = front;
        front = front->next;
        if (!front) rear = nullptr;
        delete temp;
    }
}


int main() {
   
    Node* transactionHead = nullptr;       
    UndoStack operatorUndo;                
    CarQueue pumpQueue;                    
    Node* alertTop = nullptr;              
    Node* truckFront = nullptr;            
    Node* truckRear = nullptr;             

    int choice;
    string inputData;

    while (true) {
      
        cout << "  \n    SMART FUEL STATION CONTROL PANEL       " << endl;
        cout << "---------------------------------------------" << endl;
        cout << "1 - Car Arrives (Enqueue Car to Pump)" << endl;
        cout << "2 - Fuel Car (Dequeue Car from Pump)" << endl;
        cout << "3 - Fuel Truck Arrives (Enqueue Supply Truck)" << endl;
        cout << "4 - Unload Fuel Truck (Dequeue Supply Truck)" << endl;
        cout << "5 - Trigger IoT Sensor Alert (Push Alert)" << endl;
        cout << "6 -  Resolve Most Recent Alert (Pop Alert)" << endl;
        cout << "7 -  Undo Last Fueling Action" << endl;
        cout << "8 -  View Daily Logs (Print Transaction History)" << endl;
        cout << "9 -  Shutdown System (Exit)" << endl;
        cout << "Choose an operation (1-9) : \n";
        cin >> choice;
        cin.ignore();

        switch (choice) {
            case 1:
                cout << "Enter Car Details ( for e.g , Toyota Tundra ABC-123 ) :\n ";
                getline(cin, inputData);
                enqueueCar(pumpQueue, inputData);
                break;
            case 2:
                dequeueCar(pumpQueue, transactionHead, operatorUndo);
                break;
            case 3:
                cout << "Enter Truck ID ( for e.g , Truck-09 ) :\n ";
                getline(cin, inputData);
                enqueueTruck(truckFront, truckRear, inputData);
                break;
            case 4:
                dequeueTruck(truckFront, truckRear, transactionHead);
                break;
            case 5:
                cout << "Enter Emergency Alert Description :\n ";
                getline(cin, inputData);
                pushAlert(alertTop, inputData);
                break;
            case 6:
                resolveAlert(alertTop, transactionHead);
                break;
            case 7:
                popUndo(operatorUndo);
                break;
            case 8:
                displayTransactions(transactionHead);
                break;
            case 9:
                cout << "\nShutting down station management system . Goodbye !\n" << endl;
                return 0;
            default:
                cout << "\n Invalid choice! Please select between 1 and 9 ." << endl;
        }
    }
    return 0;
}   