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

    if (methodId === "variance") {
      return metricOnly({ variance: varianceSample(series) }, series);
    }

    if (methodId === "stdev") {
      return metricOnly({ stdev: Math.sqrt(varianceSample(series)) }, series);
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
