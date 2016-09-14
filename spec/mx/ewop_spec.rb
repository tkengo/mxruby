require 'spec_helper'

describe MX do
  describe 'addition' do
    before do
      @add = left + right
    end

    context 'of 1-dimention arrays' do
      let(:left)  { MX.arange(1, 5) }
      let(:right) { MX.arange(2, 6) }

      it 'returns result of additional operation of two arrays' do
        expect(@add).to match_array([ 3, 5, 7, 9 ])
      end
    end

    context 'of 2-dimention arrays' do
      let(:left)  { MX.arange(1,  10).reshape([ 3, 3 ]) }
      let(:right) { MX.arange(11, 20).reshape([ 3, 3 ]) }

      it 'returns result of additional operation of two arrays' do
        expected = [ [ 12, 14, 16 ],
                     [ 18, 20, 22 ],
                     [ 24, 26, 28 ] ]
        expect(@add).to match_array(expected)
      end
    end

    context 'of array and scalar' do
      let(:left)  { MX.arange(10) }
      let(:right) { 2 }

      it 'returns result of additional operation of two arrays' do
        expected = ((left.first + right)..(left.last + right)).to_a
        expect(@add).to match_array(expected)
      end
    end

    context 'of scalar and array' do
      let(:left)  { 2 }
      let(:right) { MX.arange(10) }

      it 'returns result of additional operation of two arrays' do
        expected = ((right.first + left)..(right.last + left)).to_a
        expect(@add).to match_array(expected)
      end
    end

    context 'of array and floating number' do
      let(:left)  { MX.arange(5) }
      let(:right) { 2.1 }

      it 'returns result of additional operation of two arrays' do
        expect(@add).to match_array([ 2.1, 3.1, 4.1, 5.1, 6.1 ])
      end
    end

    context 'of floating number and array' do
      let(:left)  { 2.1 }
      let(:right) { MX.arange(5) }

      it 'returns result of additional operation of two arrays' do
        expect(@add).to match_array([ 2.1, 3.1, 4.1, 5.1, 6.1 ])
      end
    end
  end

  describe 'subtraction' do
    before do
      @sub = left - right
    end

    context 'of 1-dimention arrays' do
      let(:left)  { MX.arange(1, 5) }
      let(:right) { MX.arange(2, 6) }

      it 'returns result of additional operation of two arrays' do
        expect(@sub).to match_array([ -1, -1, -1, -1 ])
      end
    end

    context 'of 2-dimention arrays' do
      let(:left)  { MX.arange(1,  10).reshape([ 3, 3 ]) }
      let(:right) { MX.arange(11, 20).reshape([ 3, 3 ]) }

      it 'returns result of additional operation of two arrays' do
        expected = [ [ -10, -10, -10 ],
                     [ -10, -10, -10 ],
                     [ -10, -10, -10 ] ]
        expect(@sub).to match_array(expected)
      end
    end

    context 'of array and scalar' do
      let(:left)  { MX.arange(10) }
      let(:right) { 2 }

      it 'returns result of additional operation of two arrays' do
        expected = ((left.first - right)..(left.last - right)).to_a
        expect(@sub).to match_array(expected)
      end
    end

    context 'of array and floating number' do
      let(:left)  { MX.arange(5) }
      let(:right) { 2.5 }

      it 'returns result of additional operation of two arrays' do
        expect(@sub).to match_array([ -2.5, -1.5, -0.5, 0.5, 1.5 ])
      end
    end
  end

  describe 'multiplication' do
    before do
      @mul = left * right
    end

    context 'of 1-dimention arrays' do
      let(:left)  { MX.arange(1, 5) }
      let(:right) { MX.arange(2, 6) }

      it 'returns result of additional operation of two arrays' do
        expect(@mul).to match_array([ 2, 6, 12, 20 ])
      end
    end

    context 'of 2-dimention arrays' do
      let(:left)  { MX.arange(1,  10).reshape([ 3, 3 ]) }
      let(:right) { MX.arange(11, 20).reshape([ 3, 3 ]) }

      it 'returns result of additional operation of two arrays' do
        expected = [ [ 11,  24,  39  ],
                     [ 56,  75,  96  ],
                     [ 119, 144, 171 ] ]
        expect(@mul).to match_array(expected)
      end
    end

    context 'of array and scalar' do
      let(:left)  { MX.arange(10) }
      let(:right) { 2 }

      it 'returns result of additional operation of two arrays' do
        expected = ((left.first * right)..(left.last * right)).step(right).to_a
        expect(@mul).to match_array(expected)
      end
    end

    context 'of scalar and array' do
      let(:left)  { 2 }
      let(:right) { MX.arange(10) }

      it 'returns result of additional operation of two arrays' do
        expected = ((right.first * left)..(right.last * left)).step(left).to_a
        expect(@mul).to match_array(expected)
      end
    end

    context 'of array and floating number' do
      let(:left)  { MX.arange(5) }
      let(:right) { 2.5 }

      it 'returns result of additional operation of two arrays' do
        expect(@mul).to match_array([ 0.0, 2.5, 5.0, 7.5, 10.0 ])
      end
    end

    context 'of floating number and array' do
      let(:left)  { 2.5 }
      let(:right) { MX.arange(5) }

      it 'returns result of additional operation of two arrays' do
        expect(@mul).to match_array([ 0.0, 2.5, 5.0, 7.5, 10.0 ])
      end
    end
  end

  describe 'power' do
    before do
      @pow = left ** right
    end

    context 'of 1-dimention arrays' do
      let(:left)  { MX.arange(1, 5) }
      let(:right) { MX.arange(2, 6) }

      it 'returns result of additional operation of two arrays' do
        expect(@pow).to match_array([ 1, 8, 81, 1024 ])
      end
    end

    context 'of 2-dimention arrays' do
      let(:left)  { MX.arange(11, 17).reshape([ 2, 3 ]) }
      let(:right) { MX.arange( 1,  7).reshape([ 2, 3 ]) }

      it 'returns result of additional operation of two arrays' do
        expected = [ [    11,     144,      2197 ],
                     [ 38416,  759375,  16777216 ] ]
        expect(@pow).to match_array(expected)
      end
    end

    context 'of array and scalar' do
      let(:left)  { MX.arange(10) }
      let(:right) { 2 }

      it 'returns result of additional operation of two arrays' do
        expected = left.to_a.map{|v| v ** right }
        expect(@pow).to match_array(expected)
      end
    end

    context 'of floating array and scalar' do
      let(:left)  { MX.linspace(1, 2, 5) }
      let(:right) { 2 }

      it 'returns result of additional operation of two arrays' do
        expected = left.to_a.map{|v| v ** right }
        expect(@pow).to match_array(expected)
      end
    end

    context 'of array and floating number' do
      let(:left)  { MX.arange(5) }
      let(:right) { 2.5 }

      it 'returns result of additional operation of two arrays' do
        expect(@pow).to match_array([ 0.0, 2.5, 5.0, 7.5, 10.0 ])
      end
    end
  end
end
