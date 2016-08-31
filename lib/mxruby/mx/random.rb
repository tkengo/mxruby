class MX
  class Random
    class << self
      def rand(shape)
        gen(shape){|r| r.rand }
      end

      def randn(shape)
        gen(shape){|r| r.randn }
      end

      private

      def gen(shape, &method)
        shape = [ shape ] unless shape.is_a?(Array)
        r = Random.new

        initial_array = []
        shape.inject(1){|sum, v| sum *= v }.times do
          initial_array << yield(r)
        end

        return MX.new(shape: shape, initial_value: initial_array)
      end
    end
  end
end
