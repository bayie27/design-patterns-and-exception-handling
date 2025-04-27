#include <iostream>
#include <string>
#include <fstream>
#include <iomanip>
#include <memory>

using namespace std;

// Custom exceptions
class ECommerceException : public exception {
private:
    string message;
public:
    ECommerceException(const string& msg) : message(msg) {}
    virtual const char* what() const noexcept override {
        return message.c_str();
    }
};

class ProductNotFoundException : public ECommerceException {
public:
    ProductNotFoundException(const string& id) 
        : ECommerceException("Product with ID '" + id + "' not found!") {}
};

class InvalidInputException : public ECommerceException {
public:
    InvalidInputException(const string& msg) 
        : ECommerceException("Invalid input: " + msg) {}
};

class ArrayFullException : public ECommerceException {
public:
    ArrayFullException(const string& arrayName) 
        : ECommerceException(arrayName + " is full. Cannot add more items.") {}
};

// Product class
class Product {
private:
    string id;
    string name;
    double price;
    
public:    
    // Constructor
    Product(const string& _id, const string& _name, double _price) 
    : id(_id), name(_name), price(_price) {}
    
    // Getters
    string getId() const { return id; }
    string getName() const { return name; }
    double getPrice() const { return price; }
    
    // Display product info
    void display() const {
        cout << left << setw(15) << id
             << setw(20) << name
             << setw(10) << fixed << setprecision(2) << price << endl;
    }
};

// Cart Item class
class CartItem {
private:
    shared_ptr<Product> product;
    int quantity;
    bool initialized;
    
public:
    // Default constructor
    CartItem() : product(nullptr), quantity(0), initialized(false) {}
    
    // Constructor
    CartItem(shared_ptr<Product> _product, int _quantity) 
        : product(_product), quantity(_quantity), initialized(true) {}
    
    // Getters
    shared_ptr<Product> getProduct() const { return product; }
    int getQuantity() const { return quantity; }
    double getTotalPrice() const { return product ? product->getPrice() * quantity : 0.0; }
    bool isInitialized() const { return initialized; }
    
    // Display cart item info
    void display() const {
        if (initialized && product) {
            cout << left 
                 << setw(15) << product->getId()
                 << setw(20) << product->getName()
                 << setw(10) << fixed << setprecision(2) << product->getPrice()
                 << setw(10) << quantity << endl;
        }
    }
};

// Order class
class Order {
private:
    int orderId;
    CartItem items[10];
    int itemCount;
    string paymentMethod;
    double totalAmount;
    bool initialized;
    
public:
    // Default constructor
    Order() : orderId(0), itemCount(0), paymentMethod(""), totalAmount(0.0), initialized(false) {}
    
    // Constructor
    Order(int _orderId, const CartItem* _items, int _itemCount, const string& _paymentMethod)
        : orderId(_orderId), itemCount(_itemCount), paymentMethod(_paymentMethod), 
          totalAmount(0.0), initialized(true) {
        
        // Copy items
        for (int i = 0; i < _itemCount && i < 10; i++) {
            items[i] = _items[i];
        }
        
        calculateTotal();
    }
    
    // Calculate total amount
    void calculateTotal() {
        totalAmount = 0;
        for (int i = 0; i < itemCount; i++) {
            if (items[i].isInitialized()) {
                totalAmount += items[i].getTotalPrice();
            }
        }
    }
    
    // Getters
    int getOrderId() const { return orderId; }
    const CartItem* getItems() const { return items; }
    int getItemCount() const { return itemCount; }
    string getPaymentMethod() const { return paymentMethod; }
    double getTotalAmount() const { return totalAmount; }
    bool isInitialized() const { return initialized; }
    
    // Display order details
    void display() const {
        if (!initialized) return;
        
        cout << "\nOrder ID: " << orderId << endl;
        cout << "Total Amount: " << fixed << setprecision(2) << totalAmount << endl;
        cout << "Payment Method: " << paymentMethod << endl;
        cout << "Order Details:" << endl;
        cout << left << setw(15) << "Product ID" 
             << setw(20) << "Name" 
             << setw(10) << "Price" 
             << setw(10) << "Quantity" << endl;
        
        for (int i = 0; i < itemCount; i++) {
            items[i].display();
        }
        cout << endl;
    }
};

// Shopping Cart class
class ShoppingCart {
private:
    CartItem items[10];
    int itemCount;
    
public:
    // Constructor
    ShoppingCart() : itemCount(0) {}
    
    // Add item to cart
    void addItem(shared_ptr<Product> product, int quantity) {
        if (itemCount >= 10) {
            throw ArrayFullException("Shopping Cart");
        }
        
        items[itemCount++] = CartItem(product, quantity);
    }
    
    // Clear cart
    void clear() {
        for (int i = 0; i < itemCount; i++) {
            items[i] = CartItem(); // Reset to default
        }
        itemCount = 0;
    }
    
    // Get items in cart
    const CartItem* getItems() const {
        return items;
    }
    
    // Get item count
    int getItemCount() const {
        return itemCount;
    }
    
    // Calculate total amount
    double getTotalAmount() const {
        double total = 0;
        for (int i = 0; i < itemCount; i++) {
            total += items[i].getTotalPrice();
        }
        return total;
    }
    
    // Check if cart is empty
    bool isEmpty() const {
        return itemCount == 0;
    }
    
    // Display cart contents
    void display() const {
        if (isEmpty()) {
            cout << "Your shopping cart is empty." << endl;
            return;
        }
        
        cout << "\n----- Shopping Cart -----" << endl;
        cout << left << setw(15) << "Product ID" 
             << setw(20) << "Name" 
             << setw(10) << "Price" 
             << setw(10) << "Quantity" << endl;
        
        for (int i = 0; i < itemCount; i++) {
            items[i].display();
        }
        
        cout << "\nTotal Amount: ₱" << fixed << setprecision(2) << getTotalAmount() << endl;
    }
};

// Inventory class for product management
class Inventory {
private:
    shared_ptr<Product> products[5]; 
    int productCount;
    
public:
    // Constructor with initial products
    Inventory() : productCount(5) {
        products[0] = make_shared<Product>("A1B2C3", "C2 Green Tea", 32.0);
        products[1] = make_shared<Product>("X9Y8Z7", "Zesto Juice Drink", 14.0);
        products[2] = make_shared<Product>("P4Q5R6", "Cobra Energy Drink", 29.0);
        products[3] = make_shared<Product>("M7N8O9", "1.5L Royal", 75.0);
        products[4] = make_shared<Product>("J1K2L3", "Milo", 12.5);
    }

    shared_ptr<Product> findProduct(const string& id) const {
        // Manually convert input ID to uppercase
        string upperId = id;
        for (size_t i = 0; i < upperId.length(); i++) {
            upperId[i] = toupper(upperId[i]);
        }
    
        // Compare with stored uppercase IDs
        for (int i = 0; i < productCount; i++) {
            if (products[i]->getId() == upperId) {
                return products[i];
            }
        }
        throw ProductNotFoundException(id);
    }
    
    // Display all products
    void displayProducts() const {
        cout << "\n----- Available Products -----" << endl;
        cout << left << setw(15) << "Product ID" 
             << setw(20) << "Name" 
             << setw(10) << "Price" << endl;
        
        for (int i = 0; i < productCount; i++) {
            products[i]->display();
        }
    }
};

// Strategy Pattern for Payment Methods
class PaymentStrategy {
public:
    virtual ~PaymentStrategy() = default;
    virtual bool processPayment(double amount) = 0;
    virtual string getMethodName() const = 0;
};

class CashPayment : public PaymentStrategy {
public:
    bool processPayment(double amount) override {
        cout << "Processing cash payment of ₱" << fixed << setprecision(2) << amount << endl;
        return true;
    }
    
    string getMethodName() const override {
        return "Cash";
    }
};

class CardPayment : public PaymentStrategy {
public:
    bool processPayment(double amount) override {
        cout << "Processing credit/debit card payment of ₱" << fixed << setprecision(2) << amount << endl;
        return true;
    }
    
    string getMethodName() const override {
        return "Credit / Debit Card";
    }
};

class GCashPayment : public PaymentStrategy {
public:
    bool processPayment(double amount) override {
        cout << "Processing GCash payment of ₱" << fixed << setprecision(2) << amount << endl;
        return true;
    }
    
    string getMethodName() const override {
        return "GCash";
    }
};

// Singleton Pattern for Payment Processor
class PaymentProcessor {
private:
    static PaymentProcessor* instance;
    int nextOrderId;
    Order orders[10];
    int orderCount;
    
    // Private constructor for singleton
    PaymentProcessor() : nextOrderId(1), orderCount(0) {}
    
public:
    // Get singleton instance
    static PaymentProcessor* getInstance() {
        if (instance == nullptr) {
            instance = new PaymentProcessor();
        }
        return instance;
    }
    
    // Process payment and create order
    Order processPayment(const ShoppingCart& cart, PaymentStrategy* paymentStrategy) {
        double amount = cart.getTotalAmount();
        
        try {
            bool success = paymentStrategy->processPayment(amount);
            
            if (orderCount >= 10) {
                throw ArrayFullException("Orders database");
            }
            
            // Create new order
            Order order(nextOrderId++, cart.getItems(), cart.getItemCount(), paymentStrategy->getMethodName());
            orders[orderCount++] = order;
            
            // Log the order
            logOrder(order);
            
            return order;
        } catch (const exception& e) {
            throw ECommerceException("Payment failed with method: " + paymentStrategy->getMethodName());
        }
    }
    
    // Log order to file
    void logOrder(const Order& order) {
        try {
            ofstream logFile("orders.log", ios::app);
            if (!logFile) {
                cerr << "Warning: Could not open log file." << endl;
                return;
            }
            
            logFile << "[LOG] -> Order ID: " << order.getOrderId() 
                    << " has been successfully checked out and paid using " 
                    << order.getPaymentMethod() << endl;
            
            logFile.close();
        } catch (const exception& e) {
            cerr << "Warning: Failed to log order: " << e.what() << endl;
        }
    }
    
    // Get orders
    const Order* getOrders() const {
        return orders;
    }
    
    // Get order count
    int getOrderCount() const {
        return orderCount;
    }
};

// Initialize static instance pointer
PaymentProcessor* PaymentProcessor::instance = nullptr;

// E-commerce System class
class ECommerceSystem {
private:
    Inventory inventory;
    ShoppingCart cart;
    
    // Input validation helper
    int getIntInput(const string& prompt) {
        string input;
        int value;
        bool validInput = false;
    
        while (!validInput) {
            cout << prompt;
            getline(cin, input);
    
            try {
                if (input.empty()) {
                    throw InvalidInputException("Input cannot be empty. Please try again.");
                }
    
                // Check if the input contains only digits
                for (char c : input) {
                    if (!isdigit(c)) {
                        throw InvalidInputException("Input must be a valid positive whole integer.");
                    }
                }
    
                value = stoi(input);
    
                if (value == 0) {
                    throw InvalidInputException("Input cannot be zero. Please try again.");
                }
    
                validInput = true; // Input is valid
            } catch (const ECommerceException& e) {
                cout << e.what() << endl;
            }
        }
    
        return value;
    }
    
    string getStringInput(const string& prompt) {
        string input;
        bool validInput = false;
        
        while (!validInput) {
            cout << prompt;
            getline(cin, input);
            
            if (input.empty()) {
                cout << "Input cannot be empty. Please try again." << endl;
            } else {
                return input;
            }
        }
        return "";
    }
    
    char getCharInput(const string& prompt) {
        string input;
        bool validInput = false;

        while (!validInput) {
            cout << prompt;
            getline(cin, input);
    
            if (input.empty()) {
                cout << "Input cannot be empty. Please try again." << endl;
            } else if (input.length() > 1 || (toupper(input[0]) != 'Y' && toupper(input[0]) != 'N')) {
                cout << "Invalid input. Please enter 'Y' or 'N'." << endl;
            } else {
                return toupper(input[0]); // Return the validated character in uppercase
            }
        }
        return ' ';
    }
    
    PaymentStrategy* selectPaymentStrategy() {
        int choice;
        bool validChoice = false;
        
        while (!validChoice) {
            cout << "\nSelect payment method:" << endl;
            cout << "1. Cash" << endl;
            cout << "2. Credit / Debit Card" << endl;
            cout << "3. GCash" << endl;
    
            choice = getIntInput("Enter your choice (1-3): ");
            switch (choice) {
                case 1: return new CashPayment();
                case 2: return new CardPayment();
                case 3: return new GCashPayment();
                default:
                    cout << "Invalid choice. Please enter a number between 1 and 3." << endl;
            }
        }
        return 0;
    }
    
    void viewProducts() {
        inventory.displayProducts();
        bool addAgain = true;
        
        while (addAgain) {
            try {
                string productId = getStringInput("\nEnter the ID of the product you want to add in the shopping cart: ");
        
                auto product = inventory.findProduct(productId);
                int quantity = getIntInput("Enter quantity: ");
                
                cart.addItem(product, quantity);
                cout << "Product added successfully!" << endl;
                
                char addMore = getCharInput("Do you want to add another product? (Y/N): ");
                if (toupper(addMore) != 'Y') {
                    break;
                }
            } catch (const ECommerceException& e) {
                cout << e.what() << endl;
                char tryAgain = getCharInput("Do you want to try again? (Y/N): ");
                if (toupper(tryAgain) != 'Y') {
                    break;
                }
            }
        }
    }
    
    void viewCart() {
        if (cart.isEmpty()) {
            cout << "Your shopping cart is empty. Please add products before checking out." << endl;
            return;
        }
        
        cart.display();
        
        char checkout = getCharInput("\nDo you want to check out all the products? (Y/N): ");
        if (toupper(checkout) != 'Y') {
            return;
        }
        
        try {
            PaymentStrategy* paymentStrategy = selectPaymentStrategy();
            Order order = PaymentProcessor::getInstance()->processPayment(cart, paymentStrategy);
            
            cout << "\nYou have successfully checked out the products!" << endl;
            
            delete paymentStrategy; // Clean up
            cart.clear(); // Clear cart after successful checkout
        } catch (const ECommerceException& e) {
            cout << "Error: " << e.what() << endl;
        }
    }
    
    void viewOrders() {
        PaymentProcessor* processor = PaymentProcessor::getInstance();
        int orderCount = processor->getOrderCount();
        
        if (orderCount == 0) {
            cout << "No orders to display." << endl;
            return;
        }
        
        cout << "\n----- Order History -----" << endl;
        const Order* orders = processor->getOrders();
        
        for (int i = 0; i < orderCount; i++) {
            orders[i].display();
        }
    }
    
public:
    void run() {
        cout << "===== Welcome to the Daniboy's E-commerce System =====" << endl;
        bool running = true;
        
        while (running) {
            try {
                cout << "\n===== Main Menu =====" << endl;
                cout << "1. View Products" << endl;
                cout << "2. View Shopping Cart" << endl;
                cout << "3. View Orders" << endl;
                cout << "4. Exit" << endl;
                
                int choice = getIntInput("Enter your choice (1-4): ");
                if (choice < 1 || choice > 4) {
                    cout << "Invalid choice. Please enter a number between 1 and 4." << endl;
                    continue;
                }
                
                switch (choice) {
                    case 1:
                        viewProducts();
                        break;
                    case 2:
                        viewCart();
                        break;
                    case 3:
                        viewOrders();
                        break;
                    case 4:
                        cout << "Thank you for using the E-commerce System. Goodbye!" << endl;
                        return;
                    default:
                        cout << "Invalid choice. Please try again." << endl;
                }
            } catch (const exception& e) {
                cout << "An error occurred: " << e.what() << endl;
                cout << "Please try again." << endl;
            }
        }
    }
};

int main() {
    ECommerceSystem system;
    system.run();
    
    return 0;
}