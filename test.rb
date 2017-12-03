$:.unshift(File.dirname(__FILE__) + '/lib')

require 'mxruby'

# 学習数
EPOCH = 100

# 学習率
ETA = 0.001

# 予測関数
def h(x, theta)
  theta[0] + x * theta[1]
end

# 目的関数
def e(x, y, theta)
  0.5 * ((y - h(x, theta)) ** 2).sum
end

# 目的関数のパラメータによる微分
def d(x, y, theta, i)
  ETA * ((h(x, theta) - y) * x ** i).sum
end

# 正解の関数
def f(x)
  h(x, [ 3.4, -5.33 ])
end

# 学習用データを100個用意。ランダムにノイズを発生させる
x = MX.linspace(-5, 5, 100)
y = f(x) + MX::Random.randn(100)

# パラメータ初期化
theta = MX::Random.randn(2)

# 学習する
EPOCH.times do
  theta = MX.new([
   theta[0] - d(x, y, theta, 0),
   theta[1] - d(x, y, theta, 1)
  ])
end

puts theta
