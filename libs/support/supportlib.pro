# Subdir relative project main directory: ./supportlib/src
# Target is an application:  ../../release/support

################################ Template ####################################

TEMPLATE = lib

########################### Library Version ##################################

unix {
    isEmpty( VERSION ) {
        VERSION = 0.0.3
    }
}
############################# Header Files ##################################

HEADERS += 	./include/perf_timer.h
		   	
############################# Source Files ##################################

SOURCES += 	./src/perf_timer.cpp \
			./src/loggerlib.cpp
            
############################# Target File ##################################

CONFIG(debug, debug|release) {
	win32 {
		TARGET = ./../debug/supportd
	} else {
		TARGET = ./../debug/supportd
	}
} else {
	win32 {
		TARGET = ./../release/support
	} else {
		TARGET = ./../release/support
	}
}

############################# Include Paths ##################################

INCLUDEPATH = 	./include \
				./../common/include \

############################# Dependencies ##################################

DEPENDPATH += ;$$INCLUDEPATH

############################# Library Paths ##################################

CONFIG(debug, debug|release) {
    LIBS += -L./../debug
}
else {
    LIBS += -L./../release
}

############################### Libraries ####################################

### Linux ###

############################# QT Libraries ##################################

QT += 

########################## Configuration Options ############################

CONFIG += 	warn_on \
			qt \
            silent \
			dll

###################### Intermediate Output Directories ######################

CONFIG(debug, debug|release) {
    win32 {
        MOC_DIR = ./src/moc-dbg
        OBJECTS_DIR = ./src/obj-dbg
    } else {
        MOC_DIR = ./src/lmoc-dbg
        OBJECTS_DIR = ./src/lobj-dbg
    }
} else {
    win32 {
        MOC_DIR = ./src/moc-rel
        OBJECTS_DIR = ./src/obj-rel
    } else {
        MOC_DIR = ./src/lmoc-rel
        OBJECTS_DIR = ./src/lobj-rel
    }
}

############################# DEFINES ##################################

DEFINES += CREATE_DLL __SUPPORTLIB_BUILD__
DEFINES -= UNICODE

#############################################################################
