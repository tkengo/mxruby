class MX
  class Random
    class << self
      def rand(shape)
        shape = [ shape ] unless shape.is_a?(Array)

        r = ::Random.new

        initial_array = []
        shape.inject(1){|sum, v| sum *= v }.times do
          initial_array << r.rand
        end

        return MX.new(shape: shape, initial_value: initial_array)
      end
    end
  end
end
