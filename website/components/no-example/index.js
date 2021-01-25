import { withTranslation } from '../../i18n';

import Panel from "../panel"

import styles from "./styles.module.scss";

function NoExample({ t }) {
  return (
    <div className={styles.wrapper}>
      <div className={styles.view}>
        <Panel>
          {t('No example selected.')}<br />
          {t('Select one from the sidebar.')}
        </Panel>
      </div>
    </div>
  );
}

export default withTranslation('examples')(NoExample);
