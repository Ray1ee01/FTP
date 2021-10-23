# -*- coding: utf-8 -*-

# Form implementation generated from reading ui file 'mainwindow.ui'
#
# Created by: PyQt5 UI code generator 5.15.2
#
# WARNING: Any manual changes made to this file will be lost when pyuic5 is
# run again.  Do not edit this file unless you know what you are doing.


from PyQt5 import QtCore, QtGui, QtWidgets
from PyQt5.QtWidgets import QApplication, QMainWindow, QTreeWidget, QTreeWidgetItem, QMessageBox, QTextBrowser, QFileDialog
from PyQt5.QtWidgets import QMainWindow
import socket
import re
import os
import time
from thread import *
class Ui_MainWindow(object):
    def setupUi(self, MainWindow):
        MainWindow.setObjectName("MainWindow")
        MainWindow.resize(1200, 900)
        self.centralwidget = QtWidgets.QWidget(MainWindow)
        self.centralwidget.setObjectName("centralwidget")
        self.horizontalLayoutWidget = QtWidgets.QWidget(self.centralwidget)
        self.horizontalLayoutWidget.setGeometry(QtCore.QRect(-1, 0, 1201, 41))
        self.horizontalLayoutWidget.setObjectName("horizontalLayoutWidget")
        self.horizontalLayout = QtWidgets.QHBoxLayout(self.horizontalLayoutWidget)
        self.horizontalLayout.setContentsMargins(0, 0, 0, 0)
        self.horizontalLayout.setObjectName("horizontalLayout")
        self.label = QtWidgets.QLabel(self.horizontalLayoutWidget)
        self.label.setObjectName("label")
        self.horizontalLayout.addWidget(self.label)
        self.ipEdit = QtWidgets.QLineEdit(self.horizontalLayoutWidget)
        self.ipEdit.setObjectName("ipEdit")
        self.horizontalLayout.addWidget(self.ipEdit)
        self.label_2 = QtWidgets.QLabel(self.horizontalLayoutWidget)
        self.label_2.setObjectName("label_2")
        self.horizontalLayout.addWidget(self.label_2)
        self.portEdit = QtWidgets.QLineEdit(self.horizontalLayoutWidget)
        self.portEdit.setObjectName("portEdit")
        self.horizontalLayout.addWidget(self.portEdit)
        self.label_3 = QtWidgets.QLabel(self.horizontalLayoutWidget)
        self.label_3.setObjectName("label_3")
        self.horizontalLayout.addWidget(self.label_3)
        self.userEdit = QtWidgets.QLineEdit(self.horizontalLayoutWidget)
        self.userEdit.setObjectName("userEdit")
        self.horizontalLayout.addWidget(self.userEdit)
        self.label_4 = QtWidgets.QLabel(self.horizontalLayoutWidget)
        self.label_4.setObjectName("label_4")
        self.horizontalLayout.addWidget(self.label_4)
        self.passEdit = QtWidgets.QLineEdit(self.horizontalLayoutWidget)
        self.passEdit.setObjectName("passEdit")
        self.horizontalLayout.addWidget(self.passEdit)
        self.pasvButton = QtWidgets.QRadioButton(self.horizontalLayoutWidget)
        self.pasvButton.setObjectName("pasvButton")
        self.horizontalLayout.addWidget(self.pasvButton)
        self.portButton = QtWidgets.QRadioButton(self.horizontalLayoutWidget)
        self.portButton.setObjectName("portButton")
        self.horizontalLayout.addWidget(self.portButton)
        self.connectButton = QtWidgets.QPushButton(self.horizontalLayoutWidget)
        self.connectButton.setObjectName("connectButton")
        self.horizontalLayout.addWidget(self.connectButton)
        self.cmdBrowser = QtWidgets.QTextBrowser(self.centralwidget)
        self.cmdBrowser.setGeometry(QtCore.QRect(10, 80, 541, 541))
        self.cmdBrowser.setObjectName("cmdBrowser")
        self.localWidget = QtWidgets.QTreeWidget(self.centralwidget)
        self.localWidget.setGeometry(QtCore.QRect(570, 80, 621, 211))
        self.localWidget.setObjectName("localWidget")
        self.localWidget.headerItem().setText(0, "1")
        self.localWidget.setHeaderLabels(['TYPE','Name', 'Size', 'Last Time'])

        self.remoteWidget = QtWidgets.QTreeWidget(self.centralwidget)
        self.remoteWidget.setGeometry(QtCore.QRect(570, 300, 621, 231))
        self.remoteWidget.setObjectName("remoteWidget")
        self.remoteWidget.headerItem().setText(0, "1")
        self.remoteWidget.setHeaderLabels(['TYPE','Name', 'Size', 'Last Time'])

        self.label_5 = QtWidgets.QLabel(self.centralwidget)
        self.label_5.setGeometry(QtCore.QRect(10, 60, 72, 15))
        self.label_5.setObjectName("label_5")
        self.horizontalLayoutWidget_2 = QtWidgets.QWidget(self.centralwidget)
        self.horizontalLayoutWidget_2.setGeometry(QtCore.QRect(570, 540, 621, 41))
        self.horizontalLayoutWidget_2.setObjectName("horizontalLayoutWidget_2")
        self.horizontalLayout_2 = QtWidgets.QHBoxLayout(self.horizontalLayoutWidget_2)
        self.horizontalLayout_2.setContentsMargins(0, 0, 0, 0)
        self.horizontalLayout_2.setObjectName("horizontalLayout_2")
        self.mkdButton = QtWidgets.QPushButton(self.horizontalLayoutWidget_2)
        self.mkdButton.setObjectName("mkdButton")
        self.horizontalLayout_2.addWidget(self.mkdButton)
        self.rnButton = QtWidgets.QPushButton(self.horizontalLayoutWidget_2)
        self.rnButton.setObjectName("rnButton")
        self.horizontalLayout_2.addWidget(self.rnButton)
        self.rmdButton = QtWidgets.QPushButton(self.horizontalLayoutWidget_2)
        self.rmdButton.setObjectName("rmdButton")
        self.horizontalLayout_2.addWidget(self.rmdButton)
        self.retrButton = QtWidgets.QPushButton(self.horizontalLayoutWidget_2)
        self.retrButton.setObjectName("retrButton")
        self.horizontalLayout_2.addWidget(self.retrButton)
        self.storButton = QtWidgets.QPushButton(self.horizontalLayoutWidget_2)
        self.storButton.setObjectName("storButton")
        self.horizontalLayout_2.addWidget(self.storButton)
        self.transWidget = QtWidgets.QTreeWidget(self.centralwidget)
        self.transWidget.setGeometry(QtCore.QRect(10, 640, 1181, 251))
        self.transWidget.setObjectName("transWidget")
        self.transWidget.headerItem().setText(0, "1")
        self.transWidget.header().setSortIndicatorShown(False)
        self.transWidget.setHeaderLabels(
            ['Down/Up','Local','Remote','Size','Status']
        )
        self.mkdEdit = QtWidgets.QLineEdit(self.centralwidget)
        self.mkdEdit.setGeometry(QtCore.QRect(570, 590, 113, 23))
        self.mkdEdit.setObjectName("mkdEdit")
        self.renameEdit = QtWidgets.QLineEdit(self.centralwidget)
        self.renameEdit.setGeometry(QtCore.QRect(700, 590, 113, 23))
        self.renameEdit.setObjectName("renameEdit")
        self.aborButton = QtWidgets.QPushButton(self.centralwidget)
        self.aborButton.setGeometry(QtCore.QRect(820, 590, 121, 24))
        self.aborButton.setObjectName("aborButton")
        self.restButton = QtWidgets.QPushButton(self.centralwidget)
        self.restButton.setGeometry(QtCore.QRect(950, 590, 111, 24))
        self.restButton.setObjectName("restButton")
        MainWindow.setCentralWidget(self.centralwidget)
        self.statusbar = QtWidgets.QStatusBar(MainWindow)
        self.statusbar.setObjectName("statusbar")
        MainWindow.setStatusBar(self.statusbar)

        self.retranslateUi(MainWindow)
        QtCore.QMetaObject.connectSlotsByName(MainWindow)

    def retranslateUi(self, MainWindow):
        _translate = QtCore.QCoreApplication.translate
        MainWindow.setWindowTitle(_translate("MainWindow", "MainWindow"))
        self.label.setText(_translate("MainWindow", "IP"))
        self.label_2.setText(_translate("MainWindow", "PORT"))
        self.label_3.setText(_translate("MainWindow", "USER"))
        self.label_4.setText(_translate("MainWindow", "PASS"))
        self.pasvButton.setText(_translate("MainWindow", "PASV"))
        self.portButton.setText(_translate("MainWindow", "PORT"))
        self.connectButton.setText(_translate("MainWindow", "connect"))
        self.label_5.setText(_translate("MainWindow", "Command"))
        self.mkdButton.setText(_translate("MainWindow", "Create"))
        self.rnButton.setText(_translate("MainWindow", "Rename"))
        self.rmdButton.setText(_translate("MainWindow", "Remove"))
        self.retrButton.setText(_translate("MainWindow", "Download"))
        self.storButton.setText(_translate("MainWindow", "Upload"))
        self.aborButton.setText(_translate("MainWindow", "Pause"))
        self.restButton.setText(_translate("MainWindow", "Continue"))


class FTPClient(QMainWindow,Ui_MainWindow):
    def __init__(self,parent=None):
        super(FTPClient,self).__init__(parent)
        self.setupUi(self)
        self.conn_fd = None
        self.tran_fd = None
        self.listen_fd = None
        self.tran_mode = 'PASV'
        self.local_ip = ''
        self.last_resp = ''
        self.port = 21
        self.remotedir='/home/ubuntu/test'
        self.localdir='E:/ftp'
        self.offset=0
        self.thread=None
        self.pasvButton.clicked.connect(self.set_PASV)
        self.portButton.clicked.connect(self.set_PORT)
        self.connectButton.clicked.connect(self.login)
        self.mkdButton.clicked.connect(self.MKD)
        self.rnButton.clicked.connect(self.rename)
        self.remoteWidget.doubleClicked.connect(self.CWD)
        self.rmdButton.clicked.connect(self.RMD)
        self.retrButton.clicked.connect(self.RETR)
        self.storButton.clicked.connect(self.STOR)
        self.aborButton.clicked.connect(self.ABOR)
        self.restButton.clicked.connect(self.REST)

        self.ipEdit.setText('49.232.106.46')
        self.portEdit.setText('6789')
        self.userEdit.setText('anonymous')
        self.passEdit.setText('123')

        # self.LIST()

        # TODO connect signal and slot
    def get_msg(self):
        self.last_resp=self.conn_fd.recv(8192).decode().replace('\r\n','')
        print(self.last_resp)
        self.cmdBrowser.append('server: '+self.last_resp)
        # self.cmdBrowser.moveCursor(self.cmdBrowser.textCursor().End) 
    def post_msg(self,msg):
        msg+='\r\n'
        # print(self.conn_fd)
        self.conn_fd.send(msg.encode('utf-8'))
        self.cmdBrowser.append('client: '+msg)

        
    # combine connect, USER and PASS
    def login(self):        
        # get local ip
        try:
            # s = socket.socket(socket.AF_INET, socket.SOCK_DGRAM)
            # s.connect(('8.8.8.8', 80))
            # self.local_ip = s.getsockname()[0]
            import requests
            self.local_ip = requests.get('https://checkip.amazonaws.com').text.strip()
        except Exception as e:
            print(str(e))
        # get available port
        try:
            s = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
            s.bind(("", 0))
            self.port = s.getsockname()[1]
        finally:
            s.close()
        # connect
        try:
            self.conn_fd = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
            self.conn_fd.connect((self.ipEdit.text(),int(self.portEdit.text())))
            self.get_msg()
            self.post_msg('USER '+self.userEdit.text())
            self.get_msg()
            self.post_msg('PASS '+self.passEdit.text())
            self.get_msg()
            self.SYST()
            self.TYPE()
            self.LIST()
            self.LocalLIST()
        except Exception as e:
            print(str(e))
    def set_PASV(self):
        self.tran_mode='PASV'
    def set_PORT(self):      
        self.tran_mode='PORT' 
    def ABOR(self):
        # self.post_msg('ABOR')
        self.tran_fd.close()
        self.get_msg()
        # self.get_msg()
        print('abor')
    def REST(self):
        self.BuildDTP()
        self.offset+=8192
        self.post_msg('REST '+str(self.offset))
        if(self.tran_mode=='PORT'):
            self.tran_fd,_=self.listen_fd.accept()
            self.listen_fd.close()
        self.get_msg()
        item=self.transWidget.currentItem()
        filepath=item.text(1)
        def update(offset):
            item.setText(4,str(offset/int(item.text(3))))  
        if item.text(0)=='Download':
            self.thread=DownloadThread(self,filepath)
            self.thread.update_signal.connect(update)
            self.thread.complete_signal.connect(self.finish)
            self.thread.start()
        elif item.text(0)=='Upload':
            self.thread=UploadThread(self,filepath)
            self.thread.update_signal.connect(update)
            self.thread.complete_signal.connect(self.finish)
            self.thread.start()
        
    def PASV(self):
        self.post_msg('PASV')
        self.get_msg()
        host = re.search(r'(\d*),(\d*),(\d*),(\d*),(\d*),(\d*)',self.last_resp).group().split(',')
        ip='.'.join(host[:4])
        port = int(host[4])*256+int(host[5])
        self.tran_fd = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
        self.tran_fd.connect((ip, port))
    def PORT(self):
        s=socket.socket(socket.AF_INET, socket.SOCK_STREAM)
        s.setsockopt(socket.SOL_SOCKET, socket.SO_REUSEADDR, 1)
        s.bind((self.local_ip,0))
        port = s.getsockname()[1]
        print(self.local_ip,port)
        self.listen_fd = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
        self.listen_fd.setsockopt(socket.SOL_SOCKET, socket.SO_REUSEADDR, 1)
        self.listen_fd.bind((self.local_ip,port))
        self.listen_fd.listen(5)
        self.post_msg('PORT '+str(self.local_ip).replace('.',',')+','+str(int(port/256))+','+str(port%256))
        self.get_msg()


    def TYPE(self):
        self.post_msg('TYPE I')
        self.get_msg()
    def SYST(self):
        self.post_msg('SYST')
        self.get_msg()
    def PWD(self):
        self.post_msg('PWD')
        self.get_msg()
    def CWD(self):
        item = self.remoteWidget.currentItem()
        self.remotedir+= ('/'+item.text(1))
        if item.text(0)=="Folder":
            self.post_msg('CWD '+item.text(1))
        self.get_msg()
        self.LIST()
    def MKD(self):
        self.post_msg('MKD '+self.mkdEdit.text())
        self.get_msg()
        self.LIST()

    def RMD(self):
        self.post_msg('RMD '+self.remoteWidget.currentItem().text(1))
        self.get_msg()
    def rename(self):
        self.RNFR(self.remoteWidget.currentItem().text(1))
        self.RNTO(self.renameEdit.text())
        self.LIST()
    def RNFR(self,params):
        self.post_msg('RNFR '+params)
        self.get_msg()
    def RNTO(self,params):
        self.post_msg('RNTO '+params)
        self.get_msg()
    def LIST(self,params=str('')):
        self.remoteWidget.clear()
        self.BuildDTP()
        self.post_msg('LIST')
        if(self.tran_mode=='PORT'):
            self.tran_fd,_=self.listen_fd.accept()
            self.listen_fd.close()
        self.get_msg()        
        file_list=''
        while True:
            data=self.tran_fd.recv(8192).decode()
            file_list+=data
            if not data:break
        file_list=file_list.split('\n')[1:-1] # 最后一行是空行
        for file in file_list:
            file = re.sub('\s+', ' ',file).strip('\r')
            file=file.split(' ')
            if file[-1]=='':
                file=file[:-1]
            item = QTreeWidgetItem(self.remoteWidget)
            if file[0][0]=='d':
                item.setText(0,'Folder')
            else: item.setText(0,'File')
            item.setText(1,file[-1])
            item.setText(2,file[4])
            item.setText(3,' '.join(file[5:8]))
        self.tran_fd.close()
        self.get_msg()
    def LocalLIST(self):
        self.localWidget.clear()
        for file in os.listdir(self.localdir):
            filepath=os.path.join(self.localdir,file)
            stat = os.stat(filepath)
            item =QTreeWidgetItem(self.localWidget)
            if os.path.isdir(filepath):
                item.setText(0,'Folder')
            else : item.setText(0,'File')
            item.setText(1,file)
            item.setText(2,str(stat.st_size))
            item.setText(3,time.strftime('%b %d %H:%M', time.gmtime(stat.st_mtime)))
    def finish(self):
        self.get_msg()
        print('finish')
        self.LIST()
        self.LocalLIST()
    def RETR(self):            
        self.BuildDTP()
        item=self.remoteWidget.currentItem()
        filepath=self.localdir+'/'+item.text(1)
        self.post_msg('RETR '+item.text(1))
        if(self.tran_mode=='PORT'):
            self.tran_fd,_=self.listen_fd.accept()
            self.listen_fd.close()
        self.get_msg()
        item2=QTreeWidgetItem(self.transWidget)
        item2.setText(0,'Download')
        item2.setText(1,filepath)
        item2.setText(2,self.remotedir+'/'+item.text(1))
        item2.setText(3,item.text(2))
        item2.setText(4,str(0))
        def update(offset):
            item2.setText(4,str(offset/int(item.text(2))))     
        self.thread=DownloadThread(self,filepath)
        self.thread.update_signal.connect(update)
        self.thread.complete_signal.connect(self.finish)
        self.thread.start()
    def STOR(self):
        self.BuildDTP()
        item=self.localWidget.currentItem()
        filepath=self.localdir+'/'+item.text(1)
        self.post_msg('STOR '+item.text(1))
        if(self.tran_mode=='PORT'):
            self.tran_fd,_=self.listen_fd.accept()
            self.listen_fd.close()
        self.get_msg()
        item2=QTreeWidgetItem(self.transWidget)
        item2.setText(0,'Upload')
        item2.setText(1,filepath)
        item2.setText(2,self.remotedir+'/'+item.text(1))
        item2.setText(3,item.text(2))
        item2.setText(4,str(0))
        def update(offset):
            item2.setText(4,str(offset/int(item.text(2))))   
        self.thread=UploadThread(self,filepath)
        self.thread.update_signal.connect(update)
        self.thread.complete_signal.connect(self.finish)
        self.thread.start()
    def BuildDTP(self):
        if self.tran_mode == 'PASV':
            self.PASV()
        elif self.tran_mode == 'PORT':
            self.PORT()
    