import SyntaxHighlighter from "react-syntax-highlighter";
import { vs } from "react-syntax-highlighter/dist/cjs/styles/hljs";
import { withTranslation } from "next-i18next";

import Panel from "../panel";

import styles from "./styles.module.scss";

vs.background = "transparent";

function Example({ title, children, t }) {
  return (
    <section className={styles.example}>
      <header>
        <h4>{t(title)}</h4>
      </header>
      <Panel stretch noOverflow>
        <div className={styles.panel}>
          <SyntaxHighlighter
            showLineNumbers
            showInlineLineNumbers
            language="ruby"
            style={vs}
            customStyle={{ background: "rgba(255, 255, 255, 0.4)" }}
          >
            {children}
          </SyntaxHighlighter>
        </div>
      </Panel>
    </section>
  );
}

export default withTranslation("common")(Example);
