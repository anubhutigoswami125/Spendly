import createSpendlyModule from "./spendly.js";

const $ = (id) => document.getElementById(id);
const storageKey = "spendly.records.v1";
let app;
let transactionType = "expense";

const money = (value) => new Intl.NumberFormat("en-IN", {
  style: "currency", currency: "INR", minimumFractionDigits: 2
}).format(value);

function records() {
  return JSON.parse(app.getTransactionsJson());
}

function save() {
  localStorage.setItem(storageKey, app.exportData());
}

function refresh() {
  const summary = JSON.parse(app.getSummaryJson());
  $("balance").textContent = money(summary.balance);
  $("income").textContent = money(summary.income);
  $("expenses").textContent = money(summary.expenses);

  const selected = $("filter").value;
  const visible = records().filter((item) => selected === "all" || item.type === selected);
  const body = $("transaction-list");
  body.replaceChildren();
  $("record-count").textContent = records().length + (records().length === 1 ? " record" : " records");

  if (!visible.length) {
    const row = document.createElement("tr");
    const cell = document.createElement("td");
    cell.colSpan = 5;
    cell.className = "empty-state";
    cell.textContent = records().length ? "No records match this filter." : "No transactions yet. Add your first record.";
    row.append(cell);
    body.append(row);
    return;
  }

  visible.sort((a, b) => b.id - a.id).forEach((item) => {
    const row = document.createElement("tr");
    const details = document.createElement("td");
    const title = document.createElement("span");
    title.className = "record-title";
    title.textContent = item.description;
    const sub = document.createElement("span");
    sub.className = "record-sub";
    sub.textContent = item.category + (item.type === "Expense" ? " · " + item.payment : "");
    details.append(title, sub);

    const date = document.createElement("td");
    date.textContent = item.date;
    const type = document.createElement("td");
    type.innerHTML = item.type === "Income" ? '<span class="kind">Income</span>' : '<span class="kind">Expense</span>';
    const amount = document.createElement("td");
    amount.className = "amount-col " + (item.type === "Income" ? "amount-positive" : "amount-negative");
    amount.textContent = (item.type === "Income" ? "+" : "−") + money(item.amount);
    const action = document.createElement("td");
    const remove = document.createElement("button");
    remove.className = "delete-button";
    remove.type = "button";
    remove.textContent = "Delete";
    remove.setAttribute("aria-label", "Delete transaction " + item.id);
    remove.addEventListener("click", () => {
      if (window.confirm("Delete this transaction?")) {
        if (app.removeTransaction(item.id)) { save(); refresh(); }
      }
    });
    action.append(remove);
    row.append(details, date, type, amount, action);
    body.append(row);
  });
}

function setType(type) {
  transactionType = type;
  document.querySelectorAll(".type-button").forEach((button) => {
    const active = button.dataset.type === type;
    button.classList.toggle("active", active);
    button.setAttribute("aria-pressed", String(active));
  });
  $("expense-fields").hidden = type !== "expense";
  $("income-fields").hidden = type !== "income";
  $("category").required = type === "expense";
  $("payment").required = type === "expense";
  $("source").required = type === "income";
  $("submit-button").innerHTML = type === "expense" ? 'Add expense <span aria-hidden="true">→</span>' : 'Add income <span aria-hidden="true">→</span>';
  $("form-error").textContent = "";
}

function exportBackup() {
  const blob = new Blob([app.exportData()], { type: "text/plain;charset=utf-8" });
  const url = URL.createObjectURL(blob);
  const link = document.createElement("a");
  link.href = url;
  link.download = "spendly-backup.txt";
  link.click();
  URL.revokeObjectURL(url);
}

async function start() {
  $("today-date").textContent = new Intl.DateTimeFormat("en-IN", {
    day: "numeric", month: "long", year: "numeric"
  }).format(new Date());
  $("date").value = new Date(Date.now() - new Date().getTimezoneOffset() * 60000).toISOString().slice(0, 10);

  try {
    const Module = await createSpendlyModule();
    app = new Module.SpendlyApp();
    app.importData(localStorage.getItem(storageKey) || "");
    refresh();
  } catch (error) {
    $("form-error").textContent = "The C++ application module could not load. Refresh the page or check the deployment build.";
    console.error(error);
    return;
  }

  document.querySelectorAll(".type-button").forEach((button) =>
    button.addEventListener("click", () => setType(button.dataset.type)));
  $("filter").addEventListener("change", refresh);
  $("export-button").addEventListener("click", exportBackup);
  $("transaction-form").addEventListener("submit", (event) => {
    event.preventDefault();
    const amount = Number($("amount").value);
    const date = $("date").value;
    const description = $("description").value.trim();
    let success = false;

    if (transactionType === "expense") {
      success = app.addExpense(amount, date, $("category").value,
        $("payment").value, description);
    } else {
      success = app.addIncome(amount, date, $("source").value.trim(), description);
    }

    if (!success) {
      $("form-error").textContent = app.getError() || "Check the information and try again.";
      return;
    }

    save();
    event.target.reset();
    $("date").value = new Date().toISOString().slice(0, 10);
    $("form-error").textContent = "";
    refresh();
  });
}

start();
