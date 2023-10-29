const localeSubpaths = {};

const i18n = require("./i18n");

const basePath = process.env.NODE_ENV === "production" ? "/carbuncle" : "";

module.exports = {
  i18n,
  env: {
    basePath,
  },
  basePath,
  assetPrefix: `${basePath}/`,
  publicRuntimeConfig: {
    localeSubpaths,
  },
};
