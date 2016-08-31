class MX
  class Random
    class << self
      def rand(shape, seed = nil)
        Random.new(seed).rand(shape)
      end

      def randn(shape, seed = nil)
        Random.new(seed).randn(shape)
      end
    end
  end
end
