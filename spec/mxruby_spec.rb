require 'spec_helper'

describe MX do
  describe 'new' do
    let(:array)  { [ 1, 2, 3 ] }

    it 'creates new matrix with initial array' do
      mx = MX.new(array)

      expect(mx.dim).to eq(1)
      expect(mx.size).to eq(3)
      expect(mx[0]).to eq(array[0].to_f)
      expect(mx[1]).to eq(array[1].to_f)
      expect(mx[2]).to eq(array[2].to_f)
    end

    it 'creates new matrix with initial array and data type' do
      mx = MX.new(array, dtype: :int32)

      expect(mx.dim).to eq(1)
      expect(mx.size).to eq(3)
      expect(mx[0]).to eq(array[0])
      expect(mx[1]).to eq(array[1])
      expect(mx[2]).to eq(array[2])
    end
  end

  describe 'empty' do
    let(:rows) { 2 }
    let(:cols) { 3 }

    it 'creates an empty matrix with single shape' do
      mx = MX.empty(rows)
      expect(mx.dim).to eq(1)
      expect(mx.size).to eq(rows)
    end

    it 'creates a matrix filled by the value with array shape' do
      mx = MX.empty([ rows, cols ])
      expect(mx.dim).to eq(2)
      expect(mx.size).to eq(rows * cols)
    end
  end
end
