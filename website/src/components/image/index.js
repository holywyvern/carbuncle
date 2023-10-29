function Image({ src, ...props }) {
  const path = process.env.basePath;
  return (
    <img
      src={`${path}${src}`}
      {...props}
    />
  );
}

export default Image;
