include(GenerateExportHeader)
add_compiler_export_flags()

if(enable-qt4)
	include(../../cmake-qt4.txt)
	set(MGL_QT4_FILES ../qt.cpp ../../include/mgl2/qt.h ../../include/mgl2/qmathgl.h)
	mgl_add_lib(qt4 ${MGL_QT4_FILES})
	qt4_use_modules(mgl-qt4 ${MGL_QT4_LIBS})
	qt4_use_modules(mgl-qt4-static ${MGL_QT4_LIBS})
	target_link_libraries(mgl-qt4 mgl)
	target_link_libraries(mgl-qt4-static mgl)
	if(NOT enable-qt5asqt)
		mgl_add_lib(qt ${MGL_QT4_FILES})
		qt4_use_modules(mgl-qt ${MGL_QT4_LIBS})
		qt4_use_modules(mgl-qt-static ${MGL_QT4_LIBS})
		target_link_libraries(mgl-qt mgl)
		target_link_libraries(mgl-qt-static mgl)
	endif(NOT enable-qt5asqt)

	if(MGL_HAVE_FLTK AND NOT enable-qt5)
		mgl_add_lib(wnd ${MGL_QT4_FILES} ../fltk.cpp ../../include/mgl2/fltk.h)
		target_include_directories(mgl-wnd SYSTEM PUBLIC ${FLTK_INCLUDE_DIR})
		target_include_directories(mgl-wnd-static SYSTEM PUBLIC ${FLTK_INCLUDE_DIR})
		qt4_use_modules(mgl-wnd ${MGL_QT4_LIBS})
		qt4_use_modules(mgl-wnd-static ${MGL_QT4_LIBS})
		target_link_libraries(mgl-wnd mgl ${FLTK_LIBRARIES})
		target_link_libraries(mgl-wnd-static mgl ${FLTK_LIBRARIES})
	endif(MGL_HAVE_FLTK AND  NOT enable-qt5)
endif(enable-qt4)
