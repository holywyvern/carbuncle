import styles from "./styles.module.scss";
import { withTranslation } from '../../i18n';

import Link from "./link";
import Image from '../image';

function Header({ t }) {
  return (
    <header className={styles.header}>
      <Image className={styles.logo} src="/img/logo.svg" alt="Carbuncle Logo" width='auto' height='auto' />
      <div className={styles.container}>
        <h1 className={styles.title}>
          Ca<span>r</span>buncle
          <sup className={styles.sup}>beta</sup>
        </h1>
        <div className={styles.subtitle}>
          <h2>Game Engine</h2>
          <div className={styles.links}>
            <Link to="https://github.com/holywyvern/carbuncle">{t('Source')}</Link>
            <Link to={`${process.env.basePath}/examples`}>{t('Examples')}</Link>
          </div>
        </div>
      </div>
    </header>
  )
}

export default withTranslation('common')(Header);
