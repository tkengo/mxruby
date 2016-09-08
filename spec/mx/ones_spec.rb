require 'spec_helper'

describe MX do
  describe 'ones' do
    let(:rows) { 2 }
    let(:cols) { 3 }

    it 'creates a matrix whose all elements is one with single shape' do
      mx = MX.ones(rows)
      expect(mx.dim).to eq(1)
      expect(mx.size).to eq(rows)
      rows.times{|r| expect(mx[r]).to eq(1) }
    end

    it 'creates a matrix whose all elements is one with array shape' do
      mx = MX.ones([ rows, cols ])
      expect(mx.dim).to eq(2)
      expect(mx.size).to eq(rows * cols)
      rows.times do |r|
        cols.times{|c| expect(mx[r, c]).to eq(1) }
      end
    end
  end
end
