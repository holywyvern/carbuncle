import { FontAwesomeIcon } from "@fortawesome/react-fontawesome";
import {
  faWindows,
  faLinux,
  faApple,
} from "@fortawesome/free-brands-svg-icons";
import { faGlobe } from "@fortawesome/free-solid-svg-icons";
import { withTranslation } from "next-i18next";

import Panel from "../panel";

import styles from "./styles.module.scss";

const Platform = withTranslation("common")(
  ({ t, icon, title, download, version }) => {
    return (
      <div className={styles.platform}>
        <FontAwesomeIcon icon={icon} size="4x" />
        <h4>{t(title)}</h4>
        <a href={download}>{version}</a>
      </div>
    );
  }
);

function DownloadSection({ t }) {
  return (
    <Panel stretch>
      <h3>{t("Downloads")}</h3>
      <div className={styles.downloads}>
        <Platform
          icon={faWindows}
          title="Windows Builds"
          download="https://github.com/holywyvern/carbuncle/releases/download/0.6.1/win32-x64.zip"
          version="0.6.1 (x64)"
        />
        <Platform icon={faApple} title="OS X Builds" version="(Pending)" />
        <Platform icon={faLinux} title="Linux Builds" version="(Pending)" />
        <Platform
          icon={faGlobe}
          title="Web Builds"
          download="https://github.com/holywyvern/carbuncle/releases/download/0.6.1/web.zip"
          version="0.6.1"
        />
      </div>
    </Panel>
  );
}

export default withTranslation("common")(DownloadSection);
