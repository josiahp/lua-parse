project('lua', 'cpp',
  version : '0.1',
  default_options : ['warning_level=3', 'cpp_std=c++17'])

add_project_arguments('-fno-exceptions', language : 'cpp')

gtest_proj = subproject('gtest')
gtest_dep = dependency('gtest', main : true, required : false)
gmock_dep = gtest_proj.get_variable('gmock_dep')
gtest_main = gtest_proj.get_variable('gmock_dep')

inc = include_directories('src')
src = [ 'src/lua.cpp', 'src/lexer.cpp', 'src/token.cpp', 'src/parser.cpp' ]
test_lexer_src = [ 'test/lexer_test.cpp', 'src/lexer.cpp', 'src/token.cpp' ]
test_parser_src = [ 'test/parser_test.cpp', 'src/lexer.cpp', 'src/token.cpp', 'src/parser.cpp' ]

exe = executable('lua', src,
  install : true)

test_lexer = executable('test_lexer', test_lexer_src, include_directories: inc, dependencies: [ gtest_dep, gmock_dep])
test('test_lexer', test_lexer)

test_parser = executable('test_parser', test_parser_src, include_directories: inc, dependencies: [ gtest_dep, gmock_dep])
test('test_parser', test_parser)