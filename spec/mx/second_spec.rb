require 'spec_helper'

describe MX do
  describe 'second' do
    before do
      @second = mx.second
    end

    context 'of empty array' do
      let(:mx) { MX.arange(0) }

      it 'returns nil' do
        expect(@second).to be_nil
      end
    end

    context 'of an array that has only one element' do
      let(:mx) { MX.arange(1) }

      it 'returns nil' do
        expect(@second).to be_nil
      end
    end

    context 'of an array that has over two elements' do
      let(:mx) { MX.arange(3, 10) }
      it 'returns a second element' do
        expect(@second).to eq(4)
      end
    end
  end
end
