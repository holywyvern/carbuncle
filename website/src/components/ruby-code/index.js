import SyntaxHighlighter from "react-syntax-highlighter";
import { vs } from "react-syntax-highlighter/dist/cjs/styles/hljs";

function RubyCode({ children }) {
  return (
    <SyntaxHighlighter
      showLineNumbers
      showInlineLineNumbers
      language="ruby"
      style={vs}
      customStyle={{ background: "rgba(255, 255, 255, 0.4)" }}
    >
      {children}
    </SyntaxHighlighter>
  );
}

export default RubyCode;
