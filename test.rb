$:.unshift(File.dirname(__FILE__) + '/lib')
require 'mxruby'

# 多項式フィッティング
# x = [ x1 ]
# theta = [ t0, t1 ]

# 正解の関数
def f(x)
  h(x, [ 3.4, -5.33 ])
end

# 学習用データを100個用意。ランダムにノイズを発生させる
x = MX.linspace(-5, 5, 100)
y = f(x) + MX::Random.randn(100)

# パラメータ
theta = MX::Random.randn(2)

# 予測関数
def h(x, theta)
  x * theta[1] + theta[0]
end

# 目的関数
def e(y_h, y, theta)
  (y_h - y) ** 2
end

puts x
