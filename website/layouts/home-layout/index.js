import styles from "./styles.module.scss";

import Footer from "../../components/footer"

function HomeLayout({ children }) {
  return (
    <div className={styles.wrapper}>
      <div className={styles.page}>
        {children}
      </div>
      <Footer />
    </div>
  );
}

export default HomeLayout;