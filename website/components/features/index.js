import Feature from "./feature";

import styles from "./styles.module.scss"

function Features() {
  return (
    <div className={styles.features}>
      <Feature title="An Open Source Framework" logo="faGithub">
        Completeley Open Source, and free to use.
      </Feature>
      <Feature title="Out of the Box" logo="faBoxOpen">
        Contains many features including graphics, audio, input, networking and GUI!
      </Feature>
      <Feature title="Cross Platform" logo="faLaptopCode">
        Works on Windows, Mac OSX and Linux with planned iOS and Android support.
      </Feature>
      <Feature title="Web Support" logo="faGlobe">
        Deploy it as a website without much effort thanks to&nbsp;
        <a href="https://emscripten.org/" target="_blank">Emscripten</a>.
      </Feature>
    </div>
  )
}

export default Features;
