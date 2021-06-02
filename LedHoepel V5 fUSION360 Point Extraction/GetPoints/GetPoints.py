#Author-
#Description-

import adsk.core, adsk.fusion, adsk.cam, traceback
import os

def run(context):
    ui = None
    try:
        app = adsk.core.Application.get()
        ui  = app.userInterface
        ui.messageBox('{}\n{}'.format(os.getcwd(), os.path.dirname(__file__)))
        # ui.messageBox('Hello script')
        pntEntity = ui.selectEntity('Select a sketch point', 'SketchPoints') 

    except:
        if ui:
            ui.messageBox('Failed:\n{}'.format(traceback.format_exc()))
