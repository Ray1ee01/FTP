from PyQt5 import QtGui, QtWidgets
from mainwindow import *
import sys


if __name__ == '__main__':
    app = QtWidgets.QApplication(sys.argv)
    client = FTPClient()
    client.show()
    app.exec_()
