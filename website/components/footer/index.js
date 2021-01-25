import styles from "./styles.module.scss";
import { withTranslation } from '../../i18n';

function Footer({ t }) {
  return (
    <footer className={styles.footer}>
      <div className={styles.content}>
        <section>
          <h4>{t('Special Thanks')}</h4>
          <ul>
            <li><a href="https://mruby.org/" target="_blank">MRuby</a></li>
            <li><a href="https://www.raylib.com/" target="_blank">Raylib</a></li>
          </ul>
        </section>
        <section>
          <header>
            <h4>{t('Developers')}</h4>
          </header>
          <ul>
            <li><a href="https://ramirorojo.com/" target="_blank">Ramiro Rojo</a></li>
            <li><a href="https://github.com/chronno" target="_blank">chronno</a></li>
          </ul>
        </section>
      </div>
    </footer>
  );
}

export default withTranslation('common')(Footer);
