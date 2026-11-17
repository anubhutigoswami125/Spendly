# Spendly

**Spendly** is a personal expense tracker built as a CSE236 Object-Oriented Programming project. Its transaction logic is written in C++17 and compiled to WebAssembly for use by the browser interface.

## Website features

- Add income and expenses with dates, categories or sources, payment methods and descriptions.
- View the transaction list and filter by income or expenses.
- See total income, total expenses and available balance.
- Delete individual transactions.
- Keep records in browser local storage on the current device.
- Export a text backup of recorded transactions.
- Responsive layout for desktop and mobile.
- Privacy Policy and Terms and Conditions pages describing this student-project version.

## C++ implementation

- `spendly.cpp`: standalone C++17 console application for the academic demonstration.
- `web_core.cpp`: C++ transaction engine compiled to WebAssembly and called by the website.
- `Transaction`, `Expense`, and `Income`: demonstrate classes, inheritance and polymorphic design in the console application.
- `ExpenseManager`: demonstrates encapsulation, vectors, transaction operations, calculations and file handling.
- The browser interface handles presentation and browser storage; transaction validation, record mutation and financial calculations run in the C++ WebAssembly module.

## Run the console application

Requirements: GCC/Clang/MSVC with C++17. CMake 3.10 or later is optional.

```bash
g++ -std=c++17 -Wall -Wextra -pedantic spendly.cpp -o spendly
./spendly
```

Or use CMake:

```bash
cmake -S . -B build
cmake --build build
```

The console application stores its records in `spendly_data.txt` in the working directory.

## Build and publish the website

The GitHub Actions workflow in `.github/workflows/deploy.yml` compiles `web_core.cpp` with Emscripten, assembles the static site and deploys it to GitHub Pages when changes are pushed to `main`.

One-time repository setup:

1. Open **Settings → Pages** in the GitHub repository.
2. Under **Build and deployment**, select **GitHub Actions** as the source.
3. Open the **Actions** tab and check the **Build and deploy Spendly** workflow.
4. After a successful deployment, use the Pages URL shown in the workflow deployment summary.

To preview locally, compile the WebAssembly module with Emscripten:

```bash
em++ web_core.cpp -std=c++17 -O2 --bind -s MODULARIZE=1 -s EXPORT_ES6=1 -s ENVIRONMENT=web -s ALLOW_MEMORY_GROWTH=1 -o spendly.js
```

Then serve the repository directory with a local HTTP server. Opening `index.html` directly as a `file://` URL will not load the WebAssembly module reliably.

## Data and privacy notes

The website stores transaction records in browser local storage. Data does not automatically sync between browsers or devices. Clearing browser storage can remove records; use **Export records** to keep a backup. Do not enter bank credentials, card security codes or passwords.

The included Privacy Policy and Terms and Conditions are student-project drafts describing the current implementation. They are not legal advice or a substitute for legal review before any public commercial launch.

## Current scope and limitations

This is an academic project, not a bank, payment processor or financial advisory service. It does not connect to bank accounts. It currently provides manual record keeping, basic totals, filtering, deletion and backup export. Authentication, cloud synchronization, editing existing records, budgets and advanced reports are not implemented.

## Project structure

- `spendly.cpp` — standalone C++ console application
- `web_core.cpp` — C++ WebAssembly transaction engine
- `index.html`, `styles.css`, `app.js` — website interface
- `privacy.html`, `terms.html` — policy and terms drafts
- `.github/workflows/deploy.yml` — automated website build and deployment
