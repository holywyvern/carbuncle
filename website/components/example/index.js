import SyntaxHighlighter from 'react-highlight.js';
import { withTranslation } from '../../i18n';

import Panel from '../panel';

import styles from "./styles.module.scss";

function Example({ title, children, t }) {
  return (
    <section className={styles.example}>
      <header>
        <h4>{t(title)}</h4>
      </header>
      <Panel stretch noOverflow>
        <div className={styles.panel}>
          <SyntaxHighlighter language="ruby">
            {children}
          </SyntaxHighlighter>
        </div>
      </Panel>
    </section>
  )
}

export default withTranslation('common')(Example);