guard :rspec, cmd: "bundle exec rspec" do
  require "guard/rspec/dsl"
  dsl = Guard::RSpec::Dsl.new(self)

  # RSpec files
  rspec = dsl.rspec
  watch(rspec.spec_helper) { rspec.spec_dir }
  watch(rspec.spec_support) { rspec.spec_dir }
  watch(rspec.spec_files)

  watch('lib/mxruby.rb') { rspec.spec_dir }
  watch('lib/mxruby/mx.rb') { rspec.spec_dir }
  watch('lib/mxruby/mxruby.bundle') { rspec.spec_dir }
  watch(/^lib\/mxruby\/extend\/.+\.rb$/) { rspec.spec_dir }
end
