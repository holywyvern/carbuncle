const path = require("path");

const basePath = process.env.NODE_ENV === "production" ? "/carbuncle" : "";

module.exports = {
  locales: ["en", "es"],
  defaultLocale: "en",
  localePath: path.resolve("./public/static/locales"),
  detection: {
    order: ["querystring", "localStorage", "cookie", "navigator", "htmlTag"],
  },
  backend: {
    loadPath: `${basePath}/static/locales/{{lng}}/{{ns}}.json`,
  },
};
