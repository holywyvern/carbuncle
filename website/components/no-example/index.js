import Panel from "../panel"

import styles from "./styles.module.scss";

function NoExample() {
  return (
    <div className={styles.wrapper}>
      <div className={styles.view}>
        <Panel>
          No example selected.<br />
          Select one from the sidebar.
        </Panel>
      </div>
    </div>
  );
}

export default NoExample;
