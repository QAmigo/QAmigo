
QT_DIR= $$[QT_HOST_BINS]

win32:QMAKE_BIN= $$QT_DIR/qmake.exe
win32:DEPLOYER = %cqtdeployer%


contains(QMAKE_HOST.os, Linux):{
    QMAKE_BIN= $$QT_DIR/qmake
    DEPLOYER = cqtdeployer
}

sopprted_versions = 3.1 3.0 2.0
for(val, sopprted_versions) {

    exists( $$QT_DIR/../../../Tools/QtInstallerFramework/$$val/bin/ ) {
          message( "QtInstallerFramework v$$val: yes" )
          BINARY_LIST += $$QT_DIR/../../../Tools/QtInstallerFramework/$$val/bin/binarycreator
    }
}

isEmpty (BINARY_LIST) {
      error( "QtInstallerFramework not found!" )
}

win32:EXEC=$$first(BINARY_LIST).exe

contains(QMAKE_HOST.os, Linux):{
    unix:EXEC=$$first(BINARY_LIST)
    win32:EXEC=wine $$first(BINARY_LIST).exe
}

message( selected $$EXEC )

mkpath( $$PWD/../Distro)
win32:OUT_FILE = QAmigoInstaller.exe
unix:OUT_FILE = QAmigoInstaller.run

DEPLOY_TARGET = $$PWD/../build/release

DATA_DIR = $$PWD/QIF/packages/QAmigo/data/
META_DIR = $$PWD/QIF/packages/QAmigo/meta/

win32:OUT_LIB= -libOut .
win32:OUT_BIN= -binOut .

BASE_DEPLOY_FLAGS = clear -qmake $$QMAKE_BIN -libDir $$PWD/../ -recursiveDepth 4 -ignoreEnv $$DEPLOY_TARGET
BASE_DEPLOY_FLAGS_CQT = $$BASE_DEPLOY_FLAGS -targetDir $$DATA_DIR $$OUT_LIB $$OUT_BIN

deploy_dep.commands += $$DEPLOYER -bin $$DEPLOY_TARGET $$BASE_DEPLOY_FLAGS_CQT

CONFIG_FILE = $$PWD/QIF/config/config.xml

deployOffline.commands = $$EXEC \
                       --offline-only \
                       -c $$CONFIG_FILE \
                       -p $$PWD/QIF/packages \
                       $$PWD/../Distro/$$OUT_FILE

deploy.depends = deploy_dep
deploy.depends += deployOffline

QMAKE_EXTRA_TARGETS += \
    deploy_dep \
    deployOffline \
    deploy
