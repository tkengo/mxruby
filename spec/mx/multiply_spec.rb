require 'spec_helper'

describe MX do
  describe 'element-wise multiply' do
    context '1-dimention' do
      let(:left) { MX.new([1, 2, 3], dtype: :int64) }

      before do
        @m = left * right
      end

      context 'array by array' do
        let(:right) { MX.new([2, 3, 4], dtype: :int64) }

        it 'multiply element-wise' do
          expect(@m[0]).to eq(2)
          expect(@m[1]).to eq(6)
          expect(@m[2]).to eq(12)
          expect(@m.dtype).to eq(:int64)
        end
      end

      context 'array by float array' do
        let(:right) { MX.new([2.1, 3.1, 4.1]) }

        it 'multiply element-wise' do
          expect(@m[0]).to eq(2.1)
          expect(@m[1]).to eq(6.2)
          expect(@m[2]).to eq(12.299999999999999)
          expect(@m.dtype).to eq(:float64)
        end
      end

      context 'array by int scalar' do
        let(:right) { 5 }

        it 'multiply all elements of array by scalar value' do
          expect(@m[0]).to eq(5)
          expect(@m[1]).to eq(10)
          expect(@m[2]).to eq(15)
          expect(@m.dtype).to eq(:int64)
        end
      end

      context 'array by float scalar' do
        let(:right) { 5.0 }

        it 'multiply all elements of array by scalar value' do
          expect(@m[0]).to eq(5.0)
          expect(@m[1]).to eq(10.0)
          expect(@m[2]).to eq(15.0)
          expect(@m.dtype).to eq(:float64)
        end
      end
    end

    context '2-dimention' do
      let(:left) { MX.new([ [ 1, 2, 3 ], [ 4, 5, 6 ] ], dtype: :int64) }

      before do
        @m = left * right
      end

      context 'array by array' do
        let(:right) { MX.new([ [ 2, 3, 4 ], [ 5, 6, 7 ] ], dtype: :int64) }

        it 'multiply element-wise' do
          expect(@m[0, 0]).to eq(2)
          expect(@m[0, 1]).to eq(6)
          expect(@m[0, 2]).to eq(12)
          expect(@m[1, 0]).to eq(20)
          expect(@m[1, 1]).to eq(30)
          expect(@m[1, 2]).to eq(42)
          expect(@m.dtype).to eq(:int64)
        end
      end

      context 'array by float array' do
        let(:right) { MX.new([ [ 2.1, 3.1, 4.1 ], [ 5.1, 6.1, 7.1 ] ]) }

        it 'multiply element-wise' do
          expect(@m[0, 0]).to eq(2.1)
          expect(@m[0, 1]).to eq(6.2)
          expect(@m[0, 2]).to eq(12.299999999999999)
          expect(@m[1, 0]).to eq(20.4)
          expect(@m[1, 1]).to eq(30.5)
          expect(@m[1, 2]).to eq(42.599999999999994)
          expect(@m.dtype).to eq(:float64)
        end
      end

      context 'array by int scalar' do
        let(:right) { 5 }

        it 'multiply all elements of array by scalar value' do
          expect(@m[0, 0]).to eq(5)
          expect(@m[0, 1]).to eq(10)
          expect(@m[0, 2]).to eq(15)
          expect(@m[1, 0]).to eq(20)
          expect(@m[1, 1]).to eq(25)
          expect(@m[1, 2]).to eq(30)
          expect(@m.dtype).to eq(:int64)
        end
      end

      context 'array by float scalar' do
        let(:right) { 5.0 }

        it 'multiply all elements of array by scalar value' do
          expect(@m[0, 0]).to eq(5.0)
          expect(@m[0, 1]).to eq(10.0)
          expect(@m[0, 2]).to eq(15.0)
          expect(@m[1, 0]).to eq(20.0)
          expect(@m[1, 1]).to eq(25.0)
          expect(@m[1, 2]).to eq(30.0)
          expect(@m.dtype).to eq(:float64)
        end
      end
    end

    context 'different shape each other' do
      it 'raise exception' do
        # 1 by 3 and 1 by 2
        expect{ MX.new([ 1, 2, 3 ]) * MX.new([ 1, 2 ]) }.to raise_error(DataTypeError)

        # 2 by 3 and 3 by 2
        expect{ MX.new([ [ 1, 2, 3 ], [ 4, 5, 6 ] ]) * MX.new([ [ 1, 2 ], [ 3, 4 ], [ 5, 6 ] ]) }.to raise_error(DataTypeError)
      end
    end
  end
end
