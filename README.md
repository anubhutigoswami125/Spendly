# Spendly

Spendly is a browser-based personal finance tracker with an object-oriented C++ calculation engine compiled to WebAssembly.

## Features

- Dashboard showing all-time balance and current-month income, expenses and savings rate
- Income and expense transaction records with category breakdown
- Monthly category budgets and savings goals
- Manual investment, debt and subscription records
- JSON backup export and a full local-data reset
- Browser-local storage; no Spendly account or finance-server connection

## Technology

- **C++17-compatible source:** classes, encapsulation, inheritance, abstraction, virtual functions, operator overloading, STL containers and smart pointers
- **WebAssembly:** C++ module compiled with Emscripten
- **Frontend:** HTML, CSS and JavaScript
- **Deployment:** GitHub Actions and GitHub Pages

The C++ ledger handles transaction classification and totals, balance and savings-rate calculations. C++ budget calculations are also used when rendering budget status. The browser interface and local storage are handled by JavaScript. A JavaScript fallback keeps the dashboard usable if the WebAssembly module cannot load.

## Run locally

Do not rely on opening `index.html` directly when testing WebAssembly. Serve the project over HTTP:

```bash
python -m http.server 8000
```

Then open `http://localhost:8000`.

To rebuild the WebAssembly module, install and activate the [Emscripten SDK](https://emscripten.org/docs/getting_started/downloads.html), then run this command from the project root:

```bash
em++ src/spendly.cpp -O2 -s WASM=1 -s MODULARIZE=1 -s EXPORT_NAME=createSpendlyModule -s EXPORTED_FUNCTIONS='["_resetLedger","_addLedgerTransaction","_getLedgerIncome","_getLedgerExpenses","_getLedgerBalance","_getLedgerSavingsRate","_getLedgerCount","_calculateBudgetRemaining","_calculateGoalProgress","_validateAmount"]' -s EXPORTED_RUNTIME_METHODS='["cwrap"]' -o spendly.js
```

The deployment workflow runs this build automatically before publishing the site.

## Suggested manual checks

1. Add an income transaction and an expense transaction; confirm the dashboard totals, balance and savings rate.
2. Add a category budget and an expense in that category; confirm the budget progress updates.
3. Add a savings goal and verify that its name, target and saved amount are retained.
4. Delete a transaction and confirm that the dashboard recalculates.
5. Export a JSON backup and check that it contains the records you entered.
6. Use **Reset account** only after exporting; confirm that all records are cleared.

## Data and limitations

Records are stored in this browser's local storage. They are not uploaded to a Spendly account server. Data is specific to the browser profile and device; clearing browser data can remove it. Export a JSON backup regularly.

Investment, debt and subscription entries are manual records. Spendly does not connect to banks, fetch market prices, process payments or provide financial advice. Figures are for personal tracking and educational demonstration only.
