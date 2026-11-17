#include <cmath>
#include <iomanip>
#include <sstream>
#include <string>
#include <vector>
#include <emscripten/bind.h>

struct Record {
    int id;
    std::string type;
    double amount;
    std::string date;
    std::string category;
    std::string payment;
    std::string description;
};

class SpendlyApp {
    std::vector<Record> records;
    int nextId = 1;
    std::string error;

    static std::string escapeJson(const std::string& value) {
        std::string out;
        for (char c : value) {
            if (c == '"' || c == '\\') { out += '\\'; out += c; }
            else if (c == '\n') out += "\\n";
            else if (c == '\r') out += "\\r";
            else if (c == '\t') out += "\\t";
            else out += c;
        }
        return out;
    }

    static std::vector<std::string> split(const std::string& line) {
        std::vector<std::string> parts;
        std::stringstream stream(line);
        std::string part;
        while (std::getline(stream, part, '|')) parts.push_back(part);
        return parts;
    }

    bool valid(double amount, const std::string& date,
               const std::string& category, const std::string& description) {
        if (!std::isfinite(amount) || amount <= 0) error = "Enter an amount greater than zero.";
        else if (date.empty() || category.empty() || description.empty()) error = "Complete all required fields.";
        else if (date.find('|') != std::string::npos || category.find('|') != std::string::npos ||
                 description.find('|') != std::string::npos) error = "The | character is not allowed.";
        else { error.clear(); return true; }
        return false;
    }

public:
    bool addExpense(double amount, const std::string& date, const std::string& category,
                    const std::string& payment, const std::string& description) {
        if (!valid(amount, date, category, description) || payment.empty()) return false;
        records.push_back({nextId++, "Expense", amount, date, category, payment, description});
        return true;
    }

    bool addIncome(double amount, const std::string& date, const std::string& source,
                   const std::string& description) {
        if (!valid(amount, date, source, description)) return false;
        records.push_back({nextId++, "Income", amount, date, source, "-", description});
        return true;
    }

    bool removeTransaction(int id) {
        for (auto it = records.begin(); it != records.end(); ++it) {
            if (it->id == id) { records.erase(it); error.clear(); return true; }
        }
        error = "Transaction not found.";
        return false;
    }

    std::string getError() const { return error; }

    std::string getTransactionsJson() const {
        std::ostringstream out;
        out << "[";
        for (std::size_t i = 0; i < records.size(); ++i) {
            const auto& r = records[i];
            if (i) out << ",";
            out << "{\"id\":" << r.id << ",\"type\":\"" << escapeJson(r.type)
                << "\",\"amount\":" << std::setprecision(15) << r.amount
                << ",\"date\":\"" << escapeJson(r.date)
                << "\",\"category\":\"" << escapeJson(r.category)
                << "\",\"payment\":\"" << escapeJson(r.payment)
                << "\",\"description\":\"" << escapeJson(r.description) << "\"}";
        }
        out << "]";
        return out.str();
    }

    std::string getSummaryJson() const {
        double income = 0, expenses = 0;
        for (const auto& r : records) {
            if (r.type == "Income") income += r.amount;
            else expenses += r.amount;
        }
        std::ostringstream out;
        out << std::fixed << std::setprecision(2)
            << "{\"income\":" << income << ",\"expenses\":" << expenses
            << ",\"balance\":" << income - expenses << "}";
        return out.str();
    }

    std::string exportData() const {
        std::ostringstream out;
        for (const auto& r : records) {
            out << r.type << "|" << r.id << "|" << std::setprecision(15) << r.amount
                << "|" << r.date << "|" << r.category << "|" << r.payment
                << "|" << r.description << "\n";
        }
        return out.str();
    }

    void importData(const std::string& data) {
        records.clear();
        nextId = 1;
        std::stringstream input(data);
        std::string line;
        while (std::getline(input, line)) {
            auto p = split(line);
            if (p.size() != 7) continue;
            try {
                int id = std::stoi(p[1]);
                double amount = std::stod(p[2]);
                if (id <= 0 || !std::isfinite(amount) || amount <= 0) continue;
                if (p[0] != "Expense" && p[0] != "Income") continue;
                records.push_back({id, p[0], amount, p[3], p[4], p[5], p[6]});
                if (id >= nextId) nextId = id + 1;
            } catch (...) { /* Ignore damaged records and continue. */ }
        }
    }
};

EMSCRIPTEN_BINDINGS(spendly_module) {
    emscripten::class_<SpendlyApp>("SpendlyApp")
        .constructor<>()
        .function("addExpense", &SpendlyApp::addExpense)
        .function("addIncome", &SpendlyApp::addIncome)
        .function("removeTransaction", &SpendlyApp::removeTransaction)
        .function("getError", &SpendlyApp::getError)
        .function("getTransactionsJson", &SpendlyApp::getTransactionsJson)
        .function("getSummaryJson", &SpendlyApp::getSummaryJson)
        .function("exportData", &SpendlyApp::exportData)
        .function("importData", &SpendlyApp::importData);
}
