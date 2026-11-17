#include <cmath>
#include <fstream>
#include <iomanip>
#include <iostream>
#include <limits>
#include <sstream>
#include <string>
#include <vector>

// Base class: common information shared by every transaction.
class Transaction {
protected:
    int id;
    double amount;
    std::string date;
    std::string description;

public:
    Transaction(int transactionId, double transactionAmount,
                const std::string& transactionDate,
                const std::string& transactionDescription)
        : id(transactionId), amount(transactionAmount), date(transactionDate),
          description(transactionDescription) {}

    virtual ~Transaction() = default;

    int getId() const { return id; }
    double getAmount() const { return amount; }
    const std::string& getDate() const { return date; }
    const std::string& getDescription() const { return description; }

    virtual void display() const = 0;
};

// Expense inherits common transaction details from Transaction.
class Expense : public Transaction {
private:
    std::string category;
    std::string paymentMethod;

public:
    Expense(int id, double amount, const std::string& date,
            const std::string& description, const std::string& category,
            const std::string& paymentMethod)
        : Transaction(id, amount, date, description), category(category),
          paymentMethod(paymentMethod) {}

    void display() const override {
        std::cout << std::left << std::setw(5) << id << std::setw(13) << date
                  << std::setw(12) << "Expense" << std::right << std::setw(10)
                  << std::fixed << std::setprecision(2) << amount << "  "
                  << std::left << std::setw(14) << category << std::setw(12)
                  << paymentMethod << description << '\n';
    }

    const std::string& getCategory() const { return category; }
    const std::string& getPaymentMethod() const { return paymentMethod; }
};

// Income also inherits common transaction details from Transaction.
class Income : public Transaction {
private:
    std::string source;

public:
    Income(int id, double amount, const std::string& date,
           const std::string& description, const std::string& source)
        : Transaction(id, amount, date, description), source(source) {}

    void display() const override {
        std::cout << std::left << std::setw(5) << id << std::setw(13) << date
                  << std::setw(12) << "Income" << std::right << std::setw(10)
                  << std::fixed << std::setprecision(2) << amount << "  "
                  << std::left << std::setw(14) << source << std::setw(12)
                  << "-" << description << '\n';
    }

    const std::string& getSource() const { return source; }
};

// ExpenseManager handles records, calculations, and file operations.
class ExpenseManager {
private:
    std::vector<Expense> expenses;
    std::vector<Income> incomes;
    int nextId = 1;
    const std::string dataFile = "spendly_data.txt";

public:
    ExpenseManager() { loadFromFile(); }

    void addExpense(double amount, const std::string& date,
                    const std::string& description, const std::string& category,
                    const std::string& paymentMethod) {
        expenses.emplace_back(nextId++, amount, date, description, category, paymentMethod);
        std::cout << "Expense added.\n";
    }

    void addIncome(double amount, const std::string& date,
                   const std::string& description, const std::string& source) {
        incomes.emplace_back(nextId++, amount, date, description, source);
        std::cout << "Income added.\n";
    }

    void showTransactions() const {
        if (expenses.empty() && incomes.empty()) {
            std::cout << "No transactions recorded yet.\n";
            return;
        }

        std::cout << "\nID   Date         Type              Amount  Category/Source  Payment     Description\n";
        std::cout << "-------------------------------------------------------------------------------\n";
        for (const Expense& expense : expenses) expense.display();
        for (const Income& income : incomes) income.display();
    }

    void showSummary() const {
        double totalIncome = 0.0;
        double totalExpenses = 0.0;

        for (const Income& income : incomes) totalIncome += income.getAmount();
        for (const Expense& expense : expenses) totalExpenses += expense.getAmount();

        std::cout << "\n========== SPENDLY SUMMARY ==========\n" << std::fixed << std::setprecision(2);
        std::cout << "Total income:   Rs. " << totalIncome << '\n';
        std::cout << "Total expenses: Rs. " << totalExpenses << '\n';
        std::cout << "Balance:        Rs. " << totalIncome - totalExpenses << '\n';
        std::cout << "=====================================\n";
    }

    bool deleteTransaction(int id) {
        for (auto it = expenses.begin(); it != expenses.end(); ++it) {
            if (it->getId() == id) {
                expenses.erase(it);
                return true;
            }
        }

        for (auto it = incomes.begin(); it != incomes.end(); ++it) {
            if (it->getId() == id) {
                incomes.erase(it);
                return true;
            }
        }

        return false;
    }

    bool saveToFile() const {
        std::ofstream file(dataFile);
        if (!file) return false;

        // Each record is stored on one line in a simple pipe-separated format.
        for (const Expense& expense : expenses) {
            file << "E|" << expense.getId() << '|' << std::setprecision(15)
                 << expense.getAmount() << '|' << expense.getDate() << '|'
                 << expense.getCategory() << '|' << expense.getPaymentMethod() << '|'
                 << expense.getDescription() << '\n';
        }

        for (const Income& income : incomes) {
            file << "I|" << income.getId() << '|' << std::setprecision(15)
                 << income.getAmount() << '|' << income.getDate() << '|'
                 << income.getSource() << '|' << income.getDescription() << '\n';
        }

        return file.good();
    }

    bool loadFromFile() {
        std::ifstream file(dataFile);
        if (!file) return true; // A missing file is normal on the first run.

        expenses.clear();
        incomes.clear();
        nextId = 1;

        std::string line;
        while (std::getline(file, line)) {
            std::stringstream stream(line);
            std::string type, idText, amountText, date, fieldOne, fieldTwo, description;

            if (!std::getline(stream, type, '|') ||
                !std::getline(stream, idText, '|') ||
                !std::getline(stream, amountText, '|') ||
                !std::getline(stream, date, '|') ||
                !std::getline(stream, fieldOne, '|')) {
                continue;
            }

            try {
                int recordId = std::stoi(idText);
                double recordAmount = std::stod(amountText);

                if (recordId <= 0 || !std::isfinite(recordAmount) || recordAmount <= 0.0) {
                    continue;
                }

                if (type == "E") {
                    if (!std::getline(stream, fieldTwo, '|') ||
                        !std::getline(stream, description)) {
                        continue;
                    }
                    expenses.emplace_back(recordId, recordAmount, date, description,
                                           fieldOne, fieldTwo);
                } else if (type == "I") {
                    if (!std::getline(stream, description)) continue;
                    incomes.emplace_back(recordId, recordAmount, date, description, fieldOne);
                } else {
                    continue;
                }

                if (recordId >= nextId) nextId = recordId + 1;
            } catch (const std::exception&) {
                // Ignore a malformed line and continue loading valid records.
            }
        }

        return true;
    }
};

namespace {
std::string readText(const std::string& prompt) {
    std::string value;

    while (true) {
        std::cout << prompt;
        std::getline(std::cin, value);

        if (std::cin.fail()) return "";
        if (value.empty()) {
            std::cout << "This field cannot be empty. Try again.\n";
        } else if (value.find('|') != std::string::npos) {
            std::cout << "Please do not use the | character. Try again.\n";
        } else {
            return value;
        }
    }
}

double readAmount() {
    double amount;

    while (true) {
        std::cout << "Amount (Rs.): ";

        if (std::cin >> amount && std::isfinite(amount) && amount > 0.0) {
            std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
            return amount;
        }

        std::cout << "Enter a valid amount greater than zero.\n";
        std::cin.clear();
        std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
    }
}

int readInteger(const std::string& prompt) {
    int value;

    while (true) {
        std::cout << prompt;

        if (std::cin >> value) {
            std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
            return value;
        }

        std::cout << "Enter a whole number.\n";
        std::cin.clear();
        std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
    }
}

void showMenu() {
    std::cout << "\n========== SPENDLY ==========\n"
              << "1. Add expense\n"
              << "2. Add income\n"
              << "3. View transactions\n"
              << "4. View financial summary\n"
              << "5. Delete a transaction\n"
              << "6. Save and exit\n"
              << "=============================\n";
}
} // namespace

int main() {
    ExpenseManager manager;
    std::cout << "Welcome to Spendly. Your personal expense tracker.\n";

    bool running = true;

    while (running) {
        showMenu();
        int choice = readInteger("Choose an option: ");

        switch (choice) {
        case 1: {
            double amount = readAmount();
            std::string date = readText("Date (DD-MM-YYYY): ");
            std::string category = readText("Category (Food/Travel/Education/Other): ");
            std::string payment = readText("Payment method (Cash/UPI/Bank): ");
            std::string description = readText("Description: ");
            manager.addExpense(amount, date, description, category, payment);
            break;
        }
        case 2: {
            double amount = readAmount();
            std::string date = readText("Date (DD-MM-YYYY): ");
            std::string source = readText("Income source: ");
            std::string description = readText("Description: ");
            manager.addIncome(amount, date, description, source);
            break;
        }
        case 3:
            manager.showTransactions();
            break;
        case 4:
            manager.showSummary();
            break;
        case 5: {
            int id = readInteger("Enter transaction ID to delete: ");
            if (manager.deleteTransaction(id)) {
                std::cout << "Transaction deleted.\n";
            } else {
                std::cout << "No transaction found with that ID.\n";
            }
            break;
        }
        case 6:
            if (manager.saveToFile()) {
                std::cout << "Data saved. Goodbye.\n";
                running = false;
            } else {
                std::cout << "Could not save data. Check folder permissions and try again.\n";
            }
            break;
        default:
            std::cout << "Please choose an option from 1 to 6.\n";
        }
    }

    return 0;
}
