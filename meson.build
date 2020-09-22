project('purplebox', 'cpp',
  license : 'GPL',
  version : '0.1',
  default_options : ['warning_level=3', 'cpp_std=c++2a'])

inc = include_directories('src/')

src = [
  'src/main.cpp',
  'src/core/cdrom.cpp',
  'src/core/interpretter/gekko.cpp',
  'src/utils/log.cpp',
  'src/utils/arch.cpp',
  'src/core/memory/ram.cpp',
  'src/core/memory/bus.cpp',
  'src/utils/cmdargs.cpp',
  'src/core/formats/Format.cpp',
  'src/core/formats/DFormat.cpp',
  'src/core/formats/XfxFormat.cpp']

executable('purplebox',
  sources : src,
  include_directories : inc,
  cpp_args : '-DVERSION=@0@'.format(meson.project_version())
)
