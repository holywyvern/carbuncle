import SyntaxHighlighter from "react-syntax-highlighter";
import { zenburn } from "react-syntax-highlighter/dist/cjs/styles/hljs";

function RubyCode({ children, style = zenburn, background = "transparent" }) {
  return (
    <SyntaxHighlighter
      language="ruby"
      style={style}
      customStyle={{ background }}
    >
      {children}
    </SyntaxHighlighter>
  );
}

export default RubyCode;
