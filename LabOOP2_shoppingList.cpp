#include <iostream>
#include <list>
#include <string>
#include <fstream>
#include <iomanip>
#include <utility>

template <typename T>
class Node {

public:
    T data;
    Node<T>* next = nullptr;
    Node<T>* prev = nullptr;

    Node(T dat) : data(dat) {}
};

template <typename T>
class DoublyLinkedList {

private:
    Node<T>* head;
    size_t size = 0;

public:
    DoublyLinkedList() {}

    void push_back(T value) {
        Node<T>* new_node = new Node<T>(value);
        if (head == nullptr) {
            head = new_node;
        } else {
            Node<T>* current = head;
            while (current->next != nullptr) {
                current = current->next;
            }
            current->next = new_node;
            new_node->prev = current;
        }
        size++;
    }

    void erase(size_t l=0, size_t r=0) {
        if (l == 0 && r == 0) {
            Node<T>* current = head;
            while (current != nullptr) {
                Node<T>* next_node = current->next;
                delete current;
                current = next_node;
            }
            head = nullptr;
            size = 0;
        } else {
            if (l > r) {
                std::swap(l, r);
            }
            Node<T>* current = head;
            for (size_t i = 1; i < l && current != nullptr; i++) {
                current = current->next;
            }
            Node<T>* start_node = current;
            for (size_t i = l; i <= r && current != nullptr; i++) {
                current = current->next;
            }
            Node<T>* end_node = current;
            if (start_node != nullptr) {
                start_node->next = end_node;
            } else {
                head = end_node;
            }
            if (end_node != nullptr) {
                end_node->prev = start_node;
            }
            current = start_node;
            while (current != nullptr) {
                Node<T>* next_node = current->next;
                delete current;
                current = next_node;
            }
            size -= (r - l + 1);
        }
    }

    class Iterator {

    private:
        Node<T>* current;

    public:
        Iterator(Node<T>* start) : current(start) {};

        Iterator& operator++() {
            current = current->next;
            return *this;
        }

        Iterator& operator--() {
            current = current->prev;
            return *this;
        }

        bool operator!=(const Iterator& other) const {
            return current != other.current;
        }

        bool operator==(const Iterator& other) const {
            return current == other.current;
        }

        T& operator*() {
            return current->data;
        }
    };

    Iterator begin() const {
        return Iterator(head);
    }

    Iterator end() const {
        return Iterator(nullptr);
    }
};





class shoppingItem {

private:
    std::string label;
    int price;
    int amount;

public:

    shoppingItem(std::string _name, int _price, int _amount) {
        label = std::move(_name);
        price = _price;
        amount = _amount;
    }

    friend std::ostream &operator<<(std::ostream &os, const shoppingItem &item) {
        os << item.label << '\n' << item.price << '\n' << item.amount << '\n';
        return os;
    }

    std::string fancyFormat(){
        return label + " price: " + std::to_string(price) + " quantity: " + std::to_string(amount);
    }
};

class shoppingList {

private:

    DoublyLinkedList<shoppingItem> items;
    std::string filename;
    int total = 0;

public:

    shoppingList(const std::string& name) {
        filename = name;
        std::fstream file;
        file.open(filename, std::ios::in);
        if (file.is_open()) {


            std::string label;
            int price;
            int amount;

            while (std::getline(file >> std::ws, label)){
                file >> price >> amount;
                items.push_back(shoppingItem(label, price, amount));

                total += price * amount;
            }

            file.close();

        } else {
            file.open(filename,std::ios::out);
            file.close();
        }
    }

    void saveList() {

        std::fstream file;
        file.open(filename,std::ios::out); /// override all data

        for (const auto& i : items) {
            file << i;
        }
        file.close();
    }

    void appendItem(shoppingItem item) {
        items.push_back(item);
    }

    friend std::ostream &operator<<(std::ostream &os, const shoppingList &list) {
        auto it = list.items.begin();
        int i = 1;
        while (it != list.items.end()){

            os << i << ". " << *it;
            ++i;
            ++it;
        }
        return os;
    }

    int getTotal() const {
        return total;
    }

    void eraseList() {
        std::fstream file;
        file.open(filename, std::ios::out);
        file << '\n';
        file.close();
//        items.erase()
    }

};


int main() {

    shoppingList list("goods.txt");

    std::cout << "\nHello, this is an shopping list app.\n"
                 "Here you can add items to your shopping list\n"
                 "There are such queries avalible:\n\n"
                 "1. append (syntaxis: item_name NEWLINE item_price NEWLINE item_quantity NEWLINE)\n"
                 "2. show\n"
                 "3. erase\n"
                 "4. save (necessary if you want to save a shopping list in a file)\n"
                 "Type in a number of a query you want to execute and hit ENTER\n"
                 "To exit type -1 and hit ENTER\n";

    bool isShopping = true;
    while (isShopping) {
        int n;
        std::cin >> n;

        if (n == 1) {
            std::string name;
            int price;
            int quantity;

            std::getline(std::cin >> std::ws, name);
            std::cin >> price >> quantity;

            list.appendItem(shoppingItem(name,price,quantity));

        } else if (n == 2) {
            std::cout << list;

        } else if (n == 3) {
            list.eraseList();

        } else if (n == 4) {
            list.saveList();

        } else if (n == -1) {
            isShopping = false;
        }
    }

}
