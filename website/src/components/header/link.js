import styles from "./styles.module.scss";

function HeaderLink({ to, children }) {
  return <a href={to} target="_blank" className={styles.link}>{children}</a>
}

export default HeaderLink;
