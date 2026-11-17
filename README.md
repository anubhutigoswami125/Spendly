# Spendly

Spendly is a beginner-friendly, console-based personal finance tracker written primarily in C++. It is being developed as a CSE236 Object-Oriented Programming project and as the foundation for a personal expense-tracking application.

## Current C++ features

- Add expenses with date, category, payment method, and description.
- Add income with date, source, and description.
- View recorded transactions.
- Calculate total income, total expenses, and balance.
- Delete a transaction by its ID.
- Save records to a local text file and load them on the next run.
- Validate positive numeric amounts and protect the text-file delimiter.

## C++ concepts demonstrated

- **Classes and objects:** `Transaction`, `Expense`, `Income`, and `ExpenseManager`.
- **Inheritance:** `Expense` and `Income` derive from the abstract `Transaction` base class.
- **File handling:** `ExpenseManager` reads and writes `spendly_data.txt` using file streams.
- **Encapsulation:** transaction data is stored in class members and accessed through member functions.

## Requirements

- A C++17-compatible compiler such as GCC, Clang, or MSVC.
- CMake 3.10 or later (optional if compiling manually).

## Build with CMake

    cmake -S . -B build
    cmake --build build

Run the generated `spendly` executable from the project directory. The data file is created in the current working directory when you choose **Save and exit**.

## Build manually with GCC

    g++ -std=c++17 -Wall -Wextra -pedantic spendly.cpp -o spendly

## Data storage

The application stores records in a local text file named `spendly_data.txt`. It uses a simple pipe-separated format intended for this learning project. Do not enter the `|` character in text fields.

## Project status

This is the first C++ backend milestone. A graphical/web frontend, transaction editing, date validation, budgets, reports, and additional safeguards are planned for later iterations. The project is not presented as a launched financial service. A public launch will not happen until the Privacy Policy and Terms and Conditions pages are implemented and reviewed.
