import { FontAwesomeIcon } from "@fortawesome/react-fontawesome";
import * as icons from "@fortawesome/free-solid-svg-icons";
import * as brands from "@fortawesome/free-brands-svg-icons";
import { withTranslation } from "next-i18next";

function Feature({ title, logo, children, t }) {
  return (
    <section>
      <h3>
        <FontAwesomeIcon icon={icons[logo] || brands[logo]} />
        &nbsp;
        {t(title)}
      </h3>
      <p>{children}</p>
    </section>
  );
}

export default withTranslation("common")(Feature);
