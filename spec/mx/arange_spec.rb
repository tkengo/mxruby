require 'spec_helper'

describe MX do
  describe 'arange' do
    context 'with first argument' do
      before do
        @mx = MX.arange(stop)
      end

      context 'zero' do
        let(:stop) { 0 }

        it 'creates empty array' do
          expect(@mx).to be_empty
        end
      end

      context 'positive number' do
        let(:stop) { 10 }

        it 'creates sequence from 0 to stop number' do
          expect(@mx).to match_array((0...stop).to_a)
        end
      end

      context 'floating point number' do
        let(:stop) { 5.4 }

        it 'creates empty array' do
          expect(@mx).to match_array((0...stop.ceil).to_a)
        end
      end
    end

    context 'with first two arguments' do
      before do
        @mx = MX.arange(start, stop)
      end

      context do
        let(:start) { 0 }
        let(:stop)  { 10 }

        it 'creates sequence from start to stop' do
          expect(@mx).to match_array((start...stop).to_a)
        end
      end

      context 'including negative number' do
        let(:start) { -5 }
        let(:stop)  { 5 }

        it 'creates empty array' do
          expect(@mx).to match_array((start...stop).to_a)
        end
      end

      context 'floating point number' do
        let(:start) { 2.3 }
        let(:stop)  { 8.9 }

        it 'creates empty array' do
          diff = start - start.to_i
          expected = (start.to_i...stop.ceil).map{|v| v + diff }

          expect(@mx).to match_array(expected)
        end
      end
    end

    context 'with three arguments' do
      before do
        @mx = MX.arange(start, stop, step)
      end

      context 'two step' do
        let(:start) { 0 }
        let(:stop)  { 10 }
        let(:step)  { 2 }

        it 'creates sequence from start to stop by step' do
          expect(@mx).to match_array((start...stop).step(step).to_a)
        end
      end

      context 'negative step' do
        let(:start) { 10 }
        let(:stop)  { 0 }
        let(:step)  { -2 }

        it 'creates reverse sequence' do
          expect(@mx).to match_array([ 10, 8, 6, 4, 2 ])
        end
      end

      context 'floating number step' do
        let(:start) { 5 }
        let(:stop)  { 10 }
        let(:step)  { 0.5 }

        it 'creates reverse sequence' do
          expect(@mx.size).to eq((stop - start) / step)
          expect(@mx.first).to eq(start)
          expect(@mx.second).to eq(start + step)
          expect(@mx.last).to eq(stop - step)
        end
      end

      context 'negative floating number step' do
        let(:start) { 10 }
        let(:stop)  { 5 }
        let(:step)  { -0.5 }

        it 'creates reverse sequence' do
          expect(@mx.size).to eq((stop - start) / step)
          expect(@mx.first).to eq(start)
          expect(@mx.second).to eq(start + step)
          expect(@mx.last).to eq(stop - step)
        end
      end
    end

    it 'has int64 data type as a default type' do
      expect(MX.arange(5).dtype).to eq(:int64)
    end

    it 'has int64 data type if non exists data type was specified' do
      expect(MX.arange(5, dtype: :int0).dtype).to eq(:int64)
    end

    it 'has specified data type' do
      DTYPES.each do |dtype|
        expect(MX.arange(5, dtype: dtype).dtype).to eq(dtype)
      end
    end

    context 'raise error' do
      it 'raises ArgumentError if step is 0' do
        expect{ MX.arange(0, 10, 0) }.to raise_error(ArgumentError)
      end

      it 'raises ArgumentError if start > stop or start < stop' do
        expect{ MX.arange(-5) }.to raise_error(ArgumentError)
        expect{ MX.arange(10, 0) }.to raise_error(ArgumentError)
      end
    end
  end
end
