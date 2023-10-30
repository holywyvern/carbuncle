import styles from "./styles.module.scss";
import { withTranslation } from "next-i18next";

import Link from "./link";
import Image from "../image";
import { FontAwesomeIcon } from "@fortawesome/react-fontawesome";
import { faGithub } from "@fortawesome/free-brands-svg-icons";
import { faBook } from "@fortawesome/free-solid-svg-icons";

function Header({ t }) {
  return (
    <header className={styles.header}>
      <Image
        className={styles.logo}
        src="/img/logo.svg"
        alt="Carbuncle Logo"
        width="auto"
        height="auto"
      />
      <div className={styles.container}>
        <h1 className={styles.title}>
          Ca<span>rb</span>uncle
          <sup className={styles.sup}>beta</sup>
        </h1>
        <div className={styles.subtitle}>
          <h2>Game Engine</h2>
          <div className={styles.links}>
            <Link to="https://github.com/holywyvern/carbuncle">
              <FontAwesomeIcon icon={faGithub} />
              &nbsp;
              {t("Source")}
            </Link>
            <Link to={`${process.env.basePath}/docs`}>
              <FontAwesomeIcon icon={faBook} />
              &nbsp;
              {t("API Docs")}
            </Link>
          </div>
        </div>
      </div>
    </header>
  );
}

export default withTranslation("common")(Header);
