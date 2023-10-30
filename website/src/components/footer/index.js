import styles from "./styles.module.scss";
import { withTranslation } from "next-i18next";

const FooterSection = withTranslation("common")(({ t, title, links }) => {
  return (
    <section>
      <h4>{t(title)}</h4>
      <ul>
        {links.map(({ label, url }) => {
          return (
            <li key={label}>
              <a href={url} target="_blank">
                {label}
              </a>
            </li>
          );
        })}
      </ul>
    </section>
  );
});

function Footer() {
  return (
    <footer className={styles.footer}>
      <div className={styles.content}>
        <FooterSection
          title="Special Thanks"
          links={[
            { label: "MRuby", url: "https://mruby.org/" },
            { label: "Raylib", url: "https://www.raylib.com/" },
          ]}
        />
        <FooterSection
          title="Developers"
          links={[
            { label: "Ramiro Rojo", url: "https://ramirorojo.com/" },
            { label: "chronno", url: "https://github.com/chronno" },
          ]}
        />
      </div>
    </footer>
  );
}

export default withTranslation("common")(Footer);
