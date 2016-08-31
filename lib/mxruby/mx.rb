class MX
  def initialize(arg1, arg2 = nil)
    shape = []
    initial_value = nil
    opt = {}

    if arg1.is_a?(Hash)
      shape = arg1.delete(:shape) if arg1.has_key?(:shape)
      shape = [ shape ] unless shape.is_a?(Array)
      initial_value = arg1.delete(:initial_value) if arg1.has_key?(:initial_value)
      initial_value = initial_value.flatten if initial_value.is_a?(Array)
      opt = arg1
    else
      initial_value = arg1.flatten
      while arg1.is_a?(Array)
        shape << arg1.size
        arg1 = arg1.first
      end

      opt = arg2 if !arg2.nil? && arg2.is_a?(Hash)
    end

    __set__(shape, initial_value, opt)
  end

  class << self
    def zeros(shape)
      return MX.fill(shape, 0)
    end

    def empty(shape)
      return MX.new(shape: shape)
    end

    def fill(shape, fill_value)
      return MX.new(shape: shape, initial_value: fill_value)
    end
  end
end
