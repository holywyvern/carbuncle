const basePath = process.env.NODE_ENV === 'production' ? '/carbuncle' : '';

module.exports = {
  env: {
    basePath
  },
  basePath,
  assetPrefix: `${basePath}/`
};
