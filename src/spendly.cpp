#include <array>
#include <cmath>
#include <memory>
#include <stdexcept>
#include <vector>
#ifdef __EMSCRIPTEN__
#include <emscripten.h>
#define SPENDLY_API EMSCRIPTEN_KEEPALIVE
#else
#define SPENDLY_API
#endif

class Money {
    double value;
public:
    explicit Money(double amount = 0.0) : value(amount) {
        if (!std::isfinite(amount)) throw std::invalid_argument("Non-finite amount");
    }
    double getValue() const { return value; }
    Money operator+(const Money& other) const { return Money(value + other.value); }
    Money operator-(const Money& other) const { return Money(value - other.value); }
};

class Transaction {
protected:
    Money amount;
public:
    explicit Transaction(double value) : amount(value) {
        if (value <= 0.0) throw std::invalid_argument("Amount must be positive");
    }
    virtual ~Transaction() = default;
    virtual Money signedAmount() const = 0;
    virtual bool isIncome() const = 0;
};
class Income final : public Transaction {
public:
    explicit Income(double value) : Transaction(value) {}
    Money signedAmount() const override { return amount; }
    bool isIncome() const override { return true; }
};
class Expense final : public Transaction {
public:
    explicit Expense(double value) : Transaction(value) {}
    Money signedAmount() const override { return Money(0.0) - amount; }
    bool isIncome() const override { return false; }
};

class Ledger {
    std::vector<std::unique_ptr<Transaction>> records;
public:
    void add(int type, double amount) {
        if (type == 1) records.push_back(std::make_unique<Income>(amount));
        else if (type == 0) records.push_back(std::make_unique<Expense>(amount));
        else throw std::invalid_argument("Unknown transaction type");
    }
    void clear() { records.clear(); }
    double incomeTotal() const {
        double total = 0.0;
        for (const auto& item : records)
            if (item->isIncome()) total = (Money(total) + item->signedAmount()).getValue();
        return total;
    }
    double expenseTotal() const {
        double total = 0.0;
        for (const auto& item : records)
            if (!item->isIncome()) total += 0.0 - item->signedAmount().getValue();
        return total;
    }
    double balance() const {
        Money total;
        for (const auto& item : records) total = total + item->signedAmount();
        return total.getValue();
    }
    int count() const { return static_cast<int>(records.size()); }
};
class Budget {
    Money limit;
public:
    explicit Budget(double amount) : limit(amount) {
        if (amount < 0.0) throw std::invalid_argument("Negative budget");
    }
    double remaining(double spent) const { return (limit - Money(spent)).getValue(); }
};
class SavingsGoal {
    Money target;
public:
    explicit SavingsGoal(double amount) : target(amount) {
        if (amount <= 0.0) throw std::invalid_argument("Invalid target");
    }
    double progress(double saved) const {
        const double percentage = saved / target.getValue() * 100.0;
        return percentage < 0.0 ? 0.0 : (percentage > 100.0 ? 100.0 : percentage);
    }
};

static std::array<Ledger, 2> ledgers;
extern "C" {
SPENDLY_API void resetLedger(int period) {
    if (period >= 0 && period < 2) ledgers[period].clear();
}
SPENDLY_API int addLedgerTransaction(int type, double amount, int period) {
    try {
        if (period < 0 || period > 1) return 0;
        ledgers[period].add(type, amount);
        return 1;
    } catch (...) { return 0; }
}
SPENDLY_API double getLedgerIncome(int period) {
    return period >= 0 && period < 2 ? ledgers[period].incomeTotal() : 0.0;
}
SPENDLY_API double getLedgerExpenses(int period) {
    return period >= 0 && period < 2 ? ledgers[period].expenseTotal() : 0.0;
}
SPENDLY_API double getLedgerBalance(int period) {
    return period >= 0 && period < 2 ? ledgers[period].balance() : 0.0;
}
SPENDLY_API double getLedgerSavingsRate(int period) {
    if (period < 0 || period > 1) return 0.0;
    const double income = ledgers[period].incomeTotal();
    return income > 0.0 ? ((income - ledgers[period].expenseTotal()) / income) * 100.0 : 0.0;
}
SPENDLY_API int getLedgerCount(int period) {
    return period >= 0 && period < 2 ? ledgers[period].count() : 0;
}
SPENDLY_API double calculateBudgetRemaining(double budgetLimit, double amountSpent) {
    try { return Budget(budgetLimit).remaining(amountSpent); }
    catch (...) { return 0.0; }
}
SPENDLY_API double calculateGoalProgress(double target, double saved) {
    try { return SavingsGoal(target).progress(saved); }
    catch (...) { return 0.0; }
}
SPENDLY_API int validateAmount(double amount) {
    return std::isfinite(amount) && amount > 0.0 ? 1 : 0;
}
}
