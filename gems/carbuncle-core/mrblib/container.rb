module Carbuncle
  class Container
    include Enumerable

    def initialize
      @children = []
    end

    def children
      @children.dup
    end

    def add_child(child)
      @children.push(child) unless @children.include?(child)
      self
    end

    alias << add_child

    def add_children(*children)
      children.each { |c| add_child(c) }
      self
    end

    def remove_child(child)
      @children.delete(child)
      self
    end

    alias >> remove_child

    def remove_children(*children)
      each { |c| remove_child(c) }
      self      
    end

    def update(dt)
      each { |c| c.update(dt) }
    end

    def draw
      each(&:draw)
    end

    def each(&block)
      @children.each(&block)
    end
  end
end
