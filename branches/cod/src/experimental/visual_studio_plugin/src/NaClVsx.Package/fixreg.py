#! -*- python -*-
#
# Copyright (c) 2011 The Native Client Authors. All rights reserved.
# Use of this source code is governed by a BSD-style license that can be
# found in the LICENSE file.

"""Fixes up the registry entries for the VSX plugin installed components
"""

__author__ = 'mlinck@google.com (Michael Linck)'

import fileinput
import os.path
import shutil
import sys


def main(argv):
  """Replaces absolute paths with a variable.

  This script searches through the registry information file created for the
  installer and replaces absolute paths with a tag that can be easily found and
  overwritten at install time.

  Besides replacing all the path pointing at bin\Debug (the CodeBase), it also
  overwrites the path to the Templates directory.  In the installed product
  these should be the same.  Note that this script is run only by the Visual
  Studio Project itself and thus we assume we always get the correct input.

  Unbeknownst to the caller, but to ensure that this script cannot cause any
  regressions relative to the fixreg.pl script that it replaces, it also has to
  replace the location of the windows system32 folder in case other machines
  keep it somewhere else.

  Args:
    argv: The arguments passed to the script by the shell.  We expect the
          following arguments in the following order:
          1. code_directory
          2. old_registry_entry_file (The file generated by RegPkg.exe)
          2. new_registry_entry_file (This script's output)
          3. search_tag
  """
  code_base_string = argv[0].rstrip("\\ \n");
  code_base_string = code_base_string.replace("\\", "\\\\")
  search_tag = argv[3]  
  template_base_string = "%s\\\\..\\\\.." % search_tag
  old_registry_info_file = argv[1];
  new_registry_info_file = argv[2];

  shutil.copy(old_registry_info_file, new_registry_info_file)
  for line in fileinput.input(new_registry_info_file, inplace=1, mode='U'):
    line = line.replace(code_base_string, search_tag)
    line = line.replace(template_base_string, search_tag)
    line = line.replace("/C:\\\\Windows\\\\system32\\\\", "[SystemFolder]")
    sys.stdout.write(line)


if __name__ == '__main__':
  main(sys.argv[1:])