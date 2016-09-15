require 'spec_helper'

describe MX do
  describe 'shape' do
    before do
      @shape = MX.new(array).shape
    end

    context '1-dimensional array' do
      let(:array) { [ 1, 2, 3 ] }

      it 'returns a shape array' do
        expect(@shape).to match_array([ 3 ])
      end
    end

    context '2-dimensional array' do
      let(:array) { [ [ 1, 2, 3 ], [ 4, 5, 6 ] ] }

      it 'returns a shape array' do
        expect(@shape).to match_array([ 2, 3 ])
      end
    end

    context '3-dimensional array' do
      let(:array) { [ [ [ 1, 2, 3 ], [ 4, 5, 6 ] ], [ [ 7, 8, 9 ], [ 10, 11, 12 ] ] ] }

      it 'returns a shape array' do
        expect(@shape).to match_array([ 2, 2, 3 ])
      end
    end
  end
end
