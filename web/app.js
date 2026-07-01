const dataInput = document.querySelector("#dataInput");
const xInput = document.querySelector("#xInput");
const windowInput = document.querySelector("#windowInput");
const thresholdInput = document.querySelector("#thresholdInput");
const chart = document.querySelector("#chart");
const metrics = document.querySelector("#metrics");
const jsonOutput = document.querySelector("#jsonOutput");
const eyebrow = document.querySelector("#eyebrow");
const viewTitle = document.querySelector("#viewTitle");
const sampleButton = document.querySelector("#sampleButton");
const langButtons = document.querySelectorAll(".lang-button");

const ctx = chart.getContext("2d");
let activeTab = "summary";
let currentLang = localStorage.getItem("olympcore-lang") || "ru";

const samples = [
  {
    data: "10, 11, 12, 14, 15, 18, 21, 22, 26, 30",
    x: "1, 2, 3, 4, 5, 6, 7, 8, 9, 10"
  },
  {
    data: "3.1, 3.4, 3.6, 4.2, 4.7, 5.2, 9.8, 5.6, 5.9, 6.1",
    x: "0, 1, 2, 3, 4, 5, 6, 7, 8, 9"
  },
  {
    data: "42, 41, 40, 39, 38, 36, 35, 34, 33, 31, 30",
    x: "1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11"
  }
];
let sampleIndex = 0;

const i18n = {
  ru: {
    docTitle: "OlympCore Демо",
    tagline: "Демо математических методов C++",
    dataLabel: "Данные",
    xLabel: "Значения X",
    windowLabel: "Окно",
    thresholdLabel: "Порог выбросов",
    methodsLabel: "Методы",
    sampleTitle: "Загрузить пример",
    chartLabel: "График",
    summaryTab: "Σ",
    regressionTab: "Линия",
    normalTab: "Норма",
    smoothingTab: "Средн.",
    outliersTab: "Z",
    summaryTitle: "Описательная статистика",
    summaryEyebrow: "Сводка",
    regressionTitle: "Линейная регрессия",
    regressionEyebrow: "Регрессия",
    normalTitle: "Нормальное распределение",
    normalEyebrow: "Распределение",
    smoothingTitle: "Скользящее среднее",
    smoothingEyebrow: "Сглаживание",
    outliersTitle: "Z-score выбросы",
    outliersEyebrow: "Выбросы",
    count: "Кол-во",
    mean: "Среднее",
    median: "Медиана",
    stdev: "Ст. откл.",
    slope: "Наклон",
    intercept: "Сдвиг",
    rSquared: "R квадрат",
    points: "Точки",
    window: "Окно",
    first: "Первое",
    last: "Последнее",
    values: "Значения",
    threshold: "Порог",
    found: "Найдено"
  },
  en: {
    docTitle: "OlympCore Demo",
    tagline: "C++ methods demo",
    dataLabel: "Data",
    xLabel: "X values",
    windowLabel: "Window",
    thresholdLabel: "Outlier threshold",
    methodsLabel: "Methods",
    sampleTitle: "Load sample data",
    chartLabel: "Chart",
    summaryTab: "Sigma",
    regressionTab: "Line",
    normalTab: "Bell",
    smoothingTab: "Avg",
    outliersTab: "Z",
    summaryTitle: "Descriptive Statistics",
    summaryEyebrow: "Summary",
    regressionTitle: "Linear Fit",
    regressionEyebrow: "Regression",
    normalTitle: "Normal PDF and CDF",
    normalEyebrow: "Distribution",
    smoothingTitle: "Simple Moving Average",
    smoothingEyebrow: "Smoothing",
    outliersTitle: "Z-Score Detection",
    outliersEyebrow: "Outliers",
    count: "Count",
    mean: "Mean",
    median: "Median",
    stdev: "Stdev",
    slope: "Slope",
    intercept: "Intercept",
    rSquared: "R squared",
    points: "Points",
    window: "Window",
    first: "First",
    last: "Last",
    values: "Values",
    threshold: "Threshold",
    found: "Found"
  }
};

const titleKeys = {
  summary: ["summaryEyebrow", "summaryTitle"],
  regression: ["regressionEyebrow", "regressionTitle"],
  normal: ["normalEyebrow", "normalTitle"],
  smoothing: ["smoothingEyebrow", "smoothingTitle"],
  outliers: ["outliersEyebrow", "outliersTitle"]
};

function t(key) {
  return i18n[currentLang][key] || i18n.en[key] || key;
}

function parseNumbers(text) {
  return text
    .split(/[\s,;]+/)
    .map((item) => item.trim())
    .filter(Boolean)
    .map(Number)
    .filter((value) => Number.isFinite(value));
}

function mean(values) {
  return values.reduce((sum, value) => sum + value, 0) / values.length;
}

function median(values) {
  const sorted = [...values].sort((a, b) => a - b);
  const middle = Math.floor(sorted.length / 2);
  return sorted.length % 2 ? sorted[middle] : (sorted[middle - 1] + sorted[middle]) / 2;
}

function varianceSample(values) {
  if (values.length < 2) return 0;
  const avg = mean(values);
  return values.reduce((sum, value) => sum + (value - avg) ** 2, 0) / (values.length - 1);
}

function summary(values) {
  const variance = varianceSample(values);
  return {
    count: values.length,
    min: Math.min(...values),
    max: Math.max(...values),
    mean: mean(values),
    median: median(values),
    variance_sample: variance,
    stdev_sample: Math.sqrt(variance)
  };
}

function regression(xs, ys) {
  const xMean = mean(xs);
  const yMean = mean(ys);
  let ssXY = 0;
  let ssXX = 0;
  let ssYY = 0;
  xs.forEach((x, index) => {
    const dx = x - xMean;
    const dy = ys[index] - yMean;
    ssXY += dx * dy;
    ssXX += dx * dx;
    ssYY += dy * dy;
  });
  const slope = ssXY / ssXX;
  const intercept = yMean - slope * xMean;
  return {
    slope,
    intercept,
    r_squared: ssYY === 0 ? 1 : (ssXY * ssXY) / (ssXX * ssYY)
  };
}

function normalPdf(x, avg, stdev) {
  const z = (x - avg) / stdev;
  return Math.exp(-0.5 * z * z) / (stdev * Math.sqrt(2 * Math.PI));
}

function erf(x) {
  const sign = Math.sign(x) || 1;
  const a1 = 0.254829592;
  const a2 = -0.284496736;
  const a3 = 1.421413741;
  const a4 = -1.453152027;
  const a5 = 1.061405429;
  const p = 0.3275911;
  const t = 1 / (1 + p * Math.abs(x));
  const y = 1 - (((((a5 * t + a4) * t) + a3) * t + a2) * t + a1) * t * Math.exp(-x * x);
  return sign * y;
}

function normalCdf(x, avg, stdev) {
  return 0.5 * (1 + erf((x - avg) / (stdev * Math.SQRT2)));
}

function movingAverage(values, windowSize) {
  const result = [];
  for (let i = 0; i <= values.length - windowSize; i += 1) {
    result.push(mean(values.slice(i, i + windowSize)));
  }
  return result;
}

function outliers(values, threshold) {
  const avg = mean(values);
  const stdev = Math.sqrt(varianceSample(values));
  if (stdev === 0) return [];
  return values
    .map((value, index) => ({ index, value, z_score: (value - avg) / stdev }))
    .filter((item) => Math.abs(item.z_score) >= threshold);
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
  document.querySelector(".tabs").setAttribute("aria-label", t("methodsLabel"));
  chart.setAttribute("aria-label", t("chartLabel"));
  sampleButton.setAttribute("title", t("sampleTitle"));
  document.querySelector('[data-tab="summary"]').textContent = t("summaryTab");
  document.querySelector('[data-tab="summary"]').setAttribute("title", t("summaryTitle"));
  document.querySelector('[data-tab="regression"]').textContent = t("regressionTab");
  document.querySelector('[data-tab="regression"]').setAttribute("title", t("regressionTitle"));
  document.querySelector('[data-tab="normal"]').textContent = t("normalTab");
  document.querySelector('[data-tab="normal"]').setAttribute("title", t("normalTitle"));
  document.querySelector('[data-tab="smoothing"]').textContent = t("smoothingTab");
  document.querySelector('[data-tab="smoothing"]').setAttribute("title", t("smoothingTitle"));
  document.querySelector('[data-tab="outliers"]').textContent = t("outliersTab");
  document.querySelector('[data-tab="outliers"]').setAttribute("title", t("outliersTitle"));
  langButtons.forEach((button) => {
    button.classList.toggle("active", button.dataset.lang === currentLang);
  });
}

function setMetrics(items) {
  metrics.innerHTML = items.map(([label, value]) => (
    `<div class="metric"><span>${label}</span><strong>${format(value)}</strong></div>`
  )).join("");
}

function clearChart() {
  ctx.clearRect(0, 0, chart.width, chart.height);
  ctx.fillStyle = "#ffffff";
  ctx.fillRect(0, 0, chart.width, chart.height);
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

function drawLine(points, color, width = 3) {
  const scale = scales(points);
  drawAxes(scale);
  drawLineOnScale(points, color, scale, width);
  return scale;
}

function drawLineOnScale(points, color, scale, width = 3) {
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

function drawPoints(points, color, scale = scales(points)) {
  points.forEach((point) => {
    ctx.beginPath();
    ctx.fillStyle = color;
    ctx.arc(scale.sx(point.x), scale.sy(point.y), 6, 0, Math.PI * 2);
    ctx.fill();
  });
}

function drawBars(values, color) {
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

function render() {
  clearChart();
  const values = parseNumbers(dataInput.value);
  const xs = parseNumbers(xInput.value);
  const [eyebrowKey, titleKey] = titleKeys[activeTab];
  eyebrow.textContent = t(eyebrowKey);
  viewTitle.textContent = t(titleKey);

  if (!values.length) {
    metrics.innerHTML = "";
    jsonOutput.textContent = "{}";
    return;
  }

  if (activeTab === "summary") {
    const result = summary(values);
    drawBars(values, "#0f766e");
    setMetrics([
      [t("count"), result.count],
      [t("mean"), result.mean],
      [t("median"), result.median],
      [t("stdev"), result.stdev_sample]
    ]);
    jsonOutput.textContent = JSON.stringify(result, null, 2);
  }

  if (activeTab === "regression") {
    const n = Math.min(xs.length, values.length);
    const xSeries = xs.slice(0, n);
    const ySeries = values.slice(0, n);
    const result = regression(xSeries, ySeries);
    const points = xSeries.map((x, index) => ({ x, y: ySeries[index] }));
    const fit = [
      { x: Math.min(...xSeries), y: result.slope * Math.min(...xSeries) + result.intercept },
      { x: Math.max(...xSeries), y: result.slope * Math.max(...xSeries) + result.intercept }
    ];
    const scale = scales([...points, ...fit]);
    drawAxes(scale);
    drawLineOnScale(fit, "#b23a48", scale, 4);
    drawPoints(points, "#2f5f98", scale);
    setMetrics([
      [t("slope"), result.slope],
      [t("intercept"), result.intercept],
      [t("rSquared"), result.r_squared],
      [t("points"), n]
    ]);
    jsonOutput.textContent = JSON.stringify(result, null, 2);
  }

  if (activeTab === "normal") {
    const result = summary(values);
    const stdev = result.stdev_sample || 1;
    const minX = result.mean - 4 * stdev;
    const maxX = result.mean + 4 * stdev;
    const curve = Array.from({ length: 120 }, (_, index) => {
      const x = minX + ((maxX - minX) * index) / 119;
      return { x, y: normalPdf(x, result.mean, stdev) };
    });
    drawLine(curve, "#0f766e", 4);
    setMetrics([
      [t("mean"), result.mean],
      [t("stdev"), stdev],
      ["PDF(mean)", normalPdf(result.mean, result.mean, stdev)],
      ["CDF(mean)", normalCdf(result.mean, result.mean, stdev)]
    ]);
    jsonOutput.textContent = JSON.stringify({
      mean: result.mean,
      stdev,
      pdf_at_mean: normalPdf(result.mean, result.mean, stdev),
      cdf_at_mean: normalCdf(result.mean, result.mean, stdev)
    }, null, 2);
  }

  if (activeTab === "smoothing") {
    const windowSize = Math.max(1, Math.min(Number(windowInput.value) || 1, values.length));
    const smoothed = movingAverage(values, windowSize);
    const rawPoints = values.map((y, x) => ({ x, y }));
    const smoothPoints = smoothed.map((y, index) => ({ x: index + windowSize - 1, y }));
    const scale = scales([...rawPoints, ...smoothPoints]);
    drawAxes(scale);
    drawLineOnScale(rawPoints, "#b9c4bc", scale, 2);
    drawLineOnScale(smoothPoints, "#c88719", scale, 4);
    drawPoints(rawPoints, "#2f5f98", scale);
    setMetrics([
      [t("window"), windowSize],
      [t("first"), smoothed[0]],
      [t("last"), smoothed[smoothed.length - 1]],
      [t("values"), smoothed.length]
    ]);
    jsonOutput.textContent = JSON.stringify(smoothed, null, 2);
  }

  if (activeTab === "outliers") {
    const threshold = Math.max(0.1, Number(thresholdInput.value) || 2);
    const result = outliers(values, threshold);
    const scale = drawBars(values, "#2f5f98");
    result.forEach((item) => {
      ctx.beginPath();
      ctx.fillStyle = "#b23a48";
      ctx.arc(scale.sx(item.index), scale.sy(item.value), 10, 0, Math.PI * 2);
      ctx.fill();
    });
    setMetrics([
      [t("threshold"), threshold],
      [t("found"), result.length],
      [t("mean"), mean(values)],
      [t("stdev"), Math.sqrt(varianceSample(values))]
    ]);
    jsonOutput.textContent = JSON.stringify(result, null, 2);
  }
}

document.querySelectorAll(".tab").forEach((button) => {
  button.addEventListener("click", () => {
    document.querySelectorAll(".tab").forEach((item) => item.classList.remove("active"));
    button.classList.add("active");
    activeTab = button.dataset.tab;
    render();
  });
});

langButtons.forEach((button) => {
  button.addEventListener("click", () => {
    currentLang = button.dataset.lang;
    localStorage.setItem("olympcore-lang", currentLang);
    applyLanguage();
    render();
  });
});

[dataInput, xInput, windowInput, thresholdInput].forEach((input) => {
  input.addEventListener("input", render);
});

sampleButton.addEventListener("click", () => {
  sampleIndex = (sampleIndex + 1) % samples.length;
  dataInput.value = samples[sampleIndex].data;
  xInput.value = samples[sampleIndex].x;
  render();
});

applyLanguage();
render();
