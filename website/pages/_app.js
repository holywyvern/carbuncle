import Head from "next/head";

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
      </Head>
      <Component {...pageProps} />
    </>
  );
}

export default CarbuncleApp;
