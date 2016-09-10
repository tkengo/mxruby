require 'spec_helper'

describe MX do
  describe 'first' do
    before do
      @first = mx.first
    end

    context 'of empty array' do
      let(:mx) { MX.arange(0) }

      it 'returns nil' do
        expect(@first).to be_nil
      end
    end

    context 'of an array that has over two elements' do
      let(:mx) { MX.arange(3, 10) }
      it 'returns a first element' do
        expect(@first).to eq(3)
      end
    end
  end
end
