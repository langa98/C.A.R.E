# CRISIS AID RESOURCE ENGINE (C.A.R.E.)

A C++ console-based crisis management system for handling supplies, shipments, shelter data, aid requests, and emergency logistics.

## 📌 Overview
The CRISIS AID RESOURCE ENGINE (C.A.R.E.) is designed to simulate how crisis-response organizations manage inventory, requests, distribution, and field operations. It demonstrates:  
- File handling  
- Object-oriented programming  
- Recursion  
- Linked lists  
- Doubly linked lists  
- Stacks  
- Priority queues  
- Binary Search Trees (BST)  
- Sorting algorithms  
- Input validation  
- Full inventory and request workflow logic  

## ✨ Features

### 🔐 Login System
The system requires login before use.  
Roles:  
- **Admin** – full access including approvals  
- **Manager** – limited access (view/add requests, shipments)

### 📦 Supply Management
Supplies include: Food, Water, Medicine, Clothes, Blankets, Tents, Hygiene Kits.  
Functions:  
- View supplies  
- Search for supply  
- Reduce quantities  
- Sort by quantity (bubble sort)

### 🚚 Shipment Tracking (Doubly Linked List)
Each shipment contains:  
- Supply name  
- Quantity  
- Destination  
Features include:  
- Add shipments  
- View all shipments  
- Update shipments

### 🆘 Aid Request Handling (Priority Queue)
Each request stores:  
- Location  
- Number of affected people  
- Severity rating (used to prioritize automatically)

### 🏠 Shelter Management (Stack)
Shelter entries include:  
- Shelter name  
- Capacity  
- Address  
Features include pushing, popping, and viewing shelter data.

### 🌲 Warehouse Management (Binary Search Tree)
BST nodes store:  
- Quantity  
- Restocking date  
Functions: insert, search, and display supplies in sorted order.

### 📁 File Handling
At startup, the system loads from:  
- `Shipments.txt`  
- `Requests.txt`  
When the program closes, updated data is saved back.

## 🖥️ How to Compile and Run

### 1. Clone the repository
git clone https://github.com/langa98/C.A.R.E

### 2. Open in your preferred C++ IDE
Supported: Visual Studio, Code::Blocks, Dev-C++, MinGW (g++).

### 3. Compile using C++17 or newer

g++ main.cpp -o care


### 4. Run the application

## 📂 Project Structure
/src
main.cpp

/data
Shipments.txt
Requests.txt

README.md
.gitignore

## 🤝 Contributing
Pull requests are welcome. You may fork the repository and add improvements such as UI, optimizations, or new modules.

## 📜 License
```
This project is provided for educational and personal use. No warranty is expressed or implied
```
