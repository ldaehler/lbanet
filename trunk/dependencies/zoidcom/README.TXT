=======================================================
      Zoidcom Automated Networking System 0.6.11
          Copyright ©2002-2008 Joerg Rueppel

                 http://www.zoidcom.com
=======================================================


Documentation can be found in html form in the doc subdir.
Latest changes can be found in changelog.txt.

For each development tool, link against the lib in the 'Linker'
column, and provide the binary file under 'Redistribute' to the
users of your applications. The redistribution files have to be
in the same folder as the application binary resides, or somewhere
in the system's shared object searchpath.

Add the 'include' subdirectory to your compiler's include search
path or copy the include files into a system include directory.

DevCpp users are ready to go once the .DevPak is installed and
don't need to setup or copy anything.


               Linker                 Redistribute
------------------------------------------------------
-- Release --
MSVC users     zoidcom_vc.lib         zoidcom_vc.dll
MingW32        libzoidcom_mw.a        zoidcom_mw.dll
GCC/Linux      libzoidcom.so          libzoidcom.so
-- Debug --
MSVC users     zoidcom_vcd.lib        zoidcom_vcd.dll
MingW32        libzoidcom_mwd.a       zoidcom_mwd.dll
GCC/Linux      libzoidcomd.so         libzoidcomd.so
------------------------------------------------------

The debug libs do not contain debug symbols, they perform lots
of error checking, use a lot more bandwidth and are also unable 
to communicate with the non debug versions.

Examples
--------

Precompiled example binaries are available in the samples
directory under bin. All samples come with project
files for DevCpp, Code::Blocks, MSVC.NET 2003 and GNU Make
files for Linux. The .workspace file is also for Code::Blocks
and contains a workspace with all samples, as much as the .sln
file does for Visual Studio.

Each example comes with a small readme describing the main function.

Feedback
--------

I am open to feature requests, suggestions and feedback in any
form. Also in case the documentation is insufficient or misleading
I'd like to hear about that so I can fix it.

email: joerg@sharky-x.de
forums: http://www.zoidcom.com/ -> forum

Restrictions
------------

This version is available for noncommercial use only. Commercial
licensing options will be made available for future versions.

----------------------------------------------------------------------------
Zoidcom End User License Agreement
----------------------------------------------------------------------------

Zoidcom's names, sources, documentation and binaries contained within the 
distributed archive, except for Allegro's shared library alleg42.dll,
are copyright © 2002-2008 Joerg Rueppel.

The contents of the Zoidcom distribution archive may not be redistributed, 
reproduced, modified, transmitted, broadcast, published or adapted in any 
way, shape or form, without the prior written consent of the owner, 
Joerg Rueppel, be it by tangible or non tangible media.

Zoidcom's redistribution binaries as enlisted above may be redistributed 
without the authors prior permission, and must remain unmodified.  
The use of dll 'static linking' tools is prohibited.

THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
``AS IS'' AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR
A PARTICULAR PURPOSE ARE DISCLAIMED.  IN NO EVENT SHALL THE REGENTS OR
CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL,
EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO,
PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR
PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF
LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING
NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.

----------------------------------------------------------------------------
JThread License Agreement
----------------------------------------------------------------------------

Copyright (c) 2000-2001  Jori Liesenborgs (jori@lumumba.luc.ac.be)

Permission is hereby granted, free of charge, to any person obtaining a
copy of this software and associated documentation files (the "Software"),
to deal in the Software without restriction, including without limitation
the rights to use, copy, modify, merge, publish, distribute, sublicense,
and/or sell copies of the Software, and to permit persons to whom the
Software is furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in
all copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT.  IN NO EVENT SHALL
THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING
FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER
DEALINGS IN THE SOFTWARE.

