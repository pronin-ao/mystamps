import sys
from PyQt5.QtWidgets import QApplication


from PyQt5.QtQml import qmlRegisterType, QQmlComponent, QQmlApplicationEngine


def window():
    app = QApplication(sys.argv)
    engine = QQmlApplicationEngine('main.qml')
    if engine is not None:
        sys.exit(app.exec_())

window()