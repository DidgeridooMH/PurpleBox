project('purplebox', 'cpp',
  license : 'GPL',
  version : '0.1',
  default_options : ['warning_level=3', 'cpp_std=c++17'])

inc = include_directories('src/')

src = [
  'src/core/cdrom.cpp',
  'src/core/interpretter/gekko.cpp',
  'src/core/interpretter/integer_ops.cpp',
  'src/core/interpretter/memory_ops.cpp',
  'src/core/interpretter/special_reg_ops.cpp',
  'src/utils/log.cpp',
  'src/utils/arch.cpp',
  'src/core/memory/ram.cpp',
  'src/core/memory/bus.cpp',
  'src/utils/cmdargs.cpp',
  'src/core/formats/format.cpp',
  'src/core/formats/dformat.cpp',
  'src/core/formats/xfxformat.cpp'
]

executable('purplebox',
  sources : [src, 'src/main.cpp'],
  include_directories : inc,
  cpp_args : '-DVERSION=@0@'.format(meson.project_version())
)

gtest = subproject('gtest')
gtest_dep = gtest.get_variable('gtest_dep')
gmock_dep = gtest.get_variable('gmock_dep')

test_src = [
  'test/test.cpp',
  'test/utils/arch.cpp',
  'test/utils/cmdargs.cpp',
  'test/core/memory/ram.cpp',
  'test/core/interpretter/gekkotest.cpp'
]
test_src += src

test_inc = []
test_inc += inc

purplebox_test = executable(
  'purplebox-test',
  test_src,
  include_directories : test_inc,
  dependencies : [
    gtest_dep,
    gmock_dep
  ]
)

test('PurpleBox Tests', purplebox_test)
