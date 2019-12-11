
QT_DIR= $$[QT_HOST_BINS]

win32:QMAKE_BIN= $$QT_DIR/qmake.exe
win32:DEPLOYER = %cqtdeployer%


contains(QMAKE_HOST.os, Linux):{
    QMAKE_BIN= $$QT_DIR/qmake
    DEPLOYER = cqtdeployer
}

sopprted_versions = 3.2 3.1 3.0 2.0
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

deploy_dep.commands += $$DEPLOYER -qmake $$QMAKE_BIN -confFile $$PWD/deploy.json

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
