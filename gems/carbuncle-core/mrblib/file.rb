module Carbuncle
  class File
    class << self
      def require(file)
        @required_files ||= {}
        name = file.to_s
        return false if @required_files[name]

        self.load(file)
        @required_files[name] = true
      end

      def required_files
        @required_files.keys
      end

      def join(*names)
        return '' if names.empty?

        names.filter!(&:present?)
        names.map! do |name|
          case name
          when String
            name
          when Array
            raise ArgumentError, 'recursive array' if names == name

            join(*name)
          else
            raise TypeError, "no implicit conversion of #{name.class} into String"
          end
        end
        return names[0] if names.size == 1

        s =
          if names[0][-1] == Carbuncle::File::SEPARATOR
            names[0][0..-2]
          else
            names[0].dup
          end

        (1..names.size - 2).each do |i|
          t = names[i]
          if t[0] == Carbuncle::File::SEPARATOR && t[-1] == Carbuncle::File::SEPARATOR
            t = t[1..-2]
          elsif t[0] == Carbuncle::File::SEPARATOR
            t = t[1..-1]
          elsif t[-1] == Carbuncle::File::SEPARATOR
            t = t[0..-2]
          end
          s += Carbuncle::File::SEPARATOR + t if t != ''
        end
        s +=
          if names[-1][0] == Carbuncle::File::SEPARATOR
            Carbuncle::File::SEPARATOR + names[-1][1..-1]
          else
            Carbuncle::File::SEPARATOR + names[-1]
          end
        s
      end

      def readlines(name, *args)
        open_args = [name]
        read_args = []
        case args.size
        when 3
          open_args = [name, args[2]]
          read_args = [args[0], args[1]]
        when 2
          if args[0].is_a?(String)
            open_args = [name, args[1]]
            read_args = [args[0]]
          else
            read_args = args
          end
        when 1
          read_args = args
        when 0
          # Default values
        else
          raise ArgumentError, 'Invalid number of arguments'
        end
        fp = self.open(*open_args)
        lines = fp.readlines(*read_args)
        fp.close
        lines
      end

      def write(path, content, open_args = 'w')
        result = nil
        self.open(path, open_args) { |f| result = f.write(content) }
        result
      end

      def read(path, *opts)
        result = nil
        self.open(path, 'r') { |f| result = f.read(*opts) }
        result
      end
    end

    def readlines(arg1 = nil, arg2 = nil)
      limit, sep = -1, "\n"
      if arg2
        limit = arg2
        sep = arg1 || sep
      elsif arg1
        if arg1.is_a?(Numeric)
          limit = arg1
        else
          sep = arg1
        end
      end
      [].tap do |lines|
        line = readline(sep)
        while line && (limit == -1 || limit < lines.length)
          lines.push(line)
          line = readline(sep)
        end
      end
    end
  end
end
