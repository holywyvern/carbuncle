import cx from "classnames";

import styles from "./styles.module.scss";

function Panel({ children, stretch }) {
  const className = cx({
    [styles.panel]: true,
    [styles.stretch]: stretch,
  })
  return (
    <div className={className}>
      <div className={styles.tr} />
      <div className={styles.bl} />
      <div className={styles.left} />
      <div className={styles.right} />
      <div className={styles.top} />
      <div className={styles.bottom} />
      <div className={styles.contents}>
        {children}
      </div>
    </div>
  );
}

export default Panel;