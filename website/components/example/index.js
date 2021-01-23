import SyntaxHighlighter from 'react-highlight.js';

import Panel from '../panel';

import styles from "./styles.module.scss";

function Example({ title, children }) {
  return (
    <section className={styles.example}>
      <header>
        <h4>{title}</h4>
      </header>
      <Panel stretch>
        <SyntaxHighlighter language="ruby">
          {children}
        </SyntaxHighlighter>
      </Panel>
    </section>
  )
}

export default Example;