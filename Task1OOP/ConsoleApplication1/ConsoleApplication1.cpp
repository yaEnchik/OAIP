#include <iostream>
#include <vector>
#include <fstream>
#include <sstream>
#include <memory>
#include <algorithm>

// Абстрактный класс Flower
class Flower {
public:
    Flower(const std::string& name, double price, int freshness, int stemLength)
        : name(name), price(price), freshness(freshness), stemLength(stemLength) {}

    virtual ~Flower() = default;

    double getPrice() const { return price; }
    int getFreshness() const { return freshness; }
    int getStemLength() const { return stemLength; }

    virtual void displayInfo() const = 0;

protected:
    std::string name;
    double price;
    int freshness;
    int stemLength;
};

// Класс Rose, наследующий от Flower
class Rose : public Flower {
public:
    Rose(double price, int freshness, int stemLength)
        : Flower("Роза", price, freshness, stemLength) {}

    void displayInfo() const override {
        std::cout << "Роза - Цена: " << price << ", Свежесть: " << freshness << ", Длина стебля: " << stemLength << " см" << std::endl;
    }
};

// Класс Tulip, наследующий от Flower
class Tulip : public Flower {
public:
    Tulip(double price, int freshness, int stemLength)
        : Flower("Тюльпан", price, freshness, stemLength) {}

    void displayInfo() const override {
        std::cout << "Тюльпан - Цена: " << price << ", Свежесть: " << freshness << ", Длина стебля: " << stemLength << " см" << std::endl;
    }
};

// Класс Accessory
class Accessory {
public:
    Accessory(const std::string& name, double price)
        : name(name), price(price) {}

    double getPrice() const { return price; }

    void displayInfo() const {
        std::cout << "Аксессуар: " << name << ", Цена: " << price << std::endl;
    }

private:
    std::string name;
    double price;
};

// Класс Bouquet
class Bouquet {
public:
    void addFlower(std::shared_ptr<Flower> flower) {
        flowers.push_back(flower);
    }

    void addAccessory(const Accessory& accessory) {
        accessories.push_back(accessory);
    }

    double getTotalPrice() const {
        double total = 0.0;
        for (const auto& flower : flowers) {
            total += flower->getPrice();
        }
        for (const auto& accessory : accessories) {
            total += accessory.getPrice();
        }
        return total;
    }

    void sortFlowersByFreshness() {
        std::sort(flowers.begin(), flowers.end(), [](const std::shared_ptr<Flower>& a, const std::shared_ptr<Flower>& b) {
            return a->getFreshness() > b->getFreshness();
            });
    }

    std::shared_ptr<Flower> findFlowerByStemLength(int minLength, int maxLength) const {
        for (const auto& flower : flowers) {
            if (flower->getStemLength() >= minLength && flower->getStemLength() <= maxLength) {
                return flower;
            }
        }
        return nullptr;
    }

    void displayBouquet() const {
        std::cout << "Букет:" << std::endl;
        for (const auto& flower : flowers) {
            flower->displayInfo();
        }
        for (const auto& accessory : accessories) {
            accessory.displayInfo();
        }
        std::cout << "Общая стоимость: " << getTotalPrice() << std::endl;
    }

private:
    std::vector<std::shared_ptr<Flower>> flowers;
    std::vector<Accessory> accessories;
};

// Функция для инициализации цветов и аксессуаров из файла
void initializeFromFile(const std::string& filename, Bouquet& bouquet) {
    std::ifstream file(filename);
    if (!file.is_open()) {
        std::cerr << "Невозможно открыть файл " << filename << std::endl;
        return;
    }

    std::string line;
    while (std::getline(file, line)) {
        std::istringstream iss(line);
        std::string type;
        iss >> type;
        if (type == "Роза") {
            double price;
            int freshness, stemLength;
            iss >> price >> freshness >> stemLength;
            bouquet.addFlower(std::make_shared<Rose>(price, freshness, stemLength));
        }
        else if (type == "Тюльпан") {
            double price;
            int freshness, stemLength;
            iss >> price >> freshness >> stemLength;
            bouquet.addFlower(std::make_shared<Tulip>(price, freshness, stemLength));
        }
        else if (type == "Аксессуар") {
            std::string name;
            double price;
            iss >> name >> price;
            bouquet.addAccessory(Accessory(name, price));
        }
    }

    file.close();
}

int main() {
    setlocale(LC_ALL,"Russian");
    Bouquet bouquet;

    // Инициализация из файла
    initializeFromFile("initialization.txt", bouquet);

    // Показать букет
    bouquet.displayBouquet();

    // Сортировка по свежести и показ результата
    bouquet.sortFlowersByFreshness();
    std::cout << "\nСортировка по свежести:" << std::endl;
    bouquet.displayBouquet();

    // Поиск цветка по длине стебля
    int minLength = 20, maxLength = 40;
    auto flower = bouquet.findFlowerByStemLength(minLength, maxLength);
    if (flower) {
        std::cout << "\nЦветок с длиной стержня в диапазоне " << minLength << " и " << maxLength << " см:" << std::endl;
        flower->displayInfo();
    }
    else {
        std::cout << "\nНе найдено цветов с длиной стебля между " << minLength << " и " << maxLength << " см." << std::endl;
    }
    system("pause");
    return 0;
}