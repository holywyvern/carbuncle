import { FontAwesomeIcon }  from "@fortawesome/react-fontawesome";
import { faWindows, faLinux, faApple } from '@fortawesome/free-brands-svg-icons';
import { faGlobe } from '@fortawesome/free-solid-svg-icons';

import Panel from "../panel"

import styles from "./styles.module.scss";

function DownloadSection() {
  return (
    <Panel stretch>
      <h3>Downloads</h3>
      <div className={styles.downloads}>
        <div className={styles.platform}>
          <FontAwesomeIcon icon={faWindows} size="4x"/>
          <h4>Windows Builds</h4>
          <a href="https://github.com/holywyvern/carbuncle/releases/download/0.5.1/win32-x64.zip">
            0.5.1 (x64)
          </a>
        </div>
        <div className={styles.platform}>
          <FontAwesomeIcon icon={faApple} size="4x"/>
          <h4>OS X Builds</h4>
          <a>(Pending)</a>
        </div>
        <div className={styles.platform}>
          <FontAwesomeIcon icon={faLinux} size="4x"/>
          <h4>Linux Builds</h4>
          <a>(Pending)</a>
        </div>
        <div className={styles.platform}>
          <FontAwesomeIcon icon={faGlobe} size="4x"/>
          <h4>Web Builds</h4>
          <a>(Pending)</a>
        </div>
      </div>
    </Panel>
  )
}

export default DownloadSection;
