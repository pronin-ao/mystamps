'''import sys
from PyQt5.QtWidgets import QApplication, QWidget, QLabel
from PyQt5.QtGui import QIcon, QPixmap
from PyQt5.QtCore import pyqtSlot


from PyQt5.QtQml import qmlRegisterType, QQmlComponent, QQmlApplicationEngine'''



'''

import urllib.request as ur

import sys
from PyQt5.QtWidgets import QApplication, QWidget, QLabel
from PyQt5.QtGui import QPixmap

def test_window(data):
    print('111')
    app = QApplication(sys.argv)
    print('222')
    widget = QWidget()
    widget.setStyleSheet('background-color: red;')
    print('333')
    text_label = QLabel('TEXT', widget)
    link = data['USSR']['default'][0]['images'][0]
    try:
        data = ur.urlopen(link).read()
        image = QPixmap()
        image.loadFromData(data)
        text_label.setPixmap(image)
    except:
        text_label.setText('FAILED')
    text_label.move(0, 0)
    text_label.resize(100, 100)
    print('444')
    widget.show()
    print('555')
    ret = app.exec_()
    print('!!', ret)
    sys.exit(ret)
'''

'''
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
        '''