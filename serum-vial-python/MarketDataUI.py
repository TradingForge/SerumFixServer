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
from serum_modules.models import *

class Widget(QWidget):
    def __init__(self):
        super(Widget, self).__init__()
        self.load_ui()
        self.load_wrapper()
        self.connect()

    def load_ui(self):
        loader = QUiLoader()
        path = os.fspath(Path(__file__).resolve().parent / "UI/form.ui")
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

        self.__wr.start()

    
    def connect(self):
        self.ui.SubscribeLevel1.clicked.connect(self.subscribe_lv1_event)
        self.ui.UnsubscribeLevel1.clicked.connect(self.unsubscribe_lv1_event)

        self.ui.SubscribeLevel2.clicked.connect(self.subscribe_lv2_event)
        self.ui.UnsubscribeLevel2.clicked.connect(self.unsubscribe_lv2_event)
        
        
    def subscribe_lv1_event(self):
        if self.ui.Base.text() == "" or self.ui.Quote.text() == "":
            self.__logger.error("Input fields must not be empty")
            return

        self.__wr.subscribe(
            Channels.Level1, 
            Instrument(self.ui.Base.text().upper(), self.ui.Quote.text().upper())
        )

    def unsubscribe_lv1_event(self):
        if self.ui.Base.text() == "" or self.ui.Quote.text() == "":
            self.__logger.error("Input fields must not be empty")
            return

        self.__wr.unsubscribe(
            Channels.Level1, 
            Instrument(self.ui.Base.text().upper(), self.ui.Quote.text().upper())
        )

    def subscribe_lv2_event(self):
        if self.ui.Base.text() == "" or self.ui.Quote.text() == "":
            self.__logger.error("Input fields must not be empty")
            return

        self.__wr.subscribe(
            Channels.Level2, 
            Instrument(self.ui.Base.text().upper(), self.ui.Quote.text().upper())
        )

    def unsubscribe_lv2_event(self):
        if self.ui.Base.text() == "" or self.ui.Quote.text() == "":
            self.__logger.error("Input fields must not be empty")
            return

        self.__wr.unsubscribe(
            Channels.Level2, 
            Instrument(self.ui.Base.text().upper(), self.ui.Quote.text().upper())
        )


if __name__ == "__main__":
    app = QApplication([])
    widget = Widget()
    widget.show()
    sys.exit(app.exec_())
