require 'spec_helper'

describe MX do
  describe 'dtype checking' do
    context 'operation between array and array' do
      it 'has correct dtype after addition operation' do
        DTYPES.each do |dtype1|
          DTYPES.each do |dtype2|
            new_mx = MX.arange(5, dtype: dtype1) + MX.arange(5, dtype: dtype2)
            expected_dtype = DTYPES_PRIO[dtype1] > DTYPES_PRIO[dtype2] ? dtype1 : dtype2
            expect(new_mx.dtype).to eq(expected_dtype)
          end
        end
      end

      it 'has correct dtype after subtraction operation' do
        DTYPES.each do |dtype1|
          DTYPES.each do |dtype2|
            new_mx = MX.arange(5, dtype: dtype1) - MX.arange(5, dtype: dtype2)
            expected_dtype = DTYPES_PRIO[dtype1] > DTYPES_PRIO[dtype2] ? dtype1 : dtype2
            expect(new_mx.dtype).to eq(expected_dtype)
          end
        end
      end

      it 'has correct dtype after multiplication operation' do
        DTYPES.each do |dtype1|
          DTYPES.each do |dtype2|
            new_mx = MX.arange(5, dtype: dtype1) * MX.arange(5, dtype: dtype2)
            expected_dtype = DTYPES_PRIO[dtype1] > DTYPES_PRIO[dtype2] ? dtype1 : dtype2
            expect(new_mx.dtype).to eq(expected_dtype)
          end
        end
      end

      it 'has correct dtype after power operation' do
        DTYPES.each do |dtype1|
          DTYPES_INT.each do |dtype2|
            new_mx = MX.arange(5, dtype: dtype1) ** MX.arange(5, dtype: dtype2)
            expect(new_mx.dtype).to eq(dtype1)
          end
          DTYPES_FLOAT.each do |dtype2|
            new_mx = MX.arange(5, dtype: dtype1) ** MX.arange(5, dtype: dtype2)
            expect(new_mx.dtype).to eq(:float64)
          end
        end
      end
    end

    context 'operation between array and scalar' do
      it 'has correct dtype after addition operation' do
        DTYPES.each do |dtype1|
          new_mx = MX.arange(5, dtype: dtype1) + 5
          expect(new_mx.dtype).to eq(dtype1)

          new_mx = MX.arange(5, dtype: dtype1) + 5.0
          expect(new_mx.dtype).to eq(:float64)
        end
      end

      it 'has correct dtype after subtraction operation' do
        DTYPES.each do |dtype1|
          new_mx = MX.arange(5, dtype: dtype1) - 5
          expect(new_mx.dtype).to eq(dtype1)

          new_mx = MX.arange(5, dtype: dtype1) - 5.0
          expect(new_mx.dtype).to eq(:float64)
        end
      end

      it 'has correct dtype after multiplication operation' do
        DTYPES.each do |dtype1|
          new_mx = MX.arange(5, dtype: dtype1) * 5
          expect(new_mx.dtype).to eq(dtype1)

          new_mx = MX.arange(5, dtype: dtype1) * 5.0
          expect(new_mx.dtype).to eq(:float64)
        end
      end

      it 'has correct dtype after multiplication operation' do
        DTYPES.each do |dtype1|
          new_mx = MX.arange(5, dtype: dtype1) ** 5
          expect(new_mx.dtype).to eq(dtype1)

          new_mx = MX.arange(5, dtype: dtype1) ** 5.0
          expect(new_mx.dtype).to eq(:float64)
        end
      end
    end
  end
end
