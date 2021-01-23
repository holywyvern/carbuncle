import Head from "next/head";

import './reset.global.scss';
import './styles.global.scss';
import '@fortawesome/fontawesome-svg-core/styles.css';

function CarbuncleApp({ Component, pageProps }) {
  return (
    <>
      <Head>
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
