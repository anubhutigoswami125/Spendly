(() => {
  const view = document.getElementById("pageView");
  const crumb = document.getElementById("crumb");
  const eyebrow = document.getElementById("eyebrow");
  const title = document.getElementById("pageTitle");
  const subtitle = document.getElementById("pageSubtitle");
  const month = document.querySelector(".month-select");

  const panels = {
    credits: {
      label: "CREDITS & ACKNOWLEDGEMENTS",
      title: "The people and tools behind Spendly.",
      subtitle: "A small independent project built to make everyday money tracking clearer.",
      body: `
        <section class="page-panel legal-card">
          <p><strong>Created and developed by</strong></p>
          <h2>Anubhuti Goswami</h2>
          <p>Concept, product direction, interface design and development.</p>
        </section>
        <section class="page-panel legal-card">
          <h2>Built with</h2>
          <ul><li>HTML, CSS and JavaScript for the interface and interactions.</li><li>C++ compiled to WebAssembly for the core financial calculations, with a JavaScript fallback.</li><li>Browser local storage for saving your workspace on this device.</li></ul>
        </section>
        <section class="page-panel legal-card">
          <h2>Type</h2>
          <p>Bowlby One SC and Space Grotesk, served through Google Fonts.</p>
        </section>
        <p class="legal-note">Spendly is an independent student project and is not affiliated with a bank, payment provider or financial institution.</p>`
    },
    privacy: {
      label: "PRIVACY POLICY",
      title: "Your data. Your browser. Your call.",
      subtitle: "Last updated: 18 November 2026",
      body: `
        <section class="page-panel legal-card"><h2>1. What Spendly stores</h2><p>Spendly stores the information you enter, such as transactions, budgets, savings goals, investments, debts and subscriptions. This information is saved in your browser's local storage on the device and browser you use.</p></section>
        <section class="page-panel legal-card"><h2>2. Where your data goes</h2><p>Spendly is designed as a local, browser-based tracker. Your financial entries are not sent to a Spendly account server because Spendly does not provide account sign-in or cloud synchronisation. Anyone with access to your browser profile or device may be able to access locally stored data.</p></section>
        <section class="page-panel legal-card"><h2>3. Third-party services</h2><p>The interface loads fonts from Google Fonts. When those fonts are requested, your browser connects to Google's font-serving infrastructure, which may receive standard connection information such as your IP address. Spendly does not intentionally include advertising or analytics trackers.</p></section>
        <section class="page-panel legal-card"><h2>4. Your choices and control</h2><p>You can export a JSON backup using the export control. You can remove your entries using Reset account, or clear this site's data through your browser settings. Resetting or clearing browser data is permanent unless you have a backup.</p></section>
        <section class="page-panel legal-card"><h2>5. Children's privacy and changes</h2><p>Spendly is a general-purpose personal project and is not designed to collect information from children. This policy may be updated as the project changes. The latest version will be shown here.</p></section>
        <section class="page-panel legal-card"><h2>6. Contact</h2><p>For questions about this policy or the project, contact the project creator through the public contact details on their profile.</p></section>
        <p class="legal-note">This policy describes the current implementation and is not legal advice.</p>`
    },
    terms: {
      label: "TERMS & CONDITIONS",
      title: "A few ground rules.",
      subtitle: "Last updated: 18 November 2026",
      body: `
        <section class="page-panel legal-card"><h2>1. What Spendly is</h2><p>Spendly is an independent personal finance-recording tool intended for organisation and educational use. It is not a bank, wallet, payment processor, accounting service or investment adviser.</p></section>
        <section class="page-panel legal-card"><h2>2. Your responsibility</h2><p>You are responsible for the accuracy of the information you enter, keeping your device and browser secure, and maintaining your own backups. Spendly does not verify transactions or connect to financial institutions.</p></section>
        <section class="page-panel legal-card"><h2>3. No financial advice</h2><p>Figures, summaries and calculations are provided for personal tracking only. They are not financial, tax, legal or investment advice. Do not rely on Spendly as the sole basis for financial decisions.</p></section>
        <section class="page-panel legal-card"><h2>4. Data and availability</h2><p>Your entries are stored locally in your browser. Clearing browser data, changing browsers or devices, using private browsing, or resetting your account may make entries unavailable. Export backups regularly. The project is provided without a guarantee of uninterrupted availability or error-free operation.</p></section>
        <section class="page-panel legal-card"><h2>5. Acceptable use</h2><p>Use Spendly lawfully and do not attempt to disrupt, exploit or misuse the application or its hosting services.</p></section>
        <section class="page-panel legal-card"><h2>6. Changes</h2><p>Features and these terms may change as the project develops. Continued use after an update means you accept the revised terms.</p></section>
        <p class="legal-note">If you do not agree with these terms, stop using Spendly and clear its locally stored data through your browser settings.</p>`
    }
  };

  document.addEventListener("click", event => {
    const link = event.target.closest("[data-legal-panel]");
    if (link) {
      const key = link.dataset.legalPanel;
      const panel = panels[key];
      if (!panel) return;
      event.preventDefault();
      crumb.textContent = key === "terms" ? "Terms & Conditions" : key === "privacy" ? "Privacy Policy" : "Credits";
      eyebrow.textContent = panel.label;
      title.textContent = panel.title;
      subtitle.textContent = panel.subtitle;
      month.hidden = true;
      view.innerHTML = '<div class="page-list legal-list">' + panel.body +
        '<button class="outline-button legal-back" data-legal-back>← Back to overview</button></div>';
      window.scrollTo({top: 0, behavior: "smooth"});
    }
    if (event.target.closest("[data-legal-back]")) {
      document.querySelector('[data-page="overview"]').click();
      month.hidden = false;
    }
  });
  document.querySelectorAll(".nav-item").forEach(button => button.addEventListener("click", () => {
    if (month) month.hidden = false;
  }));
})();