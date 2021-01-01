import sys
from PyQt5.QtWidgets import QApplication, QWidget, QLabel
from PyQt5.QtGui import QIcon, QPixmap
from PyQt5.QtCore import pyqtSlot


from PyQt5.QtQml import qmlRegisterType, QQmlComponent, QQmlApplicationEngine

import urllib.request as ur

def test_window(data):
    app = QApplication(sys.argv)
    widget = QWidget()

    textLabel = QLabel('TEXT', widget)
    link = data['USSR']['default'][0]['images'][0]
    try:
        data = ur.urlopen(link).read()
        image = QPixmap()
        image.loadFromData(data)
        textLabel.setPixmap(image)
    except:
        textLabel.setText('FAILED')
    textLabel.move(0, 0)

    widget.setGeometry(50, 50, 320, 200)
    widget.setWindowTitle("PyQt5 Example")
    widget.show()
    sys.exit(app.exec_())

def window(data):
    # app = QApplication(sys.argv)

    app = QApplication(sys.argv)

    # Create a QML engine.
    engine = QQmlApplicationEngine('main.qml')
    print('0')
    # engine.load()
    print('1')

    # Create a component factory and load the QML script.
    # component = QQmlComponent(engine)
    # print('2')
    # component.loadUrl(QUrl('main.qml'))
    # print('3')

    # Create an instance of the component.
    # qml = component.create()
    print('4')

    if engine is not None:
        sys.exit(app.exec_())