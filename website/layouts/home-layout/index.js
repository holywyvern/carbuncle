import styles from "./styles.module.scss";

function HomeLayout({ children }) {
  return (
    <div className={styles.wrapper}>
      <div className={styles.page}>
        {children}
      </div>
    </div>
  );
}

export default HomeLayout;