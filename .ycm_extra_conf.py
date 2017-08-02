# Copyright 2017 The BerryDB Authors. All rights reserved.
# Use of this source code is governed by a BSD-style license that can be
# found in the LICENSE file.

import os

BASE_FLAGS = [
    '-Wall',
    '-Wextra',
    '-Werror',
    '-DUSE_CLANG_COMPLETER',  # YCM needs this.
    '-xc++',  # YCM needs this to avoid compiling headers as C code.
]

SOURCE_EXTENSIONS = [ '.cc' ]

HEADER_EXTENSIONS = [ '.h' ]

SOURCE_DIRECTORIES = [ 'src' ]

HEADER_DIRECTORIES = [ 'include', 'platform' ]

def DirectoryOfThisScript():
  return os.path.dirname(os.path.abspath(__file__))

def IsHeaderFile( filename ):
  extension = os.path.splitext(filename)[1]
  return extension in HEADER_EXTENSIONS

def MakeRelativePathsInFlagsAbsolute(flags, build_root):
  new_flags = []
  make_next_absolute = False
  path_flags = [ '-isystem', '-I', '-iquote', '--sysroot=' ]
  for flag in flags:
    new_flag = flag

    if make_next_absolute:
      make_next_absolute = False
      if not flag.startswith('/'):
        new_flag = os.path.join(build_root, flag)

    for path_flag in path_flags:
      if flag == path_flag:
        make_next_absolute = True
        break

      if flag.startswith(path_flag):
        path = flag[ len(path_flag): ]
        new_flag = path_flag + os.path.join(build_root, path)
        break

    if new_flag:
      new_flags.append(new_flag)
  return new_flags

def FindNearest(path, target, build_root):
    candidate = os.path.join(path, target)
    if os.path.isfile(candidate):
        return candidate

    if path == build_root:
        return None

    parent = os.path.dirname(path)
    if parent == path:
        return None

    return FindNearest(parent, target, build_root)

def FlagsForClangComplete(file_path, build_root):
    clang_complete_path = FindNearest(file_path, '.clang_complete', build_root)
    clang_complete_flags = open(clang_complete_path, 'r').read().splitlines()
    return clang_complete_flags

def FlagsForFile(filename, **kwargs):
    build_root = DirectoryOfThisScript()
    file_path = os.path.realpath(filename)

    flags = BASE_FLAGS
    clang_flags = FlagsForClangComplete(file_path, build_root)
    if clang_flags:
        flags = flags + clang_flags

    final_flags = MakeRelativePathsInFlagsAbsolute(flags, build_root)

    return { 'flags': final_flags }
