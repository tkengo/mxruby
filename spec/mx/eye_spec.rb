require 'spec_helper'

describe MX do
  describe 'eye' do
    context 'with only first (row) argument' do
      it 'creates an array whose diagonal elements are one' do
        expect(MX.eye 3).to match_array([[1.0, 0.0, 0.0], [0.0, 1.0, 0.0], [0.0, 0.0, 1.0]])
      end
    end

    context 'with two (row and col) arguments' do
      it 'creates an array whose diagonal elements are one, but it is not square' do
        expect(MX.eye 3, 2).to match_array([[1.0, 0.0], [0.0, 1.0], [0.0, 0.0]])
      end
    end

    it 'has correct dtype' do
      DTYPES.each do |dtype|
        expect(MX.eye(5, dtype: dtype).dtype).to eq(dtype)
      end
    end

    it 'raises ArgumentError with illegal arguments' do
      expect{ MX.eye(-3) }.to raise_error(ArgumentError)
      expect{ MX.eye(3, -3) }.to raise_error(ArgumentError)
    end
  end
end
