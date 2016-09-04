require 'mkmf'

have_library('cblas')
dir_config('cblas', '/usr/local/opt/openblas/include', '/usr/local/opt/openblas/lib')

have_header('cblas.h')

create_makefile('mxruby')
