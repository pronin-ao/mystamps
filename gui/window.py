from PyQt5.QtWidgets import QWidget, QLabel
from PyQt5.QtGui import QPixmap

VER = '4'


def test_window():

    widget = QWidget(windowTitle="Stamps((")
    widget.setStyleSheet('background-color: blue;')
    text_label = QLabel('TEXT ' + VER, widget)

    text_label.move(0, 0)
    text_label.resize(300, 80)
    text_label.setStyleSheet('background-color: black; color: cyan;')
    return widget