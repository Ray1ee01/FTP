from PyQt5.QtWidgets import *
from PyQt5.QtGui import *
from PyQt5.QtCore import *
import socket
import time

class DownloadThread(QThread):
    update_signal=pyqtSignal(int)
    complete_signal = pyqtSignal()
    def __init__(self,client,filepath):
        super(DownloadThread,self).__init__()
        self.client=client
        self.filepath=filepath
    def run(self):
        try:
            with open(self.filepath,'ab+') as file:
                count=0
                while True:
                    data = self.client.tran_fd.recv(8192)
                    if not data:
                        break
                    self.client.offset+=len(data)
                    file.write(data)
                    count+=1
                    if count%500==0:
                        self.update_signal.emit(self.client.offset)
        except:
            self.update_signal.emit(self.client.offset)
            self.complete_signal.emit()
            # self.client.tran_fd.close()
        else:
            self.update_signal.emit(self.client.offset)
            self.client.offset=0
            self.complete_signal.emit()
            self.client.tran_fd.close()
        finally:
            self.exit()

class UploadThread(QThread):
    update_signal=pyqtSignal(int)
    complete_signal = pyqtSignal()
    def __init__(self,client,filepath):
        super(UploadThread,self).__init__()
        self.client=client
        self.filepath=filepath
    def run(self):
        try:
            with open(self.filepath,'rb+') as file:
                file.seek(self.client.offset)
                count=0
                while True:
                    data = file.read(8192)
                    count+=1
                    if not data:
                        break
                    if self.client.tran_fd.send(data)<=0:
                        break
                    self.client.offset+=len(data)
                    if count%500==0:
                        print(count)
                        self.update_signal.emit(self.client.offset)
        except:
            self.update_signal.emit(self.client.offset)
            # self.client.tran_fd.close()
            self.complete_signal.emit()
            print('except')
        else:
            self.update_signal.emit(self.client.offset)
            self.client.tran_fd.close()
            self.client.offset=0
            self.complete_signal.emit()
            print('else')
        finally:
            print('exit')
            self.exit()