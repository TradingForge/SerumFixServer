import datetime 
from PySide2.QtWidgets import QPlainTextEdit

class Logger:
    def __init__(self, output_device: QPlainTextEdit) -> None:
        self.__od = output_device
        self.__now = datetime.datetime.now
        
    
    def __log(self, msg):
        self.__od.insertPlainText(msg)
        
    def info(self, msg):
        self.__log((f'{self.__now().hour}:{self.__now().minute}:{self.__now().second} | <-- info -- {msg}\n'))
        
    def error(self, msg):
        self.__log((f'{self.__now().hour}:{self.__now().minute}:{self.__now().second} | <-- error -- {msg}\n'))
        
    def debug(self, msg):
        self.__log((f'{self.__now().hour}:{self.__now().minute}:{self.__now().second} | <-- debug -- {msg}\n'))