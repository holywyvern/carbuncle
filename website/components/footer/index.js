import styles from "./styles.module.scss";

function Footer() {
  return (
    <footer className={styles.footer}>
      <div className={styles.content}>
        <section>
          <h4>Special Thanks</h4>
          <ul>
            <li><a href="https://mruby.org/" target="_blank">MRuby</a></li>
            <li><a href="https://www.raylib.com/" target="_blank">Raylib</a></li>
          </ul>
        </section>
        <section>
          <header>
            <h4>Developers</h4>
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

export default Footer;
