function Image({ src, ...props }) {
  return (
    <img
      src={`${process.env.basePath}/${src}`}
      {...props}
    />
  );
}

export default Image;
