import { useState } from "react";
import { FontAwesomeIcon }  from "@fortawesome/react-fontawesome";

import cx from "classnames";
import { useRouter } from "next/router";
import Link from "next/link";

import { faEye, faEyeSlash } from '@fortawesome/free-solid-svg-icons';

import styles from './styles.module.scss';

function Item({ label, to }) {
  const router = useRouter();
  const link = `/examples${to}`;
  const className = cx({
    [styles.link]: true,
    [styles.active]: link === router.pathname,
  });
  return (
    <li>
      <Link href={link}>
        <a className={className}>
          {label}
        </a>
      </Link>
    </li>
  )
}

function MenuButton({ onClick, visible }) {
  const icon = visible ? faEye : faEyeSlash;
  return (
    <button type="button" onClick={onClick} className={styles.button}>
      <FontAwesomeIcon icon={icon} />
    </button>
  )
}

function MenuGroup({ label, children }) {
  const [visible, setVisible] = useState(true);
  const onToggle = () => setVisible(!visible);
  return (
    <div className={styles.group}>
      <div className={styles['group-header']}>
        <strong>{label}</strong>
        <MenuButton onClick={onToggle} visible={visible} />
      </div>
      {visible && (
        <ul className={styles.item}>
          {children}
        </ul>
      )}
    </div>
  ) 
}

function ExampleList() {
  return (
    <nav className={styles.menu}>
      <MenuGroup label="Core examples">
        <Item
          to="/core/hello_world"
          label="Hello World"
        />
      </MenuGroup>
    </nav>
  )
}

export default ExampleList;
