import cx from "classnames";

import styles from "./styles.module.scss";

function Panel({ children, stretch, noOverflow }) {
  const className = cx({
    [styles.panel]: true,
    [styles.stretch]: stretch,
  })
  const contentClassNames = cx({
    [styles.contents]: true,
    [styles['no-overflow']]: noOverflow
  })
  return (
    <div className={className}>
      <div className={styles.tr} />
      <div className={styles.bl} />
      <div className={styles.left} />
      <div className={styles.right} />
      <div className={styles.top} />
      <div className={styles.bottom} />
      <div className={contentClassNames}>
        {children}
      </div>
    </div>
  );
}

export default Panel;