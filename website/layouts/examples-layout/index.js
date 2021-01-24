import ExampleList from "./list";

import styles from "./styles.module.scss";

function ExamplesLayout({ children }) {
  return (
    <div className={styles.page}>
      <ExampleList />
      <div className={styles.view}>
        {children}
      </div>
    </div>
  )
}

export default ExamplesLayout;
