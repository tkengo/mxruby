# coding: utf-8
lib = File.expand_path('../lib', __FILE__)
$LOAD_PATH.unshift(lib) unless $LOAD_PATH.include?(lib)
require 'mxruby/version'

Gem::Specification.new do |spec|
  spec.name          = "mxruby"
  spec.version       = Mxruby::VERSION
  spec.authors       = ["Kengo Tateishi"]
  spec.email         = ["embrace.ddd.flake.peace@gmail.com"]
  spec.summary       = %q{Extended array for Ruby}
  spec.description   = %q{Fast and easy to use.}
  spec.homepage      = ""
  spec.license       = "MIT"
  spec.extensions    = %w[ext/mxruby/extconf.rb]

  spec.files         = `git ls-files`.split($/)
  spec.executables   = spec.files.grep(%r{^bin/}) { |f| File.basename(f) }
  spec.test_files    = spec.files.grep(%r{^(test|spec|features)/})
  spec.require_paths = ["lib"]

  spec.add_development_dependency "bundler", "~> 1.5"
  spec.add_development_dependency "rake"
  spec.add_development_dependency "rake-compiler"
  spec.add_development_dependency "rspec"
  spec.add_development_dependency "guard"
  spec.add_development_dependency "guard-rspec"
end
