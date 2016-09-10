$:.unshift(File.dirname(__FILE__) + '/lib')
require 'mxruby'

# 多項式フィッティング
# x = [ x1 ]
# theta = [ t0, t1 ]

# 正解の関数
def f(x)
  x * -5.33 + 3.4
end

# 学習用データを100個用意。ランダムにノイズを発生させる

x = f(MX.linspace(-5, 5, 100)) + MX::Random.randn(100)

puts x
