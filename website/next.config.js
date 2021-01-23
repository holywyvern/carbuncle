const basePath = process.env.NODE_ENV === 'production' ? '/carbuncle' : '';

module.exports = {
  basePath,
  assetPrefix: `${basePath}/`
};
