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
class DoublyLinkedListIterator {
public:
    DoublyLinkedListIterator(Node<T>* node) : current(node) {}

    bool operator!=(const DoublyLinkedListIterator& other) const {
        return current != other.current;
    }

    T& operator*() const {
        return current->data;
    }

    DoublyLinkedListIterator& operator++() {
        current = current->next;
        return *this;
    }

private:
    Node<T>* current;
};



template <typename T>
class DoublyLinkedList {

private:
    Node<T>* head;
    Node<T>* tail;
    int size;

public:
    DoublyLinkedList() : head(nullptr), tail(nullptr), size(0) {}

    DoublyLinkedListIterator<T> begin() const {
        return DoublyLinkedListIterator<T>(head);
    }

    DoublyLinkedListIterator<T> end() const {
        return DoublyLinkedListIterator<T>(nullptr);
    }

    void push_back(T value) {
        Node<T>* newNode = new Node<T>(value);
        if (size == 0) {
            head = tail = newNode;
        }
        else {
            tail->next = newNode;
            newNode->prev = tail;
            tail = newNode;
        }
        size++;
    }

    void erase() {
        Node<T>* currentNode = head;
        while (currentNode != nullptr) {
            Node<T>* nextNode = currentNode->next;
            delete currentNode;
            currentNode = nextNode;
        }
        head = tail = nullptr;
        size = 0;
    }

    void erase(int a, int b) {
        if (a < 0 || b >= size || a > b) {
            throw std::out_of_range("Invalid range!");
        }
        Node<T>* prevNode;

        Node<T>* nodeToDelete = nullptr;
        if (a == 0) {
            nodeToDelete = head;
            head = nodeToDelete->next;
            if (head) {
                head->prev = nullptr;
            }
        }
        else {
            prevNode = head;
            for (int i = 0; i < a - 1; i++) {
                prevNode = prevNode->next;
            }
            nodeToDelete = prevNode->next;
            prevNode->next = nodeToDelete->next;
            if (nodeToDelete->next) {
                nodeToDelete->next->prev = prevNode;
            }
        }

        for (int i = a; i <= b; i++) {
            Node<T>* nextNode = nodeToDelete->next;
            delete nodeToDelete;
            nodeToDelete = nextNode;
            size--;
        }

        if (a == 0) {
            if (size == 0) {
                tail = nullptr;
            }
        }
        else if (nodeToDelete == nullptr) {
            tail = prevNode;
        }
    }

    int getSize() const {
        return size;
    }

    void printList() const {
        Node<T>* currentNode = head;
        while (currentNode != nullptr) {
            std::cout << currentNode->data << " ";
            currentNode = currentNode->next;
        }
        std::cout << std::endl;
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

    int getPrice() const {
        return price;
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
            total = 0;
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
        total += item.getPrice();
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
        items.erase();
    }

    void eraseRange(int a, int b) {
        items.erase(a,b);
    }

};


int main() {

    shoppingList list("goods.txt");

    std::cout << "\nHello, this is an shopping list app.\n"
                 "Here you can add items to your shopping list\n"
                 "There are such queries avalible:\n\n"
                 "1. append (syntax: item_name NEWLINE item_price NEWLINE item_quantity NEWLINE)\n"
                 "2. show\n"
                 "3. erase\n"
                 "4. save (necessary if you want to save a shopping list in a file)\n"
                 "5. get total\n"
                 "6. erase range (syntax: lower_index WHITESPACE higher_index)\n"
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
            std::cout << '\n' << list;

        } else if (n == 3) {
            list.eraseList();

        } else if (n == 4) {
            list.saveList();

        } else if (n == 5) {
            std::cout << "Total: " << list.getTotal() << '\n';

        } else if (n == 6) {
            int a, b;
            std::cin >> a >> b;
            list.eraseRange(a,b);

        } else if (n == -1) {
            isShopping = false;
        }
    }

}
