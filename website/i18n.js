const NextI18Next = require('next-i18next').default;
const { localeSubpaths } = require('next/config').default().publicRuntimeConfig;
const path = require('path');

module.exports = new NextI18Next({
  whitelist: ['en', 'es'],
  otherLanguages: ['es'],
  localeSubpaths,
  localePath: path.resolve('./public/static/locales'),
  saveMissingTo: "current",
  fallbackLng: process.NODE_ENV !== 'production' ? false : 'en',
  saveMissing: process.NODE_ENV !== 'production'
})
