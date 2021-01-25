import { FontAwesomeIcon }  from "@fortawesome/react-fontawesome";
import { faWindows, faLinux, faApple } from '@fortawesome/free-brands-svg-icons';
import { faGlobe } from '@fortawesome/free-solid-svg-icons';
import { withTranslation } from '../../i18n';

import Panel from "../panel"

import styles from "./styles.module.scss";

function DownloadSection({ t }) {
  return (
    <Panel stretch>
      <h3>{t('Downloads')}</h3>
      <div className={styles.downloads}>
        <div className={styles.platform}>
          <FontAwesomeIcon icon={faWindows} size="4x"/>
          <h4>{t('Windows Builds')}</h4>
          <a href="https://github.com/holywyvern/carbuncle/releases/download/0.6.0/win32-x64.zip">
            0.6.0 (x64)
          </a>
        </div>
        <div className={styles.platform}>
          <FontAwesomeIcon icon={faApple} size="4x"/>
          <h4>{t('OS X Builds')}</h4>
          <a>{t('(Pending)')}</a>
        </div>
        <div className={styles.platform}>
          <FontAwesomeIcon icon={faLinux} size="4x"/>
          <h4>{t('Linux Builds')}</h4>
          <a>{t('(Pending)')}</a>
        </div>
        <div className={styles.platform}>
          <FontAwesomeIcon icon={faGlobe} size="4x"/>
          <h4>{t('Web Builds')}</h4>
          <a href="https://github.com/holywyvern/carbuncle/releases/download/0.6.0/web.zip">
            0.6.0
          </a>
        </div>
      </div>
    </Panel>
  )
}

export default withTranslation('common')(DownloadSection);
