module Carbuncle
  module Console
    module_function

    def header(title)
      puts '================================================================================'
      puts " ** #{title}."
      puts '--------------------------------------------------------------------------------'
    end
  end
end
