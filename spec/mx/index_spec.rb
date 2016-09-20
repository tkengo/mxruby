require 'spec_helper'

describe MX do
  describe 'indexing' do
    context '1-dimension array' do
      before do
        @mx = MX.arange(10)
      end

      it 'returns a value at specified index' do
        expect(@mx[0]).to eq(0)
        expect(@mx[5]).to eq(5)
        expect(@mx[9]).to eq(9)
      end

      it 'returns nil if index is out of range' do
        expect(@mx[10]).to be_nil
        expect(@mx[0, 0]).to be_nil
      end

      it 'returns a new array if range was specified as an index' do
        expect(@mx[0..5]).to match_array([0, 1, 2, 3, 4, 5])
      end

      it 'returns a new array even if range was over' do
        expect(@mx[0..12]).to match_array(@mx.to_a)
      end
    end

    context '2-dimension array' do
      before do
        @mx = MX.new([[1, 2, 3], [4, 5, 6]])
      end

      it 'returns a value at specified index' do
        expect(@mx[0, 0]).to eq(1)
        expect(@mx[0, 1]).to eq(2)
        expect(@mx[1, 1]).to eq(5)
        expect(@mx[1, 2]).to eq(6)
      end

      it 'returns a row as a mxarray' do
        expect(@mx[0]).to match_array([1, 2, 3])
        expect(@mx[1]).to match_array([4, 5, 6])
      end

      it 'returns nil if index is out of range' do
        expect(@mx[2]).to be_nil
        expect(@mx[0, 8]).to be_nil
      end
    end

    context '3-dimension array' do
      before do
        @mx = MX.new([[[1, 2], [3, 4]], [[5, 6], [7, 8]]])
      end

      it 'returns a value at specified index' do
        expect(@mx[0, 0, 0]).to eq(1)
        expect(@mx[0, 0, 1]).to eq(2)
        expect(@mx[1, 1, 0]).to eq(7)
        expect(@mx[1, 1, 1]).to eq(8)
      end

      it 'returns a matrix as a mxarray' do
        expect(@mx[0]).to match_array([[1, 2], [3, 4]])
        expect(@mx[1]).to match_array([[5, 6], [7, 8]])

        expect(@mx[0, 0]).to match_array([1, 2])
        expect(@mx[0, 1]).to match_array([3, 4])
        expect(@mx[1, 0]).to match_array([5, 6])
        expect(@mx[1, 1]).to match_array([7, 8])
      end
    end
  end
end
