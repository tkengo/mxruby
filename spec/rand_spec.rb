require 'spec_helper'

describe MX::Random do
  describe 'rand' do
    let(:rows) { 2 }
    let(:cols) { 3 }

    it 'creates a matrix whose all elements are random value' do
      mx = MX::Rand.rand(rows)
      expect(mx.dim).to eq(1)
      expect(mx.size).to eq(rows)
    end

    it 'creates a matrix whose all elements are random value with array shape' do
      mx = MX::Rand.rand([ rows, cols ])
      expect(mx.dim).to eq(2)
      expect(mx.size).to eq(rows * cols)
    end
  end
end
