# require "mxruby/version"

require './mxruby.bundle'

class MX
  def initialize(initial_array, opt = {})
    shape = []

    p = initial_array
    while p.is_a?(Array)
      shape << p.size
      p = p.first
    end

    __set__(shape, initial_array.flatten, opt)
  end

  def inspect
  end
end
