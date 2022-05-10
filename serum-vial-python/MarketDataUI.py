# This Python file uses the following encoding: utf-8
import os
from pathlib import Path
import sys

from PySide2.QtWidgets import QApplication, QWidget
from PySide2.QtCore import QFile
from PySide2.QtUiTools import QUiLoader
from logger import Logger
from events import EventHandler
from serum_modules.wrapper import Wrapper
from serum_modules.config import KEYPAIR_PHANTOM

class Widget(QWidget):
    def __init__(self):
        super(Widget, self).__init__()
        self.load_ui()
        self.load_wrapper()

    def load_ui(self):
        loader = QUiLoader()
        path = os.fspath(Path(__file__).resolve().parent / "ui/form.ui")
        ui_file = QFile(path)
        ui_file.open(QFile.ReadOnly)
        self.ui = loader.load(ui_file, self)
        ui_file.close()


    def load_wrapper(self):
        self.__logger = Logger(self.ui.LoggerBox)
        self.__event_handler = EventHandler(self.__logger)
        self.__wr = Wrapper(
            KEYPAIR_PHANTOM,
            self.__logger,
            self.__event_handler.message_event,
            self.__event_handler.information_event
        )
        
        

    
    # def subscribe_lv1_event(self):
    #     if self.ui.Base.getText():
    

if __name__ == "__main__":
    app = QApplication([])
    widget = Widget()
    widget.show()
    sys.exit(app.exec_())
