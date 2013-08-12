"=============================================================================
" FILE: aeroglass.vim
" AUTHOR: sgur <sgurrr+vim@gmail.com>
" License: MIT license  {{{
"     Permission is hereby granted, free of charge, to any person obtaining
"     a copy of this software and associated documentation files (the
"     "Software"), to deal in the Software without restriction, including
"     without limitation the rights to use, copy, modify, merge, publish,
"     distribute, sublicense, and/or sell copies of the Software, and to
"     permit persons to whom the Software is furnished to do so, subject to
"     the following conditions:
"
"     The above copyright notice and this permission notice shall be included
"     in all copies or substantial portions of the Software.
"
"     THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS
"     OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF
"     MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT.
"     IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY
"     CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT,
"     TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE
"     SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
" }}}
"=============================================================================

if has('win64')
  let s:aeroglass_dll = expand('<sfile>:p:h') . '\aeroglass64.dll'
else
  let s:aeroglass_dll = expand('<sfile>:p:h') . '\aeroglass32.dll'
endif

if !exists('s:aeroglass_state')
  let s:aeroglass_state = 0
endif

function! aeroglass#enable()
  call libcallnr(s:aeroglass_dll, 'aeroglass_enable', v:windowid)
  let s:aeroglass_state = 1
endfunction

function! aeroglass#disable()
  call libcallnr(s:aeroglass_dll, 'aeroglass_disable', v:windowid)
  let s:aeroglass_state = 0
endfunction

function! aeroglass#toggle()
  if s:aeroglass_state
    call aeroglass#disable()
  else
    call aeroglass#enable()
  endif
endfunction
