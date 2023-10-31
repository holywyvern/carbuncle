import styles from "./styles.module.scss";

function HeaderLink({ to, children, popup }) {
  return (
    <a href={to} target="_blank" tip={popup} className={styles.link}>
      {children}
    </a>
  );
}

export default HeaderLink;
