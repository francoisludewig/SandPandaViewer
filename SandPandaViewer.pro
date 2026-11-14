QT       += core gui openglwidgets
greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

CONFIG += c++17

# Nom de l'exécutable (utilisé aussi pour l'icône et le fichier .desktop)
TARGET = sandpandaviewer

SOURCES += \
    Model/cone.cpp \
    Model/disk.cpp \
    Model/elbow.cpp \
    Model/plan.cpp \
    Model/solid.cpp \
    Model/sphere.cpp \
    Model/velocity.cpp \
    Repository/containerrepository.cpp \
    Repository/sphererepository.cpp \
    main.cpp \
    mainwindow.cpp \
    myglwidget.cpp \
    stripeonsphere.cpp

HEADERS += \
    Model/cone.h \
    Model/disk.h \
    Model/elbow.h \
    Model/plan.h \
    Model/solid.h \
    Model/sphere.h \
    Model/velocity.h \
    Repository/containerrepository.h \
    Repository/sphererepository.h \
    mainwindow.h \
    myglwidget.h \
    stripeonsphere.h

FORMS += \
    mainwindow.ui

LIBS += -lGL -lGLU -lglut

# ---------------------------------------------------------------------------
#  Installation Linux : "make install"
#  Par défaut dans ~/.local (pas besoin de sudo).
#  Pour une installation système :  qmake PREFIX=/usr/local  puis  sudo make install
# ---------------------------------------------------------------------------
unix:!android {
    isEmpty(PREFIX): PREFIX = $$(HOME)/.local

    # Fichier .desktop généré à partir du modèle (remplace $$PREFIX et $$TARGET)
    desktopfile.input  = $$PWD/deploy/sandpandaviewer.desktop.in
    desktopfile.output = $$OUT_PWD/$${TARGET}.desktop
    QMAKE_SUBSTITUTES += desktopfile

    # Exécutable
    target.path = $$PREFIX/bin

    # Lanceur (menu des applications / dock)
    desktop.path  = $$PREFIX/share/applications
    desktop.files = $$OUT_PWD/$${TARGET}.desktop

    # Icône
    icon.path  = $$PREFIX/share/icons/hicolor/scalable/apps
    icon.files = $$PWD/deploy/$${TARGET}.svg

    # Rafraîchit les caches de GNOME après installation
    refresh.path  = $$PREFIX
    refresh.extra = update-desktop-database $$PREFIX/share/applications || true ; \
                    gtk-update-icon-cache -f -t $$PREFIX/share/icons/hicolor || true

    INSTALLS += target desktop icon refresh
}
