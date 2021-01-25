import { useState, useEffect } from "react";
import SyntaxHighlighter from 'react-highlight.js';
import { FontAwesomeIcon }  from "@fortawesome/react-fontawesome";
import { withTranslation } from '../../i18n';

import Panel from "../../components/panel";

import ExamplesLayout from "../examples-layout";

import { faEye, faEyeSlash } from '@fortawesome/free-solid-svg-icons';

import styles from "./styles.module.scss";

function CodeBlock({ id, t, i18n  }) {
  const [code, setCode] = useState(null);

  useEffect(() => {
    fetch(`${process.env.basePath}/data/examples${id}/main_${i18n.language}.rb`)
      .then(response => {
        if (response.ok && response.status === 200) {
          return response.text()
        }
        return fetch(`${process.env.basePath}/data/examples${id}/main.rb`)
          .then(r => r.text())
      })
      .then(text => setCode(text))
      .catch(console.error);
  }, [id, i18n.language]);  
  if (!code) {
    return (
      <Panel>{t('Loading...')}</Panel>
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

function Button({ onClick, visible, t }) {
  const icon = visible ? faEye : faEyeSlash;
  return (
    <button type="button" onClick={onClick} className={styles.button}>
      <FontAwesomeIcon icon={icon} />&nbsp;
      {visible ? t('Hide code') : t('Show code')}
    </button>
  )
}

function CarbuncleExample({ id, t }) {
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
              <CodeBlock id={id} t={t} />
            </div>
          </div>          
        )}
        <Button onClick={toggleCode} visible={showCode} t={t} />
      </div>
    </ExamplesLayout>
  );
}

export default withTranslation('examples')(CarbuncleExample);
