require 'spec_helper'

describe MX do
  describe 'fill' do
    let(:rows) { 2 }
    let(:cols) { 3 }
    let(:fill_value) { 2 }

    it 'creates a matrix filled by the value with single shape' do
      mx = MX.fill(rows, fill_value)
      expect(mx.dim).to eq(1)
      expect(mx.size).to eq(rows)
      rows.times{|r| expect(mx[r]).to eq(fill_value) }
    end

    it 'creates a matrix filled by the value with array shape' do
      mx = MX.fill([ rows, cols ], fill_value)
      expect(mx.dim).to eq(2)
      expect(mx.size).to eq(rows * cols)
      rows.times do |r|
        cols.times{|c| expect(mx[r, c]).to eq(fill_value) }
      end
    end
  end
end
