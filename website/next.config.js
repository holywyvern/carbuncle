
const { nextI18NextRewrites } = require('next-i18next/rewrites')

const localeSubpaths = {}

const basePath = process.env.NODE_ENV === 'production' ? '/carbuncle' : '';

module.exports = {
  rewrites: async () => nextI18NextRewrites(localeSubpaths),
  env: {
    basePath
  },
  basePath,
  assetPrefix: `${basePath}/`,
  publicRuntimeConfig: {
    localeSubpaths,
  },
};
