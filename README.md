# Spendly
A beginner-friendly finance tracker with a C++ calculation engine compiled to WebAssembly.

## Features
- Dashboard: balance, income, expenses and savings rate
- Transaction tracking and category spending breakdown
- Monthly budgets and savings goals
- Manual investment, debt and subscription records
- JSON backup export
- Browser-local storage; no account or server required

## Technology
- C++ compiled to WebAssembly with Emscripten
- HTML, CSS and JavaScript for the interface and browser storage
- GitHub Actions and GitHub Pages for deployment

## Local use
Open index.html in a browser. The JavaScript calculation fallback works without a WebAssembly build. To compile the C++ module, install Emscripten and run:

    emcc src/spendly.cpp -O2 -s WASM=1 -s MODULARIZE=1 -s EXPORT_NAME=createSpendlyModule -s EXPORTED_FUNCTIONS='["_calculateBalance","_calculateSavingsRate","_calculateBudgetRemaining"]' -s EXPORTED_RUNTIME_METHODS='["cwrap"]' -o spendly.js

## Data
Records stay in this browser's local storage and are not uploaded. Export a JSON backup regularly. Clearing browser storage removes records. Investment entries are manual notes, not live market prices or investment advice.
