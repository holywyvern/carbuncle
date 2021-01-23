const withOptimizedImages = require('next-optimized-images');

const basePath = process.env.NODE_ENV === 'production' ? '/carbuncle' : '';

module.exports = withOptimizedImages({
  env: {
    basePath
  },
  basePath,
  assetPrefix: `${basePath}/`
});
