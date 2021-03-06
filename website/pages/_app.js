import App from 'next/app';
import Head from "next/head";

import { appWithTranslation } from '../i18n';

import './reset.global.scss';

import '@fortawesome/fontawesome-svg-core/styles.css';
import 'highlight.js/styles/vs.css';

import './styles.global.scss';

function CarbuncleApp({ Component, pageProps }) {
  return (
    <>
      <Head>
        <meta name="viewport" content="width=device-width, initial-scale=1.0" />
        <link
          href="https://fonts.googleapis.com/css?family=Montserrat|Raleway:500|Roboto+Mono|Fredericka+the+Great&display=fallback" rel="stylesheet"
        />
        <link
          href="https://cdnjs.cloudflare.com/ajax/libs/font-awesome/5.15.2/css/all.min.css"
        />
        <link rel="shortcut icon" href="https://holywyvern.github.io/carbuncle/favicon.ico" />
      </Head>
      <Component {...pageProps} />
    </>
  );
}

CarbuncleApp.getInitialProps = App.getInitialProps;

export default appWithTranslation(CarbuncleApp);
