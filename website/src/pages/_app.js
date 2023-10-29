import App from "next/app";

import { appWithTranslation } from "next-i18next";

import "./reset.global.scss";

import "@fortawesome/fontawesome-svg-core/styles.css";
import "highlight.js/styles/vs.css";

import "./styles.global.scss";

function CarbuncleApp({ Component, pageProps }) {
  return (
    <>
      <Component {...pageProps} />
    </>
  );
}

CarbuncleApp.getInitialProps = App.getInitialProps;

export default appWithTranslation(CarbuncleApp);
