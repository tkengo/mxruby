require './mxruby.bundle'

a=[];5000.times{|ai| a[ai]=[]; 5000.times{|bi| a[ai][bi]=ai*bi}};MX.new(a)
