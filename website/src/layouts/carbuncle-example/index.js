import { useState, useEffect } from "react";
import SyntaxHighlighter from "react-syntax-highlighter";
import { vs } from "react-syntax-highlighter/dist/cjs/styles/hljs";
import { FontAwesomeIcon } from "@fortawesome/react-fontawesome";
import { i18n, withTranslation } from "next-i18next";

import Panel from "../../components/panel";

import ExamplesLayout from "../examples-layout";

import { faEye, faEyeSlash, faCopy } from "@fortawesome/free-solid-svg-icons";

import styles from "./styles.module.scss";

vs.background = "transparent";

const copyToClipboard = (str) => {
  const el = document.createElement("textarea");
  el.value = str;
  el.setAttribute("readonly", "");
  el.style.position = "absolute";
  el.style.left = "-9999px";
  document.body.appendChild(el);
  el.select();
  document.execCommand("copy");
  document.body.removeChild(el);
};

function CodeBlock({ id, t, code }) {
  if (!code) {
    return <Panel>{t("Loading...")}</Panel>;
  }
  return (
    <div className={styles["code-container"]}>
      <SyntaxHighlighter
        showLineNumbers
        showInlineLineNumbers
        language="ruby"
        style={vs}
        customStyle={{ background: "rgba(255, 255, 255, 0.4)" }}
      >
        {code}
      </SyntaxHighlighter>
    </div>
  );
}

function Button({ onClick, visible, t }) {
  const icon = visible ? faEye : faEyeSlash;
  return (
    <button type="button" onClick={onClick} className={styles.button}>
      <FontAwesomeIcon icon={icon} />
      &nbsp;
      {visible ? t("Hide code") : t("Show code")}
    </button>
  );
}

function CopyButton({ onClick, disabled, t }) {
  return (
    <button
      type="button"
      onClick={onClick}
      className={styles.copy}
      disabled={disabled}
    >
      <FontAwesomeIcon icon={faCopy} />
      &nbsp;
      {t("Copy Example")}
    </button>
  );
}

function CarbuncleExample({ id, t }) {
  const [code, setCode] = useState(null);
  const [showCode, setShowCode] = useState(false);
  useEffect(() => {
    fetch(
      `${process.env.basePath}/data/examples${id}/main_${i18n?.language}.rb`
    )
      .then((response) => {
        if (response.ok && response.status === 200) {
          return response.text();
        }
        return fetch(`${process.env.basePath}/data/examples${id}/main.rb`).then(
          (r) => r.text()
        );
      })
      .then((text) => setCode(text))
      .catch(console.error);
  }, [id, i18n?.language]);

  const toggleCode = () => setShowCode(!showCode);
  const copyCode = () => copyToClipboard(code);
  return (
    <ExamplesLayout>
      <div className={styles.wrapper}>
        <div className={styles.frame}>
          <iframe
            src={`${process.env.basePath}/data/examples${id}/mruby-carbuncle.html`}
            width="640"
            height="480"
            frameBorder="0"
          />
        </div>
        {showCode && (
          <div className={styles.code}>
            <div className={styles["code-wrapper"]}>
              <CodeBlock id={id} t={t} code={code} />
            </div>
          </div>
        )}
        <Button onClick={toggleCode} visible={showCode} t={t} />
        <CopyButton onClick={copyCode} disabled={!code} t={t} />
      </div>
    </ExamplesLayout>
  );
}

export default withTranslation("examples")(CarbuncleExample);
