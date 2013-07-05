#!/bin/env python

# Copyright (c) 2013 Caitlin Potter and Contributors
# All rights reserved.
#
# Redistribution and use in source and binary forms, with or without
# modification, are permitted provided that the following conditions are
# met:
#
#  * Redistributions of source code must retain the above copyright
# notice, this list of conditions and the following disclaimer.
#  * Redistributions in binary form must reproduce the above copyright
# notice, this list of conditions and the following disclaimer in the
# documentation and/or other materials provided with the distribution.
#
# THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
# ``AS IS'' AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
# LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR
# A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT
# HOLDER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL,
# SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT
# LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE,
# DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY
# THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
# (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
# OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.

import sys, os
from waflib import Task
from waflib.Build import BuildContext

APPNAME="timedtext"
VERSION="0.1"

SUBDIRS=["src","test","include"]

def options(ctx):
	ctx.load("compiler_cxx compiler_c waf_unit_test xcode msvs make eclipse")
	ctx.recurse(SUBDIRS)

def configure(ctx):
	ctx.load("compiler_cxx compiler_c waf_unit_test xcode msvs make eclipse")
	ctx.recurse(SUBDIRS)

def build(ctx):
	ctx.recurse(SUBDIRS)

for y in 'check'.split():
	class tmp(BuildContext):
		cmd=y
		fun='build'

def init(ctx):
	#Quieter task output.
	default_task=Task.Task.__str__
	def quiet_task(self):
		env=self.env
		name=None
		task=self.__class__.__name__.replace('_task','')

		#Completely silent tasks (no output)
		if task in ['utest']:
			return ''
		#Output something if we aren't using a spammy toolchain (MSVC)
		if ((task in ['cxx'] and env.CXXNAME not in ['msvc','wsdk'])
		    or (task in ['c'] and env.CCNAME not in ['msvc','wsdk'])):
			task='compiling'
			name=self.inputs[0]
		elif task.endswith('program') or task.endswith('lib'):
			task='linking'
			name=self.outputs[0]
		else: return default_task(self)
		return '%s: %s\n'%(task,name)
	Task.Task.__str__=quiet_task

	# Routine to find source files recursively
	def sources(ctx,**kw):
		_kw=kw
		ctxpath=ctx.path.abspath()
		path=ctxpath if not 'path' in kw else kw['path']
		skip=[] if not 'skip' in kw else kw['skip']
		ext=['.cpp','.cc','.c'] if not 'ext' in kw else kw['ext']
		src=[]
		for e in os.listdir(path):
			if e not in skip:
				full=os.path.abspath(os.path.join(path,e))
				if os.path.isdir(full):
					_kw['path']=full
					src.extend(ctx.sources(**_kw))
				elif os.path.isfile(full):
					for x in ext:
						if e.endswith(x):
							full=os.path.relpath(full,ctxpath)
							src.append(full)
							break
		return src
	BuildContext.sources=sources