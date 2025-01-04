#include <iostream>
#include <vector>
#include <string>
#include <iomanip>
using namespace std;

// Product class to manage inventory items
class Product {
private:
    string name;
    int productID;
    double price;
    int quantity;

public:
    Product(string name, int productID, double price, int quantity)
        : name(name), productID(productID), price(price), quantity(quantity) {}

    string getName() const { return name; }
    int getProductID() const { return productID; }
    double getPrice() const { return price; }
    int getQuantity() const { return quantity; }

    void updateQuantity(int newQuantity) { quantity = newQuantity; }

    void displayProduct() const {
        cout << left << setw(10) << productID << setw(20) << name
             << setw(10) << price << setw(10) << quantity << endl;
    }
};

// Customer class to manage customer details
class Customer {
private:
    string name;
    int customerID;
    string contact;

public:
    Customer(string name, int customerID, string contact)
        : name(name), customerID(customerID), contact(contact) {}

    string getName() const { return name; }
    int getCustomerID() const { return customerID; }
    string getContact() const { return contact; }

    void displayCustomer() const {
        cout << left << setw(10) << customerID << setw(20) << name << setw(15) << contact << endl;
    }
};

// Invoice class to manage invoices
class Invoice {
private:
    int invoiceID;
    Customer customer;
    vector<pair<Product, int>> items;
    double totalAmount;

public:
    Invoice(int invoiceID, Customer customer)
        : invoiceID(invoiceID), customer(customer), totalAmount(0.0) {}

    void addItem(const Product& product, int quantity) {
        items.push_back(make_pair(product, quantity));
        totalAmount += product.getPrice() * quantity;
    }

    void printInvoice() const {
        cout << "\nInvoice ID: " << invoiceID << endl;
        cout << "Customer: " << customer.getName() << "\n";
        cout << "----------------------------------------" << endl;
        cout << left << setw(10) << "ProductID" << setw(20) << "Product Name"
             << setw(10) << "Price" << setw(10) << "Quantity" << setw(10) << "Total" << endl;
        cout << "----------------------------------------" << endl;

        for (const auto& item : items) {
            cout << left << setw(10) << item.first.getProductID() << setw(20) << item.first.getName()
                 << setw(10) << item.first.getPrice() << setw(10) << item.second
                 << setw(10) << item.first.getPrice() * item.second << endl;
        }
        cout << "----------------------------------------" << endl;
        cout << "Total Amount: " << totalAmount << "\n\n";
    }
};

// Main application class
class InvoicingSoftware {
private:
    vector<Product> inventory;
    vector<Customer> customers;
    vector<Invoice> invoices;
    int nextCustomerID;
    int nextInvoiceID;

public:
    InvoicingSoftware() : nextCustomerID(1), nextInvoiceID(1) {}

    void addProduct(const string& name, double price, int quantity) {
        inventory.emplace_back(name, inventory.size() + 1, price, quantity);
    }

    void addCustomer(const string& name, const string& contact) {
        customers.emplace_back(name, nextCustomerID++, contact);
    }

    void createInvoice() {
        cout << "\nEnter Customer ID: ";
        int customerID;
        cin >> customerID;

        auto it = find_if(customers.begin(), customers.end(), [customerID](const Customer& c) {
            return c.getCustomerID() == customerID;
        });

        if (it == customers.end()) {
            cout << "Customer not found!\n";
            return;
        }

        Invoice invoice(nextInvoiceID++, *it);

        while (true) {
            cout << "\nEnter Product ID (or 0 to finish): ";
            int productID;
            cin >> productID;

            if (productID == 0) break;

            auto prodIt = find_if(inventory.begin(), inventory.end(), [productID](const Product& p) {
                return p.getProductID() == productID;
            });

            if (prodIt == inventory.end()) {
                cout << "Product not found!\n";
                continue;
            }

            cout << "Enter Quantity: ";
            int quantity;
            cin >> quantity;

            if (quantity > prodIt->getQuantity()) {
                cout << "Insufficient stock!\n";
                continue;
            }

            prodIt->updateQuantity(prodIt->getQuantity() - quantity);
            invoice.addItem(*prodIt, quantity);
        }

        invoice.printInvoice();
        invoices.push_back(invoice);
    }

    void listInventory() const {
        cout << "\nInventory:\n";
        cout << left << setw(10) << "ProductID" << setw(20) << "Name"
             << setw(10) << "Price" << setw(10) << "Quantity" << endl;
        cout << "----------------------------------------" << endl;
        for (const auto& product : inventory) {
            product.displayProduct();
        }
    }

    void listCustomers() const {
        cout << "\nCustomers:\n";
        cout << left << setw(10) << "CustomerID" << setw(20) << "Name"
             << setw(15) << "Contact" << endl;
        cout << "----------------------------------------" << endl;
        for (const auto& customer : customers) {
            customer.displayCustomer();
        }
    }
};

int main() {
    InvoicingSoftware app;

    app.addProduct("Laptop", 1000.00, 10);
    app.addProduct("Mouse", 25.50, 50);
    app.addProduct("Keyboard", 45.00, 30);

    app.addCustomer("John Doe", "123-456-7890");
    app.addCustomer("Jane Smith", "987-654-3210");

    int choice;

    do {
        cout << "\n1. List Inventory\n";
        cout << "2. List Customers\n";
        cout << "3. Create Invoice\n";
        cout << "4. Exit\n";
        cout << "Enter your choice: ";
        cin >> choice;

        switch (choice) {
        case 1:
            app.listInventory();
            break;
        case 2:
            app.listCustomers();
            break;
        case 3:
            app.createInvoice();
            break;
        case 4:
            cout << "Exiting...\n";
            break;
        default:
            cout << "Invalid choice!\n";
        }
    } while (choice != 4);

    return 0;
}
