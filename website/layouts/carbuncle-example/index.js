import { useState, useEffect } from "react";
import SyntaxHighlighter from 'react-highlight.js';
import { FontAwesomeIcon }  from "@fortawesome/react-fontawesome";

import Panel from "../../components/panel";

import ExamplesLayout from "../examples-layout";

import { faEye, faEyeSlash } from '@fortawesome/free-solid-svg-icons';

import styles from "./styles.module.scss";

function CodeBlock({ id }) {
  const [code, setCode] = useState(null);

  useEffect(() => {
    fetch(`${process.env.basePath}/data/examples${id}/main.rb`)
      .then(response => response.text())
      .then(text => setCode(text))
      .catch(console.error);
  }, []);  
  if (!code) {
    return (
      <Panel>Loading...</Panel>
    )
  }
  return (
    <Panel>
      <SyntaxHighlighter language="ruby">
        {code}
      </SyntaxHighlighter>
    </Panel>
  )
}

function Button({ onClick, visible }) {
  const icon = visible ? faEye : faEyeSlash;
  return (
    <button type="button" onClick={onClick} className={styles.button}>
      <FontAwesomeIcon icon={icon} />&nbsp;
      {visible ? 'Hide code' : 'Show code'}
    </button>
  )
}

function CarbuncleExample({ id }) {
  const [showCode, setShowCode] = useState(false);

  const toggleCode = () => setShowCode(!showCode);
  return (
    <ExamplesLayout>
      <div className={styles.wrapper}>
        <div className={styles.frame}>
          <iframe src={`${process.env.basePath}/data/examples${id}/mruby-carbuncle.html`} width="640" height="480" frameBorder="0" />
        </div>
        {showCode && (
          <div className={styles.code}>
            <div className={styles['code-wrapper']}>
              <CodeBlock id={id} />
            </div>
          </div>          
        )}
        <Button onClick={toggleCode} visible={showCode} />
      </div>
    </ExamplesLayout>
  );
}

export default CarbuncleExample;
