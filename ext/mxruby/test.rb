$:.unshift(File.dirname(__FILE__) + '/../../lib')
require 'mxruby'

# 多項式フィッティング
# x = [ x1 ]
# theta = [ t0, t1 ]

# 正解の関数
def f(x)
  3.4 - 5.33 * x
end

# 学習用データを100個用意。ランダムにノイズを発生させる

x = MX::Random.rand([5, 5])

