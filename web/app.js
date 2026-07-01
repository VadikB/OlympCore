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
const catalogFilter = document.querySelector("#catalogFilter");
const catalogStats = document.querySelector("#catalogStats");
const methodCatalog = document.querySelector("#methodCatalog");

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
    found: "Найдено",
    catalogEyebrow: "Карта библиотеки",
    catalogTitle: "Методы OlympCore",
    filterAll: "Все группы",
    readyStatus: "готово",
    portingStatus: "портинг",
    readyCount: "Интерактивно",
    portingCount: "К переносу",
    totalCount: "Всего",
    sourceLabel: "Источник",
    groups: {
      descriptive: "Описательная статистика",
      distributions: "Распределения",
      inference: "Статистические тесты",
      regression: "Регрессия",
      smoothing: "Сглаживание",
      timeSeries: "Временные ряды",
      mle: "Оценка параметров",
      outliers: "Выбросы",
      imputation: "Заполнение пропусков",
      processing: "Обработка данных",
      utilities: "Утилиты"
    }
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
    found: "Found",
    catalogEyebrow: "Library map",
    catalogTitle: "OlympCore Methods",
    filterAll: "All groups",
    readyStatus: "ready",
    portingStatus: "porting",
    readyCount: "Interactive",
    portingCount: "To port",
    totalCount: "Total",
    sourceLabel: "Source",
    groups: {
      descriptive: "Descriptive statistics",
      distributions: "Distributions",
      inference: "Statistical tests",
      regression: "Regression",
      smoothing: "Smoothing",
      timeSeries: "Time series",
      mle: "Parameter fitting",
      outliers: "Outliers",
      imputation: "Imputation",
      processing: "Data processing",
      utilities: "Utilities"
    }
  }
};

const methodGroups = [
  {
    id: "descriptive",
    methods: [
      ["Summary statistics", "ready", "count, min, max, mean, median, sample variance and standard deviation.", "количество, минимум, максимум, среднее, медиана, выборочная дисперсия и отклонение.", "web + src/stats.cpp"],
      ["Average", "porting", "Mean over all values and dimensions.", "среднее по всем значениям и измерениям.", "Stats/Descriptive/average.*"],
      ["Median", "ready", "Median value for a numeric sample.", "медиана числовой выборки.", "src/stats.cpp"],
      ["Quantile inclusive", "ready", "Inclusive percentile interpolation.", "инклюзивная интерполяция процентиля.", "src/stats.cpp"],
      ["Quantile exclusive", "porting", "Exclusive quantile calculation.", "эксклюзивный расчёт квантиля.", "Stats/Descriptive/quantile_exc.*"],
      ["Percent rank inc/exc", "porting", "Inclusive and exclusive percentile rank.", "инклюзивный и эксклюзивный процентный ранг.", "Stats/Descriptive/percentrank_*"],
      ["Variance sample/population", "ready", "Sample variance is interactive now; population variance is queued.", "выборочная дисперсия уже интерактивна; генеральная в очереди.", "src/stats.cpp, Stats/Descriptive/var_*"],
      ["Standard deviation sample/population", "ready", "Sample standard deviation is interactive now; population version is queued.", "выборочное стандартное отклонение уже интерактивно; генеральное в очереди.", "src/stats.cpp, Stats/Descriptive/stdev_*"],
      ["Covariance and Pearson", "porting", "Covariance, population covariance, Pearson correlation.", "ковариация, ковариация генеральной совокупности, корреляция Пирсона.", "Stats/Descriptive/cov*, pearson.*"],
      ["Skew, kurtosis, moments", "porting", "Shape metrics and raw/central moments.", "метрики формы распределения и начальные/центральные моменты.", "Stats/Descriptive/skew.*, kurt.*, moment*"],
      ["Rank average/equal", "porting", "Ranking with average or equal-rank handling.", "ранжирование со средними или равными рангами.", "Stats/Descriptive/rank_*"],
      ["Large, small, range", "porting", "Order statistics and numeric range.", "порядковые статистики и размах.", "Stats/Descriptive/large.*, small.*, range.*"],
      ["ANOVA", "porting", "Single-factor, two-factor and repeated ANOVA.", "однофакторный, двухфакторный и повторный ANOVA.", "Stats/Descriptive/anova_*"],
      ["Crosstabulate numbers", "porting", "Numeric crosstabulation.", "числовая перекрёстная таблица.", "Stats/Descriptive/crosstabulate_numbers.*"],
      ["AveDev, DevSq, Geo/Harmonic mean", "porting", "Average deviation, sum of square deviations, geometric and harmonic means.", "среднее отклонение, сумма квадратов отклонений, геометрическое и гармоническое среднее.", "Stats/Descriptive/avedev.*, devsq.*, geoharmean.*"]
    ]
  },
  {
    id: "distributions",
    methods: [
      ["Normal distribution", "ready", "Normal PDF, CDF and inverse CDF.", "нормальные PDF, CDF и обратная CDF.", "src/stats.cpp, Stats/dist/norm.*"],
      ["Beta", "porting", "Beta distribution and random sampling.", "бета-распределение и генерация выборок.", "Stats/dist/beta*"],
      ["Binomial", "porting", "Binomial distribution and random sampling.", "биномиальное распределение и генерация выборок.", "Stats/dist/binom*"],
      ["Bernoulli", "porting", "Bernoulli distribution and random sampling.", "распределение Бернулли и генерация выборок.", "Stats/dist/bernoulli*"],
      ["Cauchy", "porting", "Cauchy distribution and random sampling.", "распределение Коши и генерация выборок.", "Stats/dist/cauchy*"],
      ["Chi-square", "porting", "Chi-square distribution and random sampling.", "хи-квадрат распределение и генерация выборок.", "Stats/dist/chisq*"],
      ["Exponential", "porting", "Exponential distribution and random sampling.", "экспоненциальное распределение и генерация выборок.", "Stats/dist/expon*"],
      ["F and Fisher", "porting", "F-family distribution helpers.", "семейство F-распределений.", "Stats/dist/f*, fisher.*"],
      ["Gamma", "porting", "Gamma distribution and random sampling.", "гамма-распределение и генерация выборок.", "Stats/dist/gamma*"],
      ["Geometric", "porting", "Geometric distribution and random sampling.", "геометрическое распределение и генерация выборок.", "Stats/dist/geometric*"],
      ["GEV, Gumbel, Rayleigh", "porting", "Extreme value and Rayleigh distributions.", "распределения экстремальных значений и Рэлея.", "Stats/dist/gev.*, gumbel.*, rayleigh*"],
      ["Hypergeometric", "porting", "Hypergeometric distribution and random sampling.", "гипергеометрическое распределение и генерация выборок.", "Stats/dist/hypgeom*"],
      ["Laplace, Logistic, Lognormal", "porting", "Laplace, logistic and lognormal distributions.", "распределения Лапласа, логистическое и логнормальное.", "Stats/dist/laplace.*, logistic.*, lognorm*"],
      ["Negative binomial, Pareto, Poisson", "porting", "Discrete and heavy-tail distributions.", "дискретные распределения и распределения с тяжёлым хвостом.", "Stats/dist/negbinom*, pareto*, poisson*"],
      ["Student t, Uniform, Weibull", "porting", "T, uniform and Weibull distributions.", "t-распределение, равномерное и Вейбулла.", "Stats/dist/t.*, uniform*, weibull*"]
    ]
  },
  {
    id: "inference",
    methods: [
      ["Z-test", "porting", "Z hypothesis test.", "Z-критерий.", "Stats/Inference/z_test.*"],
      ["T-test", "porting", "Student t-test.", "t-критерий Стьюдента.", "Stats/Inference/t_test.*"],
      ["Chi-square test", "porting", "Chi-square hypothesis test.", "критерий хи-квадрат.", "Stats/Inference/chisq_test.*"],
      ["Fisher exact test", "porting", "Fisher exact test.", "точный критерий Фишера.", "Stats/Inference/fisher_test.*"],
      ["Kolmogorov-Smirnov", "porting", "One-sample and two-sample KS tests.", "одновыборочный и двухвыборочный критерии Колмогорова-Смирнова.", "Stats/Inference/ks*_test.*"],
      ["Mann-Whitney", "porting", "Mann-Whitney U test.", "критерий Манна-Уитни.", "Stats/Inference/mw_test.*"],
      ["Wilcoxon signed-rank", "porting", "Wilcoxon signed-rank test.", "знаково-ранговый критерий Уилкоксона.", "Stats/Inference/wsr_test.*"]
    ]
  },
  {
    id: "regression",
    methods: [
      ["Linear regression", "ready", "Slope, intercept and R squared are interactive.", "наклон, сдвиг и R квадрат уже интерактивны.", "src/stats.cpp"],
      ["Slope", "porting", "Legacy slope operator.", "старый оператор наклона.", "Stats/Regression/slope.*"],
      ["Intercept", "porting", "Legacy intercept operator.", "старый оператор свободного члена.", "Stats/Regression/intercept.*"],
      ["Forecast", "porting", "Linear forecast from known X/Y values.", "линейный прогноз по известным X/Y.", "Stats/Regression/forecast.*"],
      ["STEYX", "porting", "Standard error of predicted Y.", "стандартная ошибка предсказанного Y.", "Stats/Regression/steyx.*"],
      ["Naive Bayes", "porting", "Classification helper.", "вспомогательный классификатор.", "Stats/Regression/naive_bayes.cpp"]
    ]
  },
  {
    id: "smoothing",
    methods: [
      ["Simple moving average", "ready", "Interactive rolling average by window.", "интерактивное скользящее среднее по окну.", "src/stats.cpp"],
      ["Cumulative moving average", "porting", "Cumulative average over time.", "накопительное среднее по времени.", "Stats/Smoothing/moving_average_cumulative.*"],
      ["Weighted moving average", "porting", "Weighted rolling average.", "взвешенное скользящее среднее.", "Stats/Smoothing/moving_average_weighted.*"],
      ["Triangular moving average", "porting", "Triangular smoothing window.", "треугольное сглаживающее окно.", "Stats/Smoothing/moving_average_triangular.*"],
      ["Exponential moving average", "porting", "EMA smoothing.", "экспоненциальное скользящее среднее.", "Stats/Smoothing/moving_average_exponential.*"]
    ]
  },
  {
    id: "timeSeries",
    methods: [
      ["ADF test", "porting", "Augmented Dickey-Fuller stationarity test.", "расширенный тест Дики-Фуллера на стационарность.", "Stats/TimeSeries/adf_test.*"],
      ["Autocorrelation ACF", "porting", "Autocorrelation function.", "автокорреляционная функция.", "Stats/TimeSeries/timeseries_acf.cpp"],
      ["Partial autocorrelation PACF", "porting", "Partial autocorrelation function.", "частная автокорреляция.", "Stats/TimeSeries/timeseries_pacf.cpp"],
      ["Autoregression AR", "porting", "Autoregressive model helpers.", "вспомогательные функции авторегрессии.", "Stats/TimeSeries/timeseries_ar.*"],
      ["AIC/BIC", "porting", "Information criteria helpers.", "информационные критерии.", "Stats/TimeSeries/timeseries_aicbic.cpp"],
      ["Time series forecast", "porting", "Forecast helpers for time series.", "прогнозирование временных рядов.", "Stats/TimeSeries/time_series_forecast.cpp"]
    ]
  },
  {
    id: "mle",
    methods: [
      ["Normal fit", "porting", "Maximum-likelihood normal fit.", "MLE-подгонка нормального распределения.", "Stats/MLE/norm_fit.*"],
      ["Exponential fit", "porting", "Maximum-likelihood exponential fit.", "MLE-подгонка экспоненциального распределения.", "Stats/MLE/expon_fit.*"],
      ["Poisson fit", "porting", "Maximum-likelihood Poisson fit.", "MLE-подгонка распределения Пуассона.", "Stats/MLE/poisson_fit.*"],
      ["Binomial fit", "porting", "Maximum-likelihood binomial fit.", "MLE-подгонка биномиального распределения.", "Stats/MLE/binom_fit.*"]
    ]
  },
  {
    id: "outliers",
    methods: [
      ["Z-score outliers", "ready", "Interactive outlier detection by z-score threshold.", "интерактивный поиск выбросов по z-score.", "src/stats.cpp"],
      ["ESD", "porting", "Extreme studentized deviate test.", "критерий экстремального студентизированного отклонения.", "Stats/Outliers/esd.*"],
      ["Dixon Q", "porting", "Dixon Q outlier test.", "Q-критерий Диксона для выбросов.", "Stats/Outliers/dq_test.*"]
    ]
  },
  {
    id: "imputation",
    methods: [
      ["Mean imputation", "porting", "Fill missing values with means.", "заполнение пропусков средними.", "Stats/Imputation/imputation_mean.*"],
      ["Match imputation", "porting", "Match-based imputation.", "заполнение пропусков по совпадениям.", "Stats/Imputation/imputation_match.cpp"],
      ["LVCF", "porting", "Last value carried forward.", "перенос последнего известного значения вперёд.", "Stats/Imputation/lvcf.*"]
    ]
  },
  {
    id: "processing",
    methods: [
      ["Standardize", "porting", "Data standardization.", "стандартизация данных.", "Stats/DataProcessing/standardize.cpp"]
    ]
  },
  {
    id: "utilities",
    methods: [
      ["Random permutation", "porting", "Random permutation utility.", "утилита случайной перестановки.", "Stats/Utilities/rand_perm.*"],
      ["RNG helpers", "porting", "Random generation utilities.", "вспомогательные функции генерации случайных чисел.", "Stats/dist/TCRNGUtils.*"]
    ]
  }
];

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

function groupTitle(groupId) {
  return i18n[currentLang].groups[groupId] || i18n.en.groups[groupId] || groupId;
}

function allMethods() {
  return methodGroups.flatMap((group) => group.methods.map((method) => ({
    group: group.id,
    name: method[0],
    status: method[1],
    description: currentLang === "ru" ? method[3] : method[2],
    source: method[4]
  })));
}

function renderCatalogOptions() {
  const selected = catalogFilter.value || "all";
  catalogFilter.innerHTML = [
    `<option value="all">${t("filterAll")}</option>`,
    ...methodGroups.map((group) => `<option value="${group.id}">${groupTitle(group.id)}</option>`)
  ].join("");
  catalogFilter.value = [...catalogFilter.options].some((option) => option.value === selected) ? selected : "all";
  catalogFilter.setAttribute("aria-label", t("catalogTitle"));
}

function renderCatalog() {
  renderCatalogOptions();
  const selectedGroup = catalogFilter.value || "all";
  const methods = allMethods();
  const visible = selectedGroup === "all" ? methods : methods.filter((method) => method.group === selectedGroup);
  const ready = methods.filter((method) => method.status === "ready").length;
  const porting = methods.length - ready;

  catalogStats.innerHTML = [
    [t("readyCount"), ready],
    [t("portingCount"), porting],
    [t("totalCount"), methods.length]
  ].map(([label, value]) => (
    `<div class="catalog-stat"><span>${label}</span><strong>${value}</strong></div>`
  )).join("");

  methodCatalog.innerHTML = visible.map((method) => {
    const statusClass = method.status === "ready" ? "status-ready" : "status-porting";
    const statusText = method.status === "ready" ? t("readyStatus") : t("portingStatus");
    return `
      <article class="method-card">
        <header>
          <h4>${method.name}</h4>
          <span class="status ${statusClass}">${statusText}</span>
        </header>
        <p>${groupTitle(method.group)}. ${method.description}</p>
        <code>${t("sourceLabel")}: ${method.source}</code>
      </article>
    `;
  }).join("");
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
  renderCatalog();
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

catalogFilter.addEventListener("change", renderCatalog);

sampleButton.addEventListener("click", () => {
  sampleIndex = (sampleIndex + 1) % samples.length;
  dataInput.value = samples[sampleIndex].data;
  xInput.value = samples[sampleIndex].x;
  render();
});

applyLanguage();
render();
