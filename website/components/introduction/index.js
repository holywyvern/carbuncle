import { withTranslation } from "next-i18next";

import styles from "./styles.module.scss";

function Introduction({ t }) {
  return (
    <section className={styles.intro}>
      <p>
        {t("A Game library build with")}&nbsp;
        <a href="https://mruby.org/" target="_blank">
          MRuby
        </a>
        ,&nbsp;
        <a href="https://www.raylib.com/" target="_blank">
          Raylib
        </a>
        &nbsp;
        {t(
          "and more as a base for developing games while having fun across multiple platforms."
        )}
      </p>
    </section>
  );
}

export default withTranslation("common")(Introduction);
