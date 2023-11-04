"use client";
import React from "react";

import { FontAwesomeIcon } from "@fortawesome/react-fontawesome";
import { faPlay } from "@fortawesome/free-solid-svg-icons";

import Editor from "@monaco-editor/react";

import Image from "../../components/image";

import styles from "./styles.module.scss";
import { useRouter } from "next/router";

const exampleGame = `
  include Carbuncle

  class Playground < Game
    attr_accessor :text

    def start
      Font.default_size = 48
      self.text = Text.new
      text.value = 'Hello from the playground'
      text.position.set(
        (screen.width - text.width) / 2,
        (screen.height - text.height) / 2
      )      
      self << self.text
    end
  end
`;

const baseURL = `${process.env.basePath}/data/playground.html`;

const defaultURL = `${baseURL}?code=${encodeURIComponent(exampleGame)}`;

function Playground() {
  const [value, setValue] = React.useState(exampleGame);
  const [url, setURL] = React.useState(defaultURL);
  const router = useRouter();
  const onHeaderClick = () => {
    router.push("/");
  };
  const onPlaygroundUpdate = () =>
    setURL(`${baseURL}?code=${encodeURIComponent(value)}`);
  const onValueChange = (value) => setValue(value);
  return (
    <div className={styles.playground}>
      <div className={styles.toolbar}>
        <div className={styles.header} onClick={onHeaderClick}>
          <Image src="/img/logo.svg" className={styles.logo} />
          <div>
            <h1>
              Ca<span className={styles.highlight}>rb</span>uncle
            </h1>
            <h2>Playground</h2>
          </div>
        </div>
        <div className={styles.buttons}>
          <button
            className={styles.button}
            type="button"
            onClick={onPlaygroundUpdate}
          >
            <FontAwesomeIcon icon={faPlay} fixedWidth />
          </button>
        </div>
      </div>
      <div className={styles.wrapper}>
        <div className={styles.editor}>
          <Editor
            width="100%"
            height="100%"
            defaultLanguage="ruby"
            defaultValue={exampleGame}
            onChange={onValueChange}
          />
        </div>
        <div className={styles.frame}>
          <iframe
            key={url}
            src={url}
            width="640"
            height="480"
            frameBorder="0"
          />
        </div>
      </div>
    </div>
  );
}

export default Playground;
