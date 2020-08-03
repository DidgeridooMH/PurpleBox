project('purplebox', 'cpp',
  version : '0.1',
  default_options : ['warning_level=3', 'cpp_std=c++17'])

src = [
  'src/main.cpp',
  'src/core/cdrom.cpp',
  'src/utils/log.cpp',
  'src/utils/memory.cpp']
inc = include_directories('src/')

executable('purplebox',
  sources : src,
  include_directories : inc,
  cpp_args : '-DVERSION=@0@'.format(meson.project_version())
)
