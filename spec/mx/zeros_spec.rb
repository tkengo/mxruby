require 'spec_helper'

describe MX do
  describe 'zeros' do
    let(:rows) { 2 }
    let(:cols) { 3 }

    it 'creates a zeros matrix with single shape' do
      mx = MX.zeros(rows)
      expect(mx.dim).to eq(1)
      expect(mx.size).to eq(rows)
      rows.times{|r| expect(mx[r]).to eq(0) }
    end

    it 'creates a zeros matrix with array shape' do
      mx = MX.zeros([ rows, cols ])
      expect(mx.dim).to eq(2)
      expect(mx.size).to eq(rows * cols)
      rows.times do |r|
        cols.times{|c| expect(mx[r, c]).to eq(0) }
      end
    end
  end
end
