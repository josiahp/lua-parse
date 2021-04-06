project('lua', 'cpp',
  version : '0.1',
  default_options : ['warning_level=3', 'cpp_std=c++17'])

add_project_arguments('-fno-exceptions', language : 'cpp')

gtest_proj = subproject('gtest')
gtest_dep = dependency('gtest', main : true, required : false)
gmock_dep = gtest_proj.get_variable('gmock_dep')
gtest_main = gtest_proj.get_variable('gmock_dep')

inc = include_directories('src')
src = [ 'src/lua.cpp', 'src/lexer.cpp', 'src/token.cpp' ]
test = [ 'test/lexer_test.cpp', 'src/lexer.cpp', 'src/token.cpp' ]

exe = executable('lua', src,
  install : true)

gtest_lexer = executable('gtest-lexer', test, include_directories: inc, dependencies: [ gtest_dep, gmock_dep])
test('gtest-lexer', gtest_lexer)