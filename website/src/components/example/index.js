import { withTranslation } from "next-i18next";

import Panel from "../panel";

import styles from "./styles.module.scss";
import RubyCode from "../ruby-code";

function Example({ title, children, t }) {
  return (
    <section className={styles.example}>
      <header>
        <h4>{t(title)}</h4>
      </header>
      <Panel stretch noOverflow>
        <div className={styles.panel}>
          <RubyCode children={children} />
        </div>
      </Panel>
    </section>
  );
}

export default withTranslation("common")(Example);
