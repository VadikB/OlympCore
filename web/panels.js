window.OlympCorePanels = (() => {
  function renderInputs(container, method, labels) {
    container.innerHTML = method.inputs.map((input) => {
      const label = labels.fields[input.id] || input.id;
      if (input.type === "series" || input.type === "matrix" || input.type === "text") {
        return `
          <label class="field panel-field">
            <span>${label}</span>
            <textarea data-input="${input.id}" spellcheck="false">${input.sample ?? ""}</textarea>
          </label>
        `;
      }

      if (input.type === "checkbox") {
        return `
          <label class="check-field">
            <input data-input="${input.id}" type="checkbox" ${input.sample ? "checked" : ""}>
            <span>${label}</span>
          </label>
        `;
      }

      return `
        <label class="field panel-field">
          <span>${label}</span>
          <input data-input="${input.id}" type="number" value="${input.sample ?? ""}" ${attr("min", input.min)} ${attr("max", input.max)} ${attr("step", input.step)}>
        </label>
      `;
    }).join("");
  }

  function readInputs(container) {
    return [...container.querySelectorAll("[data-input]")].reduce((values, element) => {
      values[element.dataset.input] = element.type === "checkbox" ? element.checked : element.value;
      return values;
    }, {});
  }

  function attr(name, value) {
    return value === undefined ? "" : `${name}="${value}"`;
  }

  return { renderInputs, readInputs };
})();
