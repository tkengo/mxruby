require 'spec_helper'

describe MX do
  describe 'multiplication' do
    it do
      dtype_sizes = { int8: 1, int16: 2, int32: 3, int64: 4, float32: 5, float64: 6 }
      dtypes = dtype_sizes.keys

      dtypes.each do |dtype1|
        dtypes.each do |dtype2|
          mx1 = MX.arange(5, dtype: dtype1)
          mx2 = MX.arange(5, dtype: dtype2)

          puts dtype1
          puts dtype2
          puts '---------'
          new_mx = mx1 * mx2
          expected_dtype = dtype_sizes[dtype1] > dtype_sizes[dtype2] ? dtype1 : dtype2
          expect(new_mx.dtype).to eq(expected_dtype)
          puts '========='
        end
      end
    end
  end
end
