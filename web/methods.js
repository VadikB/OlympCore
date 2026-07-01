window.OlympCoreMethods = (() => {
  const i18n = {
    ru: {
      docTitle: "OlympCore Демо",
      tagline: "Демо математических методов C++",
      searchLabel: "Поиск метода",
      groupLabel: "Группа",
      statusLabel: "Статус",
      parametersTitle: "Параметры",
      runMethod: "Выполнить",
      sampleTitle: "Загрузить пример",
      chartLabel: "График",
      catalogEyebrow: "Карта библиотеки",
      catalogTitle: "Методы OlympCore",
      filterAll: "Все группы",
      statusAll: "Все статусы",
      readyStatus: "готово",
      portingStatus: "портинг",
      readyCount: "Интерактивно",
      portingCount: "К переносу",
      totalCount: "Всего",
      sourceLabel: "Источник",
      notPorted: "Метод найден в старом коде, но ещё не перенесён в новый portable-слой.",
      noMethods: "Методы не найдены",
      resultTitle: "Результат",
      fields: {
        values: "Данные",
        xValues: "Значения X",
        yValues: "Значения Y",
        x: "X",
        mean: "Среднее",
        stdev: "Ст. откл.",
        probability: "Вероятность",
        percentile: "Процентиль",
        window: "Окно",
        threshold: "Порог",
        alpha: "Alpha",
        tail: "Хвост",
        cumulative: "Накопительная",
        sampleA: "Выборка A",
        sampleB: "Выборка B",
        categories: "Категории",
        matrix: "Матрица"
      },
      metrics: {
        count: "Кол-во",
        min: "Мин",
        max: "Макс",
        mean: "Среднее",
        median: "Медиана",
        variance: "Дисперсия",
        stdev: "Ст. откл.",
        slope: "Наклон",
        intercept: "Сдвиг",
        rSquared: "R квадрат",
        points: "Точки",
        pdf: "PDF",
        cdf: "CDF",
        first: "Первое",
        last: "Последнее",
        values: "Значения",
        found: "Найдено",
        percentile: "Процентиль",
        zScore: "Z-score"
      },
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
      searchLabel: "Search method",
      groupLabel: "Group",
      statusLabel: "Status",
      parametersTitle: "Parameters",
      runMethod: "Run",
      sampleTitle: "Load sample data",
      chartLabel: "Chart",
      catalogEyebrow: "Library map",
      catalogTitle: "OlympCore Methods",
      filterAll: "All groups",
      statusAll: "All statuses",
      readyStatus: "ready",
      portingStatus: "porting",
      readyCount: "Interactive",
      portingCount: "To port",
      totalCount: "Total",
      sourceLabel: "Source",
      notPorted: "This method exists in the legacy code but is not yet ported to the new portable layer.",
      noMethods: "No methods found",
      resultTitle: "Result",
      fields: {
        values: "Data",
        xValues: "X values",
        yValues: "Y values",
        x: "X",
        mean: "Mean",
        stdev: "Stdev",
        probability: "Probability",
        percentile: "Percentile",
        window: "Window",
        threshold: "Threshold",
        alpha: "Alpha",
        tail: "Tail",
        cumulative: "Cumulative",
        sampleA: "Sample A",
        sampleB: "Sample B",
        categories: "Categories",
        matrix: "Matrix"
      },
      metrics: {
        count: "Count",
        min: "Min",
        max: "Max",
        mean: "Mean",
        median: "Median",
        variance: "Variance",
        stdev: "Stdev",
        slope: "Slope",
        intercept: "Intercept",
        rSquared: "R squared",
        points: "Points",
        pdf: "PDF",
        cdf: "CDF",
        first: "First",
        last: "Last",
        values: "Values",
        found: "Found",
        percentile: "Percentile",
        zScore: "Z-score"
      },
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

  const fieldPresets = {
    series: [{ id: "values", type: "series", sample: "10, 11, 12, 14, 15, 18, 21, 22, 26, 30" }],
    paired: [
      { id: "xValues", type: "series", sample: "1, 2, 3, 4, 5, 6, 7, 8, 9, 10" },
      { id: "yValues", type: "series", sample: "10, 11, 12, 14, 15, 18, 21, 22, 26, 30" }
    ],
    twoSamples: [
      { id: "sampleA", type: "series", sample: "8, 9, 10, 11, 12" },
      { id: "sampleB", type: "series", sample: "10, 11, 13, 14, 15" },
      { id: "alpha", type: "number", sample: "0.05", min: "0", max: "1", step: "0.01" }
    ],
    distribution: [
      { id: "x", type: "number", sample: "0" },
      { id: "mean", type: "number", sample: "0" },
      { id: "stdev", type: "number", sample: "1", min: "0.0001", step: "0.1" },
      { id: "cumulative", type: "checkbox", sample: true }
    ]
  };

  const groups = [
    {
      id: "descriptive",
      methods: [
        method("summary", "Summary statistics", "ready", "count, min, max, mean, median, sample variance and standard deviation.", "количество, минимум, максимум, среднее, медиана, выборочная дисперсия и отклонение.", "web + src/stats.cpp", fieldPresets.series),
        method("average", "Average", "ready", "Mean over all values.", "среднее по всем значениям.", "src/stats.cpp", fieldPresets.series),
        method("median", "Median", "ready", "Median value for a numeric sample.", "медиана числовой выборки.", "src/stats.cpp", fieldPresets.series),
        method("quantile_inc", "Quantile inclusive", "ready", "Inclusive percentile interpolation.", "инклюзивная интерполяция процентиля.", "src/stats.cpp", [...fieldPresets.series, { id: "percentile", type: "number", sample: "0.25", min: "0", max: "1", step: "0.01" }]),
        method("quantile_exc", "Quantile exclusive", "porting", "Exclusive quantile calculation.", "эксклюзивный расчёт квантиля.", "Stats/Descriptive/quantile_exc.*", [...fieldPresets.series, { id: "percentile", type: "number", sample: "0.25", min: "0", max: "1", step: "0.01" }]),
        method("percentrank", "Percent rank inc/exc", "porting", "Inclusive and exclusive percentile rank.", "инклюзивный и эксклюзивный процентный ранг.", "Stats/Descriptive/percentrank_*", [...fieldPresets.series, { id: "x", type: "number", sample: "14" }]),
        method("variance", "Variance sample/population", "ready", "Sample variance is interactive now; population variance is queued.", "выборочная дисперсия уже интерактивна; генеральная в очереди.", "src/stats.cpp, Stats/Descriptive/var_*", fieldPresets.series),
        method("stdev", "Standard deviation sample/population", "ready", "Sample standard deviation is interactive now; population version is queued.", "выборочное стандартное отклонение уже интерактивно; генеральное в очереди.", "src/stats.cpp, Stats/Descriptive/stdev_*", fieldPresets.series),
        method("cov_pearson", "Covariance and Pearson", "porting", "Covariance, population covariance, Pearson correlation.", "ковариация, ковариация генеральной совокупности, корреляция Пирсона.", "Stats/Descriptive/cov*, pearson.*", fieldPresets.paired),
        method("moments", "Skew, kurtosis, moments", "porting", "Shape metrics and raw/central moments.", "метрики формы распределения и начальные/центральные моменты.", "Stats/Descriptive/skew.*, kurt.*, moment*", fieldPresets.series),
        method("rank", "Rank average/equal", "porting", "Ranking with average or equal-rank handling.", "ранжирование со средними или равными рангами.", "Stats/Descriptive/rank_*", fieldPresets.series),
        method("order_stats", "Large, small, range", "porting", "Order statistics and numeric range.", "порядковые статистики и размах.", "Stats/Descriptive/large.*, small.*, range.*", [...fieldPresets.series, { id: "x", type: "number", sample: "2" }]),
        method("anova", "ANOVA", "porting", "Single-factor, two-factor and repeated ANOVA.", "однофакторный, двухфакторный и повторный ANOVA.", "Stats/Descriptive/anova_*", [{ id: "matrix", type: "matrix", sample: "8,9,6\n7,10,8\n9,11,7" }, { id: "alpha", type: "number", sample: "0.05", step: "0.01" }]),
        method("crosstabulate", "Crosstabulate numbers", "porting", "Numeric crosstabulation.", "числовая перекрёстная таблица.", "Stats/Descriptive/crosstabulate_numbers.*", [{ id: "categories", type: "text", sample: "A, B, A, C, B" }, ...fieldPresets.series]),
        method("deviation_means", "AveDev, DevSq, Geo/Harmonic mean", "porting", "Average deviation, sum of square deviations, geometric and harmonic means.", "среднее отклонение, сумма квадратов отклонений, геометрическое и гармоническое среднее.", "Stats/Descriptive/avedev.*, devsq.*, geoharmean.*", fieldPresets.series)
      ]
    },
    {
      id: "distributions",
      methods: [
        method("normal_distribution", "Normal distribution", "ready", "Normal PDF and CDF.", "нормальные PDF и CDF.", "src/stats.cpp, Stats/dist/norm.*", fieldPresets.distribution),
        ...["Beta", "Binomial", "Bernoulli", "Cauchy", "Chi-square", "Exponential", "F and Fisher", "Gamma", "Geometric", "GEV, Gumbel, Rayleigh", "Hypergeometric", "Laplace, Logistic, Lognormal", "Negative binomial, Pareto, Poisson", "Student t, Uniform, Weibull"].map((name) =>
          method(slug(name), name, "porting", `${name} distribution helpers and random sampling.`, `${name}: функции распределения и генерация выборок.`, `Stats/dist/${slug(name)}*`, fieldPresets.distribution)
        )
      ]
    },
    group("inference", [
      ["z_test", "Z-test", "Z hypothesis test.", "Z-критерий.", "Stats/Inference/z_test.*", fieldPresets.twoSamples],
      ["t_test", "T-test", "Student t-test.", "t-критерий Стьюдента.", "Stats/Inference/t_test.*", fieldPresets.twoSamples],
      ["chisq_test", "Chi-square test", "Chi-square hypothesis test.", "критерий хи-квадрат.", "Stats/Inference/chisq_test.*", [{ id: "matrix", type: "matrix", sample: "10,12\n8,11" }]],
      ["fisher_test", "Fisher exact test", "Fisher exact test.", "точный критерий Фишера.", "Stats/Inference/fisher_test.*", [{ id: "matrix", type: "matrix", sample: "1,9\n11,3" }]],
      ["ks_test", "Kolmogorov-Smirnov", "One-sample and two-sample KS tests.", "одновыборочный и двухвыборочный критерии Колмогорова-Смирнова.", "Stats/Inference/ks*_test.*", fieldPresets.twoSamples],
      ["mw_test", "Mann-Whitney", "Mann-Whitney U test.", "критерий Манна-Уитни.", "Stats/Inference/mw_test.*", fieldPresets.twoSamples],
      ["wsr_test", "Wilcoxon signed-rank", "Wilcoxon signed-rank test.", "знаково-ранговый критерий Уилкоксона.", "Stats/Inference/wsr_test.*", fieldPresets.twoSamples]
    ]),
    {
      id: "regression",
      methods: [
        method("linear_regression", "Linear regression", "ready", "Slope, intercept and R squared.", "наклон, сдвиг и R квадрат.", "src/stats.cpp", fieldPresets.paired),
        method("slope", "Slope", "porting", "Legacy slope operator.", "старый оператор наклона.", "Stats/Regression/slope.*", fieldPresets.paired),
        method("intercept", "Intercept", "porting", "Legacy intercept operator.", "старый оператор свободного члена.", "Stats/Regression/intercept.*", fieldPresets.paired),
        method("forecast", "Forecast", "porting", "Linear forecast from known X/Y values.", "линейный прогноз по известным X/Y.", "Stats/Regression/forecast.*", [...fieldPresets.paired, { id: "x", type: "number", sample: "11" }]),
        method("steyx", "STEYX", "porting", "Standard error of predicted Y.", "стандартная ошибка предсказанного Y.", "Stats/Regression/steyx.*", fieldPresets.paired),
        method("naive_bayes", "Naive Bayes", "porting", "Classification helper.", "вспомогательный классификатор.", "Stats/Regression/naive_bayes.cpp", [{ id: "matrix", type: "matrix", sample: "1,0,1\n0,1,0\n1,1,1" }])
      ]
    },
    {
      id: "smoothing",
      methods: [
        method("simple_moving_average", "Simple moving average", "ready", "Interactive rolling average by window.", "интерактивное скользящее среднее по окну.", "src/stats.cpp", [...fieldPresets.series, { id: "window", type: "number", sample: "3", min: "1", step: "1" }]),
        method("cumulative_moving_average", "Cumulative moving average", "porting", "Cumulative average over time.", "накопительное среднее по времени.", "Stats/Smoothing/moving_average_cumulative.*", fieldPresets.series),
        method("weighted_moving_average", "Weighted moving average", "porting", "Weighted rolling average.", "взвешенное скользящее среднее.", "Stats/Smoothing/moving_average_weighted.*", fieldPresets.series),
        method("triangular_moving_average", "Triangular moving average", "porting", "Triangular smoothing window.", "треугольное сглаживающее окно.", "Stats/Smoothing/moving_average_triangular.*", fieldPresets.series),
        method("exponential_moving_average", "Exponential moving average", "porting", "EMA smoothing.", "экспоненциальное скользящее среднее.", "Stats/Smoothing/moving_average_exponential.*", [...fieldPresets.series, { id: "alpha", type: "number", sample: "0.3", min: "0", max: "1", step: "0.01" }])
      ]
    },
    group("timeSeries", [
      ["adf_test", "ADF test", "Augmented Dickey-Fuller stationarity test.", "расширенный тест Дики-Фуллера на стационарность.", "Stats/TimeSeries/adf_test.*", fieldPresets.series],
      ["acf", "Autocorrelation ACF", "Autocorrelation function.", "автокорреляционная функция.", "Stats/TimeSeries/timeseries_acf.cpp", fieldPresets.series],
      ["pacf", "Partial autocorrelation PACF", "Partial autocorrelation function.", "частная автокорреляция.", "Stats/TimeSeries/timeseries_pacf.cpp", fieldPresets.series],
      ["ar", "Autoregression AR", "Autoregressive model helpers.", "вспомогательные функции авторегрессии.", "Stats/TimeSeries/timeseries_ar.*", fieldPresets.series],
      ["aicbic", "AIC/BIC", "Information criteria helpers.", "информационные критерии.", "Stats/TimeSeries/timeseries_aicbic.cpp", fieldPresets.series],
      ["time_series_forecast", "Time series forecast", "Forecast helpers for time series.", "прогнозирование временных рядов.", "Stats/TimeSeries/time_series_forecast.cpp", fieldPresets.series]
    ]),
    group("mle", [
      ["norm_fit", "Normal fit", "Maximum-likelihood normal fit.", "MLE-подгонка нормального распределения.", "Stats/MLE/norm_fit.*", fieldPresets.series],
      ["expon_fit", "Exponential fit", "Maximum-likelihood exponential fit.", "MLE-подгонка экспоненциального распределения.", "Stats/MLE/expon_fit.*", fieldPresets.series],
      ["poisson_fit", "Poisson fit", "Maximum-likelihood Poisson fit.", "MLE-подгонка распределения Пуассона.", "Stats/MLE/poisson_fit.*", fieldPresets.series],
      ["binom_fit", "Binomial fit", "Maximum-likelihood binomial fit.", "MLE-подгонка биномиального распределения.", "Stats/MLE/binom_fit.*", fieldPresets.series]
    ]),
    {
      id: "outliers",
      methods: [
        method("z_score_outliers", "Z-score outliers", "ready", "Interactive outlier detection by z-score threshold.", "интерактивный поиск выбросов по z-score.", "src/stats.cpp", [...fieldPresets.series, { id: "threshold", type: "number", sample: "2", min: "0.1", step: "0.1" }]),
        method("esd", "ESD", "porting", "Extreme studentized deviate test.", "критерий экстремального студентизированного отклонения.", "Stats/Outliers/esd.*", [...fieldPresets.series, { id: "alpha", type: "number", sample: "0.05", step: "0.01" }]),
        method("dixon_q", "Dixon Q", "porting", "Dixon Q outlier test.", "Q-критерий Диксона для выбросов.", "Stats/Outliers/dq_test.*", fieldPresets.series)
      ]
    },
    group("imputation", [
      ["mean_imputation", "Mean imputation", "Fill missing values with means.", "заполнение пропусков средними.", "Stats/Imputation/imputation_mean.*", fieldPresets.series],
      ["match_imputation", "Match imputation", "Match-based imputation.", "заполнение пропусков по совпадениям.", "Stats/Imputation/imputation_match.cpp", [{ id: "matrix", type: "matrix", sample: "1,,3\n2,4,\n,5,6" }]],
      ["lvcf", "LVCF", "Last value carried forward.", "перенос последнего известного значения вперёд.", "Stats/Imputation/lvcf.*", fieldPresets.series]
    ]),
    group("processing", [
      ["standardize", "Standardize", "Data standardization.", "стандартизация данных.", "Stats/DataProcessing/standardize.cpp", fieldPresets.series]
    ]),
    group("utilities", [
      ["random_permutation", "Random permutation", "Random permutation utility.", "утилита случайной перестановки.", "Stats/Utilities/rand_perm.*", fieldPresets.series],
      ["rng_helpers", "RNG helpers", "Random generation utilities.", "вспомогательные функции генерации случайных чисел.", "Stats/dist/TCRNGUtils.*", [{ id: "values", type: "series", sample: "10" }]]
    ])
  ];

  function method(id, name, status, en, ru, source, inputs) {
    return { id, name, status, description: { en, ru }, source, inputs };
  }

  function group(id, rows) {
    return {
      id,
      methods: rows.map(([methodId, name, en, ru, source, inputs]) => method(methodId, name, "porting", en, ru, source, inputs))
    };
  }

  function slug(value) {
    return value.toLowerCase().replace(/[^a-z0-9]+/g, "_").replace(/^_|_$/g, "");
  }

  const methods = groups.flatMap((groupItem) => groupItem.methods.map((methodItem) => ({
    ...methodItem,
    group: groupItem.id
  })));

  return { groups, methods, i18n };
})();
