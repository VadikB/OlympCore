window.OlympCoreCalculators = (() => {
  function parseNumbers(text) {
    return String(text)
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

  function percentileInc(values, percentile) {
    const sorted = [...values].sort((a, b) => a - b);
    if (sorted.length === 1) return sorted[0];
    const rank = percentile * (sorted.length - 1);
    const lower = Math.floor(rank);
    const upper = Math.ceil(rank);
    return sorted[lower] + (sorted[upper] - sorted[lower]) * (rank - lower);
  }

  function percentileExc(values, percentile) {
    const sorted = [...values].sort((a, b) => a - b);
    const rank = percentile * (sorted.length + 1);
    if (rank <= 1 || rank >= sorted.length) throw new Error("Percentile is outside exclusive range");
    const lower = Math.floor(rank) - 1;
    return sorted[lower] + (sorted[lower + 1] - sorted[lower]) * (rank - Math.floor(rank));
  }

  function variancePopulation(values) {
    const avg = mean(values);
    return values.reduce((sum, value) => sum + (value - avg) ** 2, 0) / values.length;
  }

  function covarianceSample(xs, ys) {
    const n = Math.min(xs.length, ys.length);
    const xSeries = xs.slice(0, n);
    const ySeries = ys.slice(0, n);
    const xMean = mean(xSeries);
    const yMean = mean(ySeries);
    return xSeries.reduce((sum, x, index) => sum + (x - xMean) * (ySeries[index] - yMean), 0) / (n - 1);
  }

  function pearsonCorrelation(xs, ys) {
    return covarianceSample(xs, ys) / (Math.sqrt(varianceSample(xs)) * Math.sqrt(varianceSample(ys)));
  }

  function skewnessSample(values) {
    const avg = mean(values);
    const sd = Math.sqrt(varianceSample(values));
    const n = values.length;
    const sum3 = values.reduce((sum, value) => sum + ((value - avg) / sd) ** 3, 0);
    return (n / ((n - 1) * (n - 2))) * sum3;
  }

  function kurtosisExcessSample(values) {
    const avg = mean(values);
    const sd = Math.sqrt(varianceSample(values));
    const n = values.length;
    const sum4 = values.reduce((sum, value) => sum + ((value - avg) / sd) ** 4, 0);
    return (n * (n + 1) * sum4 / ((n - 1) * (n - 2) * (n - 3)))
      - (3 * (n - 1) ** 2 / ((n - 2) * (n - 3)));
  }

  function rankAverage(values) {
    const indexed = values.map((value, index) => ({ value, index })).sort((a, b) => a.value - b.value);
    const ranks = Array(values.length);
    let i = 0;
    while (i < indexed.length) {
      let j = i + 1;
      while (j < indexed.length && indexed[j].value === indexed[i].value) j += 1;
      const rank = ((i + 1) + j) / 2;
      for (let k = i; k < j; k += 1) ranks[indexed[k].index] = rank;
      i = j;
    }
    return ranks;
  }

  function standardize(values) {
    const avg = mean(values);
    const sd = Math.sqrt(varianceSample(values));
    return values.map((value) => (value - avg) / sd);
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

  function movingAverageCumulative(values) {
    let running = 0;
    return values.map((value, index) => {
      running += value;
      return running / (index + 1);
    });
  }

  function movingAverageExponential(values, alpha) {
    if (alpha <= 0 || alpha > 1) throw new Error("Alpha must be in (0, 1]");
    const result = [values[0]];
    for (let i = 1; i < values.length; i += 1) {
      result.push(alpha * values[i] + (1 - alpha) * result[i - 1]);
    }
    return result;
  }

  function zScoreOutliers(values, threshold) {
    const avg = mean(values);
    const stdev = Math.sqrt(varianceSample(values));
    if (stdev === 0) return [];
    return values
      .map((value, index) => ({ index, value, z_score: (value - avg) / stdev }))
      .filter((item) => Math.abs(item.z_score) >= threshold);
  }

  function calculate(methodId, values) {
    const series = parseNumbers(values.values);
    const xs = parseNumbers(values.xValues);
    const ys = parseNumbers(values.yValues);

    if (methodId === "summary") {
      const variance = varianceSample(series);
      return {
        metrics: { count: series.length, min: Math.min(...series), max: Math.max(...series), mean: mean(series), median: median(series), variance, stdev: Math.sqrt(variance) },
        json: { count: series.length, min: Math.min(...series), max: Math.max(...series), mean: mean(series), median: median(series), variance_sample: variance, stdev_sample: Math.sqrt(variance) },
        chart: { type: "bars", values: series }
      };
    }

    if (methodId === "average") {
      return metricOnly({ mean: mean(series) }, series);
    }

    if (methodId === "median") {
      return metricOnly({ median: median(series) }, series);
    }

    if (methodId === "quantile_inc") {
      const percentile = Number(values.percentile);
      return metricOnly({ percentile: percentileInc(series, percentile) }, series, { percentile, value: percentileInc(series, percentile) });
    }

    if (methodId === "quantile_exc") {
      const percentile = Number(values.percentile);
      return metricOnly({ percentile: percentileExc(series, percentile) }, series, { percentile, value: percentileExc(series, percentile) });
    }

    if (methodId === "percentrank") {
      const x = Number(values.x);
      const sorted = [...series].sort((a, b) => a - b);
      const below = sorted.filter((value) => value < x).length;
      const equal = sorted.filter((value) => value === x).length;
      const rawRank = equal ? (below + (equal - 1) / 2) / (sorted.length - 1) : below / (sorted.length - 1);
      const rank = Math.max(0, Math.min(1, rawRank));
      return metricOnly({ percentile: rank }, series, { x, percent_rank: rank });
    }

    if (methodId === "variance") {
      return metricOnly({ variance: varianceSample(series), populationVariance: variancePopulation(series) }, series, {
        variance_sample: varianceSample(series),
        variance_population: variancePopulation(series)
      });
    }

    if (methodId === "stdev") {
      return metricOnly({ stdev: Math.sqrt(varianceSample(series)), populationStdev: Math.sqrt(variancePopulation(series)) }, series, {
        stdev_sample: Math.sqrt(varianceSample(series)),
        stdev_population: Math.sqrt(variancePopulation(series))
      });
    }

    if (methodId === "cov_pearson") {
      const n = Math.min(xs.length, ys.length);
      const xSeries = xs.slice(0, n);
      const ySeries = ys.slice(0, n);
      return {
        metrics: { covariance: covarianceSample(xSeries, ySeries), pearson: pearsonCorrelation(xSeries, ySeries), points: n },
        json: { covariance_sample: covarianceSample(xSeries, ySeries), pearson: pearsonCorrelation(xSeries, ySeries), points: n },
        chart: { type: "regression", xs: xSeries, ys: ySeries, result: regression(xSeries, ySeries) }
      };
    }

    if (methodId === "moments") {
      const avg = mean(series);
      const central2 = series.reduce((sum, value) => sum + (value - avg) ** 2, 0) / series.length;
      const central3 = series.reduce((sum, value) => sum + (value - avg) ** 3, 0) / series.length;
      return metricOnly({ skewness: skewnessSample(series), kurtosis: kurtosisExcessSample(series), moment2: central2, moment3: central3 }, series, {
        skewness_sample: skewnessSample(series),
        kurtosis_excess_sample: kurtosisExcessSample(series),
        central_moment_2: central2,
        central_moment_3: central3
      });
    }

    if (methodId === "rank") {
      const ranks = rankAverage(series);
      return { metrics: { min: Math.min(...ranks), max: Math.max(...ranks), values: ranks.length }, json: ranks, chart: { type: "bars", values: ranks } };
    }

    if (methodId === "order_stats") {
      const k = Math.max(1, Math.min(Math.round(Number(values.x)), series.length));
      const sorted = [...series].sort((a, b) => a - b);
      return metricOnly({ min: sorted[0], max: sorted[sorted.length - 1], range: sorted[sorted.length - 1] - sorted[0], kthSmall: sorted[k - 1], kthLarge: sorted[sorted.length - k] }, series, {
        min: sorted[0],
        max: sorted[sorted.length - 1],
        range: sorted[sorted.length - 1] - sorted[0],
        kth_small: sorted[k - 1],
        kth_large: sorted[sorted.length - k]
      });
    }

    if (methodId === "deviation_means") {
      const avg = mean(series);
      const aveDev = mean(series.map((value) => Math.abs(value - avg)));
      const devSq = series.reduce((sum, value) => sum + (value - avg) ** 2, 0);
      if (series.some((value) => value <= 0)) {
        throw new Error("Geometric and harmonic means require positive values");
      }
      const geo = Math.exp(mean(series.map((value) => Math.log(value))));
      const harmonic = series.length / series.reduce((sum, value) => sum + 1 / value, 0);
      return metricOnly({ averageDeviation: aveDev, devsq: devSq, geometricMean: geo, harmonicMean: harmonic }, series, {
        average_deviation: aveDev,
        devsq: devSq,
        geometric_mean: geo,
        harmonic_mean: harmonic
      });
    }

    if (methodId === "linear_regression") {
      const n = Math.min(xs.length, ys.length);
      const xSeries = xs.slice(0, n);
      const ySeries = ys.slice(0, n);
      const result = regression(xSeries, ySeries);
      return {
        metrics: { slope: result.slope, intercept: result.intercept, rSquared: result.r_squared, points: n },
        json: result,
        chart: { type: "regression", xs: xSeries, ys: ySeries, result }
      };
    }

    if (["slope", "intercept", "forecast", "steyx"].includes(methodId)) {
      const n = Math.min(xs.length, ys.length);
      const xSeries = xs.slice(0, n);
      const ySeries = ys.slice(0, n);
      const result = regression(xSeries, ySeries);
      const residuals = xSeries.map((x, index) => ySeries[index] - (result.slope * x + result.intercept));
      const steyx = Math.sqrt(residuals.reduce((sum, value) => sum + value * value, 0) / (n - 2));
      const forecastX = methodId === "forecast" ? Number(values.x) : null;
      const forecast = forecastX == null ? null : result.slope * forecastX + result.intercept;
      const metricMap = {
        slope: { slope: result.slope },
        intercept: { intercept: result.intercept },
        forecast: { forecast },
        steyx: { steyx }
      };
      return {
        metrics: { ...metricMap[methodId], points: n },
        json: {
          slope: result.slope,
          intercept: result.intercept,
          ...(forecastX == null ? {} : { forecast_x: forecastX, forecast }),
          steyx,
          r_squared: result.r_squared
        },
        chart: { type: "regression", xs: xSeries, ys: ySeries, result }
      };
    }

    if (methodId === "normal_distribution") {
      const x = Number(values.x);
      const avg = Number(values.mean);
      const stdev = Number(values.stdev);
      const pdf = normalPdf(x, avg, stdev);
      const cdf = normalCdf(x, avg, stdev);
      return {
        metrics: { pdf, cdf, mean: avg, stdev },
        json: { x, mean: avg, stdev, pdf, cdf },
        chart: { type: "normal", mean: avg, stdev, x }
      };
    }

    if (methodId === "simple_moving_average") {
      const windowSize = Math.max(1, Math.min(Number(values.window) || 1, series.length));
      const smoothed = movingAverage(series, windowSize);
      return {
        metrics: { first: smoothed[0], last: smoothed[smoothed.length - 1], values: smoothed.length },
        json: smoothed,
        chart: { type: "smoothing", values: series, smoothed, windowSize }
      };
    }

    if (methodId === "cumulative_moving_average") {
      const smoothed = movingAverageCumulative(series);
      return { metrics: { first: smoothed[0], last: smoothed[smoothed.length - 1], values: smoothed.length }, json: smoothed, chart: { type: "smoothing", values: series, smoothed, windowSize: 1 } };
    }

    if (methodId === "exponential_moving_average") {
      const alpha = Number(values.alpha);
      const smoothed = movingAverageExponential(series, alpha);
      return { metrics: { alpha, first: smoothed[0], last: smoothed[smoothed.length - 1], values: smoothed.length }, json: smoothed, chart: { type: "smoothing", values: series, smoothed, windowSize: 1 } };
    }

    if (methodId === "standardize") {
      const result = standardize(series);
      return { metrics: { mean: mean(result), stdev: Math.sqrt(varianceSample(result)), values: result.length }, json: result, chart: { type: "bars", values: result } };
    }

    if (methodId === "z_score_outliers") {
      const threshold = Number(values.threshold);
      const result = zScoreOutliers(series, threshold);
      return {
        metrics: { threshold, found: result.length, mean: mean(series), stdev: Math.sqrt(varianceSample(series)) },
        json: result,
        chart: { type: "outliers", values: series, outliers: result }
      };
    }

    throw new Error(`No calculator registered for ${methodId}`);
  }

  function metricOnly(metrics, series, json = metrics) {
    return { metrics, json, chart: { type: "bars", values: series } };
  }

  return { calculate, parseNumbers, normalPdf };
})();
