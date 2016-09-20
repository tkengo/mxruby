require 'spec_helper'

describe MX do
  describe 'linspace' do
    context 'with first two arguments' do
      let(:shape) { 100 }

      before do
        @mx = MX.linspace(start, stop)
      end

      context 'start is bigger than stop' do
        let(:start) { 0 }
        let(:stop)  { 10 }

        it 'creates sequential array' do
          expect(@mx.size).to eq(shape)
          expect(@mx.first).to eq(start)
          expect(@mx.second.round(5)).to eq(start + ((stop - start) / (shape - 1).to_f).round(5))
          expect(@mx.last.round).to eq(stop)
        end
      end

      context 'start is smaller than stop' do
        let(:start) { 10 }
        let(:stop)  { 0 }

        it 'creates sequential array' do
          expect(@mx.size).to eq(shape)
          expect(@mx.first).to eq(start)
          expect(@mx.second).to eq(start + ((stop - start) / (shape - 1).to_f))
          expect(@mx.last.round).to eq(stop)
        end
      end

      context 'negative number' do
        let(:start) { -10 }
        let(:stop)  { -5 }

        it 'creates sequential array' do
          expect(@mx.size).to eq(shape)
          expect(@mx.first).to eq(start)
          expect(@mx.second).to eq(start + ((stop - start) / (shape - 1).to_f))
          expect(@mx.last.round).to eq(stop)
        end
      end
    end

    context 'with three two arguments' do
      before do
        @mx = MX.linspace(start, stop, shape)
      end

      context 'start is bigger than stop' do
        let(:start) { 0 }
        let(:stop)  { 10 }
        let(:shape) { 10 }

        it 'creates sequential array' do
          expect(@mx.size).to eq(shape)
          expect(@mx.first).to eq(start)
          expect(@mx.second).to eq(start + ((stop - start) / (shape - 1).to_f))
          expect(@mx.last.round).to eq(stop)
        end
      end
    end

    it 'has float64 data type as a default type' do
      expect(MX.linspace(0, 1, 5).dtype).to eq(:float64)
    end

    it 'has int64 data type if non exists data type was specified' do
      expect(MX.linspace(0, 1, 5, dtype: :int0).dtype).to eq(:float64)
    end

    it 'has specified data type' do
      DTYPES.each do |dtype|
        expect(MX.linspace(0, 1, 5, dtype: dtype).dtype).to eq(dtype)
      end
    end

    context 'raise error' do
      it 'raises ArgumentError if shape is negative' do
        expect{ MX.linspace(0, 10, -1) }.to raise_error(ArgumentError)
      end
    end
  end
end
