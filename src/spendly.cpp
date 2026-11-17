#include <emscripten.h>
// The browser calls these C++ functions through WebAssembly.
extern "C" {
EMSCRIPTEN_KEEPALIVE double calculateBalance(double income,double expenses){return income-expenses;}
EMSCRIPTEN_KEEPALIVE double calculateSavingsRate(double income,double expenses){if(income<=0.0)return 0.0;return ((income-expenses)/income)*100.0;}
EMSCRIPTEN_KEEPALIVE double calculateBudgetRemaining(double budgetLimit,double amountSpent){return budgetLimit-amountSpent;}
}