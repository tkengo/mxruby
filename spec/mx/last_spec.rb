require 'spec_helper'

describe MX do
  describe 'last' do
    before do
      @last = mx.last
    end

    context 'of empty array' do
      let(:mx) { MX.arange(0) }

      it 'returns nil' do
        expect(@last).to be_nil
      end
    end

    context 'of an array that has only one element' do
      let(:mx) { MX.arange(1) }

      it 'returns a last element' do
        expect(@last).to eq(0)
      end
    end

    context 'of an array that has over two elements' do
      let(:mx) { MX.arange(3, 10) }

      it 'returns a last element' do
        expect(@last).to eq(9)
      end
    end
  end
end
