require 'mkmf'

# unless have_library('openblas')
#   dir_config('openblas', '/usr/local/opt/openblas/include', '/usr/local/opt/openblas/lib')
# end

have_library('cblas')
dir_config('cblas', '/usr/local/opt/openblas/include', '/usr/local/opt/openblas/lib')

have_header('cblas.h')

create_makefile('mxruby/mxruby')
