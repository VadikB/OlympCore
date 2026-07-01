const registry = window.OlympCoreMethods;
const calculators = window.OlympCoreCalculators;
const panels = window.OlympCorePanels;

const chart = document.querySelector("#chart");
const ctx = chart.getContext("2d");
const metrics = document.querySelector("#metrics");
const jsonOutput = document.querySelector("#jsonOutput");
const methodTitle = document.querySelector("#methodTitle");
const methodGroupLabel = document.querySelector("#methodGroupLabel");
const methodDescription = document.querySelector("#methodDescription");
const methodStatus = document.querySelector("#methodStatus");
const methodNote = document.querySelector("#methodNote");
const inputPanel = document.querySelector("#inputPanel");
const runButton = document.querySelector("#runButton");
const sampleButton = document.querySelector("#sampleButton");
const langButtons = document.querySelectorAll(".lang-button");
const methodSearch = document.querySelector("#methodSearch");
const groupFilter = document.querySelector("#groupFilter");
const statusFilter = document.querySelector("#statusFilter");
const catalogFilter = document.querySelector("#catalogFilter");
const catalogStats = document.querySelector("#catalogStats");
const methodList = document.querySelector("#methodList");
const methodCatalog = document.querySelector("#methodCatalog");

let currentLang = localStorage.getItem("olympcore-lang") || "ru";
let activeMethodId = localStorage.getItem("olympcore-method") || "summary";

function labels() {
  return registry.i18n[currentLang] || registry.i18n.en;
}

function t(key) {
  return labels()[key] || registry.i18n.en[key] || key;
}

function groupTitle(groupId) {
  return labels().groups[groupId] || registry.i18n.en.groups[groupId] || groupId;
}

function currentMethod() {
  return registry.methods.find((method) => method.id === activeMethodId) || registry.methods[0];
}

function methodText(method) {
  return method.description[currentLang] || method.description.en;
}

function filteredMethods() {
  const query = methodSearch.value.trim().toLowerCase();
  const group = groupFilter.value || "all";
  const status = statusFilter.value || "all";
  return registry.methods.filter((method) => {
    const matchesQuery = !query ||
      method.name.toLowerCase().includes(query) ||
      methodText(method).toLowerCase().includes(query) ||
      groupTitle(method.group).toLowerCase().includes(query);
    const matchesGroup = group === "all" || method.group === group;
    const matchesStatus = status === "all" || method.status === status;
    return matchesQuery && matchesGroup && matchesStatus;
  });
}

function format(value) {
  if (!Number.isFinite(value)) return "n/a";
  return Math.abs(value) >= 1000 ? value.toExponential(4) : value.toFixed(4).replace(/\.?0+$/, "");
}

function applyLanguage() {
  document.documentElement.lang = currentLang;
  document.title = t("docTitle");
  document.querySelectorAll("[data-i18n]").forEach((element) => {
    element.textContent = t(element.dataset.i18n);
  });
  chart.setAttribute("aria-label", t("chartLabel"));
  sampleButton.setAttribute("title", t("sampleTitle"));
  langButtons.forEach((button) => button.classList.toggle("active", button.dataset.lang === currentLang));
  renderFilters();
  renderMethodList();
  selectMethod(activeMethodId, { preserveInputs: true });
  renderCatalog();
}

function renderFilters() {
  const groupValue = groupFilter.value || "all";
  const statusValue = statusFilter.value || "all";
  const groupOptions = [
    ["all", t("filterAll")],
    ...registry.groups.map((group) => [group.id, groupTitle(group.id)])
  ];
  groupFilter.innerHTML = groupOptions.map(([value, label]) => `<option value="${value}">${label}</option>`).join("");
  groupFilter.value = groupOptions.some(([value]) => value === groupValue) ? groupValue : "all";

  const statusOptions = [
    ["all", t("statusAll")],
    ["ready", t("readyStatus")],
    ["porting", t("portingStatus")]
  ];
  statusFilter.innerHTML = statusOptions.map(([value, label]) => `<option value="${value}">${label}</option>`).join("");
  statusFilter.value = statusOptions.some(([value]) => value === statusValue) ? statusValue : "all";

  const catalogValue = catalogFilter.value || "all";
  catalogFilter.innerHTML = groupOptions.map(([value, label]) => `<option value="${value}">${label}</option>`).join("");
  catalogFilter.value = groupOptions.some(([value]) => value === catalogValue) ? catalogValue : "all";
}

function renderMethodList() {
  const methods = filteredMethods();
  const ready = registry.methods.filter((method) => method.status === "ready").length;
  const porting = registry.methods.length - ready;
  catalogStats.innerHTML = [
    [t("readyCount"), ready],
    [t("portingCount"), porting],
    [t("totalCount"), registry.methods.length]
  ].map(([label, value]) => `<div class="catalog-stat"><span>${label}</span><strong>${value}</strong></div>`).join("");

  methodList.innerHTML = methods.length ? methods.map((method) => `
    <button class="method-list-item ${method.id === activeMethodId ? "active" : ""}" data-method="${method.id}" type="button">
      <span>${method.name}</span>
      <small>${groupTitle(method.group)}</small>
    </button>
  `).join("") : `<div class="empty-state">${t("noMethods")}</div>`;
}

function renderCatalog() {
  const selectedGroup = catalogFilter.value || "all";
  const visible = selectedGroup === "all"
    ? registry.methods
    : registry.methods.filter((method) => method.group === selectedGroup);
  methodCatalog.innerHTML = visible.map((method) => {
    const statusClass = method.status === "ready" ? "status-ready" : "status-porting";
    const statusText = method.status === "ready" ? t("readyStatus") : t("portingStatus");
    return `
      <article class="method-card" data-method-card="${method.id}">
        <header>
          <h4>${method.name}</h4>
          <span class="status ${statusClass}">${statusText}</span>
        </header>
        <p>${groupTitle(method.group)}. ${methodText(method)}</p>
        <code>${t("sourceLabel")}: ${method.source}</code>
      </article>
    `;
  }).join("");
}

function selectMethod(methodId, options = {}) {
  const method = registry.methods.find((item) => item.id === methodId) || registry.methods[0];
  activeMethodId = method.id;
  localStorage.setItem("olympcore-method", activeMethodId);

  methodTitle.textContent = method.name;
  methodGroupLabel.textContent = groupTitle(method.group);
  methodDescription.textContent = methodText(method);
  methodStatus.textContent = method.status === "ready" ? t("readyStatus") : t("portingStatus");
  methodStatus.className = `status ${method.status === "ready" ? "status-ready" : "status-porting"}`;
  runButton.disabled = method.status !== "ready";
  methodNote.textContent = method.status === "ready" ? "" : t("notPorted");

  const existingValues = options.preserveInputs ? panels.readInputs(inputPanel) : null;
  panels.renderInputs(inputPanel, method, labels());
  if (existingValues) {
    inputPanel.querySelectorAll("[data-input]").forEach((element) => {
      if (existingValues[element.dataset.input] === undefined) return;
      if (element.type === "checkbox") element.checked = Boolean(existingValues[element.dataset.input]);
      else element.value = existingValues[element.dataset.input];
    });
  }

  renderMethodList();
  runActiveMethod();
}

function runActiveMethod() {
  const method = currentMethod();
  clearChart();
  if (method.status !== "ready") {
    metrics.innerHTML = "";
    jsonOutput.textContent = JSON.stringify({
      status: "porting",
      method: method.name,
      source: method.source,
      inputs: panels.readInputs(inputPanel)
    }, null, 2);
    drawPlaceholder();
    return;
  }

  try {
    const result = calculators.calculate(method.id, panels.readInputs(inputPanel));
    setMetrics(result.metrics);
    jsonOutput.textContent = JSON.stringify(result.json, null, 2);
    drawChart(result.chart);
  } catch (error) {
    metrics.innerHTML = "";
    jsonOutput.textContent = JSON.stringify({ error: error.message }, null, 2);
    drawPlaceholder();
  }
}

function setMetrics(items) {
  const labelMap = labels().metrics;
  metrics.innerHTML = Object.entries(items).map(([key, value]) => (
    `<div class="metric"><span>${labelMap[key] || key}</span><strong>${format(Number(value))}</strong></div>`
  )).join("");
}

function clearChart() {
  ctx.clearRect(0, 0, chart.width, chart.height);
  ctx.fillStyle = "#ffffff";
  ctx.fillRect(0, 0, chart.width, chart.height);
}

function drawPlaceholder() {
  ctx.fillStyle = "#5c6861";
  ctx.font = "24px sans-serif";
  ctx.fillText(t("notPorted"), 56, 86);
}

function scales(points) {
  const xs = points.map((point) => point.x);
  const ys = points.map((point) => point.y);
  let minX = Math.min(...xs);
  let maxX = Math.max(...xs);
  let minY = Math.min(...ys);
  let maxY = Math.max(...ys);
  if (minX === maxX) {
    minX -= 1;
    maxX += 1;
  }
  if (minY === maxY) {
    minY -= 1;
    maxY += 1;
  }
  const padX = (maxX - minX) * 0.08;
  const padY = (maxY - minY) * 0.14;
  minX -= padX;
  maxX += padX;
  minY -= padY;
  maxY += padY;
  const left = 56;
  const right = chart.width - 26;
  const top = 26;
  const bottom = chart.height - 42;
  return {
    sx: (x) => left + ((x - minX) / (maxX - minX)) * (right - left),
    sy: (y) => bottom - ((y - minY) / (maxY - minY)) * (bottom - top),
    left,
    right,
    top,
    bottom
  };
}

function drawAxes(scale) {
  ctx.strokeStyle = "#d9dfd8";
  ctx.lineWidth = 1;
  ctx.beginPath();
  ctx.moveTo(scale.left, scale.top);
  ctx.lineTo(scale.left, scale.bottom);
  ctx.lineTo(scale.right, scale.bottom);
  ctx.stroke();
}

function drawChart(chartSpec) {
  if (chartSpec.type === "bars") drawBars(chartSpec.values);
  if (chartSpec.type === "regression") drawRegression(chartSpec);
  if (chartSpec.type === "normal") drawNormal(chartSpec);
  if (chartSpec.type === "smoothing") drawSmoothing(chartSpec);
  if (chartSpec.type === "outliers") drawOutliers(chartSpec);
}

function drawBars(values, color = "#0f766e") {
  const points = values.map((value, index) => ({ x: index, y: value }));
  const scale = scales(points);
  drawAxes(scale);
  const barWidth = Math.max(8, (scale.right - scale.left) / values.length * 0.56);
  values.forEach((value, index) => {
    const x = scale.sx(index) - barWidth / 2;
    const y = scale.sy(value);
    ctx.fillStyle = color;
    ctx.fillRect(x, y, barWidth, scale.bottom - y);
  });
  return scale;
}

function drawLine(points, color, scale, width = 3) {
  ctx.strokeStyle = color;
  ctx.lineWidth = width;
  ctx.beginPath();
  points.forEach((point, index) => {
    const x = scale.sx(point.x);
    const y = scale.sy(point.y);
    if (index === 0) ctx.moveTo(x, y);
    else ctx.lineTo(x, y);
  });
  ctx.stroke();
}

function drawPoints(points, color, scale, radius = 6) {
  points.forEach((point) => {
    ctx.beginPath();
    ctx.fillStyle = color;
    ctx.arc(scale.sx(point.x), scale.sy(point.y), radius, 0, Math.PI * 2);
    ctx.fill();
  });
}

function drawRegression(spec) {
  const points = spec.xs.map((x, index) => ({ x, y: spec.ys[index] }));
  const minX = Math.min(...spec.xs);
  const maxX = Math.max(...spec.xs);
  const fit = [
    { x: minX, y: spec.result.slope * minX + spec.result.intercept },
    { x: maxX, y: spec.result.slope * maxX + spec.result.intercept }
  ];
  const scale = scales([...points, ...fit]);
  drawAxes(scale);
  drawLine(fit, "#b23a48", scale, 4);
  drawPoints(points, "#2f5f98", scale);
}

function drawNormal(spec) {
  const minX = spec.mean - 4 * spec.stdev;
  const maxX = spec.mean + 4 * spec.stdev;
  const curve = Array.from({ length: 120 }, (_, index) => {
    const x = minX + ((maxX - minX) * index) / 119;
    return { x, y: calculators.normalPdf(x, spec.mean, spec.stdev) };
  });
  const scale = scales(curve);
  drawAxes(scale);
  drawLine(curve, "#0f766e", scale, 4);
  drawPoints([{ x: spec.x, y: calculators.normalPdf(spec.x, spec.mean, spec.stdev) }], "#b23a48", scale, 7);
}

function drawSmoothing(spec) {
  const raw = spec.values.map((y, x) => ({ x, y }));
  const smooth = spec.smoothed.map((y, index) => ({ x: index + spec.windowSize - 1, y }));
  const scale = scales([...raw, ...smooth]);
  drawAxes(scale);
  drawLine(raw, "#b9c4bc", scale, 2);
  drawLine(smooth, "#c88719", scale, 4);
  drawPoints(raw, "#2f5f98", scale, 5);
}

function drawOutliers(spec) {
  const scale = drawBars(spec.values, "#2f5f98");
  spec.outliers.forEach((item) => {
    ctx.beginPath();
    ctx.fillStyle = "#b23a48";
    ctx.arc(scale.sx(item.index), scale.sy(item.value), 10, 0, Math.PI * 2);
    ctx.fill();
  });
}

methodList.addEventListener("click", (event) => {
  const button = event.target.closest("[data-method]");
  if (!button) return;
  selectMethod(button.dataset.method);
});

methodCatalog.addEventListener("click", (event) => {
  const card = event.target.closest("[data-method-card]");
  if (!card) return;
  selectMethod(card.dataset.method);
  window.scrollTo({ top: 0, behavior: "smooth" });
});

[methodSearch, groupFilter, statusFilter].forEach((element) => {
  element.addEventListener("input", renderMethodList);
  element.addEventListener("change", renderMethodList);
});

catalogFilter.addEventListener("change", renderCatalog);
runButton.addEventListener("click", runActiveMethod);
sampleButton.addEventListener("click", () => selectMethod(activeMethodId));
inputPanel.addEventListener("input", () => {
  if (currentMethod().status === "ready") runActiveMethod();
});

langButtons.forEach((button) => {
  button.addEventListener("click", () => {
    currentLang = button.dataset.lang;
    localStorage.setItem("olympcore-lang", currentLang);
    applyLanguage();
  });
});

applyLanguage();
