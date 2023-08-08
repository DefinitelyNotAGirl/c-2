#!/usr/bin/env python3
# -*- coding:utf-8 -*-
###
# Created Date: Tuesday August 8th 2023
# Author: Lilith
# -----
# Last Modified: Tuesday August 8th 2023 12:24:02 pm
# Modified By: Lilith (definitelynotagirl115169@gmail.com)
# -----
# Copyright (c) 2023-2023 DefinitelyNotAGirl@github
# 
# Permission is hereby granted, free of charge, to any person
# obtaining a copy of this software and associated documentation
# files (the "Software"), to deal in the Software without
# restriction, including without limitation the rights to use, copy,
# modify, merge, publish, distribute, sublicense, and/or sell copies
# of the Software, and to permit persons to whom the Software is
# furnished to do so, subject to the following conditions:
# 
# The above copyright notice and this permission notice shall be
# included in all copies or substantial portions of the Software.
# 
# THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND,
# EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF
# MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND
# NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT
# HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY,
# WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
# OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER
# DEALINGS IN THE SOFTWARE.
###

import git

output = "inc/buildinfo.h"

content = ""

repo = git.Repo(search_parent_directories=False)
commitID = repo.head.commit.hexsha
#commitDATE = repo.head.commit.committed_datetime.strftime("%d.%m.%Y %H:%M:%S %Z")
commitDATE = repo.head.commit.committed_datetime
commitBRANCH = repo.active_branch

content+="#define buildCOMMIT \""+str(commitID)+"\"\n"
content+="#define buildDATE \""+str(commitDATE)+"\"\n"
content+="#define buildBRANCH \""+str(commitBRANCH)+"\"\n"

with open(output, "w") as text_file:
    text_file.write(content)