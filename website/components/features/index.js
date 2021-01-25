import Feature from "./feature";

import styles from "./styles.module.scss";

import { withTranslation } from '../../i18n';

function Features({ t }) {
  return (
    <div className={styles.features}>
      <Feature title="An Open Source Framework" logo="faGithub">
        {t('Completeley Open Source, and free to use.')}
      </Feature>
      <Feature title="Out of the Box" logo="faBoxOpen">
        {t('Contains many features including graphics, audio, input, networking and GUI!')}
      </Feature>
      <Feature title="Cross Platform" logo="faLaptopCode">
        {t('Works on Windows, Mac OSX and Linux with planned iOS and Android support.')}
      </Feature>
      <Feature title="Web Support" logo="faGlobe">
        {t('Deploy it as a website without much effort thanks to')}&nbsp;
        <a href="https://emscripten.org/" target="_blank">Emscripten</a>.
      </Feature>
    </div>
  )
}

export default withTranslation('common')(Features);
