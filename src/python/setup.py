# RUN THIS FROM PROJECT ROOT AS CWD
import os
import sys
from distutils.core import setup, Extension
import pathlib

if __name__ == "__main__":

	if not sys.platform.startswith("linux"):
		raise NotImplementedError("Only linux supported at the moment.")

	sourcefiles = [str(p) for p in pathlib.Path("src").rglob('*.cpp')]
	sourcefiles += [str(p) for p in pathlib.Path("build/CVSCODE_autogen/UVLADIE3JM/").rglob('*.cpp')]
	# headerfiles = [str(p) for p in pathlib.Path("src").rglob('*.h')]
	# headerfiles += [str(p) for p in pathlib.Path("build/CVSCODE_autogen/UVLADIE3JM/").rglob('*.h')]
	os.environ["CC"] = "/usr/bin/x86_64-linux-gnu-g++-9"
	module1 = Extension('mytest',
			include_dirs = [
				'src',
				'include',
				'build/CVSCODE_autogen/include',
				'/usr/lib/x86_64-linux-gnu/qt5/mkspecs/linux-g++',
				'/usr/include/x86_64-linux-gnu/qt5/',
				'/usr/include/x86_64-linux-gnu/qt5/QtWidgets',
				'/usr/include/x86_64-linux-gnu/qt5/QtGui',
				'/usr/include/x86_64-linux-gnu/qt5/QtCore',
				'/usr/include/x86_64-linux-gnu/qt5/QtOpenGL'
			],
			libraries = ['assimp', 'Qt5Core', 'Qt5Widgets', 'Qt5OpenGL', 'python3.8'],
			extra_compile_args=['-std=gnu++17', '-DPYTHON_USE', '-O0'],
			library_dirs = ['/home/n.terzopoulos/Projects/QtGL/lib', '/usr/lib', '/usr/lib/x86_64-linux-gnu', '/usr/lib/python3.8/config-3.8-x86_64-linux-gnu'],
			extra_link_args=['-L/usr/lib', '-lcrypt', '-lpthread', '-ldl', '-lutil', '-lm',
				'-Wl,-rpath,/usr/lib/x86_64-linux-gnu/libQt5OpenGL.so.5.12.8 /usr/lib/x86_64-linux-gnu/libpython3.8.so /usr/lib/x86_64-linux-gnu/libQt5Widgets.so.5.12.8 /usr/lib/x86_64-linux-gnu/libQt5Gui.so.5.12.8 /usr/lib/x86_64-linux-gnu/libQt5Core.so.5.12.8'
				],
			sources = sourcefiles,
	)
	setup(name = 'PackageNameTest', version = '1.0', description = 'This is a test package', ext_modules = [module1])