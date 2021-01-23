import { Light as SyntaxHighlighter } from 'react-syntax-highlighter';
import ruby from 'react-syntax-highlighter/dist/esm/languages/hljs/ruby';
import theme from 'react-syntax-highlighter/dist/esm/styles/hljs/atelier-estuary-dark';

import Panel from '../panel';

import styles from "./styles.module.scss";

SyntaxHighlighter.registerLanguage('ruby', ruby);

function Example({ title, children }) {
  return (
    <section className={styles.example}>
      <header>
        <h4>{title}</h4>
      </header>
      <Panel stretch>
        <SyntaxHighlighter language="ruby" style={theme} showLineNumbers>
          {children}
        </SyntaxHighlighter>
      </Panel>
    </section>
  )
}

export default Example;