const path = require("path");

const basePath = process.env.NODE_ENV === "production" ? "/carbuncle" : "";

module.exports = {
  locales: ["en", "es"],
  defaultLocale: "en",
  react: { useSuspense: false },
};
