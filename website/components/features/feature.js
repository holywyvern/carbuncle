import { FontAwesomeIcon }  from "@fortawesome/react-fontawesome";
import * as icons from '@fortawesome/free-solid-svg-icons';
import * as brands from '@fortawesome/free-brands-svg-icons';

function Feature({ title, logo, children }) {
  return (
    <section>
      <h3>
        <FontAwesomeIcon icon={icons[logo] || brands[logo]} />&nbsp;
        {title}
      </h3>
      <p>{children}</p>
    </section>
  )
}

export default Feature;
