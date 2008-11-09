from distutils.core import setup, Extension

mgl_module = Extension('_mathgl',
    sources=['lang/mgl_python.cpp'],
    include_dirs=['./include'],
    extra_objects=['./mgl/libmgl.so']
    )

setup (
    name = "mathgl",
    version = "1.7",
    maintainer="Dmitry Kulagin",
    maintainer_email="dik@ufp.appl.sci-nnov.ru",
    author="Alexey Balakin",
    author_email= "balakin@appl.sci-nnov.ru",
    license="Same terms as MathGL",
    platforms = ["any"],
    url = "http://mathgl.sf.net",
    description = "Python interface for MathGL",
    long_description = "MathGL is a library for the plotting of the data \
 MathGL is a free library of fast C++ routines for the plotting \
 of the data varied in one or more dimensions. It uses OpenGL \
 (www.opengl.org) for the plotting. Also there is a simple window \
 interface based on GLUT. This provides high compatibility with \
 any operating system (really any which has OpenGL-like libraries). \
 Python interface for MathGL",
    package_dir = {"": "lang"},
    py_modules = ["mathgl"],
    ext_modules = [mgl_module]
    )
