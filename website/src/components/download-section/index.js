"use client";
import React from "react";
import { FontAwesomeIcon } from "@fortawesome/react-fontawesome";
import {
  faWindows,
  faLinux,
  faApple,
} from "@fortawesome/free-brands-svg-icons";
import { faGlobe } from "@fortawesome/free-solid-svg-icons";
import { withTranslation } from "next-i18next";

import Panel from "../panel";

import styles from "./styles.module.scss";

const Platform = withTranslation("common")(
  ({ t, icon, title, download, version, loading }) => {
    return (
      <div className={styles.platform}>
        <FontAwesomeIcon icon={icon} size="4x" />
        <h4>{t(title)}</h4>
        <a href={loading ? undefined : download}>
          {loading ? t("Loading...") : version}
        </a>
      </div>
    );
  }
);

const DEFAULT_STATE = {
  loading: true,
  tag: "0.7.0",
};

function DownloadSection({ t }) {
  const [{ loading, tag }, setState] = React.useState(DEFAULT_STATE);
  React.useEffect(() => {
    fetch("https://api.github.com/repos/holywyvern/carbuncle/tags")
      .then((res) => res.json())
      .then((tags) => {
        const tag = tags[0]?.name || "0.7.0";
        setState({ loading: false, tag });
      })
      .catch(() => setState({ loading: false, tag: "0.7.0" }));
  }, []);
  return (
    <Panel stretch>
      <h3>{t("Downloads")}</h3>
      <div className={styles.downloads}>
        <Platform
          loading={loading}
          icon={faWindows}
          title="Windows Builds"
          download={`https://github.com/holywyvern/carbuncle/releases/download/${tag}/mruby-carbuncle.exe`}
          version={`${tag} (x64)`}
        />
        <Platform
          loading={loading}
          icon={faApple}
          title="OS X Builds"
          download={`https://github.com/holywyvern/carbuncle/releases/download/${tag}/mruby-carbuncle.app.zip`}
          version={`${tag} (x64)`}
        />
        <Platform
          loading={loading}
          icon={faLinux}
          title="Linux Builds"
          download={`https://github.com/holywyvern/carbuncle/releases/download/${tag}/mruby-carbuncle`}
          version={`${tag} (x64)`}
        />
        <Platform
          loading={loading}
          icon={faGlobe}
          title="Web Builds"
          download={`https://github.com/holywyvern/carbuncle/releases/download/${tag}/mruby-carbuncle.web.zip`}
          version={tag}
        />
      </div>
    </Panel>
  );
}

export default withTranslation("common")(DownloadSection);
